/**
 * Temperature Controller
 * 
 * Michael Marner
 */


#include <Servo.h>

Servo peltier;

char data[64];
char firstByte;
int bytesRead = 0;

int goal = 23;
float temperature;
int power;

int ledPin = 11;
int sensorPin = 0;
int servoPin = 9;
int temp = 0;

void setup() 
{
  //pinMode(ledPin, OUTPUT);  
  Serial.begin(9600);
  while (!Serial) {}
  
  peltier.attach(servoPin);
}


void loop() {
  temperature = getVoltage(sensorPin);
  temperature = (temperature - 0.5) * 100;
  delay(10);
  if (Serial.available() > 0)
  {
    firstByte = Serial.read();

    if (firstByte == 'G') {
      bytesRead = Serial.readBytesUntil('\n', data, 63);
      
      temp = temperature;
      while (temp < 100) {
        Serial.print(" ");
        temp *= 10;
      }
      
      Serial.print(temperature, 1);
      Serial.write("\n");
    }
    else if (firstByte == 'S') {
      bytesRead = Serial.readBytesUntil('\n', data, 63);
      data[bytesRead] = 0;
      if (bytesRead > 0) {
        goal = atoi(data);
        //analogWrite(ledPin, goal);
      }
    }
  }
  dealWithPeltier();
}


void dealWithPeltier() {
  // difference between goal and current temp...
  //float diff =  goal - temperature;
  
  // increase if goal is greater than current temperature
  if (temperature < goal) {
    //power -= 1;
    power = 180;
    peltier.write(power);
    delay(10);
  }
  // otherwise decrease
  else if (temperature > goal) {
    //power += 1;
    power = 0;
    peltier.write(power);
    delay(10);
  }    
}

float getVoltage(int pin) {
  return (analogRead(pin) * 0.004882814);
}
  
  
  
