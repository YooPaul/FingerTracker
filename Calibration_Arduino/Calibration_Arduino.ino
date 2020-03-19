#include <Wire.h>
#include <Adafruit_LIS3MDL.h>
#include <Adafruit_Sensor.h>
#include <TCA9548A.h>
#include "MagCalibration.h"
#include "CalibrationParams.h"
TCA9548A I2CMux;

Adafruit_LIS3MDL lis3mdl;
#define LIS3MDL_CLK 13
#define LIS3MDL_MISO 12
#define LIS3MDL_MOSI 11
#define LIS3MDL_CS 10

int currentMag = 5;
long lastPrintTime = 0;
int measurementCounter = 0;
boolean printAll = false;
boolean plotData = false;

void setup(void) {
  Serial.begin(1000000);
  while (!Serial) delay(10);     // will pause Zero, Leonardo, etc until serial console opens


  I2CMux.begin(Wire); 
  I2CMux.closeAll();
  I2CMux.openChannel(4);
  Serial.println("Adafruit LIS3MDL test!");
  
  // Just connect a magnetometer to the i2c bus 
  if (! lis3mdl.begin_I2C()) {          // hardware I2C mode, can pass in address & alt Wire
  //if (! lis3mdl.begin_SPI(LIS3MDL_CS)) {  // hardware SPI mode
  //if (! lis3mdl.begin_SPI(LIS3MDL_CS, LIS3MDL_CLK, LIS3MDL_MISO, LIS3MDL_MOSI)) { // soft SPI
    Serial.println("Failed to find LIS3MDL chip");
    while (1) { delay(10); }
  }

  initializeMagnetometer(4);
  initializeMagnetometer(5);
  initializeMagnetometer(6);
  initializeMagnetometer(7);
  Serial.println("LIS3MDL Found!");

  // Wire.setClock(400000); // Use fast 400khz i2c 
}

void initializeMagnetometer(int i2c_bus) {
  I2CMux.closeAll();
  I2CMux.openChannel(i2c_bus);
  
  lis3mdl.setPerformanceMode(LIS3MDL_ULTRAHIGHMODE);
  Serial.print("Performance mode set to: ");
  switch (lis3mdl.getPerformanceMode()) {
    case LIS3MDL_LOWPOWERMODE: Serial.println("Low"); break;
    case LIS3MDL_MEDIUMMODE: Serial.println("Medium"); break;
    case LIS3MDL_HIGHMODE: Serial.println("High"); break;
    case LIS3MDL_ULTRAHIGHMODE: Serial.println("Ultra-High"); break;
  }

  lis3mdl.setOperationMode(LIS3MDL_CONTINUOUSMODE);
  Serial.print("Operation mode set to: ");
  // Single shot mode will complete conversion and go into power down
  switch (lis3mdl.getOperationMode()) {
    case LIS3MDL_CONTINUOUSMODE: Serial.println("Continuous"); break;
    case LIS3MDL_SINGLEMODE: Serial.println("Single mode"); break;
    case LIS3MDL_POWERDOWNMODE: Serial.println("Power-down"); break;
  }

  lis3mdl.setDataRate(LIS3MDL_DATARATE_1000_HZ);
  // You can check the datarate by looking at the frequency of the DRDY pin
  Serial.print("Data rate set to: ");
  switch (lis3mdl.getDataRate()) {
    case LIS3MDL_DATARATE_0_625_HZ: Serial.println("0.625 Hz"); break;
    case LIS3MDL_DATARATE_1_25_HZ: Serial.println("1.25 Hz"); break;
    case LIS3MDL_DATARATE_2_5_HZ: Serial.println("2.5 Hz"); break;
    case LIS3MDL_DATARATE_5_HZ: Serial.println("5 Hz"); break;
    case LIS3MDL_DATARATE_10_HZ: Serial.println("10 Hz"); break;
    case LIS3MDL_DATARATE_20_HZ: Serial.println("20 Hz"); break;
    case LIS3MDL_DATARATE_40_HZ: Serial.println("40 Hz"); break;
    case LIS3MDL_DATARATE_80_HZ: Serial.println("80 Hz"); break;
    case LIS3MDL_DATARATE_155_HZ: Serial.println("155 Hz"); break;
    case LIS3MDL_DATARATE_300_HZ: Serial.println("300 Hz"); break;
    case LIS3MDL_DATARATE_560_HZ: Serial.println("560 Hz"); break;
    case LIS3MDL_DATARATE_1000_HZ: Serial.println("1000 Hz"); break;
  }
  
  lis3mdl.setRange(LIS3MDL_RANGE_4_GAUSS);
  Serial.print("Range set to: ");
  switch (lis3mdl.getRange()) {
    case LIS3MDL_RANGE_4_GAUSS: Serial.println("+-4 gauss"); break;
    case LIS3MDL_RANGE_8_GAUSS: Serial.println("+-8 gauss"); break;
    case LIS3MDL_RANGE_12_GAUSS: Serial.println("+-12 gauss"); break;
    case LIS3MDL_RANGE_16_GAUSS: Serial.println("+-16 gauss"); break;
  }

  lis3mdl.setIntThreshold(500);
  lis3mdl.configInterrupt(false, false, true, // enable z axis
                          true, // polarity
                          false, // don't latch
                          true); // enabled!
}

