/**
Experiment 5: Trading Temperatures.

****Storage and Playback must be enabled on your keys*****

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
#include "Wire.h"
#include "Adafruit_MPR121.h"

//mpr stuff

#ifndef _BV
#define _BV(bit) (1 << (bit)) 
#endif

Adafruit_MPR121 cap = Adafruit_MPR121();

// Keeps track of the last pins touched
uint16_t currtouched = 0;
uint16_t lasttouched = 0;
int tp = 12; //change this if you aren't using them all, start at pin 0


//**Details of your local Wifi Network

//Name of your access point
char ssid[] = "";
//Wifi password
char pass[] = "";

int status = WL_IDLE_STATUS;       // the Wifi radio's status

// pubnub keys
char pubkey[] = "";
char subkey[] = "";

// channel and ID data

const char* myID = "Kate"; // place your name here, this will be put into your "sender" value for an outgoing messsage

char publishChannel[] = "kateData"; // channel to publish YOUR data
char readChannel[] = "nickData"; // channel to read THEIR data

// JSON variables
StaticJsonDocument<200> dataToSend; // The JSON from the outgoing message
StaticJsonDocument<200> inMessage; // JSON object for receiving the incoming values
//create the names of the parameters you will use in your message
String JsonParamName1 = "publisher";
String JsonParamName2 = "pinTouched";



int serverCheckRate = 1000; //how often to read data on PN
unsigned long lastCheck; //time of last publish


const char* inMessagePublisher; 
int nickPinTouched;///the value Nick reads from Kates board via PN
int katePinTouched;   //the value I get locally




void setup() {
  
  Serial.begin(9600);

  //run this function to connect
  connectToPubNub();
  
  if (!cap.begin(0x5A)) {
    Serial.println("MPR121 not found, check wiring?");
    while (1);
  }
  Serial.println("MPR121 found!");
}


void loop() 
{
//read the capacitive sensor
checkAllPins(12);

//send and receive messages with PubNub, based on a timer
sendReceiveMessages(serverCheckRate);

///Do whatever you want with the data here!

   
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

    // wait 2 seconds for connection:
    delay(2000);
  }

  // once you are connected :
  Serial.println();
  Serial.print("You're connected to ");
  Serial.println(ssid);
  
  PubNub.begin(pubkey, subkey);
  Serial.println("Connected to PubNub Server");

  
}

void sendReceiveMessages(int pollingRate)
{
    //connect, publish new messages, and check for incoming
    if((millis()-lastCheck)>=pollingRate)
    {
      //check for new incoming messages
      readMessage(readChannel);
      
      //save the time so timer works
      lastCheck=millis();
    }


  
}



void sendMessage(char channel[]) 
{

      Serial.print("Sending Message to ");
      Serial.print(channel);
      Serial.println(" Channel");
  
  char msg[64]; // variable for the JSON to be serialized into for your outgoing message
  
  // assemble the JSON to publish
  dataToSend[JsonParamName1] = myID; // first key value is sender: yourName
  dataToSend[JsonParamName2] = nickPinTouched; // second key value is the Pin number

  serializeJson(dataToSend, msg); // serialize JSON to send - sending is the JSON object, and it is serializing it to the char msg
  Serial.println(msg);
  
  WiFiClient* client = PubNub.publish(channel, msg); // publish the variable char 
  if (!client) 
  {
    Serial.println("publishing error"); // if there is an error print it out 
  }
  else
  {
  Serial.print("   ***SUCCESS"); 
  }

}
void checkAllPins(int totalPins)
{
  // Get the currently touched pads
  currtouched = cap.touched();
  for (uint8_t i=0; i<totalPins; i++) 
  {
    // it if *is* touched and *wasnt* touched before, send a message
    if ((currtouched & _BV(i)) && !(lasttouched & _BV(i)) )
    {
      katePinTouched = i;
      sendMessage(publishChannel);
      
    }

  
  }
lasttouched = currtouched;
  
}


void readMessage(char channel[]) 
{
  String msg;
    auto inputClient = PubNub.history(channel,1);
    if (!inputClient) 
    {
        Serial.println("message error");
        delay(1000);
        return;
    }
    HistoryCracker getMessage(inputClient);
    while (!getMessage.finished()) 
    {
        getMessage.get(msg);
        //basic error check to make sure the message has content
        if (msg.length() > 0) 
        {
          Serial.print("**Received Message on ");
          Serial.print(channel);
          Serial.println(" Channel");
          Serial.println(msg);
          //parse the incoming text into the JSON object

          deserializeJson(inMessage, msg); // parse the  JSON value received

           //read the values from the message and store them in local variables 
           inMessagePublisher = inMessage[JsonParamName1]; // this is will be "their name"
           nickPinTouched = inMessage[JsonParamName2]; // the value of their Temperature sensor

        }
    }
    inputClient->stop();
  

}
