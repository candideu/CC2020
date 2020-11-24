# Experiment 5: Networking 
Things you will need for this experiment:
* Create a PubNub Account
* Download the Arduino WifiNINA library
* Download the Arduino JSON library
* Download the Arduino PubNub Library


## What is PubNub?
PuNub is a cloud-based server that receives and sends "messages" in real time. By using the PubNub API, users can subscribe (receive) and publish (send) data instantly.

 ![PubNub Diagram](README%20Images/pubnub-galaxy.gif)
 *(Diagram taken from [PubNub Data Streans](https://www.pubnub.com/docs/web-javascript/data-streams-publish-and-subscribe)

Each account can have up to 1,000,000 messages a month. 

The API works with both Arduino & JavaScript, making it easy to use an Arduino to talk to a Webpage or vice versa. 

PubNub uses *Apps* Each app has a **Publish key** and **Subscribe key**. To use the app, your project must include both of these keys. 

Each set of keys can have multiple **channels**. 

Users who are connected with the same Publish and Subscribe keys, can publish and subscribe to channels within the app. 

**To send and receive messages to other users you must be on the same channel**

This example shows everyone on the same channel. A user publishes one message, and the rest of the users (including the user who published the message) receives the message: 

## In this Experiment we will focus on CHANNELS as the main way to send/sort Data
Experiment 4 was designed around using a single channel for all messages and sorting them via the **UUID**. In Experiment 5 we will use **Channels** as the main way to sort data.
***WHY?***
**Efficiency**. While it isn't an issue for a computer/browser to sort through massive amounts of messages, we are now asking a microcontroller to do the same task. When working with these devices, you want to minimize the amount of web traffic they need to deal with. To acheive that we split the data from the 2 users into individual channels. That way you are NOT reading your own data from the server, you just handle it locally.  To do this, you will treat the channel name the same as you did the UUID in Experiment 4. You will have a channel that you name that will contain the data that you publish. Your partner(s) will also each have a channel with their name + data.
*You will publish on 1 channel and subscribe to a different channel*
 
![PubNub Diagram - Multi-Channel](README%20Images/pubnub-multi-01.png)
  

## To Get Started
1. [Create a PubNub Account](https://dashboard.pubnub.com/signup) 
2. Create an "App" in PubNub 
 ![Create PubNub App](README%20Images/create-new-app-ss.png)
 
- Give your App a name
 ![Create PubNub App Name](README%20Images/create-new-app-name-ss.png)

3. Find the Subscribe & Publish Keys 
 - These keys are unique to every "App". If others want to be able to subscribe or publish messages to your "App", you must share your keys.
  ![Find PubNub Keys](README%20Images/find-keys.png)
  ![Find PubNub Keys 2](README%20Images/find-keys-2.png)
  ![Find PubNub Keys 3](README%20Images/find-keys-3.png)

4. Turn on Playback and Storage
 
  ![Storage and Playback](README%20Images/storagePlayback.png)  

5. In the sample code, place your Publish Key in the text where it says "Your publish key here", and your Subscribe key where it says "Your subscribe key here"
 - In Arduino
 
  ![PubNub Keys for Arduino](README%20Images/arduino-keys.png)


## Connecting Arduino to Wifi

The following libraries need to be added to successfully connect your Arduino Naon IoT 33 to WiFi.

To add the libraries, open Arduino and go to Tools -> Manage Libraries. 

From there add the following:

- WiFiNINA
- ArduinoJson
- PubNub

Once complete, enter your WiFi crendentials where it says "Your Wifi password/network name here". 

**Note: You may have issues if you are using a network that contains "-5G" at the end. Often home WiFi will broadcast two networks, "network" and "network-5G". If possible, use "network", and not "network-5G"** 

![PubNub Keys for p5](README%20Images/arduino-wifi.png)

Upload the code, and open the Serial Monitor. You should see printed `Attempting to connect to the network, SSID:`. 

Once you are connected, the following will print: `You're connected to *name of your wifi connection*`.

There is a delay in the code to ensure that there is a WiFi connection, so any code written will take a few seconds to execute.

If your PubNub credentials are placed correctly, you will receive the printed message: `Connected to PubNub Server`. 
 
## Blink Blink Fade
*This example uses the onboard temperature sensor on the NANO33 IOT. It displays the local temperature value as a blink rate of an LED attached to Pin 9. It also shows the temperature value of the remote user that you get via PubNub throught the blinking rate of an LED connected to pin 10. It also calculates the average of the 2 temperatures and displays it as a pulsing LED attached to pin 11* 

*This example requires **two different channel names** but uses the **same Publish and Subscribe keys**.* 
![Blink Blink Fade connections](README%20Images/blinkblinkFade.png)

There are 2 files included in this example that show the basic design method for designing these connections

### Input your Wifi Details and PubNub keys

```c++
//**Details of your local Wifi Network

//Name of your access point
char ssid[] = "The name of your WIFI";
//Wifi password
char pass[] = "Your WIFI password";

int status = WL_IDLE_STATUS;       // the Wifi radio's status

// pubnub keys
char pubkey[] = "YOUR PUB KEY";
char subkey[] = "YOUR SUB KEY";
```

### Input your myID data and channel names.
These will be different for each user. Notice how the publish/subscribe channels are flipped

**Nick**
```c++
// channel and ID data

const char* myID = "Nick"; // place your name here, this will be put into your "sender" value for an outgoing messsage

char publishChannel[] = "nickData"; // channel to publish YOUR data
char readChannel[] = "kateData"; // channel to read THEIR data
```

**Kate**
```c++
// channel and ID data

const char* myID = "Kate"; // place your name here, this will be put into your "sender" value for an outgoing messsage

char publishChannel[] = "kateData"; // channel to publish YOUR data
char readChannel[] = "nickData"; // channel to read THEIR data

```

### Define your JSON Objects that will handle sending/receiving message and define the names of the parameters

```c++
// JSON variables
StaticJsonDocument<200> dataToSend; // The JSON from the outgoing message
StaticJsonDocument<200> inMessage; // JSON object for receiving the incoming values
//create the names of the parameters you will use in your message
String JsonParamName1 = "publisher";
String JsonParamName2 = "temperature";
```
This does the same as creating this object in javascript
```json
let dataToSend = {
  publisher:"",
  temperature:0
}
```
### Define the variables for the timer that polls the server
```c++
int serverCheckRate = 1000; //how often to publish/read data on PN
unsigned long lastCheck; //time of last publish
```
### Define the variables that will ulimately hold the information
```c++
//These are the variables that will hold the values we will be using
//some are calculated locally, some come from PubNub messages
int nickTemperature = 0;  
int kateTemperature = 0;  
int avgTemperature;
const char* inMessagePublisher; 
```
### Define the varibles for the Timers, Pins, blink/fade
```c++
///blinking
unsigned long lastNickBlink;
unsigned long lastKateBlink;
unsigned long lastAvgFade;

int nickBlinkPin = 11;
int kateBlinkPin = 10;
int avgPin = 9;

boolean nickBlinkState = false;
boolean kateBlinkState = false;
int avgBrightness;
int fadeIncrement = 10;
```

### Connecting to PubNub
This example uses a function to handle connecting to the Wifi and Pubnub
**connectToPubNub()***
```c++
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
```
### Reading / Sending Messages with a Timer
This example uses a function called **sendReceiveMessages**. It takes a single argument that determines its frequency. Within that timer, it calls the functions that send and receive the messages.
```c++
void sendReceiveMessages(int pollingRate)
{
    //connect, publish new messages, and check for incoming
    if((millis()-lastCheck)>=pollingRate)
    {
      //publish data
      sendMessage(publishChannel); // publish this value to PubNub

      //check for new incoming messages
      readMessage(readChannel);
      
      //save the time so timer works
      lastCheck=millis();
    }


  
}

```


### Sending Function
This function takes a single argument of a channel name. This determines which channel you want to publish to. (If you want to publish to multiple channels, call the function multiple times with different channel arguments). Inside the function you can see where we use the JSON object we previously defined and the parameter names to assign the values to the message.
```c++
void sendMessage(char channel[]) 
{

      Serial.print("Sending Message to ");
      Serial.print(channel);
      Serial.println(" Channel");
  
  char msg[64]; // variable for the JSON to be serialized into for your outgoing message
  
  // assemble the JSON to publish
  dataToSend[JsonParamName1] = myID; // first key value is sender: publisher
  dataToSend[JsonParamName2] = nickTemperature; // second key value is the temperature value

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
```

### Reading Function
This function takes a single argument of a channel name. This determines which channel you want to read from. (If you want to read from multiple channels, call the function multiple times with different channel arguments).  This method actually relies on the **history** functionality in Pubnub, so you need to have Storage/Playback enabled on your keyset. This method allows for both synchonous and asynchronous communication.
```c++
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
           kateTemperature = inMessage[JsonParamName2]; // the value of their Temperature sensor

           //calculate the average of the 2 temperatures
           avgTemperature = (kateTemperature+nickTemperature)/2;

           Serial.print("Current Average KateNick Temp: ");
           Serial.println(avgTemperature);
        }
    }
    inputClient->stop();
  

}
```

### Display Functions
These Functions take both the local and Network data and visualize them with LEDs
```c++
void blinkNick(int inputValue)
{
int minTemp = 0;
int maxTemp = 40;

int minBlink = 1000;
int maxBlink = 100;

int temperatureBlink = map(inputValue,minTemp,maxTemp,minBlink,maxBlink);

  if((millis()-lastNickBlink)>=temperatureBlink)
  {
   nickBlinkState = !nickBlinkState;
   digitalWrite(nickBlinkPin,nickBlinkState);
   lastNickBlink = millis();  
  }
}

void blinkKate(int inputValue)
{
int minTemp = 0;
int maxTemp = 40;

int minBlink = 1000;
int maxBlink = 100;

int temperatureBlink = map(inputValue,minTemp,maxTemp,minBlink,maxBlink);

  if((millis()-lastKateBlink)>=temperatureBlink)
  {
   kateBlinkState = !kateBlinkState;
   digitalWrite(kateBlinkPin,kateBlinkState);
   lastKateBlink = millis();  
  }
}

void fadeAverage(int inputValue)
{
int minTemp = 0;
int maxTemp = 40;

int minBrightVal = 0;     //sets the low point of the fade range
int maxBrightVal = 255;   //sets the high point of the fade range


int fadeRate = map(inputValue,minTemp,maxTemp,1,50);

    if(millis()-lastAvgFade>=fadeRate) //this very simple statement is the timer,
    {                                          //it subtracts the value of the moment in time the last blink happened, and sees if that number is larger than your set blinking value
    analogWrite(avgPin,avgBrightness);
    
    avgBrightness += fadeIncrement;
      if (avgBrightness <= minBrightVal || avgBrightness >= maxBrightVal) 
      {
        fadeIncrement *= -1;
      }
    
      lastAvgFade = millis();            //save the value in time that this switch occured, so we can use it again.
       
     }
}
```

### By organizing the code this way, our loop code is very short
```c++
void loop() 
{
//read temperature from IMU  
nickTemperature = myIMU.readTempC();

//send and receive messages with PubNub, based on a timer
sendReceiveMessages(serverCheckRate);

///Do whatever you want with the data here!
blinkNick(nickTemperature);
blinkKate(kateTemperature);
fadeAverage(avgTemperature);   
}
```








 




 
