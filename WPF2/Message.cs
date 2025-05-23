using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace WPF2
{
    public class Message
    {
        public string Username { get; set; }
        public string Text { get; set; }
        public DateTime Timestamp { get; set; }
        public bool IsSystem { get; set; }
    }
    public enum MessageType
    {
        User,
        System
    }


}
