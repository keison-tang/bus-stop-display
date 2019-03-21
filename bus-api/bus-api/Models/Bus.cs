using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;

namespace bus_api.Models
{
    public class SimpleMovement
    {
        //route number, scheduled time, due time
        public string Bus { get; set; }
        public string Sch { get; set; }
        public string Due { get; set; }
    }

    public class SimpleResponse
    {
        public string Time { get; set; }
        public List<SimpleMovement> SimpleMovements { get; set; }
    }
}