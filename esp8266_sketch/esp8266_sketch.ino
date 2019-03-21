#include <ESP8266WiFi.h>        // Boards Manager > ESP8266
#include <ESP8266HTTPClient.h>
#include <Wire.h>               //I2C
#include <Adafruit_GFX.h>       //Graphics
#include <Adafruit_SSD1306.h>   //OLED hardware driver
#include <ArduinoJson.h>        //Library for JSON parsing
#include <Credentials.h>        //Library for WiFi credentials

// OLED
#define SCREEN_WIDTH   128  //in pixels
#define SCREEN_HEIGHT   64  //in pixels
#define OLED_RESET      -1  //reset pin is unused
#define OLED_ADDR     0x3C 
#define BODY_CURSOR_Y   16  //Y pixel position of where blue part (aka body) of OLED starts
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

  display.setTextSize(1); //1: 6x8px 21x8chars, 2: 12x16px 10x4chars
  display.setCursor(0, 0);
  display.print("STOP");  //static text at the top left of the OLED (header section)
  display.setCursor(0, 8);
  display.print("3360");
  display.display(); 

  display.setTextColor(WHITE, BLACK); //to overwrite text with fresh background (otherwise text is superimposed)
  
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
  
    http.begin(uri);
    int httpCode = http.GET();
  
    if (httpCode > 0) {
      String payload = http.getString();

      // Allocate JsonBuffer for max 6 results
      const size_t capacity = JSON_ARRAY_SIZE(6) + JSON_OBJECT_SIZE(2) + 6*JSON_OBJECT_SIZE(3) + 220;
      DynamicJsonBuffer jsonBuffer(capacity);

      // Parse JSON object 
      JsonObject& json = jsonBuffer.parseObject(payload);

      // Extract & display time
      display.setTextSize(2);
      display.setCursor(31, 0);  //pixel 31 to 127 = 96 px, which is enough for 8 x size 2 chars 
      display.print(json["Time"].as<char*>());
      display.print(" "); // extra space to clear off remanant char
      display.setTextSize(1);
      
      // Extract the bus services array
      JsonArray& services = json["SimpleMovements"];
      
      // To keep count of which line we are updating
      int lineCount = 0;

      // Update appropriate number of lines with bus services
      for (JsonObject& service : services) {
        display.setCursor(0, BODY_CURSOR_Y + lineCount * 8);  //set cursor to point to appropriate line
        display.print(service["Bus"].as<char*>());
        display.print("    ");
        display.print(service["Sch"].as<char*>());
        display.print("    ");
        display.print(service["Due"].as<char*>());
        display.print("  ");  //to clear off any remanant characters from previous update
        lineCount++;
      }

      // Fill the rest of the display with blank lines when there are <6 services available
      while (lineCount <= 6) {
        display.setCursor(0, BODY_CURSOR_Y + lineCount * 8);
        display.print("-                    "); //Blank line
        lineCount++;
      }

      display.display();
      
    } else {
      Serial.println("HTTP.GET failed");      
    }
    
    http.end();
  }
  
  //delay for 10 seconds
  delay(10000);
  
}
