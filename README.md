# bus-stop-display

<center>
    <img src="images/full-view.jpg"/>
</center>

## What?
Project that aims to recreate a miniature version of the real-time boards seen at bus stops and stations around Auckland. These typically show the current time, stop number and estimated arrival times of bus services that travel past the stop.

Currently, this project is able to display the stop number and a maximum of six of the nearest bus services. 

## How?
A WiFi module periodically calls an API app hosted on Azure, which in turn calls an AT API. The purpose of the API app is to process the original payload into a simpler JSON schema containing only the necessary information required by the WiFi module. The simplified schema makes it suitable for memory and computationally constrained devices. The WiFi module then parses the JSON and then displays the information through a small OLED. 

## Components
- Software
    - Auckland Transport (AT) API
    - Microsoft Azure 
    - API App hosted on Azure App Service
- Hardware
    - WeMos D1 Mini (ESP8266 - WiFi enabled microcontroller)
    - 0.96" OLED Module (I2C monochrome version)
    - 4 x M-M jumper wires
    - Medium breadboard
    - Micro USB cable
- IDE
    - Visual Studio 2017 Community
    - Arduino IDE

## APIs
- GET /api/departures
    - Parameters
        - stop_id [required] : four digit bus stop id
        - results [optional] : max number of results you want
    - Response
        - Content Type: application/json
        - List of SimpleDeparture objects in JSON
            - Bus: short route code
            - Sch: scheduled time
            - Due: estimated arrival time in minutes, "C" = cancelled, "" = untracked bus
    - Example
        - http://[YOUR API APP NAME].azurewebsites.net/api/departures?stop_id=3211&results=5

            ```bash
            [
                {
                    "Bus": "871",
                    "Sch": "7:42 PM",
                    "Due": ""
                },
                {
                    "Bus": "871",
                    "Sch": "8:12 PM",
                    "Due": ""
                },
                {
                    "Bus": "871",
                    "Sch": "8:42 PM",
                    "Due": ""
                }
            ]
            ```

## Deploying API App to Azure
1. Create Azure account
    - You can deploy up to 10 App Services as part of the free account tier  
2. Create an API App (Under 'App Service')
3. Go to: Settings | Application Settings
4. Add an Application Setting called AT_API_KEY and put your API key in the Value
5. Deployment via IDE - you could also deploy using Azure portal
    - Open the project in VS2017
    - Restore NuGet packages, Clean & Rebuild etc
    - Build > Publish bus-api
    - Select the App service that you created in Azure portal.
    - Publish!! - if it fails, just retry
6. Make sure the API works by going to your website and using the Swagger UI to try it out.
    - Append /swagger/ui/index to the end of your home URL
        - e.g http://[API APP NAME].azurewebsites.net/swagger/ui/index
    - You can enable/disable the Swagger UI by going to SwaggerConfig.cs, ~line 192
    - There is no home landing page as it is an API app, which doesn't have a View component, only Model and Controller

## Hardware Setup
- Wiring (OLED to D1 mini)
    - GND to G
    - VCC to 3V3 (5V also works)
    - SCL to D1
    - SDA to D2
- Arduino Sketch
    - Open Arduino IDE
    - Install support for ESP8266 board: 
        - Tools > Board > Boards Manager
        - Install esp8266 by ESP8266 Community, version 2.5.0
    - Select correct target board for programming:
        - Tools > Board > LOLIN(WEMOS) D1 R2 & mini
    - Install needed libs 
        - Sketch > Include Library > Library Manager
        - Install Adafruit GFX Library by Adafruit, version 1.3.4
        - Install Adafruit SSD1306 by Adafruit, version 1.2.9
        - Install ArduinoJson by Benoit Blanchon, version 5.13.4
    - _If you want to skip the following instructions, just type your WiFi credentials and API endpoint into the sketch file. Also remove the #include <Credentials.h> line from the sketch (line 7). If you intend on putting your sketch on a public repo, I would recommend not skipping this, as, as your WiFi details won't be openly shared._ 
        - Navigate to where your Arduino libraries are stored
        - Default on Windows: */Documents/Arduino/libraries/
        - You should see all the libraries you just installed
        - Copy the example Credentials library found in this repo
        - Remove the .example suffixes
        - In the header file, replace the placeholders with your own credentials
    - Compile and upload the sketch
    - You should see something like this!
<center>
    <img src="images/oled-view-cropped.jpg" width="250"/>
</center>

## Useful Links

## Future Work
- Refactor backend API app for properness
- API return current time and display on OLED
- API handle when there are no services available - either because of outage or there just aren't any buses