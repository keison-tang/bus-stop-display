# bus-stop-display

## What?
Project that aims to create a miniature version of the arrival time displays you see at bus stops and stations.

Home bus stop - 3211

## Components
- Auckland Transport APIs
    - The source of real-time bus information
- Either Orange Pi or custom website (Azure)
    - Web Server
    - Listens to API requests from Wemos, calls AT APIs, processes, returns info
    - May need to call other APIs from Google etc..
    - Additional processing power as Wemos is quite weak
- Wemos D1 Mini 
    - Periodically call the API on the web server to get route numbers and estimated arrival times
    - Process info
    - Send info to OLED display
- 0.91" OLED 
    - Display the info to me

## APIs
### Auckland Transport
- GET Departures
    - https://api.at.govt.nz/v2/public-restricted/departures/{stopid}?subscription-key={subscription_key}

### Orange Pi / Website
- GET getTimes
    - Params: bus stop #
    - Returns: List of bus # and estimated arrival times