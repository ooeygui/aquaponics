// Created by ooeygui
// Sources located at http://github.com/ooeygui/aquaponics

#include <AccelStepper.h>
#include <Stepper.h>

#define _countof(x) (sizeof(x) / sizeof((x)[0]))

const int kStepsPerRevolution = 200;
const int kStepperDirectionPin = 11;
const int kStepperStepPin = 10;
const int kIndexSwitch = 2;
const int kStepperEnable = 8;

const int kSensors[] = {15, 16, 17, 18};

int currentSensor = _countof(kSensors); // means unknown


int motorSpeed = 3000; //maximum steps per second (about 3rps / at 16 microsteps)

AccelStepper diverterStepper(AccelStepper::DRIVER, kStepperStepPin, kStepperDirectionPin);            

// The Index wheel of the diverter has a stop for each output. The switch is configured to expand (
bool isAtStop()
{
  return digitalRead(kIndexSwitch) == 0;
}

bool isOkToMove()
{
  return digitalRead(kIndexSwitch) == 1;
}

bool isCurrentFloatSensorsTriggered()
{
  if (currentSensor == _countof(kSensors))
  {
    // This means it is unknown. See if any are triggered, then set that as current
    
    for (int = 0; i < _countof(kSensors); i++)
    {
      if (digitalRead(kSensors[i]) == 0)
      {
        currentSensor = i;
        return true;
      }
    }
    
    return false;
  }
  
  return digitalRead(kSensors[currentSensor]) == 0;
}

void monitorNextSensor()
{
  currentSensor++;
  if (currentSensor >= _countof(kSensors))
  {
    currentSensor = 0;
  }
}

void stepperEnable(bool enable);
{
   // Wake up the driver
   digitalWrite(kStepperEnable, enable?HIGH:LOW);
}

void moveToNextStop()
{
   stepperEnable(true);
   
   // Move until the switch is out of the wheel valley
   while (isAtStop())
   {
     diverterStepper.runSpeed();
   }

   Serial.println("Moving to next stop");
   // Move to next stop
   while (isOkToMove())
   {
     diverterStepper.runSpeed();
   }
   
   stepperEnable(false);
}


void setup() 
{
  Serial.begin(9600);
  Serial.println("> Ready");

  pinMode(kIndexSwitch, INPUT_PULLUP);
  pinMode(kStepperEnable, OUTPUT);
  pinMode(kFloodSensorA, INPUT_PULLUP);
  
  // Put the driver to sleep
  digitalWrite(kStepperEnable, HIGH);
   
  diverterStepper.setMaxSpeed(motorSpeed);
  diverterStepper.setSpeed(motorSpeed);
}

void loop() 
{
 if (isCurrentFloatSensorsTriggered())
 {
   Serial.println("Sensor triggered");
   moveToNextStop();
   monitorNextSensor();
 }
}

