/**
 * Temperature Controller
 * 
 * Michael Marner
 */
 
int inByte = 0;
char data[64];
char firstByte;
int bytesRead = 0;

int goal = 0;

int sensorValue = 0;
int ledPin = 9;
int sensorPin = 0;
int temp = 0;

void setup() 
{
  pinMode(ledPin, OUTPUT);
  
  Serial.begin(9600);
  while (!Serial) {}
}


void loop() {
  sensorValue = analogRead(sensorPin);
  delay(10);
  if (Serial.available() > 0)
  {
    firstByte = Serial.read();

    if (firstByte == 'G') {
      bytesRead = Serial.readBytesUntil('\n', data, 63);
      temp = sensorValue;
      while (temp < 1000) {
        Serial.print(" ");
        temp *= 10;
      }
      Serial.print(sensorValue);
      Serial.write("\n");
    }
    else if (firstByte == 'S') {
      bytesRead = Serial.readBytesUntil('\n', data, 63);
      data[bytesRead] = 0;
      if (bytesRead > 0) {
        goal = atoi(data);
        analogWrite(ledPin, goal);
        //Serial.write("OK\n");
      }
    }
  }
}


  
  
  
