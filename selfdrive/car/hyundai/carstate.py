from collections import deque
import copy
import math

from cereal import car, custom
from openpilot.common.conversions import Conversions as CV
from opendbc.can.parser import CANParser
from opendbc.can.can_define import CANDefine
from openpilot.selfdrive.car.hyundai.hyundaicanfd import CanBus
from openpilot.selfdrive.car.hyundai.values import HyundaiFlags, CAR, DBC, CAN_GEARS, CAMERA_SCC_CAR, \
                                                   CANFD_CAR, Buttons, CarControllerParams
from openpilot.selfdrive.car.interfaces import CarStateBase

PREV_BUTTON_SAMPLES = 8
CLUSTER_SAMPLE_RATE = 20  # frames
STANDSTILL_THRESHOLD = 12 * 0.03125 * CV.KPH_TO_MS


class CarState(CarStateBase):
  def __init__(self, CP):
    super().__init__(CP)
    can_define = CANDefine(DBC[CP.carFingerprint]["pt"])

    self.cruise_buttons = deque([Buttons.NONE] * PREV_BUTTON_SAMPLES, maxlen=PREV_BUTTON_SAMPLES)
    self.main_buttons = deque([Buttons.NONE] * PREV_BUTTON_SAMPLES, maxlen=PREV_BUTTON_SAMPLES)

    self.gear_msg_canfd = "GEAR_ALT" if CP.flags & HyundaiFlags.CANFD_ALT_GEARS else \
                          "GEAR_ALT_2" if CP.flags & HyundaiFlags.CANFD_ALT_GEARS_2 else \
                          "GEAR_SHIFTER"

    if CP.carFingerprint in CANFD_CAR:
      self.shifter_values = can_define.dv[self.gear_msg_canfd]["GEAR"]
    elif CP.carFingerprint in CAN_GEARS["use_cluster_gears"]:
      self.shifter_values = can_define.dv["CLU15"]["CF_Clu_Gear"]
    elif CP.carFingerprint in CAN_GEARS["use_tcu_gears"]:
      self.shifter_values = can_define.dv["TCU12"]["CUR_GR"]
    else:
      self.shifter_values = can_define.dv["LVR12"]["CF_Lvr_Gear"]

    self.accelerator_msg_canfd = "ACCELERATOR" if CP.flags & HyundaiFlags.EV else \
                                 "ACCELERATOR_ALT" if CP.flags & HyundaiFlags.HYBRID else \
                                 "ACCELERATOR_BRAKE_ALT"
    self.cruise_btns_msg_canfd = "CRUISE_BUTTONS_ALT" if CP.flags & HyundaiFlags.CANFD_ALT_BUTTONS else \
                                 "CRUISE_BUTTONS"
    self.is_metric = False
    self.buttons_counter = 0
    self.cruise_info = {}

    self.cluster_speed = 0
    self.cluster_speed_counter = CLUSTER_SAMPLE_RATE

    self.params = CarControllerParams(CP)

    # Détection spécifique pour le Hyundai Tucson 4th Gen
    if CP.carFingerprint == CAR.HYUNDAI_TUCSON_4TH_GEN:
      self.steer_ratio = 13.7
      self.steer_actuator_delay = 0.2
      self.steer_rate_cost = 1.0
      self.steer_limit_timer = 0.8

  def update(self, cp, cp_cam, frogpilot_toggles):
    if self.CP.carFingerprint in CANFD_CAR:
      return self.update_canfd(cp, cp_cam, frogpilot_toggles)

    ret = car.CarState.new_message()
    fp_ret = custom.FrogPilotCarState.new_message()
    cp_cruise = cp_cam if self.CP.carFingerprint in CAMERA_SCC_CAR else cp
    self.is_metric = cp.vl["CLU11"]["CF_Clu_SPEED_UNIT"] == 0
    speed_conv = CV.KPH_TO_MS if self.is_metric else CV.MPH_TO_MS

    ret.doorOpen = any([cp.vl["CGW1"]["CF_Gway_DrvDrSw"], cp.vl["CGW1"]["CF_Gway_AstDrSw"],
                        cp.vl["CGW2"]["CF_Gway_RLDrSw"], cp.vl["CGW2"]["CF_Gway_RRDrSw"]])

    ret.seatbeltUnlatched = cp.vl["CGW1"]["CF_Gway_DrvSeatBeltSw"] == 0

    ret.wheelSpeeds = self.get_wheel_speeds(
      cp.vl["WHL_SPD11"]["WHL_SPD_FL"],
      cp.vl["WHL_SPD11"]["WHL_SPD_FR"],
      cp.vl["WHL_SPD11"]["WHL_SPD_RL"],
      cp.vl["WHL_SPD11"]["WHL_SPD_RR"],
    )
    ret.vEgoRaw = sum([ret.wheelSpeeds.fl, ret.wheelSpeeds.fr, ret.wheelSpeeds.rl, ret.wheelSpeeds.rr]) / 4.
    ret.vEgo, ret.aEgo = self.update_speed_kf(ret.vEgoRaw)
    ret.standstill = ret.wheelSpeeds.fl <= STANDSTILL_THRESHOLD and ret.wheelSpeeds.rr <= STANDSTILL_THRESHOLD

    ret.steeringAngleDeg = cp.vl["SAS11"]["SAS_Angle"]
    ret.steeringRateDeg = cp.vl["SAS11"]["SAS_Speed"]
    ret.yawRate = cp.vl["ESP12"]["YAW_RATE"]
    ret.leftBlinker, ret.rightBlinker = self.update_blinker_from_lamp(
      50, cp.vl["CGW1"]["CF_Gway_TurnSigLh"], cp.vl["CGW1"]["CF_Gway_TurnSigRh"])
    ret.steeringTorque = cp.vl["MDPS12"]["CR_Mdps_StrColTq"]
    ret.steeringTorqueEps = cp.vl["MDPS12"]["CR_Mdps_OutTq"]
    ret.steeringPressed = abs(ret.steeringTorque) > self.params.STEER_THRESHOLD

    if self.CP.carFingerprint == CAR.HYUNDAI_TUCSON_4TH_GEN:
      ret.steerRatio = self.steer_ratio
      ret.steerActuatorDelay = self.steer_actuator_delay
      ret.steerRateCost = self.steer_rate_cost
      ret.steerLimitTimer = self.steer_limit_timer

    # Cruise state
    ret.cruiseState.available = cp_cruise.vl["SCC11"]["MainMode_ACC"] == 1
    ret.cruiseState.enabled = cp_cruise.vl["SCC12"]["ACCMode"] != 0
    ret.cruiseState.speed = cp_cruise.vl["SCC11"]["VSetDis"] * speed_conv

    ret.brakePressed = cp.vl["TCS13"]["DriverOverride"] == 2
    ret.gasPressed = cp.vl["EMS16"]["CF_Ems_AclAct"] != 0

    gear = cp.vl["LVR12"]["CF_Lvr_Gear"]
    ret.gearShifter = self.parse_gear_shifter(self.shifter_values.get(gear))

    if self.CP.carFingerprint == CAR.HYUNDAI_TUCSON_4TH_GEN:
      ret.flags |= HyundaiFlags.USE_FCA.value

    return ret, fp_ret

  def get_can_parser(self, CP):
    messages = [
      ("MDPS12", 50),
      ("TCS11", 100),
      ("CLU11", 50),
      ("ESP12", 100),
      ("CGW1", 10),
      ("WHL_SPD11", 50),
      ("SAS11", 100),
    ]
    if CP.carFingerprint == CAR.HYUNDAI_TUCSON_4TH_GEN:
      messages.append(("SCC11", 50))
      messages.append(("SCC12", 50))

    return CANParser(DBC[CP.carFingerprint]["pt"], messages, 0)
