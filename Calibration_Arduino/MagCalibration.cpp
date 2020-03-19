#include "MagCalibration.h"
#include <Arduino.h>
#include <Adafruit_SensorLab.h>
#include <Adafruit_Sensor_Calibration.h>

double mag_off_4[] = {-14.82, 34.23, -25.08}; // (0,0,1)
double mag_map_4[] = {0.994, 0.055, -0.010, 0.055, 1.015, 0.002, -0.010, 0.002, 0.993};
double mag_mag_4 = 45.78;

double mag_off_5[] = {8.43, -25.96, 3.62}; // (0,0,0)
double mag_map_5[] = {1.010, 0.051, 0.000, 0.051, 1.005, 0.002, 0.000, 0.002, 0.987}; // Magnetic Mapping, read from left to right top to bottom (Row Major)
double mag_mag_5 = 45.33;

double mag_off_6[] = {-16.62, -7.08, -8.90}; // (1,-1,0)
double mag_map_6[] = {0.996, 0.052, -0.005, 0.052, 1.003, -0.002, -0.005, -0.002, 1.004};
double mag_mag_6 = 45.34;

double mag_off_7[] = {3.15, -28.46, -15.81}; // (1,1,0)
double mag_map_7[] = {1.006, 0.048, -0.002, 0.048, 1.013, -0.006, -0.002, -0.006, 0.984};
double mag_mag_7 = 45.44;

bool use_calibrated = true;


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

double getScaledMag(double x, double y, double z, int sensorID) {
  if (!use_calibrated) {
    return x * x + y * y + z * z;
  }
  if (sensorID == 4) {
      x -= mag_off_4[0];
      y -= mag_off_4[1];
      z -= mag_off_4[2];
    
      double newX = x * mag_map_4[0] + y * mag_map_4[3] + z * mag_map_4[6];
      double newY = x * mag_map_4[1] + y * mag_map_4[4] + z * mag_map_4[7];
      double newZ = x * mag_map_4[2] + y * mag_map_4[5] + z * mag_map_4[8];
    
      double mag = (newX * newX) + (newY * newY) + (newZ * newZ);
    
      return mag;
  } else if (sensorID == 5) {
      x -= mag_off_5[0];
      y -= mag_off_5[1];
      z -= mag_off_5[2];
    
      double newX = x * mag_map_5[0] + y * mag_map_5[3] + z * mag_map_5[6];
      double newY = x * mag_map_5[1] + y * mag_map_5[4] + z * mag_map_5[7];
      double newZ = x * mag_map_5[2] + y * mag_map_5[5] + z * mag_map_5[8];
    
      double mag = (newX * newX) + (newY * newY) + (newZ * newZ);
    
      return mag;
  } else if (sensorID == 6) {
      x -= mag_off_6[0];
      y -= mag_off_6[1];
      z -= mag_off_6[2];
    
      double newX = x * mag_map_6[0] + y * mag_map_6[3] + z * mag_map_6[6];
      double newY = x * mag_map_6[1] + y * mag_map_6[4] + z * mag_map_6[7];
      double newZ = x * mag_map_6[2] + y * mag_map_6[5] + z * mag_map_6[8];
    
      double mag = (newX * newX) + (newY * newY) + (newZ * newZ);
    
      return mag;
  } else {
      x -= mag_off_7[0];
      y -= mag_off_7[1];
      z -= mag_off_7[2];
    
      double newX = x * mag_map_7[0] + y * mag_map_7[3] + z * mag_map_7[6];
      double newY = x * mag_map_7[1] + y * mag_map_7[4] + z * mag_map_7[7];
      double newZ = x * mag_map_7[2] + y * mag_map_7[5] + z * mag_map_7[8];
    
      double mag = (newX * newX) + (newY * newY) + (newZ * newZ);
    
      return mag;
  }  
}
