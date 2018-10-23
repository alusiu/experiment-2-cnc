/*
   Creation & Computation - Digital Futures, OCAD University
   Kate Hartman / Nick Puckett

   Orientation Sensor: https://github.com/adafruit/Adafruit_Sensor
   Simple Read and print of the X Y Z angles of orientation in degrees
   For Arduino Micro
   O Sensor       Arduino
   SDA*            D2
   SCL*            D3

 * *You must connect a 4.7K resistor between the SDA connection on the sensor and +5V
 * *You must connect a 4.7K resistor between the SCL connection on the sensor and +5V

*/
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

Adafruit_BNO055 orientationSensor = Adafruit_BNO055();  //create a orienation sensor object

int C2 = 1500;
int C3 = 131;
int buzzerPin = 9;
//Volume vol;

unsigned long lastRead;   //used for the sampleRate timer
int sampleRate = 100;     //the sampleRate for reading the sensor.  Without this it will crash.


float xOrientation;     //holds the X orientation    Degrees
float yOrientation;     //holds the Y orientation    Degrees
float zOrientation;      //holds the Z orientation   Degrees

float lastX;
float lastY;
float lastZ;
float velocityX;
float velocityY;
float velocityZ;
float averageVelocity = 0;
float lastVelocity;


void setup()
{
  Serial.begin(9600);

  pinMode (buzzerPin, OUTPUT);
  if (!orientationSensor.begin()) //connect to the sensor and print an error if there is a problem
  {

    Serial.println("Can't connect to the Sensor. Check the Wiring");
    while (1);
  }

  delay(1000);  ///wait for one second for everything to start up.
  //  vol.begin();
  orientationSensor.setExtCrystalUse(true);
}

void loop()
{

  if (millis() - lastRead >= sampleRate)
  {

    sensors_event_t event; //create an event variable
    orientationSensor.getEvent(&event); //pass it to the BNO055 object

    //get the values
    xOrientation = event.orientation.x;
    yOrientation = event.orientation.y;
    zOrientation = event.orientation.z;
 
    // set the orientation to a positive number for the purpose of math;
    if (xOrientation < 0) {
      xOrientation = abs(xOrientation);
    }
    if (yOrientation < 0) {
      yOrientation = abs(yOrientation);
    }
    if (zOrientation < 0) {
      zOrientation = abs(zOrientation);
    }

    // find the difference between the last angle read, and the current angle read.
    velocityX = lastX - xOrientation;
    velocityY = lastY - yOrientation;
    velocityZ = lastZ - zOrientation;

    // find velocity of each angle, and if it is a negative set it to the positive value;
    if (velocityX < 0) {
      velocityX = abs(velocityX);
    }

    if (velocityY < 0) {
      velocityY = abs(velocityY);
    }

    if (velocityZ < 0) {
      velocityZ = abs(velocityZ);
    }

   averageVelocity = ((velocityX + velocityY + velocityZ) / 3);
                      Serial.println(averageVelocity);

    Serial.println(averageVelocity);
    if (averageVelocity < lastVelocity) {
      averageVelocity = abs((10-averageVelocity));
      int thisPitch = map(averageVelocity, 2, 9, 0, 1500);
      tone(buzzerPin, thisPitch, 150);
    } else {
      tone(buzzerPin, 0, 100);
    }

    Serial.print(" vX ");
    Serial.print(velocityX);
    Serial.print(" vY ");
    Serial.print(velocityY);
    Serial.print(" vZ ");
    Serial.print(velocityZ);
    Serial.println('v: ');

    lastRead = millis();
    lastVelocity = averageVelocity;
    lastX = xOrientation;
    lastY = yOrientation;
    lastZ = zOrientation;
    //save the value of the current time so the timer works
  }
}
