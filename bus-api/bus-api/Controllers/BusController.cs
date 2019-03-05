using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Net.Http;
using System.Threading.Tasks;
using System.Web;
using System.Web.Http;
using System.IO;
using System.Text;
using Newtonsoft.Json;
using RestSharp;

using bus_api.Models;
using System.Configuration;

namespace bus_api.Controllers
{
    public class BusController : ApiController
    {
        private static HttpClient _httpClient = new HttpClient();

        /// <summary>
        /// GET api/departures
        /// Lightweight version of the GET Departures API from AT targetting embedded IoT devices
        /// </summary>
        /// <param name="stop_id">Bus stop ID number</param>
        /// <param name="results">Maximum number of results</param>
        /// <returns></returns>
        [HttpGet]
        [System.Web.Http.Route("api/departures")]
        public List<SimpleMovement> Departures(string stop_id, int? results = null)
        {
            List<SimpleMovement> simpleMovements = SimpleDepartures(stop_id);

            if ((results != null) && (results < simpleMovements.Count))
            {
                //crop and return list
                return simpleMovements.GetRange(0, (int)results);
            } else  //results = null, results = count, results > count
            {
                return simpleMovements;
            }
        }

        /// <summary>
        /// Method that returns simplified departure information, using GET Departures
        /// </summary>
        /// <param name="stop_id">Bus stop ID number</param>
        /// <returns>List of simple bus services</returns>
        private List<SimpleMovement> SimpleDepartures(string stop_id)
        {
            IRestResponse response = GetDepartures(stop_id);    //call method that calls API
            GetDeparturesObject obj = JsonConvert.DeserializeObject<GetDeparturesObject>(response.Content);     //deserialise json to object 
            List<Movement> movements = obj.response.movements;  //could probably just get the list of movements right off the bat

            List<SimpleMovement> services = new List<SimpleMovement> { };

            foreach (Movement movement in movements)
            {
                SimpleMovement simple = new SimpleMovement { };

                //Bus route
                simple.Bus = movement.route_short_name;

                //Scheduled arrival time (timetable)
                simple.Sch = UTCtoNZST(movement.scheduledArrivalTime).ToShortTimeString();    //convert UTC to NZST, then get time string

                //Due time (real time tracking)
                if (movement.monitored)
                {
                    TimeSpan diff = (movement.expectedArrivalTime - DateTime.UtcNow).GetValueOrDefault();     //difference of UTC expected time and UTC now
                    double mins = diff.TotalMinutes;

                    if (mins <= 0)
                    {
                        simple.Due = "*";
                    }
                    else
                    {
                        simple.Due = Math.Floor(mins).ToString();
                    }
                }
                else if (movement.arrivalStatus == "cancelled")
                {
                    simple.Due = "C";   //cancelled
                }
                else
                {
                    simple.Due = "";    //untracked
                }

                services.Add(simple);
            }

            return services;
        }

        /// <summary>
        /// Method that calls Auckland Transport GET Departures API
        /// </summary>
        /// <param name="stop_id">Bus stop ID number</param>
        /// <returns>Response of GET request</returns>
        private IRestResponse GetDepartures(string stop_id)
        {
            var client = new RestClient("https://api.at.govt.nz/v2/public-restricted/departures/" + stop_id);
            var request = new RestRequest(Method.GET);
            request.AddHeader("cache-control", "no-cache");
            request.AddHeader("ocp-apim-subscription-key", ConfigurationManager.AppSettings.Get("AT_API_KEY"));
            IRestResponse response = client.Execute(request);
            return response;
        }

        /// <summary>
        /// Convert UTC to NZST 
        /// </summary>
        /// <param name="utcDateTime">UTC DateTime object</param>
        /// <returns>NZST DateTime object</returns>
        private DateTime UTCtoNZST(DateTime utcDateTime)
        {
            TimeZoneInfo nzstZone = TimeZoneInfo.FindSystemTimeZoneById("New Zealand Standard Time");
            DateTime nzstTime = TimeZoneInfo.ConvertTimeFromUtc(utcDateTime, nzstZone);
            return nzstTime;
        }
    }
}