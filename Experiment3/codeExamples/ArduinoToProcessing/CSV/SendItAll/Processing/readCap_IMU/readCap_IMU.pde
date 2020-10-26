/*
DIGF 6037 Creation & Computation
   Kate Hartman & Nick Puckett

   Arduino to Processing - sending both IMU data & Capacitive

 Based on based on this Lab on the ITP Physical Computing site: 
 https://itp.nyu.edu/physcomp/labs/labs-serial-communication/two-way-duplex-serial-communication-using-an-arduino/
 */

import processing.serial.*; // import the Processing serial library
Serial myPort;              // The serial port

int totalPins =12;
int pinValues[] = new int[totalPins];
int pinValuesPrev[] = new int[totalPins];

float pitch;
float roll;

int margin = 50;

void setup() {
  size(800,400);
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
  stroke(100);
  strokeWeight(1.5);
  
  ///draw the ellipses for the cap pins
  for(int i = 0;i<pinValues.length;i++)
  {
     if(pinValues[i]==1)
     {
      fill(0);       
     } 
     else
     {
      noFill(); 
     }
     
   ellipse(margin+(i*(width-(margin*2))/pinValues.length),height/2,30,30);
   
    ///this triggers only once at the first frame of the touch
   //similar to onMousePress
   if((pinValues[i]==1)&&(pinValuesPrev[i]==0))
   {
     fill(255,0,0);
     ellipse(margin+(i*(width-(margin*2))/pinValues.length),height/2,300,300);
   }
   
   //this triggers once onlyat that first frame of release
   //similar to onMouseRelease
   if((pinValues[i]==0)&&(pinValuesPrev[i]==1))
   {
     fill(0,255,0);
     rectMode(CENTER);
     square(margin+(i*(width-(margin*2))/pinValues.length),height/2,300);
   }
   
   
  //save the previous value   
  pinValuesPrev[i] = pinValues[i];    
     
  }
  
  ///deal with the pitch/roll data
  //pitch
  pushMatrix();
  translate((width/2)-200,100);
  strokeWeight(2.5);
  stroke(0,0,255);
  noFill();
  ellipse(0,0,100,100);
  fill(0);
  text("pitch : "+pitch,0,20);
  rotate(radians(pitch));
  line(0,0,0,-50);
  textAlign(CENTER,CENTER);
  popMatrix();
 
   //roll
  pushMatrix();
  translate((width/2)+200,100);
  strokeWeight(2.5);
  stroke(0,255,255);
  noFill();
  ellipse(0,0,100,100);
  fill(0);
  text("roll : "+roll,0,20);
  rotate(radians(roll));
  line(0,0,0,-50);
  textAlign(CENTER,CENTER);
  popMatrix();
 
 
 
}





void serialEvent(Serial myPort) {
  // read the serial buffer:
  String myString = myPort.readStringUntil('\n');
  if (myString != null) {
    // println(myString);
    myString = trim(myString);
    
    //store the message as a string array
    String tempData[] = split(myString,',');
    
    //uncomment to see what the data looks like in the string array
    //printArray(tempData);
    
    ///read the first 12 items of the array into the pinValues array and convert
    for(int i=0;i<totalPins;i++)
    {
     pinValues[i]=int(tempData[i]); 
    }
    
    //convert the last 2 items in the String array to floats
    pitch = float(tempData[totalPins]);
    roll = float(tempData[totalPins+1]);

 
    
  }
  
}
