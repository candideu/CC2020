

/**
Experiment 5: Physical to Webpage networking example.

The Arduino Publishes the value of a potentionmeter to a webpage. 
**/

#include <WiFiNINA.h>
#define PubNub_BASE_CLIENT WiFiClient
#include <PubNub.h>
#include <ArduinoJson.h>
#include <SparkFunLSM6DS3.h>
#include "Wire.h"

//**Details of your local Wifi Network

//Name of your access point
extern char ssid[] = "SSID Name";
//Wifi password
extern char pass[] = "Wifi Password";

int status = WL_IDLE_STATUS;       // the Wifi radio's status

// pubnub keys
extern char pubkey[] = "YOUR PUB KEY";
extern char subkey[] = "YOUR SUB KEY";







// outgoing JSON variables

const char* myID = "Name"; // place your name here, this will be put into your "sender" value for an outgoing messsage

const static char publishChannel[] = "player1data"; // channel to publish YOUR data
const static char readChannel[] = "player2data"; // channel to read THEIR data

StaticJsonDocument<200> dataToSend; // The JSON from the outgoing message



int publishRate = 500; //how often to publish the data to PN
unsigned long lastPublish; //time of last publish


LSM6DS3 myIMU(I2C_MODE, 0x6A); //Default constructor is I2C, addr 0x6B
int currentTemp;





void setup() {
  
  Serial.begin(9600);

  //run this function to connect
  connectToPubNub();
  
  myIMU.begin();
}


void loop() 
{
currentTemp = myIMU.readTempC();

    if((millis()-lastPublish)>=publishRate)
    {
      Serial.println(currentTemp);
      sendMessage(); // publish this value to PubNub
      lastPublish=millis();
    }
}

void connectToPubNub()
{
    // attempt to connect to Wifi network:
  while ( status != WL_CONNECTED) 
  {
    Serial.print("Attempting to connect to the network, SSID: ");
    Serial.println(ssid);
    status = WiFi.begin(ssid, pass);
    Serial.print("*");

    // wait 10 seconds for connection:
    delay(2000);
  }

  // once you are connected :
  Serial.println();
  Serial.print("You're connected to ");
  Serial.println(ssid);
  
  PubNub.begin(pubkey, subkey);
  Serial.println("Connected to PubNub Server");

  
}




void sendMessage() 
{
char msg[64]; // variable for the JSON to be serialized into for your outgoing message
  
  // assemble the JSON to publish
  dataToSend["publisher"] = myID; // first key value is sender: yourName
  dataToSend["temperature"] = currentTemp; // second key value is the potiometer value: analogValue

  serializeJson(dataToSend, msg); // serialize JSON to send - sending is the JSON object, and it is serializing it to the char msg
  
  WiFiClient* client = PubNub.publish(publishChannel, msg); // publish the variable char 
  if (!client) 
  {
    Serial.println("publishing error"); // if there is an error print it out 
  }

}