void loop() {
  if (Serial.available()) {
    byte nextMag = Serial.read() - ((byte) '0');
    if (nextMag < 8) {
      Serial.print("Output mag is ");
      Serial.println(nextMag);
      currentMag = nextMag;
      printAll = false;
      plotData = false;
    } else if (nextMag == 9) {
      printAll = true;  
      plotData = false;
    } else if (nextMag == 8) {
      plotData = true;  
      printAll = false;
    } else {
      Serial.println("Input error");
    }
  }
  if (!plotData) {
    if (!printAll) {
      I2CMux.closeAll();
      I2CMux.openChannel(currentMag);
    
      runCalibration(&lis3mdl);
    } else {
      outputAll();
    }
  } else {
    I2CMux.closeAll();
    I2CMux.openChannel(5);
    sensors_event_t event; 
    lis3mdl.getEvent(&event);
    double magnitude = getScaledMag(event.magnetic.x, event.magnetic.y, event.magnetic.z, mag_off_5, mag_map_5);
    // double magnitude = (event.magnetic.x * event.magnetic.x) + (event.magnetic.y * event.magnetic.y) + (event.magnetic.z * event.magnetic.z);
    Serial.println(magnitude);
  }
  // Serial.print("Using channel ");
  // Serial.println(currentMag);
  // lis3mdl.read();      // get X Y and Z data at once
  // Then print out the raw data
  // Serial.print("\nX:  "); Serial.print(lis3mdl.x); 
  // Serial.print("  \tY:  "); Serial.print(lis3mdl.y); 
  // Serial.print("  \tZ:  "); Serial.println(lis3mdl.z); 

  /* Or....get a new sensor event, normalized to uTesla */
  // sensors_event_t event; 
  // lis3mdl.getEvent(&event);
  /* Display the results (magnetic field is measured in uTesla) */
  // Serial.print("\tX: "); Serial.print(event.magnetic.x);
  // Serial.print(" \tY: "); Serial.print(event.magnetic.y); 
  // Serial.print(" \tZ: "); Serial.print(event.magnetic.z); 
  // Serial.println(" uTesla ");
  // testMe();

  // delay(100); 
  // Serial.println();
  
  // Incremene the measurement counter
  measurementCounter++;
  // if (currentMag < 7) {
  //   currentMag++;
  // } else {
  //   currentMag = 5;
  // }

  long currentTime = millis();
  if (currentTime > lastPrintTime + 1000) {
    // Serial.print("Made ");
    // Serial.print(measurementCounter);
    // Serial.print(" measurements in 1 second\n");
    lastPrintTime = currentTime;
    measurementCounter = 0;
  }
}

void outputAll() {
  double data[12];
  for (int mag_bus = 4; mag_bus < 8; mag_bus++) {
    I2CMux.closeAll();
    I2CMux.openChannel(mag_bus);
    sensors_event_t event; 
    lis3mdl.getEvent(&event);
    data[3 * (mag_bus - 4)] = event.magnetic.x;
    data[(3 * (mag_bus - 4)) + 1] = event.magnetic.y;
    data[(3 * (mag_bus - 4)) + 2] = event.magnetic.z;
  }

  for (int mag_bus = 4; mag_bus < 8; mag_bus++) {
    Serial.print(mag_bus); Serial.print(", ");
    Serial.print(data[3 * (mag_bus - 4)]); Serial.print(",");
    Serial.print(data[(3 * (mag_bus - 4)) + 1]); Serial.print(",");
    Serial.print(data[(3 * (mag_bus - 4)) + 2]); Serial.print(" : ");
  }
  Serial.println("");
}
