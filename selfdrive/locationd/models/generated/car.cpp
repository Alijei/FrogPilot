#include "car.h"

namespace {
#define DIM 9
#define EDIM 9
#define MEDIM 9
typedef void (*Hfun)(double *, double *, double *);

double mass;

void set_mass(double x){ mass = x;}

double rotational_inertia;

void set_rotational_inertia(double x){ rotational_inertia = x;}

double center_to_front;

void set_center_to_front(double x){ center_to_front = x;}

double center_to_rear;

void set_center_to_rear(double x){ center_to_rear = x;}

double stiffness_front;

void set_stiffness_front(double x){ stiffness_front = x;}

double stiffness_rear;

void set_stiffness_rear(double x){ stiffness_rear = x;}
const static double MAHA_THRESH_25 = 3.8414588206941227;
const static double MAHA_THRESH_24 = 5.991464547107981;
const static double MAHA_THRESH_30 = 3.8414588206941227;
const static double MAHA_THRESH_26 = 3.8414588206941227;
const static double MAHA_THRESH_27 = 3.8414588206941227;
const static double MAHA_THRESH_29 = 3.8414588206941227;
const static double MAHA_THRESH_28 = 3.8414588206941227;
const static double MAHA_THRESH_31 = 3.8414588206941227;

/******************************************************************************
 *                       Code generated with SymPy 1.12                       *
 *                                                                            *
 *              See http://www.sympy.org/ for more information.               *
 *                                                                            *
 *                         This file is part of 'ekf'                         *
 ******************************************************************************/
void err_fun(double *nom_x, double *delta_x, double *out_4773823985180405484) {
   out_4773823985180405484[0] = delta_x[0] + nom_x[0];
   out_4773823985180405484[1] = delta_x[1] + nom_x[1];
   out_4773823985180405484[2] = delta_x[2] + nom_x[2];
   out_4773823985180405484[3] = delta_x[3] + nom_x[3];
   out_4773823985180405484[4] = delta_x[4] + nom_x[4];
   out_4773823985180405484[5] = delta_x[5] + nom_x[5];
   out_4773823985180405484[6] = delta_x[6] + nom_x[6];
   out_4773823985180405484[7] = delta_x[7] + nom_x[7];
   out_4773823985180405484[8] = delta_x[8] + nom_x[8];
}
void inv_err_fun(double *nom_x, double *true_x, double *out_3626594556511173842) {
   out_3626594556511173842[0] = -nom_x[0] + true_x[0];
   out_3626594556511173842[1] = -nom_x[1] + true_x[1];
   out_3626594556511173842[2] = -nom_x[2] + true_x[2];
   out_3626594556511173842[3] = -nom_x[3] + true_x[3];
   out_3626594556511173842[4] = -nom_x[4] + true_x[4];
   out_3626594556511173842[5] = -nom_x[5] + true_x[5];
   out_3626594556511173842[6] = -nom_x[6] + true_x[6];
   out_3626594556511173842[7] = -nom_x[7] + true_x[7];
   out_3626594556511173842[8] = -nom_x[8] + true_x[8];
}
void H_mod_fun(double *state, double *out_6676573484246823300) {
   out_6676573484246823300[0] = 1.0;
   out_6676573484246823300[1] = 0;
   out_6676573484246823300[2] = 0;
   out_6676573484246823300[3] = 0;
   out_6676573484246823300[4] = 0;
   out_6676573484246823300[5] = 0;
   out_6676573484246823300[6] = 0;
   out_6676573484246823300[7] = 0;
   out_6676573484246823300[8] = 0;
   out_6676573484246823300[9] = 0;
   out_6676573484246823300[10] = 1.0;
   out_6676573484246823300[11] = 0;
   out_6676573484246823300[12] = 0;
   out_6676573484246823300[13] = 0;
   out_6676573484246823300[14] = 0;
   out_6676573484246823300[15] = 0;
   out_6676573484246823300[16] = 0;
   out_6676573484246823300[17] = 0;
   out_6676573484246823300[18] = 0;
   out_6676573484246823300[19] = 0;
   out_6676573484246823300[20] = 1.0;
   out_6676573484246823300[21] = 0;
   out_6676573484246823300[22] = 0;
   out_6676573484246823300[23] = 0;
   out_6676573484246823300[24] = 0;
   out_6676573484246823300[25] = 0;
   out_6676573484246823300[26] = 0;
   out_6676573484246823300[27] = 0;
   out_6676573484246823300[28] = 0;
   out_6676573484246823300[29] = 0;
   out_6676573484246823300[30] = 1.0;
   out_6676573484246823300[31] = 0;
   out_6676573484246823300[32] = 0;
   out_6676573484246823300[33] = 0;
   out_6676573484246823300[34] = 0;
   out_6676573484246823300[35] = 0;
   out_6676573484246823300[36] = 0;
   out_6676573484246823300[37] = 0;
   out_6676573484246823300[38] = 0;
   out_6676573484246823300[39] = 0;
   out_6676573484246823300[40] = 1.0;
   out_6676573484246823300[41] = 0;
   out_6676573484246823300[42] = 0;
   out_6676573484246823300[43] = 0;
   out_6676573484246823300[44] = 0;
   out_6676573484246823300[45] = 0;
   out_6676573484246823300[46] = 0;
   out_6676573484246823300[47] = 0;
   out_6676573484246823300[48] = 0;
   out_6676573484246823300[49] = 0;
   out_6676573484246823300[50] = 1.0;
   out_6676573484246823300[51] = 0;
   out_6676573484246823300[52] = 0;
   out_6676573484246823300[53] = 0;
   out_6676573484246823300[54] = 0;
   out_6676573484246823300[55] = 0;
   out_6676573484246823300[56] = 0;
   out_6676573484246823300[57] = 0;
   out_6676573484246823300[58] = 0;
   out_6676573484246823300[59] = 0;
   out_6676573484246823300[60] = 1.0;
   out_6676573484246823300[61] = 0;
   out_6676573484246823300[62] = 0;
   out_6676573484246823300[63] = 0;
   out_6676573484246823300[64] = 0;
   out_6676573484246823300[65] = 0;
   out_6676573484246823300[66] = 0;
   out_6676573484246823300[67] = 0;
   out_6676573484246823300[68] = 0;
   out_6676573484246823300[69] = 0;
   out_6676573484246823300[70] = 1.0;
   out_6676573484246823300[71] = 0;
   out_6676573484246823300[72] = 0;
   out_6676573484246823300[73] = 0;
   out_6676573484246823300[74] = 0;
   out_6676573484246823300[75] = 0;
   out_6676573484246823300[76] = 0;
   out_6676573484246823300[77] = 0;
   out_6676573484246823300[78] = 0;
   out_6676573484246823300[79] = 0;
   out_6676573484246823300[80] = 1.0;
}
void f_fun(double *state, double dt, double *out_1082718903779600193) {
   out_1082718903779600193[0] = state[0];
   out_1082718903779600193[1] = state[1];
   out_1082718903779600193[2] = state[2];
   out_1082718903779600193[3] = state[3];
   out_1082718903779600193[4] = state[4];
   out_1082718903779600193[5] = dt*((-state[4] + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*state[4]))*state[6] - 9.8000000000000007*state[8] + stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(mass*state[1]) + (-stiffness_front*state[0] - stiffness_rear*state[0])*state[5]/(mass*state[4])) + state[5];
   out_1082718903779600193[6] = dt*(center_to_front*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(rotational_inertia*state[1]) + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])*state[5]/(rotational_inertia*state[4]) + (-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])*state[6]/(rotational_inertia*state[4])) + state[6];
   out_1082718903779600193[7] = state[7];
   out_1082718903779600193[8] = state[8];
}
void F_fun(double *state, double dt, double *out_8949493696495999361) {
   out_8949493696495999361[0] = 1;
   out_8949493696495999361[1] = 0;
   out_8949493696495999361[2] = 0;
   out_8949493696495999361[3] = 0;
   out_8949493696495999361[4] = 0;
   out_8949493696495999361[5] = 0;
   out_8949493696495999361[6] = 0;
   out_8949493696495999361[7] = 0;
   out_8949493696495999361[8] = 0;
   out_8949493696495999361[9] = 0;
   out_8949493696495999361[10] = 1;
   out_8949493696495999361[11] = 0;
   out_8949493696495999361[12] = 0;
   out_8949493696495999361[13] = 0;
   out_8949493696495999361[14] = 0;
   out_8949493696495999361[15] = 0;
   out_8949493696495999361[16] = 0;
   out_8949493696495999361[17] = 0;
   out_8949493696495999361[18] = 0;
   out_8949493696495999361[19] = 0;
   out_8949493696495999361[20] = 1;
   out_8949493696495999361[21] = 0;
   out_8949493696495999361[22] = 0;
   out_8949493696495999361[23] = 0;
   out_8949493696495999361[24] = 0;
   out_8949493696495999361[25] = 0;
   out_8949493696495999361[26] = 0;
   out_8949493696495999361[27] = 0;
   out_8949493696495999361[28] = 0;
   out_8949493696495999361[29] = 0;
   out_8949493696495999361[30] = 1;
   out_8949493696495999361[31] = 0;
   out_8949493696495999361[32] = 0;
   out_8949493696495999361[33] = 0;
   out_8949493696495999361[34] = 0;
   out_8949493696495999361[35] = 0;
   out_8949493696495999361[36] = 0;
   out_8949493696495999361[37] = 0;
   out_8949493696495999361[38] = 0;
   out_8949493696495999361[39] = 0;
   out_8949493696495999361[40] = 1;
   out_8949493696495999361[41] = 0;
   out_8949493696495999361[42] = 0;
   out_8949493696495999361[43] = 0;
   out_8949493696495999361[44] = 0;
   out_8949493696495999361[45] = dt*(stiffness_front*(-state[2] - state[3] + state[7])/(mass*state[1]) + (-stiffness_front - stiffness_rear)*state[5]/(mass*state[4]) + (-center_to_front*stiffness_front + center_to_rear*stiffness_rear)*state[6]/(mass*state[4]));
   out_8949493696495999361[46] = -dt*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(mass*pow(state[1], 2));
   out_8949493696495999361[47] = -dt*stiffness_front*state[0]/(mass*state[1]);
   out_8949493696495999361[48] = -dt*stiffness_front*state[0]/(mass*state[1]);
   out_8949493696495999361[49] = dt*((-1 - (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*pow(state[4], 2)))*state[6] - (-stiffness_front*state[0] - stiffness_rear*state[0])*state[5]/(mass*pow(state[4], 2)));
   out_8949493696495999361[50] = dt*(-stiffness_front*state[0] - stiffness_rear*state[0])/(mass*state[4]) + 1;
   out_8949493696495999361[51] = dt*(-state[4] + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*state[4]));
   out_8949493696495999361[52] = dt*stiffness_front*state[0]/(mass*state[1]);
   out_8949493696495999361[53] = -9.8000000000000007*dt;
   out_8949493696495999361[54] = dt*(center_to_front*stiffness_front*(-state[2] - state[3] + state[7])/(rotational_inertia*state[1]) + (-center_to_front*stiffness_front + center_to_rear*stiffness_rear)*state[5]/(rotational_inertia*state[4]) + (-pow(center_to_front, 2)*stiffness_front - pow(center_to_rear, 2)*stiffness_rear)*state[6]/(rotational_inertia*state[4]));
   out_8949493696495999361[55] = -center_to_front*dt*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(rotational_inertia*pow(state[1], 2));
   out_8949493696495999361[56] = -center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_8949493696495999361[57] = -center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_8949493696495999361[58] = dt*(-(-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])*state[5]/(rotational_inertia*pow(state[4], 2)) - (-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])*state[6]/(rotational_inertia*pow(state[4], 2)));
   out_8949493696495999361[59] = dt*(-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(rotational_inertia*state[4]);
   out_8949493696495999361[60] = dt*(-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])/(rotational_inertia*state[4]) + 1;
   out_8949493696495999361[61] = center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_8949493696495999361[62] = 0;
   out_8949493696495999361[63] = 0;
   out_8949493696495999361[64] = 0;
   out_8949493696495999361[65] = 0;
   out_8949493696495999361[66] = 0;
   out_8949493696495999361[67] = 0;
   out_8949493696495999361[68] = 0;
   out_8949493696495999361[69] = 0;
   out_8949493696495999361[70] = 1;
   out_8949493696495999361[71] = 0;
   out_8949493696495999361[72] = 0;
   out_8949493696495999361[73] = 0;
   out_8949493696495999361[74] = 0;
   out_8949493696495999361[75] = 0;
   out_8949493696495999361[76] = 0;
   out_8949493696495999361[77] = 0;
   out_8949493696495999361[78] = 0;
   out_8949493696495999361[79] = 0;
   out_8949493696495999361[80] = 1;
}
void h_25(double *state, double *unused, double *out_5943612926861998317) {
   out_5943612926861998317[0] = state[6];
}
void H_25(double *state, double *unused, double *out_4798810124502626147) {
   out_4798810124502626147[0] = 0;
   out_4798810124502626147[1] = 0;
   out_4798810124502626147[2] = 0;
   out_4798810124502626147[3] = 0;
   out_4798810124502626147[4] = 0;
   out_4798810124502626147[5] = 0;
   out_4798810124502626147[6] = 1;
   out_4798810124502626147[7] = 0;
   out_4798810124502626147[8] = 0;
}
void h_24(double *state, double *unused, double *out_6603452004924971224) {
   out_6603452004924971224[0] = state[4];
   out_6603452004924971224[1] = state[5];
}
void H_24(double *state, double *unused, double *out_1328410377659056050) {
   out_1328410377659056050[0] = 0;
   out_1328410377659056050[1] = 0;
   out_1328410377659056050[2] = 0;
   out_1328410377659056050[3] = 0;
   out_1328410377659056050[4] = 1;
   out_1328410377659056050[5] = 0;
   out_1328410377659056050[6] = 0;
   out_1328410377659056050[7] = 0;
   out_1328410377659056050[8] = 0;
   out_1328410377659056050[9] = 0;
   out_1328410377659056050[10] = 0;
   out_1328410377659056050[11] = 0;
   out_1328410377659056050[12] = 0;
   out_1328410377659056050[13] = 0;
   out_1328410377659056050[14] = 1;
   out_1328410377659056050[15] = 0;
   out_1328410377659056050[16] = 0;
   out_1328410377659056050[17] = 0;
}
void h_30(double *state, double *unused, double *out_4647875022952643907) {
   out_4647875022952643907[0] = state[4];
}
void H_30(double *state, double *unused, double *out_7317143083009874774) {
   out_7317143083009874774[0] = 0;
   out_7317143083009874774[1] = 0;
   out_7317143083009874774[2] = 0;
   out_7317143083009874774[3] = 0;
   out_7317143083009874774[4] = 1;
   out_7317143083009874774[5] = 0;
   out_7317143083009874774[6] = 0;
   out_7317143083009874774[7] = 0;
   out_7317143083009874774[8] = 0;
}
void h_26(double *state, double *unused, double *out_3480131221876502170) {
   out_3480131221876502170[0] = state[7];
}
void H_26(double *state, double *unused, double *out_1057306805628569923) {
   out_1057306805628569923[0] = 0;
   out_1057306805628569923[1] = 0;
   out_1057306805628569923[2] = 0;
   out_1057306805628569923[3] = 0;
   out_1057306805628569923[4] = 0;
   out_1057306805628569923[5] = 0;
   out_1057306805628569923[6] = 0;
   out_1057306805628569923[7] = 1;
   out_1057306805628569923[8] = 0;
}
void h_27(double *state, double *unused, double *out_7303240457871368145) {
   out_7303240457871368145[0] = state[3];
}
void H_27(double *state, double *unused, double *out_8906006919515733625) {
   out_8906006919515733625[0] = 0;
   out_8906006919515733625[1] = 0;
   out_8906006919515733625[2] = 0;
   out_8906006919515733625[3] = 1;
   out_8906006919515733625[4] = 0;
   out_8906006919515733625[5] = 0;
   out_8906006919515733625[6] = 0;
   out_8906006919515733625[7] = 0;
   out_8906006919515733625[8] = 0;
}
void h_29(double *state, double *unused, double *out_578032192882681040) {
   out_578032192882681040[0] = state[1];
}
void H_29(double *state, double *unused, double *out_7827374427324266958) {
   out_7827374427324266958[0] = 0;
   out_7827374427324266958[1] = 1;
   out_7827374427324266958[2] = 0;
   out_7827374427324266958[3] = 0;
   out_7827374427324266958[4] = 0;
   out_7827374427324266958[5] = 0;
   out_7827374427324266958[6] = 0;
   out_7827374427324266958[7] = 0;
   out_7827374427324266958[8] = 0;
}
void h_28(double *state, double *unused, double *out_6699924573194890628) {
   out_6699924573194890628[0] = state[0];
}
void H_28(double *state, double *unused, double *out_2744975410254736384) {
   out_2744975410254736384[0] = 1;
   out_2744975410254736384[1] = 0;
   out_2744975410254736384[2] = 0;
   out_2744975410254736384[3] = 0;
   out_2744975410254736384[4] = 0;
   out_2744975410254736384[5] = 0;
   out_2744975410254736384[6] = 0;
   out_2744975410254736384[7] = 0;
   out_2744975410254736384[8] = 0;
}
void h_31(double *state, double *unused, double *out_3194104357821852865) {
   out_3194104357821852865[0] = state[8];
}
void H_31(double *state, double *unused, double *out_431098703395218447) {
   out_431098703395218447[0] = 0;
   out_431098703395218447[1] = 0;
   out_431098703395218447[2] = 0;
   out_431098703395218447[3] = 0;
   out_431098703395218447[4] = 0;
   out_431098703395218447[5] = 0;
   out_431098703395218447[6] = 0;
   out_431098703395218447[7] = 0;
   out_431098703395218447[8] = 1;
}
#include <eigen3/Eigen/Dense>
#include <iostream>

