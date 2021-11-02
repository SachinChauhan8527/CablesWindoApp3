using CablesWindoApp3.Properties;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace CablesWindoApp3
{
    public partial class SetttingFormcs : Form
    {
        public SetttingFormcs()
        {
            InitializeComponent();
        }
        private Toupcam cam_ = null;
        private void loadAllSerialPorts()
        {
            cmbAllSerailPorts.Items.Clear();
            string[] ports = SerialPort.GetPortNames();

            Console.WriteLine("The following serial ports were found:");

            // Display each port name to the console.
            foreach (string port in ports)
            {
                Console.WriteLine(port);
                cmbAllSerailPorts.Items.Add(port);

            }
            if (cmbAllSerailPorts.Items.Count > 0)
            {
                cmbAllSerailPorts.SelectedIndex = 0;
            }
            else
            {
                MessageBox.Show("No communication port found. Please check usb cable");
            }
        }

        private void SetttingFormcs_Load(object sender, EventArgs e)
        {
            //-----------------------------Exposure-----------
            ExposureTimeSetUpdaown.Minimum = 0;
            ExposureTimeSetUpdaown.Maximum = 5000;
            ExposureTimeSetUpdaown.Value = 0;

            //-----------------------------Gain-----------
            GainUpdown.Minimum =1;
            GainUpdown.Maximum = 50;
            GainUpdown.Value = 1;

            //-----------------------------Saturation-----------
            SaturationUpDown.Minimum = 0;
            SaturationUpDown.Maximum = 255;
            SaturationUpDown.Value = 0;

            //-----------------------------Brightnes-----------
            BrightnessUpDown.Minimum = -100;
            BrightnessUpDown.Maximum = 100;
            BrightnessUpDown.Value = 0;

            //-----------------------------Contrast-----------
            ContrastUpdown.Minimum = -100;
            ContrastUpdown.Maximum = 100;
            ContrastUpdown.Value = 0;


            loadAllSerialPorts();
        }

        private void ExposureTimeSetUpdaown_Click(object sender, EventArgs e)
        {
            globalVars.form.UpdateExposure((uint)ExposureTimeSetUpdaown.Value);
        }

        private void label1_Click(object sender, EventArgs e)
        {

        }

        private void GainUpdown_ValueChanged(object sender, EventArgs e)
        {
            globalVars.form.UpdateGain((ushort)GainUpdown.Value);
        }

        private void SaturationUpDown_ValueChanged(object sender, EventArgs e)
        {
            globalVars.form.UpdateSaturation((int)SaturationUpDown.Value);
        }

        private void BrightnessUpDown_ValueChanged(object sender, EventArgs e)
        {
            globalVars.form.UpdateBrightness((int)BrightnessUpDown.Value);
        }

        private void ContrastUpdown_ValueChanged(object sender, EventArgs e)
        {
            globalVars.form.UpdateContrast((int)ContrastUpdown.Value);
        }

        private void cmbAllSerailPorts_SelectedIndexChanged(object sender, EventArgs e)
        {if (cmbAllSerailPorts.Text != "")
            {
                Settings.Default.SerialPortName = cmbAllSerailPorts.Text;
                Settings.Default.Save();
            }
        }
    }
}
