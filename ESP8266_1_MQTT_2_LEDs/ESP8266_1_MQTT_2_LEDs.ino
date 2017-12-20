/*********
  Rui Santos
  Complete project details at http://randomnerdtutorials.com  
*********/

// Loading the required libraries
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

//#include <SoftwareSerial.h>
//SoftwareSerial mySerial(RX_PIN, TX_PIN); // RX, TX

// Change the credentials below, so your ESP8266 connects to your router
const char* ssid = "REPLACE_WITH_YOUR_SSID";
const char* password = "REPLACE_WITH_YOUR_PASSWORD";

// Change the variable to your Raspberry Pi IP address, so it connects to your MQTT broker
const char* mqtt_server = "REPLACE_WITH_YOUR_MQTT_BROKER_IP";
// For example: const char* mqtt_server = "192.168.1.112";

// Initializes the espClient. You should change the espClient name if you have multiple ESPs running in your home automation system
WiFiClient espClient;
PubSubClient client(espClient);

// LED pins
const int ledPin5 = 5;
const int ledPin4 = 4;

// Don't change the function below. This function connects your ESP8266 to your router
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
  // If a message is received on the topic office/workbench, you check if the message is either true or false 
  // Turns the workbench according to the message
  if(topic=="office/workbench"){
      //mySerial.print("Changing Workbench to ");
      if(messageTemp == "true"){
        digitalWrite(ledPin5, HIGH);
        //mySerial.print("On");
      }
      else if(messageTemp == "false"){
        digitalWrite(ledPin5, LOW);
        //mySerial.print("Off");
      }
  }
  else if(topic=="office/toplight"){
      //mySerial.print("Changing Top Light to ");
      if(messageTemp == "true"){
        digitalWrite(ledPin4, HIGH);
        //mySerial.print("On");
      }
      else if(messageTemp == "false"){
        digitalWrite(ledPin4, LOW);
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
       client.connect("ESP2_Office");
     Then, for the other ESP:
       client.connect("ESP1_Kitchen");
      That should solve your MQTT multiple connections problem
    */
    if (client.connect("ESP2_Office")) {
      //mySerial.println("connected"); 
      client.subscribe("office/workbench");
      client.subscribe("office/toplight");              
    } else {
      //mySerial.print("failed, rc=");
      //mySerial.print(client.state());
      //mySerial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup(void) { 
  // Set GPIOs as Outputs   
  pinMode(ledPin5, OUTPUT);
  pinMode(ledPin4, OUTPUT);
  // mySerial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop(void){   
  if (!client.connected()) {
    reconnect();
  }
  /*
   YOU MIGHT NEED TO CHANGE THIS LINE, IF YOU'RE HAVING PROBLEMS WITH MQTT MULTIPLE CONNECTIONS
   To change the ESP device ID, you will have to give a new name to the ESP8266.
   Here's how it looks:
     client.connect("ESP2_Office");
   Then, for the other ESP:
     client.connect("ESP1_Kitchen");
    That should solve your MQTT multiple connections problem
  */
  if(!client.loop())
    client.connect("ESP2_Office");
}
