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
