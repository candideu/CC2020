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

4. In the sample code, place your Publish Key in the text where it says "Your publish key here", and your Subscribe key where it says "Your subscribe key here"
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
 
## Physical to Physical 
*This example publishes the value of your potentiometer to a channel. The potentiometer controls the brightness of one LED on your circuit. On another channel, you are subscribed to receive the value of another person's potentiometer. This value controls the brightness of the other LED on your circuit.* 

*This example requires **two different channel names** but uses the **same Publish and Subscribe keys**.* 

Connection Diagram for User 1 & 2: 

![User 1 & 2](README%20Images/nano-nano-1.png)


 




 
