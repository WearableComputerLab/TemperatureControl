/**
 * Temperature Controller
 * 
 * Michael Marner
 */

#define TEMP_BUFFER_SIZE 50

#include <Servo.h>

/**
 * What mode are we operating in? By default, we do nothing!
 */
enum {
  WAITING,
  GOAL,
  POWER,
  DEMO
} mode = WAITING;


/*
 * Peltier current controller
 */
int sensorPin = 0;
int servoPin = 9;
Servo peltier;

char data[64];
char firstByte;
int bytesRead = 0;

float goal;

int power;

float tempReadings[TEMP_BUFFER_SIZE];
int nextIndex = 0;
int numReadings = 0;
float smoothedTemperature = 0;


void setup()
{
  Serial.begin(9600);
  //while (!Serial) {}
  peltier.attach(servoPin);
}


void loop() {
  updateTemperature();
  doIO();
  
  switch (mode)
  {
    case WAITING:
      power = 90;
      goForPower();
      break;
    case GOAL:
      attackGoalTemperature();
      break;
    case POWER:
      goForPower();
      break;
    case DEMO:
      break;
  }
}


void doIO()
{
  if (Serial.available() > 0)
  {
    firstByte = Serial.read();

    if (firstByte == 'G') {
      bytesRead = Serial.readBytesUntil('\n', data, 63);
      
      Serial.print(smoothedTemperature, 2);
      Serial.write("\n");
    }
    else if (firstByte == 'S') {
      bytesRead = Serial.readBytesUntil('\n', data, 63);
      data[bytesRead] = 0;
      if (bytesRead > 0) {
        goal = atoi(data);
        mode = GOAL;
        //analogWrite(ledPin, goal);
      }
    }
  }
}

void goForPower() {
  peltier.write(power);
  delay(10);
}

void updateTemperature() {
  tempReadings[nextIndex] = getVoltage(sensorPin);
  tempReadings[nextIndex] = (tempReadings[nextIndex] - 0.5) * 100;
  
  if (numReadings < TEMP_BUFFER_SIZE)
    numReadings++;
    
  nextIndex++;
  if (nextIndex == TEMP_BUFFER_SIZE)
    nextIndex = 0;
    
 smoothedTemperature = getSmoothedTemperature();
}


float getSmoothedTemperature() {
  if (numReadings == 0) {
    updateTemperature();
    return getSmoothedTemperature();
  }
  
  float temp;
  for (int i=0; i < numReadings; i++) {
    temp += tempReadings[i];
  }
  return temp / numReadings;
}
    




void attackGoalTemperature() {
  // difference between goal and current temp...
  //float diff =  goal - temperature;
  
  // increase if goal is greater than current temperature
  if (smoothedTemperature < goal) {
    //power -= 1;
    power = 180;
    peltier.write(power);
    delay(10);
  }
  // otherwise decrease
  else if (smoothedTemperature > goal) {
    //power += 1;
    power = 0;
    peltier.write(power);
    delay(10);
  }    
}

float getVoltage(int pin) {
  return (analogRead(pin) * 0.004882814);
}
  
  
  
