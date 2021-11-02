using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using CablesWindoApp3.Properties;

namespace CablesWindoApp3
{
   
    public partial class UserInformationForm : Form
    {
       
        public UserInformationForm()
        {
            InitializeComponent();
           
            //userInformation();
        }

        //Default value in Textbox Method
       public void DefaultValue()
       {
            ReportNoTxtBox.Text = Settings.Default.ReportFolderNo.ToString();
            BatchNoTxtBox.Text  = Settings.Default.ReportFolderNo.ToString();
            TestNoTxtBox.Text = Settings.Default.TestNo.ToString(); 
            TestIdRefTxtBox.Text = Settings.Default.TestNo.ToString(); ;
            CustIdTxtBox.Text = Convert.ToString(00);
            OperIdTxtBox.Text =Convert.ToString(00);
            //TempNaTxtBox.Text = Form1.selButton;
            CustNaTxtBox.Text="Customer";
            OperNaTxtBox.Text = "Operator";
            DateTxtBox.Text =Form1.date;
            TimeTxtBox.Text =Form1.time;

            //ReportNoTxtBox.Text = Form1.selButton;
            //BatchNoTxtBox.Text = 
            //CustNaTxtBox.Text = "Operator Name";
            //CustIdTxtBox.Text = Convert.ToString(0000);
            //OperNaTxtBox.Text = "Customer Name";
            //TempNaTxtBox.Text = "Customer";
            //OperIdTxtBox.Text = "Address";
        }


        //User Information variable
        public static int ReportNo;
        public static int BatchNo;
        public static int TestNo;
        public static int TestIdRef;
        public static int CustomerId;
        public static int OperatorId;      
        //public static string TemplateName;
        public static string CustomerName;
        public static string OperatorName;
        public static string Date;
        public static string Time;

        //public static string TemplateName;
        //public static int PartNo;
        //public static string OperatorName;
        //public static int OperatorId;
        //public static string CustomerName;
        //public static string CustomerRef;
        //public static string Address;

        public void userInformation()
       {

            ReportNo = Convert.ToInt32(ReportNoTxtBox.Text);
            BatchNo = Convert.ToInt32(BatchNoTxtBox.Text);
            TestNo= Convert.ToInt32(TestNoTxtBox.Text);
            TestIdRef = Convert.ToInt32(TestIdRefTxtBox.Text);
            CustomerId = Convert.ToInt32(CustIdTxtBox.Text);
            OperatorId = Convert.ToInt32(OperIdTxtBox.Text);
            //TemplateName = TempNaTxtBox.Text;
            CustomerName = CustNaTxtBox.Text;
            OperatorName = OperNaTxtBox.Text;
            Date = DateTxtBox.Text;
            Time = TimeTxtBox.Text;

        //TemplateName = ReportNoTxtBox.Text;
        //PartNo = Convert.ToInt32(BatchNoTxtBox.Text);
        //OperatorName = CustNaTxtBox.Text;
        //OperatorId = Convert.ToInt32(CustIdTxtBox.Text);
        //CustomerName = OperNaTxtBox.Text;
        //CustomerRef = TempNaTxtBox.Text;
        //Address = OperIdTxtBox.Text;

    }
    private void UserInformationForm_Load(object sender, EventArgs e)
        {
            DefaultValue();
            
        }

        private void Okbtn_Click(object sender, EventArgs e)
        {
             userInformation();
            UserInformationForm.ActiveForm.Hide();
        }

        private void Cancelbtn_Click(object sender, EventArgs e)
        {

            UserInformationForm.ActiveForm.Hide();
        }

        private void tableLayoutPanel1_Paint(object sender, PaintEventArgs e)
        {

        }

        private void label5_Click(object sender, EventArgs e)
        {

        }

        private void label7_Click(object sender, EventArgs e)
        {

        }

        private void label4_Click(object sender, EventArgs e)
        {

        }

        private void label3_Click(object sender, EventArgs e)
        {

        }

        private void label6_Click(object sender, EventArgs e)
        {

        }

        private void label10_Click(object sender, EventArgs e)
        {

        }

        private void textBox3_TextChanged(object sender, EventArgs e)
        {

        }

        private void TestNoTxtBox_TextChanged(object sender, EventArgs e)
        {

        }
    }
  
}
