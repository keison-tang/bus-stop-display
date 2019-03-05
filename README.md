# bus-stop-display

## What?
Project that aims to create a miniature version of the arrival time displays you see at bus stops and stations.

Home bus stop - 3211

## Components
- Auckland Transport APIs
    - The source of real-time bus information
- Custom API App hosted on Azure App Service
    - Web Server
    - Listens to API requests from Wemos, calls AT APIs, processes, returns info
    - May need to call other APIs from Google etc..
    - Additional processing power as Wemos is quite weak
- WeMos D1 Mini 
    - Periodically call the API on the web server to get route numbers and estimated arrival times
    - Process info
    - Send info to OLED display
- 0.96" OLED Module
    - Display the info to me

## APIs
- GET getTimes
    - Params: bus stop #
    - Returns: List of bus # and estimated arrival times
    - When API is called
        - Call Departures API
        - Process
        - Return JSON, for example:
            ```bash
            {
                "services" : [
                    {
                        "bus" : {route number},
                        "sch" : {scheduled time},
                        "exp" : {expected time},
                    },
                    {
                        "bus" : "871",
                        "sch" : "1337",
                        "exp" : "5", 
                    }
                ]
            }
            ```

### So far:
- API App largely done 
    - May need to tweak response schema, depending on WiFi module capability
    - Deployed to Azure App Service
    - AT API key is stored in App Settings - accessed through Azure
- Next step: hardware side
    - Play around with WeMos D1 mini
    - Find out JSON capability, GET requests etc..
    - GET A SOLDERING IRON

### {Draft} Instructions for deploying API App to Azure
1. Create Azure acc
    - Part of "always free" services: Up to 10 App Services (?)
2. Create an API App (Part of App Service)
3. Go to: Settings | Application Settings
4. Add an Application Setting called AT_API_KEY and put your API key in the Value
5. Deployment via IDE (my way) - you could deploy using Azure portal
    - Open the project in VS2017
    - Restore NuGet packages, Clean & Rebuild etc
    - Build > Publish bus-api
    - Select the App service that you created in Azure portal.
    - Publish!! - if it fails, just retry
6. Make sure the API works by going to your website and using the Swagger UI to try it out.
    - Append /swagger to the end of your home URL
    - You can enable/disable the Swagger UI by going to SwaggerConfig.cs, ~line 192
    - There is no home landing page as it is an API app, which doesn't have a View component, only Model and Controller
