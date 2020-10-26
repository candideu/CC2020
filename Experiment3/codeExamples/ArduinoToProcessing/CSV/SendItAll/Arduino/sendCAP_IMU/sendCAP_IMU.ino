/* DIGF 6037 Creation & Computation
 * Kate Hartman & Nick Puckett
 * 
 * 
 * This example gets stable Pitch and Roll angles from the internal IMU
 * on the Arduino Nano33 IOT using a complimentary filter
 * 
 * Interface via the Sparkfun LSM6DS3 library: https://github.com/sparkfun/SparkFun_LSM6DS3_Arduino_Library
 * Filter Code by Trent Cleghorm: https://github.com/tcleg/Six_Axis_Complementary_Filter
 * 
 * Reads all 12 pins and writes a 1 or 0 separated by commas  
 * 
 * Combines both data streams into a single CSV message
 * 
 */
#include <Wire.h>
#include "Adafruit_MPR121.h"
#include "SparkFunLSM6DS3.h"
#include "six_axis_comp_filter.h"


//////////////////////////capacitive input
#ifndef _BV
#define _BV(bit) (1 << (bit)) 
#endif

Adafruit_MPR121 cap = Adafruit_MPR121();

// Keeps track of the last pins touched
uint16_t currtouched = 0;

int tp = 12; //change this if you aren't using them all, start at pin 0
//////////////////////////capacitive input

//////////////////////////IMU input
LSM6DS3 nano33IMU(I2C_MODE, 0x6A); //define the IMU object
CompSixAxis CompFilter(0.1, 2); //define the filter object

float pitch;
float roll;

//////////////////////////IMU input


void setup() 
{
  Serial.begin(9600);

  if (!cap.begin(0x5A)) {
    Serial.println("MPR121 not found, check wiring?");
    while (1);
  }
  Serial.println("MPR121 found!");

  //Call .begin() to configure the IMU (Inertial Measurement Unit)
  nano33IMU.begin();

  
}

void loop() 
{
//run the function to check the cap interface
checkAllPins(tp);
//run the function to check the IMU
calculatePitchAndRoll();
//make a new line to separate the message
 Serial.println();
   // put a delay so it isn't overwhelming
 delay(100);
}

void checkAllPins(int totalPins)
{
  // Get the currently touched pads
  currtouched = cap.touched();
  
  for (uint8_t i=0; i<totalPins; i++) 
  {
    // it if *is* touched set 1 if no set 0
    if ((currtouched & _BV(i)))
    {
      Serial.print(1); 
    }
    else
    {
      Serial.print(0);
    }

    Serial.print(",");
  
}

  
}

void calculatePitchAndRoll()
{
  float accelX, accelY, accelZ, // variables to store sensor values
      gyroX, gyroY, gyroZ,
      xAngle, yAngle;       

  //  Get all motion sensor (in this case LSM6DS3) parameters,
  //  If you're using a different sensor you'll have to replace the values
  accelX = nano33IMU.readFloatAccelX();
  accelY = nano33IMU.readFloatAccelY();
  accelZ = nano33IMU.readFloatAccelZ();

  gyroX = nano33IMU.readFloatGyroX();
  gyroY = nano33IMU.readFloatGyroY();
  gyroZ = nano33IMU.readFloatGyroZ();

  // Convert these values into angles using the Complementary Filter
  CompFilter.CompAccelUpdate(accelX, accelY, accelZ); // takes arguments in m/s^2
  CompFilter.CompGyroUpdate(gyroX, gyroY, gyroZ); // takes arguments un rad/s 
  CompFilter.CompUpdate();
  CompFilter.CompStart();

  // Get angle relative to X and Y axes and write them to the variables in the arguments
  //in radians
  CompFilter.CompAnglesGet(&xAngle, &yAngle);

  //convert from radians to angles
  pitch = xAngle*RAD_TO_DEG;
  roll = yAngle*RAD_TO_DEG;

  
  Serial.print(pitch);
  Serial.print(",");
  Serial.print(roll);


}
