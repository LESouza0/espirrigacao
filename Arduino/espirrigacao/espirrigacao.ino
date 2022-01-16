// libraries
#include <stdio.h>
#include <string.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h> 
#include <DNSServer.h>
#include <WiFiManager.h>
#include <ESP8266Firebase.h>
#include <Ticker.h>
#include <ArduinoJson.h>
#include <DHT.h>

// definitions
#define DHTPIN 4
#define DHTTYPE DHT11
#define FIREBASE_ID "espwater-25fb5-default-rtdb"
#define FIREBASE_FINGERPRINT "93 49 BD 13 3F AD AE EB 44 9B DA EA 6E 7F 27 A3 2E D1 73 7B"
#define PUBLISH_INTERVAL 1000*60*60

// initializations
ESP8266WebServer server(80);
WiFiManager wifiManager;
DHT dht(DHTPIN, DHTTYPE);
Firebase firebase(FIREBASE_ID, FIREBASE_FINGERPRINT);
Ticker ticker;
bool publishNewState = true;

void publish(){
  publishNewState = true;
}


void setup() 
{
  Serial.begin(115200);
  Serial.println("BEGIN");
  delay(50);

  wifiManager.setSaveConfigCallback(saveConfigCallback);
  wifiManager.autoConnect();
  dht.begin();

  ticker.attach_ms(PUBLISH_INTERVAL, publish);
}

String firebasePath = "";
 
void loop() 
{
  server.handleClient();
  
  if (firebasePath.length() > 0 && WiFi.status() == WL_CONNECTED && publishNewState) {
    float temperature = dht.readTemperature();
    float humidity = dht.readHumidity();

    firebase.setString(firebasePath+"temperature/", String(temperature));
    firebase.setString(firebasePath+"humidity/", String(humidity));

    publishNewState = false;
  }
}


void saveConfigCallback() {
  String ssid = wifiManager.getWiFiSSID();
  String password = wifiManager.getWiFiPass();
  String espIp = WiFi.localIP().toString();
  firebasePath = "networks/"+ssid+"/";

  firebase.setString(firebasePath+"ip/", espIp);
  firebase.setString(firebasePath+"password/", password);
}

void handleAction(){
  // switch case action name
}
  
