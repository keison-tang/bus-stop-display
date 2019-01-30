using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;

namespace bus_api.Models
{
    // s/o to http://json2csharp.com/

    public class Movement
    {
        public DateTime     scheduledArrivalTime        { get; set; }
        public DateTime     scheduledDepartureTime      { get; set; }
        public string       arrivalBoardingActivity     { get; set; }
        public string       arrivalPlatformName         { get; set; }
        public string       arrivalStatus               { get; set; }
        public string       departureBoardingActivity   { get; set; }
        public string       departurePlatformName       { get; set; }
        public string       destinationDisplay          { get; set; }
        public DateTime?    expectedArrivalTime         { get; set; }
        public DateTime?    expectedDepartureTime       { get; set; }
        public bool         inCongestion                { get; set; }
        public bool         monitored                   { get; set; }
        public string       route_short_name            { get; set; }
        public string       stop_code                   { get; set; }
        public DateTime     timestamp                   { get; set; }
        public string       vehicleJourneyName          { get; set; }
    }

    public class Response
    {
        public List<object> extensions { get; set; }
        public List<Movement> movements { get; set; }
    }

    public class GetDeparturesObject
    {
        public string status { get; set; }
        public Response response { get; set; }
        public object error { get; set; }
    }
}