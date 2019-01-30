using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;

namespace bus_api.Models
{
    public class SimpleMovement
    {
        //route number, scheduled time, expected time to arrive
        public string Route { get; set; }
        public string SchTime { get; set; }
        public string DueTime { get; set; }
    }
}