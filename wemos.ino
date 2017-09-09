#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiUdp.h>
#include <functional>
#include "switch.h"
#include "UpnpBroadcastResponder.h"
#include "CallbackFunction.h"

// prototypes
boolean connectWifi();

//on/off callbacks 
void officeLightsOn();
void officeLightsOff();
void kitchenLightsOn();
void kitchenLightsOff();
void ledLightsOn();
void ledLightsOff();

// Change this before you flash
//#######################################
const char* ssid = "WLAN1_Guest1"; //enter your access point/wifi router name
const char* password = "Locoto100"; //enter router password
// change gpio pins as you need it.
//I am using ESP8266 EPS-12E GPIO16 and GPIO14
const int relayPin1 = 16;
const int relayPin2 = 14;

//#######################################
boolean wifiConnected = false;

const int ledPin = 2; //BUILTIN_LED;

UpnpBroadcastResponder upnpBroadcastResponder;

Switch *office = NULL;
Switch *kitchen = NULL;
Switch *led = NULL;

void OTAsetup() {
  
}

void setup()
{
   Serial.begin(115200);
   pinMode(relayPin1, OUTPUT);
   pinMode(relayPin2, OUTPUT);
   pinMode(ledPin, OUTPUT);
   
  // Initialise wifi connection
   wifiConnected = connectWifi();
  
   if(wifiConnected){
     upnpBroadcastResponder.beginUdpMulticast();
    
     // Define your switches here. Max 14
     // Format: Alexa invocation name, local port no, on callback, off callback
     office = new Switch("office lights", 80, officeLightsOn, officeLightsOff);
     kitchen = new Switch("kitchen lights", 81, kitchenLightsOn, kitchenLightsOff);
     led = new Switch("coffee light", 82, ledOn, ledOff);

     Serial.println("Adding switches upnp broadcast responder");
     upnpBroadcastResponder.addDevice(*office);
     upnpBroadcastResponder.addDevice(*kitchen);
     upnpBroadcastResponder.addDevice(*led);
   }
}
 
void loop()
{
	 if(wifiConnected){
      upnpBroadcastResponder.serverLoop();
      
      kitchen->serverLoop();
      office->serverLoop();
      led->serverLoop();
	 }
}

void officeLightsOn() {
    Serial.print("Switch 1 turn on ...");
    digitalWrite(relayPin1, HIGH);
}

void officeLightsOff() {
    Serial.print("Switch 1 turn off ...");
    digitalWrite(relayPin1, LOW);
}

void kitchenLightsOn() {
    Serial.print("Switch 2 turn on ...");
    digitalWrite(relayPin2, HIGH);
}

void kitchenLightsOff() {
  Serial.print("Switch 2 turn off ...");
  digitalWrite(relayPin2, LOW);
}

void ledOn() {
    Serial.print("LED turn on ...");
    digitalWrite(ledPin, LOW);
}

void ledOff() {
  Serial.print("LED turn off ...");
  digitalWrite(ledPin, HIGH);
}

// connect to wifi – returns true if successful or false if not
boolean connectWifi(){
  boolean state = true;
  int i = 0;
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");
  Serial.println("Connecting to WiFi");

  // Wait for connection
  Serial.print("Connecting ...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if (i > 10){
      state = false;
      break;
    }
    i++;
  }
  
  if (state){
    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  }
  else {
    Serial.println("");
    Serial.println("Connection failed.");
  }
  
  return state;
}
