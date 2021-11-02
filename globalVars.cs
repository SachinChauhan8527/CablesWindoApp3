using algorithmLib;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CablesWindoApp3
{
    class globalVars
    {   
        public static bool inspectionRunning = false;
        public static Form1 form;
        private static string workingDirectory = Environment.CurrentDirectory;
        public static string projectDirectory = Directory.GetParent(workingDirectory).Parent.Parent.FullName;
        public static Class1 algo = new Class1();////---------------------------------------CPP
    }
}
