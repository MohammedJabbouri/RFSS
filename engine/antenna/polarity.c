#include <math.h>
#include <complex.h>
#include <stdlib.h>
#include <stdio.h>
#include "polarity.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

typedef structure {
  double t;
  double Ex;
  double Ey;
} EFieldPoint;

int main(){
  int steps = 2000;
  double m_time = 2.0 * M_PI;
  double dt = m_time/steps;
  
  double Ax = 1;
  double Ay = 1;

  double phi_linear = 0;
  double phi_rhcp = -M_PI / 2;
  double phi_lhcp = M_PI / 2;

  return 0;
}
