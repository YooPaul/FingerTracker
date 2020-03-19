#ifndef _CALIBRATION_PARAMS_H
#define _CALIBRATION_PARAMS_H

double mag_off_4[] = {-15.10, 36.46, -49.17}; // (0,0,1)
double mag_map_4[] = {0.988, 0.057, 0.003, 0.057, 1.022, 0.002, 0.003, -0.002, 0.994};
double mag_mag_4 = 43.91;

double mag_off_5[] = {9.47, -26.32, 5.05}; // (0,0,0)
double mag_map_5[] = {1.008, 0.041, -.009, 0.041, 1.0009, -.003, -0.009, -.003, .985}; // Magnetic Mapping, read from left to right top to bottom (Row Major)
double mag_mag_5 = 44.33;

double mag_off_6[] = {-16.01, -8.34, -7.81}; // (-1,1,0)
double mag_map_6[] = {0.994, 0.050, -0.013, 0.050, 1.006, -0.003, -0.013, -0.003, 1.003};
double mag_mag_6 = 44.60;

double mag_off_7[] = {4.80, -31.21, -13.02}; // (1,1,0)
double mag_map_7[] = {1.006, 0.038, -0.008, 0.038, 1.001, -0.001, -0.008, -0.001, .994};
double mag_mag_7 = 44.86;

#endif
