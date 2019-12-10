#include <PubSubClient.h>
#include <ESP8266WiFi.h>

const char* ssid = ""; //INSERT SSID HERE
const char* password = ""; //INSERT WIFI PASSWORD HERE

const char* mqttServer = ""; //INSERT MQTT IP
const int mqttPort = 1883; //INSERT MQTT PORT
const char* mqttUser = ""; //MQTT USER
const char* mqttPassword = ""; //MQTT PASS

#include <RCSwitch.h>

RCSwitch mySwitch = RCSwitch();

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(115200);
  Serial.println("Booted");
  
  mySwitch.enableReceive(D2);  // Pin D2 on Node MCU

  Serial.println("Receiving signals..");

  connectToServer();
}

void callback(char* topic, byte* payload, unsigned int length) {
 
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
 
  Serial.print("Message:");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
 
  Serial.println();
  Serial.println("-----------------------");
 
}

void connectToServer() {
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());         // Send the IP address of the ESP8266 to the computer
 
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);
 
  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");
 
    if (client.connect("ESP8266Client", mqttUser, mqttPassword )) {
 
      Serial.println("connected");
      
      String clientId = WiFi.macAddress();
      
      client.publish("esp/connected", clientId.c_str());
 
    } else {
 
      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(2000);
 
    }
  }
}
 
String macToStr(const uint8_t* mac) {
  String result;
  for (int i = 0; i < 6; ++i) {
    result += String(mac[i], 16);
    if (i < 5)
      result += ':';
  }
  return result;
}


void loop() {
  if (mySwitch.available()) {
    
    int value = mySwitch.getReceivedValue();
    
    if (value == 0) {
      Serial.print("Unknown encoding");
    } else {
      Serial.print("Received ");
      Serial.print( mySwitch.getReceivedValue() );
      Serial.print(" / ");
      Serial.print( mySwitch.getReceivedBitlength() );
      Serial.print("bit ");
      Serial.print("Protocol: ");
      Serial.println( mySwitch.getReceivedProtocol() );
      
      String message;
      message += String(mySwitch.getReceivedValue() );
      message += '/';
      message += String(mySwitch.getReceivedBitlength() );
      message += '/';
      message += String(mySwitch.getReceivedProtocol() );
      
      client.publish("esp/message", message.c_str());
    }

    mySwitch.resetAvailable();
  }
}
