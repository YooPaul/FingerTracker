#include "MagCalibration.h"
#include <Arduino.h>
#include <Adafruit_SensorLab.h>
#include <Adafruit_Sensor_Calibration.h>

double mag_off_4[] = {-14.92, 32.58, -26.22}; // (0,0,1)
double mag_map_4[] = {1.001, 0.061, -0.002, 0.061, 1.001, 0.001, -0.002, 0.001, 1.001};
double mag_mag_4 = 55.54;

double mag_off_5[] = {9.63, -24.62, 2.11}; // (0,0,0)
double mag_map_5[] = {1.025, 0.087, -0.027, 0.087, 1.029, -0.043, -0.027, -0.043, 0.958}; // Magnetic Mapping, read from left to right top to bottom (Row Major)
double mag_mag_5 = 52.89;

double mag_off_6[] = {-15.83, -7.76, -9.97}; // (1,-1,0)
double mag_map_6[] = {1.003, 0.066, -0.019, 0.066, 1.017, -0.021, -0.019, -0.021, 0.986};
double mag_mag_6 = 54.60;

double mag_off_7[] = {3.90, -30.79, -16.13}; // (1,1,0)
double mag_map_7[] = {1.020, 0.056, 0.005, 0.056, 1.011, -0.010, 0.005, -0.010, 0.972};
double mag_mag_7 = 55.94;


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
