/*********
  Rui Santos
  Complete project details at http://randomnerdtutorials.com  
*********/

// Loading the required libraries
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "Nextion.h"
#include <RCSwitch.h>

//#include <SoftwareSerial.h>
//SoftwareSerial mySerial(RX_PIN, TX_PIN); // RX, TX

// Change the credentials below, so your ESP8266 connects to your router
const char* ssid = "REPLACE_WITH_YOUR_SSID";
const char* password = "REPLACE_WITH_YOUR_PASSWORD";

// Change the variable to your Raspberry Pi IP address, so it connects to your MQTT broker
const char* mqtt_server = "REPLACE_WITH_YOUR_MQTT_BROKER_IP";
// For example: const char* mqtt_server = "192.168.1.112";

// Initialize the espClient. You should change the espClient name if you have multiple ESPs running in your home automation system
WiFiClient espClient;
PubSubClient client(espClient);

// Initialize the RC Switch component
RCSwitch mySwitch = RCSwitch();

// Declare a button object [page id:0,component id:1, component name: "b0"]
NexButton b0on = NexButton(0, 2, "b0on");
NexButton b0off = NexButton(0, 4, "b0off");
NexButton b1on = NexButton(0, 8, "b1on");
NexButton b1off = NexButton(0, 9, "b1off");
NexButton b2on = NexButton(0, 10, "b2on");
NexButton b2off = NexButton(0, 11, "b2off");
NexButton b3on = NexButton(0, 12, "b3on");
NexButton b3off = NexButton(0, 13, "b3off");

//Register a button object to the touch event list  
NexTouch *nex_listen_list[] = {
  &b0on,
  &b0off,
  &b1on,
  &b1off,
  &b2on,
  &b2off,
  &b3on,
  &b3off,
  NULL
};

// Button component push callback function 
void b0onPushCallback(void *ptr) {  
  client.publish("office/workbench/buttonState", "true");
}
void b0offPushCallback(void *ptr) {
  client.publish("office/workbench/buttonState", "false"); 
}
void b1onPushCallback(void *ptr) {
  client.publish("office/toplight/buttonState", "true");   
}
void b1offPushCallback(void *ptr) {
  client.publish("office/toplight/buttonState", "false");    
}
void b2onPushCallback(void *ptr) {
  client.publish("office/outlet1/buttonState", "true");     
}
void b2offPushCallback(void *ptr) {
  client.publish("office/outlet1/buttonState", "false");   
}
void b3onPushCallback(void *ptr) {
  client.publish("office/outlet2/buttonState", "true");   
}
void b3offPushCallback(void *ptr) {
  client.publish("office/outlet2/buttonState", "false");   
}

// Don't change the function below. 
// This function connects your ESP8266 to your router
void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  //mySerial.println();
  //mySerial.print("Connecting to ");
  //mySerial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    //mySerial.print(".");
  }
  //mySerial.println("");
  //mySerial.print("WiFi connected - ESP IP address: ");
  //mySerial.println(WiFi.localIP());
}

// This function is executed when some device publishes a message to a topic that your ESP8266 is subscribed to
// Change the function below to add logic to your program, so when a device publishes a message to a topic that 
// your ESP8266 is subscribed you can actually do something
void callback(String topic, byte* message, unsigned int length) {
  //mySerial.print("Message arrived on topic: ");
  //mySerial.print(topic);
  //mySerial.print(". Message: ");
  String messageTemp;
  
  for (int i = 0; i < length; i++) {
    //mySerial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  //mySerial.println();
  
  // Feel free to add more if statements to control more outputs with MQTT
  // If a message is received on the topic office/outlet1, you check if the message is either true or false 
  // Turns the outlet1 according to the message
  if(topic=="office/outlet1"){
      //mySerial.print("Changing Outlet 1 to ");
      if(messageTemp == "true"){
        mySwitch.send(4527445, 24);
        //mySerial.print("On");
      }
      else if(messageTemp == "false"){
        mySwitch.send(4527444, 24);
        //mySerial.print("Off");
      }
  }
  // Turns the outlet2 according to the message
  else if(topic=="office/outlet2"){
      //mySerial.print("Changing Outlet 2 to ");
      if(messageTemp == "true"){
        mySwitch.send(4539733, 24);
        //mySerial.print("On");
      }
      else if(messageTemp == "false"){
        mySwitch.send(4539732, 24);
        //mySerial.print("Off");
      }
  }
  //mySerial.println();
}

// This function reconnects your ESP8266 to your MQTT broker
// Change the function below if you want to subscribe to more topics with your ESP8266 
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    //mySerial.print("Attempting MQTT connection...");
    // Attempt to connect
    /*
     YOU MIGHT NEED TO CHANGE THIS LINE, IF YOU'RE HAVING PROBLEMS WITH MQTT MULTIPLE CONNECTIONS
     To change the ESP device ID, you will have to give a new name to the ESP8266.
     Here's how it looks:
       client.connect("ESP1_Office");
     Then, for the other ESP:
       client.connect("ESP1_Kitchen");
      That should solve your MQTT multiple connections problem
    */
    if(client.connect("ESP1_Office")) {
      //mySerial.println("connected"); 
      client.subscribe("office/outlet1");  
      client.subscribe("office/outlet2");             
    } 
    else {
      //mySerial.print("failed, rc=");
      //mySerial.print(client.state());
      //mySerial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup(void) {    
  //Set the baudrate which is for debug and communicate with Nextion screen
  nexInit();

  //Register the pop event callback function of the current button component
  b0on.attachPush(b0onPushCallback, &b0on);
  b0off.attachPush(b0offPushCallback, &b0off);
  b1on.attachPush(b1onPushCallback, &b1on);
  b1off.attachPush(b1offPushCallback, &b1off);
  b2on.attachPush(b2onPushCallback, &b2on);
  b2off.attachPush(b2offPushCallback, &b2off);
  b3on.attachPush(b3onPushCallback, &b3on);
  b3off.attachPush(b3offPushCallback, &b3off);
    
  // mySerial.begin(115200);
  
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback); 

  // Connect RF 433MHz Transmitter to GPIO 5
  mySwitch.enableTransmit(5);
    
  // SET YOUR PULSE LENGTH
   mySwitch.setPulseLength(REPLACE_WITH_YOUR_PULSE_LENGTH);
  
  // SET YOUR PROTOCOL (default is 1, will work for most outlets)
  mySwitch.setProtocol(REPLACE_WITH_YOUR_PROTOCOL);
  
  // Set number of transmission repetitions
  mySwitch.setRepeatTransmit(20);
}

void loop(void){   
  // When a push event occured every time,
  // the corresponding component[right page id and component id] in touch event list will be asked
  nexLoop(nex_listen_list);
   
  if (!client.connected()) {
    reconnect();
  }
  /*
    YOU MIGHT NEED TO CHANGE THIS LINE, IF YOU'RE HAVING PROBLEMS WITH MQTT MULTIPLE CONNECTIONS
    To change the ESP device ID, you will have to give a new name to the ESP8266.
    Here's how it looks:
      client.connect("ESP1_Office");
    Then, for the other ESP:
      client.connect("ESP1_Kitchen");
    That should solve your MQTT multiple connections problem
    */
  if(!client.loop())
    client.connect("ESP1_Office");
}
