#include <stdio.h>
#include <string.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h> 
#include <DNSServer.h>
#include <WiFiManager.h>
#include <ESP8266Firebase.h>
#include <ArduinoJson.h>
#include <DHT.h>

const char* ssid = "brisa-1397440";  // Enter SSID here
const char* password = "t1rehfdu";  //Enter Password here

ESP8266WebServer server(80);
WiFiManager wifiManager;

#define DHTPIN 4
#define DHTTYPE DHT11
#define FIREBASE_ID "espwater-25fb5-default-rtdb"
#define FIREBASE_FINGERPRINT "93 49 BD 13 3F AD AE EB 44 9B DA EA 6E 7F 27 A3 2E D1 73 7B"

DHT dht(DHTPIN, DHTTYPE);
Firebase firebase(FIREBASE_ID, FIREBASE_FINGERPRINT);

void setup() 
{
  Serial.begin(115200);
  Serial.println("BEGIN");
  delay(50);
  wifiManager.resetSettings();
  
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
   delay(500);
  }
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  wifiManager.setSaveConfigCallback(saveConfigCallback);
  wifiManager.autoConnect();
  dht.begin();
  
  server.on("/dados", handleGetData);
}

float temperature;
float humidity;
 
void loop() 
{
  server.handleClient();
  temperature = dht.readTemperature();
  humidity = dht.readHumidity(); 

  delay(500);
}


void handleGetData(){
  server.send(200, "text/plain", String(temperature));
}

void saveConfigCallback() {
  String ssid = wifiManager.getWiFiSSID();
  String password = wifiManager.getWiFiPass();
  String espIp = WiFi.localIP().toString();
  String path = "networks/"+ssid+"/";

  firebase.setString(path+"ip/", espIp);
  firebase.setString(path+"password/", password);
}
  