typedef Eigen::Matrix<double, DIM, DIM, Eigen::RowMajor> DDM;
typedef Eigen::Matrix<double, EDIM, EDIM, Eigen::RowMajor> EEM;
typedef Eigen::Matrix<double, DIM, EDIM, Eigen::RowMajor> DEM;

void predict(double *in_x, double *in_P, double *in_Q, double dt) {
  typedef Eigen::Matrix<double, MEDIM, MEDIM, Eigen::RowMajor> RRM;

  double nx[DIM] = {0};
  double in_F[EDIM*EDIM] = {0};

  // functions from sympy
  f_fun(in_x, dt, nx);
  F_fun(in_x, dt, in_F);


  EEM F(in_F);
  EEM P(in_P);
  EEM Q(in_Q);

  RRM F_main = F.topLeftCorner(MEDIM, MEDIM);
  P.topLeftCorner(MEDIM, MEDIM) = (F_main * P.topLeftCorner(MEDIM, MEDIM)) * F_main.transpose();
  P.topRightCorner(MEDIM, EDIM - MEDIM) = F_main * P.topRightCorner(MEDIM, EDIM - MEDIM);
  P.bottomLeftCorner(EDIM - MEDIM, MEDIM) = P.bottomLeftCorner(EDIM - MEDIM, MEDIM) * F_main.transpose();

  P = P + dt*Q;

  // copy out state
  memcpy(in_x, nx, DIM * sizeof(double));
  memcpy(in_P, P.data(), EDIM * EDIM * sizeof(double));
}

