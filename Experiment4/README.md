# Experiment 4: Networking - 
Created by Olivia Prior


Things you will need for this experiment:
* Create a PubNub Account

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

 ![PubNub Diagram - Single Channel](README%20Images/pubnub-01.png)
 
 Users can Publish to one channel, and subscribe to another. Alternatively, users can subscribe to multiple channels.
 
 In this example, User 1 publishes to Channel 1, but is subscribed to Channel 2. User 2 published to Channel 2, but subscribes to Channel 1. This example shows how you can listen and publish to different channels. 
 
![PubNub Diagram - Multi-Channel](README%20Images/pubnub-multi-01.png)
  
The users in the examples can be browser windows, browsers on a phone, or an Arduino.   


## What is JSON? 

JSON stands for **J**ava**S**cript **O**bject **N**otation
In the examples in this experiment, JSON is used as a way of sending and receiving multiple types of data amongst users. 


JSON uses a concept of "Key:Value" of storing data. 


In this example, the Key is message, and the Value is "Hello World"

```
let obj = { 
    message: "Hello World"
}
```
If you were to print "obj" this is what you would receive in the console:

```
print(obj);

....
console:
{message: "Hello World"}
```

If you wanted to explicity retreive the "Value" of message, you can call the specific value: 

```
print(obj.message);

....
console:
Hello World
```

Each JSON object can hold multiple Key:Value pairs

Example:
```
let obj = {
    message: "Hello World",
    user: "DF Student", 
    location: "Toronto"
}
```

To retrieve any of the individual values you can call them explicitly: 

```
print(obj.message);
print(obj.user);
print(obj.location); 

....
console:
Hello World
DF Student
Toronto
```

JSON values can be assigned to other variables. In this example the variable sender is assigned the value of obj.user. It is then printed out in the console. 

```
let obj = {
    message: "Hello World",
    user: "DF Student", 
    location: "Toronto"
}

let sender = obj.user;

print(sender);

....
console: 
DF Student
```

JSON Values can also be variables, not just Strings. In this example, the JSON values are variables, rather than Strings. 

``` 
let myMessage = "Digital Futures"
let myName = "Student";
let myCity = "Warsaw";
let myGradYear = 2022; 

let obj = {message: myMessage,
    user: MyName, 
    location: myCity, 
    year: myGradYear
}
           
print(obj.message);
print(obj.user);
print(obj.location);
print(obj.year); 

....
console:
Digital Futures
Student
Warsaw
2022
```

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

 - In a p5 Sketch
 
 ![PubNub Keys for p5](README%20Images/p5-keys.png)

 



 




 
