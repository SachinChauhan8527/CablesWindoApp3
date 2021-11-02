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
    public partial class Batch : Form
    {
        public Batch()
        {
            InitializeComponent();
        }

      

        private void button1_Click(object sender, EventArgs e)
        {

        }

        private void OKBTN_Click(object sender, EventArgs e)
        {

            Form1.num = Convert.ToInt32(GetReportNotxtbox.Text);
            //Console.WriteLine(f1.num);
            
            
            Batch.ActiveForm.Close();
            //Form1.SetTextboxValue();
        }

        private void button2_Click(object sender, EventArgs e)
        {
            //Batch.ActiveForm.Close();
            //if (bch.IsDisposed == true)
            //{
            //    bch = new Batch();
            //}
            //bch.Show();
        }
    }
}