// note: extra_args dim only correct when null space projecting
// otherwise 1
template <int ZDIM, int EADIM, bool MAHA_TEST>
void update(double *in_x, double *in_P, Hfun h_fun, Hfun H_fun, Hfun Hea_fun, double *in_z, double *in_R, double *in_ea, double MAHA_THRESHOLD) {
  typedef Eigen::Matrix<double, ZDIM, ZDIM, Eigen::RowMajor> ZZM;
  typedef Eigen::Matrix<double, ZDIM, DIM, Eigen::RowMajor> ZDM;
  typedef Eigen::Matrix<double, Eigen::Dynamic, EDIM, Eigen::RowMajor> XEM;
  //typedef Eigen::Matrix<double, EDIM, ZDIM, Eigen::RowMajor> EZM;
  typedef Eigen::Matrix<double, Eigen::Dynamic, 1> X1M;
  typedef Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> XXM;

  double in_hx[ZDIM] = {0};
  double in_H[ZDIM * DIM] = {0};
  double in_H_mod[EDIM * DIM] = {0};
  double delta_x[EDIM] = {0};
  double x_new[DIM] = {0};


  // state x, P
  Eigen::Matrix<double, ZDIM, 1> z(in_z);
  EEM P(in_P);
  ZZM pre_R(in_R);

  // functions from sympy
  h_fun(in_x, in_ea, in_hx);
  H_fun(in_x, in_ea, in_H);
  ZDM pre_H(in_H);

  // get y (y = z - hx)
  Eigen::Matrix<double, ZDIM, 1> pre_y(in_hx); pre_y = z - pre_y;
  X1M y; XXM H; XXM R;
  if (Hea_fun){
    typedef Eigen::Matrix<double, ZDIM, EADIM, Eigen::RowMajor> ZAM;
    double in_Hea[ZDIM * EADIM] = {0};
    Hea_fun(in_x, in_ea, in_Hea);
    ZAM Hea(in_Hea);
    XXM A = Hea.transpose().fullPivLu().kernel();


    y = A.transpose() * pre_y;
    H = A.transpose() * pre_H;
    R = A.transpose() * pre_R * A;
  } else {
    y = pre_y;
    H = pre_H;
    R = pre_R;
  }
  // get modified H
  H_mod_fun(in_x, in_H_mod);
  DEM H_mod(in_H_mod);
  XEM H_err = H * H_mod;

  // Do mahalobis distance test
  if (MAHA_TEST){
    XXM a = (H_err * P * H_err.transpose() + R).inverse();
    double maha_dist = y.transpose() * a * y;
    if (maha_dist > MAHA_THRESHOLD){
      R = 1.0e16 * R;
    }
  }

  // Outlier resilient weighting
  double weight = 1;//(1.5)/(1 + y.squaredNorm()/R.sum());

  // kalman gains and I_KH
  XXM S = ((H_err * P) * H_err.transpose()) + R/weight;
  XEM KT = S.fullPivLu().solve(H_err * P.transpose());
  //EZM K = KT.transpose(); TODO: WHY DOES THIS NOT COMPILE?
  //EZM K = S.fullPivLu().solve(H_err * P.transpose()).transpose();
  //std::cout << "Here is the matrix rot:\n" << K << std::endl;
  EEM I_KH = Eigen::Matrix<double, EDIM, EDIM>::Identity() - (KT.transpose() * H_err);

  // update state by injecting dx
  Eigen::Matrix<double, EDIM, 1> dx(delta_x);
  dx  = (KT.transpose() * y);
  memcpy(delta_x, dx.data(), EDIM * sizeof(double));
  err_fun(in_x, delta_x, x_new);
  Eigen::Matrix<double, DIM, 1> x(x_new);

  // update cov
  P = ((I_KH * P) * I_KH.transpose()) + ((KT.transpose() * R) * KT);

  // copy out state
  memcpy(in_x, x.data(), DIM * sizeof(double));
  memcpy(in_P, P.data(), EDIM * EDIM * sizeof(double));
  memcpy(in_z, y.data(), y.rows() * sizeof(double));
}




}
extern "C" {

void car_update_25(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<1, 3, 0>(in_x, in_P, h_25, H_25, NULL, in_z, in_R, in_ea, MAHA_THRESH_25);
}
void car_update_24(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<2, 3, 0>(in_x, in_P, h_24, H_24, NULL, in_z, in_R, in_ea, MAHA_THRESH_24);
}
void car_update_30(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<1, 3, 0>(in_x, in_P, h_30, H_30, NULL, in_z, in_R, in_ea, MAHA_THRESH_30);
}
void car_update_26(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<1, 3, 0>(in_x, in_P, h_26, H_26, NULL, in_z, in_R, in_ea, MAHA_THRESH_26);
}
void car_update_27(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<1, 3, 0>(in_x, in_P, h_27, H_27, NULL, in_z, in_R, in_ea, MAHA_THRESH_27);
}
void car_update_29(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<1, 3, 0>(in_x, in_P, h_29, H_29, NULL, in_z, in_R, in_ea, MAHA_THRESH_29);
}
void car_update_28(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<1, 3, 0>(in_x, in_P, h_28, H_28, NULL, in_z, in_R, in_ea, MAHA_THRESH_28);
}
void car_update_31(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<1, 3, 0>(in_x, in_P, h_31, H_31, NULL, in_z, in_R, in_ea, MAHA_THRESH_31);
}
void car_err_fun(double *nom_x, double *delta_x, double *out_4773823985180405484) {
  err_fun(nom_x, delta_x, out_4773823985180405484);
}
void car_inv_err_fun(double *nom_x, double *true_x, double *out_3626594556511173842) {
  inv_err_fun(nom_x, true_x, out_3626594556511173842);
}
void car_H_mod_fun(double *state, double *out_6676573484246823300) {
  H_mod_fun(state, out_6676573484246823300);
}
void car_f_fun(double *state, double dt, double *out_1082718903779600193) {
  f_fun(state,  dt, out_1082718903779600193);
}
void car_F_fun(double *state, double dt, double *out_8949493696495999361) {
  F_fun(state,  dt, out_8949493696495999361);
}
void car_h_25(double *state, double *unused, double *out_5943612926861998317) {
  h_25(state, unused, out_5943612926861998317);
}
void car_H_25(double *state, double *unused, double *out_4798810124502626147) {
  H_25(state, unused, out_4798810124502626147);
}
void car_h_24(double *state, double *unused, double *out_6603452004924971224) {
  h_24(state, unused, out_6603452004924971224);
}
void car_H_24(double *state, double *unused, double *out_1328410377659056050) {
  H_24(state, unused, out_1328410377659056050);
}
void car_h_30(double *state, double *unused, double *out_4647875022952643907) {
  h_30(state, unused, out_4647875022952643907);
}
void car_H_30(double *state, double *unused, double *out_7317143083009874774) {
  H_30(state, unused, out_7317143083009874774);
}
void car_h_26(double *state, double *unused, double *out_3480131221876502170) {
  h_26(state, unused, out_3480131221876502170);
}
void car_H_26(double *state, double *unused, double *out_1057306805628569923) {
  H_26(state, unused, out_1057306805628569923);
}
void car_h_27(double *state, double *unused, double *out_7303240457871368145) {
  h_27(state, unused, out_7303240457871368145);
}
void car_H_27(double *state, double *unused, double *out_8906006919515733625) {
  H_27(state, unused, out_8906006919515733625);
}
void car_h_29(double *state, double *unused, double *out_578032192882681040) {
  h_29(state, unused, out_578032192882681040);
}
void car_H_29(double *state, double *unused, double *out_7827374427324266958) {
  H_29(state, unused, out_7827374427324266958);
}
void car_h_28(double *state, double *unused, double *out_6699924573194890628) {
  h_28(state, unused, out_6699924573194890628);
}
void car_H_28(double *state, double *unused, double *out_2744975410254736384) {
  H_28(state, unused, out_2744975410254736384);
}
void car_h_31(double *state, double *unused, double *out_3194104357821852865) {
  h_31(state, unused, out_3194104357821852865);
}
void car_H_31(double *state, double *unused, double *out_431098703395218447) {
  H_31(state, unused, out_431098703395218447);
}
void car_predict(double *in_x, double *in_P, double *in_Q, double dt) {
  predict(in_x, in_P, in_Q, dt);
}
void car_set_mass(double x) {
  set_mass(x);
}
void car_set_rotational_inertia(double x) {
  set_rotational_inertia(x);
}
void car_set_center_to_front(double x) {
  set_center_to_front(x);
}
void car_set_center_to_rear(double x) {
  set_center_to_rear(x);
}
void car_set_stiffness_front(double x) {
  set_stiffness_front(x);
}
void car_set_stiffness_rear(double x) {
  set_stiffness_rear(x);
}
}

