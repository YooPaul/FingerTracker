#ifndef _MAG_CALIBRATION_H
#define _MAG_CALIBRATION_H

#include <Adafruit_LIS3MDL.h>

void runCalibration(Adafruit_LIS3MDL* lis3mdl);

double getScaledMag(double x, double y, double z, double* offsets, double* transform);

#endif
