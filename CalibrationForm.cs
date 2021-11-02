using CablesWindoApp3.Properties;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace CablesWindoApp3
{
    public partial class CalibrationForm : Form
    {
        public CalibrationForm()
        {
            InitializeComponent();
        }

        private void CalbrateBtn_Click(object sender, EventArgs e)
        {
            int resp =globalVars.form.calibrateSys();
            if (resp == 1)
            {
                btnSaveCalib.Enabled = true;
                lblCalibParameter.Text = globalVars.algo.mmPerPixTempProp.ToString("N5");
                lblCalibParameter.ForeColor = Color.Green;
            }
        }

        private void label1_Click(object sender, EventArgs e)
        {

        }

        private void CalibrationForm_Load(object sender, EventArgs e)
        {
           lblLastCalib.Text= Settings.Default.LastCalibrated;
            
            lblCalibParameter.Text = Settings.Default.mmPerPix.ToString();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            Settings.Default.LastCalibrated = DateTime.Now.ToString();
            globalVars.algo.mmPerPixProp = globalVars.algo.mmPerPixTempProp;
            Settings.Default.mmPerPix = globalVars.algo.mmPerPixProp;
            Settings.Default.Save();
        }

        private void CalibrationForm_Shown(object sender, EventArgs e)
        {
            btnSaveCalib.Enabled = false;
        }
    }
}
