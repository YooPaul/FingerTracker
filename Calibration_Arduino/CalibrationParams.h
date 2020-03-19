#ifndef _CALIBRATION_PARAMS_H
#define _CALIBRATION_PARAMS_H

double mag_off_4[] = {-14.85, 34.21, -50.03}; // (0,0,1)
double mag_map_4[] = {0.980, 0.048, -0.005, 0.048, 1.011, -0.003, -0.005, -0.003, 1.012};
double mag_mag_4 = 45.26;

double mag_off_5[] = {6.90, -24.74, 3.78}; // (0,0,0)
double mag_map_5[] = {1.002, 0.037, -.008, 0.037, 1.020, -0.001, -0.008, -.001, .979}; // Magnetic Mapping, read from left to right top to bottom (Row Major)
double mag_mag_5 = 44.33;

double mag_off_6[] = {-15.92, -8.41, -7.70}; // (1,-1,0)
double mag_map_6[] = {1.000, 0.052, -0.016, 0.052, 1.006, -0.008, -0.016, -0.008, 0.997};
double mag_mag_6 = 44.91;

double mag_off_7[] = {3.81, -25.75, -14.32}; // (1,1,0)
double mag_map_7[] = {0.988, 0.039, -0.011, 0.039, 1.051, 0.006, -0.011, 0.006, 0.965};
double mag_mag_7 = 42.97;

#endif
