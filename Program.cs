using System;
using System.Windows.Forms;

namespace CablesWindoApp3
{
    static class Program
    {

        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main()
        {
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            globalVars.form = new Form1();
            Application.Run(globalVars.form);           
        }
    }
}
