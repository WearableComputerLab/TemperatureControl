// Copyright 2012 Michael Marner (michael@20papercups.net) all rights reserved.
// 
// Redistribution and use in source and binary forms, with or without modification, are
// permitted provided that the following conditions are met:
// 
//    1. Redistributions of source code must retain the above copyright notice, this list of
//       conditions and the following disclaimer.
// 
//    2. Redistributions in binary form must reproduce the above copyright notice, this list
//       of conditions and the following disclaimer in the documentation and/or other materials
//       provided with the distribution.
// 
// THIS SOFTWARE IS PROVIDED BY <COPYRIGHT HOLDER> ''AS IS'' AND ANY EXPRESS OR IMPLIED
// WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
// FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> OR
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
// ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
// ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
// 
// The views and conclusions contained in the software and documentation are those of the
// authors and should not be interpreted as representing official policies, either expressed
// or implied, of the authors.

//
// Temperature controller for peltier unit!
//


#include <Servo.h>


/*
 * How many temperature samples will we use to computer the average?
 */
#define TEMP_BUFFER_SIZE 50


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

/**
 * Variables for serial IO
 */
char data[64];
char firstByte;
int bytesRead = 0;

/**
 * Goal Temperature
 */
float goal;

/**
 * Power level of the peltier
 */
int power;

/**
 * The last N temperature readings
 */
float tempReadings[TEMP_BUFFER_SIZE];

/**
 * These two variables are used to implement a circular buffer for the
 * temperature readings.
 */
int nextIndex = 0;
int numReadings = 0;

/**
 * For convenience, we'll store the smoothed (average) temperature here
 */
float smoothedTemperature = 0;

/**
 * Connect to serial for IO, prepare the servo for controlling temperature
 */
void setup()
{
  Serial.begin(9600);
  //while (!Serial) {}
  peltier.attach(servoPin);
}


/**
 * Main Loop
 */
void loop() {
  updateTemperature();
  doIO();
  
  switch (mode)
  {
    case WAITING:
      // 90 = off
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


/**
 * Communicates with the host computer.
 *
 * Valid commands are as vollows:
 *
 * G\n       The host computer is querying for the temperature
 *
 * S[0-9]+\n The host is setting the goal temerature in degrees
 */
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


/**
 * Sets the power level of the peltier to whatever is in the power variable
 */
void goForPower() {
  peltier.write(power);
  delay(10);
}


/**
 * Reads the teperature from the analogue pin, stores it in the circular buffer
 */
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


/**
 * Calculates the average temperature from the stored samples
 */
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
    

/**
 * Sets the power output of the peltier to aim towards the goal temperature.
 *
 * This is pretty basic at the moment.
 * If the temperature is less than the goal, full heat
 * If the temperature is greater than the goal, full cold
 */
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

/**
 * Returns the voltage on the specified analogue pin.
 */
float getVoltage(int pin) {
  return (analogRead(pin) * 0.004882814);
}
  
/**
 * Return the output power relative to the difference between current smoothed temp and the goal temp.
 */
int outputPower() {
  float difference = goal - smoothTemperature;
  
  float diffCalc = difference;
  if(diffCalc < 0)
    diffCalc * -1;
  
  if(difference > 10)
    return 180;
  else if(difference < -10) {
    return 0;
  else {
    return 90 + (90 * (difference / 10));
  }
  
}
  
