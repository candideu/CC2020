/**
Experiment 5: Trading Temperatures.

2 channel setup
-publish on one channel
-read on a different channel

Inputs to update
-Name of your WIFI network
-Password of you WIFI network
-publish key
-subscribe key
-your ID name
-name of your data channel to publish on
-name of your partner's channel to read from

**/

#include <WiFiNINA.h>
#define PubNub_BASE_CLIENT WiFiClient
#include <PubNub.h>
#include <ArduinoJson.h>
#include <SparkFunLSM6DS3.h>
#include "Wire.h"

//**Details of your local Wifi Network

//Name of your access point
char ssid[] = "**Name of your Wifi**";
//Wifi password
char pass[] = "**Wifi Password**";

int status = WL_IDLE_STATUS;       // the Wifi radio's status

// pubnub keys
extern char pubkey[] = "pub-c-f4f689cd-7936-4cf0-9cd1-46f8070a6e79";
extern char subkey[] = "sub-c-5691f306-e64b-11ea-89a6-b2966c0cfe96";

// channel and ID data

const char* myID = "Kate"; // place your name here, this will be put into your "sender" value for an outgoing messsage

char publishChannel[] = "kateData"; // channel to publish YOUR data
char readChannel[] = "nickData"; // channel to read THEIR data

// JSON variables

StaticJsonDocument<200> dataToSend; // The JSON from the outgoing message
StaticJsonDocument<200> inMessage; // JSON object for receiving the incoming values

// incoming JSON variables
const char* sender; // variable for the JSON value "sender" on an incoming message
int incomingValue; // variable for the incoming temperature

int publishRate = 500; //how often to publish the data to PN
unsigned long lastPublish; //time of last publish


int readRate = 500; //how often to read data from PN
unsigned long lastRead; //time of last read


//create variable for the IMU
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
//read temperature from IMU  
currentTemp = myIMU.readTempC();

    if((millis()-lastPublish)>=publishRate)
    {
      Serial.print(myID);
      Serial.print("'s temperature");
      Serial.println(currentTemp);
      sendMessage(publishChannel); // publish this value to PubNub
      lastPublish=millis();
    }

    if((millis()-lastRead)>=readRate)
    {
      readFromPubNub(publishChannel);
      lastRead=millis();
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




void sendMessage(char channel[]) 
{
char msg[64]; // variable for the JSON to be serialized into for your outgoing message
  
  // assemble the JSON to publish
  dataToSend["publisher"] = myID; // first key value is sender: yourName
  dataToSend["temperature"] = currentTemp; // second key value is the potiometer value: analogValue

  serializeJson(dataToSend, msg); // serialize JSON to send - sending is the JSON object, and it is serializing it to the char msg
  
  WiFiClient* client = PubNub.publish(channel, msg); // publish the variable char 
  if (!client) 
  {
    Serial.println("publishing error"); // if there is an error print it out 
  }

}

void readFromPubNub(char channel[]) {

  String val; // value of incoming message
  
  PubSubClient* sclient = PubNub.subscribe(channel); // subscribe to "their" channel

  SubscribeCracker pnRead(sclient); // this "SubscribeCracker" object allows for light storage and less lag on the subscribe 
  while (!pnRead.finished()) {
    pnRead.get(val); // get the incoming message
    if (val.length() > 0) { // if there is a message... 
      printMessage(val); // pass it onto the printMessage function to deserialize it to use in the code
    }
  }

  sclient->stop();
}

void printMessage(String v) 
{
  deserializeJson(inMessage, v); // parse the  JSON value received

  sender = inMessage["publisher"]; // this is will be "their name"

  incomingValue = inMessage["temperature"]; // the value of their LED brightness

  // print out incoming message results
  Serial.println("**New Message**");
  Serial.print("publisher: ");
  Serial.println(sender);
  Serial.print("temperature: ");
  Serial.println(incomingValue);

}
