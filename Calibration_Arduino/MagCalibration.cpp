#include "MagCalibration.h"
#include <Arduino.h>
#include <Adafruit_SensorLab.h>
#include <Adafruit_Sensor_Calibration.h>

// Prints data in the format that "MotionCalibration" likes.
void runCalibration(Adafruit_LIS3MDL* lis3mdl) {
  sensors_event_t event; 
  lis3mdl->getEvent(&event);
  
  Serial.print("Raw:0,0,0,0,0,0,");
  Serial.print(int(event.magnetic.x*10)); Serial.print(",");
  Serial.print(int(event.magnetic.y*10)); Serial.print(",");
  Serial.print(int(event.magnetic.z*10)); Serial.println("");

  delay(10);
}

double getScaledMag(double x, double y, double z, double* offsets, double* transform) {
  x -= offsets[0];
  y -= offsets[1];
  z -= offsets[2];

  double newX = x * transform[0] + y * transform[3] + z * transform[6];
  double newY = x * transform[1] + y * transform[4] + z * transform[7];
  double newZ = x * transform[2] + y * transform[5] + z * transform[8];

  double mag = (newX * newX) + (newY * newY) + (newZ * newZ);

  return mag;
}