const EKF car = {
  .name = "car",
  .kinds = { 25, 24, 30, 26, 27, 29, 28, 31 },
  .feature_kinds = {  },
  .f_fun = car_f_fun,
  .F_fun = car_F_fun,
  .err_fun = car_err_fun,
  .inv_err_fun = car_inv_err_fun,
  .H_mod_fun = car_H_mod_fun,
  .predict = car_predict,
  .hs = {
    { 25, car_h_25 },
    { 24, car_h_24 },
    { 30, car_h_30 },
    { 26, car_h_26 },
    { 27, car_h_27 },
    { 29, car_h_29 },
    { 28, car_h_28 },
    { 31, car_h_31 },
  },
  .Hs = {
    { 25, car_H_25 },
    { 24, car_H_24 },
    { 30, car_H_30 },
    { 26, car_H_26 },
    { 27, car_H_27 },
    { 29, car_H_29 },
    { 28, car_H_28 },
    { 31, car_H_31 },
  },
  .updates = {
    { 25, car_update_25 },
    { 24, car_update_24 },
    { 30, car_update_30 },
    { 26, car_update_26 },
    { 27, car_update_27 },
    { 29, car_update_29 },
    { 28, car_update_28 },
    { 31, car_update_31 },
  },
  .Hes = {
  },
  .sets = {
    { "mass", car_set_mass },
    { "rotational_inertia", car_set_rotational_inertia },
    { "center_to_front", car_set_center_to_front },
    { "center_to_rear", car_set_center_to_rear },
    { "stiffness_front", car_set_stiffness_front },
    { "stiffness_rear", car_set_stiffness_rear },
  },
  .extra_routines = {
  },
};

ekf_lib_init(car)
