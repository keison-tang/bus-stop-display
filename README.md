# bus-stop-display

## What?
Project that aims to create a miniature version of the arrival time displays you see at bus stops and stations.

Home bus stop - 3211

## Components
- Auckland Transport APIs
    - The source of real-time bus information
- API App hosted on Azure App Service
    - Does most of the processing as WeMos is weak and memory constrained 
    - Listens for API requests
- WeMos D1 Mini 
    - Periodically call the API on the web server to get simplified real-time info
    - Process info
    - Send info to OLED display
- 0.96" OLED Module
    - Display the info to me

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

### So far:
- API App largely done 
    - May need to tweak response schema, depending on WiFi module capability
    - Deployed to Azure App Service
    - AT API key is stored in App Settings - accessed through Azure - very secure ✔️✔️✔️
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
    - Append /swagger/ui/index to the end of your home URL
        - e.g http://[YOUR API APP NAME].azurewebsites.net/swagger/ui/index
    - You can enable/disable the Swagger UI by going to SwaggerConfig.cs, ~line 192
    - There is no home landing page as it is an API app, which doesn't have a View component, only Model and Controller
