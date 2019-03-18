#include <ESP8266WiFi.h>  //Boards Manager > ESP8266
#include <ESP8266HTTPClient.h>
#include <Wire.h> //I2C
#include <Adafruit_GFX.h> //OLED graphics
#include <Adafruit_SSD1306.h>  //OLED hardware driver
#include <ArduinoJson.h>  //Library for JSON parsing etc
#include <Credentials.h>  //custom library for credentials (path: */Documents/Arduino/libraries/Credentials)

// OLED
#define SCREEN_WIDTH 128  //pixels
#define SCREEN_HEIGHT 64  //pixels
#define OLED_RESET -1 //shared reset pin
#define OLED_ADDR 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// WiFi  
const char* ssid = WIFI_SSID;
const char* password = WIFI_PW;
const char* uri = API_URI;
HTTPClient http;

void setup() {
  Serial.begin(115200);
  
  // WiFi initialisation
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println(F("Connecting to WiFi..."));
  }

  // OLED initialisation
  if(!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }

  delay(1000);
  display.clearDisplay();
  display.display();
  display.setTextColor(WHITE);
  display.setFont(NULL);

  display.setTextSize(2); //1: 6x8px 21x8chars, 2: 12x16px 10x4chars
  display.setCursor(4, 0);
  display.print("STOP# 3211");  //static text at the top of the OLED - show whatever text you like
  display.display(); 

  display.setTextSize(1);
  
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
  
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
