#include <ESP8266WiFi.h>  //Boards Manager > ESP8266
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>  //Library ArduinoJson
#include <Credentials.h>  //custom library for credentials (path: */Documents/Arduino/libraries/Credentials)

const char* ssid = WIFI_SSID;
const char* password = WIFI_PW;
const char* uri = API_URI;

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print("Connecting... \n");
  }
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
  
    HTTPClient http;
    http.begin(uri);
    int httpCode = http.GET();
  
    if (httpCode > 0) {
      String payload = http.getString();
      Serial.println(payload + "\n");  
    }
    
    http.end();
  }
  
  //delay for 20 seconds
  delay(20000);
  
}
