/*
DIGF 6037 Creation & Computation
   Kate Hartman & Nick Puckett

   Arduino to Processing - sending 1 value from IMU step data
   also reads analog value as multiplier for the step number

 Based on based on this Lab on the ITP Physical Computing site: 
 https://itp.nyu.edu/physcomp/labs/labs-serial-communication/two-way-duplex-serial-communication-using-an-arduino/
 */

import processing.serial.*; // import the Processing serial library
Serial myPort;              // The serial port

int steps;
int pot;
float stepFactor;

void setup() {
  size(800,100);
  // List all the available serial ports in the console
  printArray(Serial.list());

  // Change the 0 to the appropriate number of the serial port
  // that your microcontroller is attached to.
  String portName = Serial.list()[0];
  myPort = new Serial(this, portName, 9600);
  // read incoming bytes to a buffer
  // until you get a linefeed (ASCII 10):
  myPort.bufferUntil('\n');
}


void draw() 
{
  background(255);
  fill(255,0,0);
  
  ellipse(steps*stepFactor,height/2, 50, 50);
  
  

 
}


void serialEvent(Serial myPort) {
  // read the serial buffer:
  String myString = myPort.readStringUntil('\n');
  if (myString != null) {
    // println(myString);
    myString = trim(myString);
    String tempData[] = split(myString,',');
    //printArray(tempData);
    steps = int(tempData[0]);
    pot = int(tempData[1]);
    println(steps +" "+pot);
    stepFactor = map(pot,0,1023,1.0,5.0);
    println(stepFactor);
    //println("Steps : " + steps);
    
  }
}
