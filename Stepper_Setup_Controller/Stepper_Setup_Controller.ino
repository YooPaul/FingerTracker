#include <AFMotor.h>
 
AF_Stepper xMotor(200, 1);
AF_Stepper yMotor(200, 2);

boolean runAuto = false;
String serialInput;

boolean runningMoveToOriginCommand = false;
boolean runningMoveToCenterCommand = false;

uint8_t xStopperDigitalPinIn = 30;
uint8_t xStopperDigitalPinOut = 31;

uint8_t yStopperDigitalPinIn = 40;
uint8_t yStopperDigitalPinOut = 41;

int32_t xStepsToCenter = 1910;
int32_t yStepsToCenter = 1040;

//To use this sketch make sure you have the AdaFruit AFMotor library installed, otherwise it won't run
void setup() {
  Serial.begin(115200);
  Serial.println("Stepper Motor Initialization");

  //Inputs an int from 1 to 255, which is relative to the maximum speed the stepper can operate at. Input 80 gives us about 1/3 power
  xMotor.setSpeed(80);
  yMotor.setSpeed(80);

  // Pin Set Up
  pinMode(xStopperDigitalPinIn, INPUT);     
  pinMode(yStopperDigitalPinIn, INPUT); 
  pinMode(xStopperDigitalPinOut, OUTPUT);     
  pinMode(yStopperDigitalPinOut, OUTPUT); 

  digitalWrite(xStopperDigitalPinOut, HIGH);
  digitalWrite(yStopperDigitalPinOut, HIGH);

  delay(1000);
}

void loop() {
  if(runningMoveToOriginCommand && runningMoveToCenterCommand) {
    moveToOrigin();
    runningMoveToOriginCommand = false;
    moveToCenter();
    runningMoveToCenterCommand = false;
    Serial.println("Command complete!");
  }
  
  if(runningMoveToOriginCommand) {
    moveToOrigin();
    runningMoveToOriginCommand = false;
    Serial.println("Command complete!");
  }
}

//Take commands from the Serial port
//Don't send commands too fast
void serialEventRun(void) {
  if(Serial.available()){
    serialInput = Serial.readStringUntil(' ');
  }else{
    //Clear the input string
    serialInput = "";
  }

  if(serialInput == "isXTriggerDown") {
    Serial.println( isXStopperTriggered() );
  }
  if(serialInput == "isYTriggerDown") {
    Serial.println( isYStopperTriggered() );
  }
  
  if(serialInput == "moveMotor") {
    serialInput = Serial.readStringUntil(' ');
    AF_Stepper *motorToRun;
    if(serialInput == "x") {
      motorToRun = &xMotor;
    }
    if(serialInput == "y") {
      motorToRun = &yMotor;
    }
    serialInput = Serial.readStringUntil(' ');
    uint16_t manualControlStepsToMove = serialInput.toInt();
    serialInput = Serial.readString();
    
    if(serialInput == "forward") {
      motorToRun->step(manualControlStepsToMove, FORWARD, SINGLE);
    }
    if(serialInput == "backward") {
      motorToRun->step(manualControlStepsToMove, BACKWARD, SINGLE);
    }
    
    motorToRun->release();
    //Give the release some time to properly shutdown
    delay(5);
    
    Serial.println("Command complete!");
  }

  if(serialInput == "moveTo") {
    serialInput = Serial.readStringUntil(' ');
    int32_t xSteps = serialInput.toInt();
    serialInput = Serial.readString();
    int32_t ySteps = serialInput.toInt();

    if(xSteps > 0) {
      xMotor.step(xSteps, FORWARD, SINGLE);
    }
    if(xSteps < 0) {
      xMotor.step(xSteps * -1, BACKWARD, SINGLE);
    }
    xMotor.release();
    delay(1000);
    
    if(ySteps > 0) {
      yMotor.step(ySteps, FORWARD, SINGLE);
    }
    if(ySteps < 0) {
      yMotor.step(ySteps * -1, BACKWARD, SINGLE);
    }
    yMotor.release();
    delay(1000);

    Serial.println("Command complete!");
  }
  
  if(serialInput == "moveToOrigin") {
    runningMoveToOriginCommand = true;
  }
  
  if(serialInput == "moveToCenter") {
    runningMoveToOriginCommand = true;
    runningMoveToCenterCommand = true;
  }
}

void moveToOrigin() {
  //Move the X Stepper to origin
  while(!isXStopperTriggered()) {
    xMotor.step(1, BACKWARD, SINGLE);
    //Maybe not needed
    delay(1);
  }
  xMotor.release();
  delay(500);

  while(!isYStopperTriggered()) {
    yMotor.step(1, BACKWARD, SINGLE);
    //Maybe not needed
    delay(1);
  }
  yMotor.release();
  delay(500);
}

boolean isXStopperTriggered() {
  uint8_t xCalOut = digitalRead(xStopperDigitalPinIn);
  //True if Pin xCalDataIn is set to high
  return xCalOut == 0;
}

boolean isYStopperTriggered() {
  uint8_t yCalOut = digitalRead(yStopperDigitalPinIn);
  //True if Pin xCalDataIn is set to high
  return yCalOut == 0;
}

//TODO: Determine the exact number of steps needed to move to the center
void moveToCenter() {
  xMotor.step(xStepsToCenter, FORWARD, SINGLE);
  xMotor.release();
  delay(1000);
  
  yMotor.step(yStepsToCenter, FORWARD, SINGLE);
  yMotor.release();
  delay(1000);
}

void moveBothMotors(int32_t xMovement, int32_t yMovement, uint8_t stepMode, int32_t stepSize) {
  if(xMovement > 0) {
    xMotor.step(xMovement * stepSize, FORWARD, stepMode);
    xMotor.release();
  }else{
    xMotor.step(-xMovement * stepSize, BACKWARD, stepMode);
    xMotor.release();
  }
  delay(100);
  if(yMovement > 0) {
    yMotor.step(yMovement * stepSize, FORWARD, stepMode);
    yMotor.release();
  }else{
    yMotor.step(-yMovement * stepSize, BACKWARD, stepMode);
    yMotor.release();
  }
  delay(100);
}
