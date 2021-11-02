using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using Npgsql;

namespace CablesWindoApp3
{
    public partial class SearchForm : Form
    {
        DataTable reportTable = new DataTable();
        CableWindowDataBaseClass db = new CableWindowDataBaseClass();
    
    public SearchForm()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {

        }
        
        void UpdateData()
        {
            DataTable dt = new DataTable();

            using (NpgsqlConnection con = db.GetConnection())
            {
                con.Open();
                string query = @"select report_no as""Report No"",date_ as ""Date"" ,timestamp_ as""Timestamp"",cabletemplate as ""CableTemplate"" from cablewindowtable";
                NpgsqlCommand cmd = new NpgsqlCommand(query, con);

                NpgsqlDataReader reader = cmd.ExecuteReader();
                if (reader.HasRows)
                {
                    dt.Load(reader);

                }

                cableDataGridView.DataSource = dt;
            }


        }

        private void SearchForm_Load(object sender, EventArgs e)
        {
            string path = @"E:\CablesWindoApp3\ReportFolder";
            DirectoryInfo obj = new DirectoryInfo(path);
            DirectoryInfo[] folders = obj.GetDirectories();     
            for (int i = 0; i < folders.Length; i++)
            {
                Console.WriteLine(folders[i].Name);
            }           

            CoreComboBox.Text = "Type of Template";
            CoreComboBox.Items.Add("CORE1");
            CoreComboBox.Items.Add("CORE2");
            CoreComboBox.Items.Add("CORE3");
            CoreComboBox.Items.Add("CORE4");
            CoreComboBox.Items.Add("CORE5");
            CoreComboBox.Items.Add("CORE6");
            //CoreComboBox.Items.Add("Multi CORE");
            //CoreComboBox.Items.Add("XLPE");
            //reportTable.Columns.Add("Report no");




            //string path = @"E:\CablesWindoApp3\ReportFolder";
            //string folder = new DirectoryInfo(path).Name;
            //Console.WriteLine(folder);

            //DirectoryInfo obj = new DirectoryInfo(path);
            //DirectoryInfo[] folders = obj.GetDirectories();

            ////Console.WriteLine("The number of directories starting with p is {0}.", dirs.Length);
            //for (int i = 0; i < folders.Length; i++)
            //{
            //    Console.WriteLine(folders[i].Name);
            //}

            UpdateData();

        }

        private void comboBox2_SelectedIndexChanged(object sender, EventArgs e)
        {

        }

        void ShowCoreWise()
        {
            DataTable dt = new DataTable();

            using (NpgsqlConnection con = db.GetConnection())
            {
                con.Open();
                string query = @"select report_no as""Report No"",date_ as ""Date"" ,timestamp_ as""Timestamp"",cabletemplate as ""CableTemplate"" from cablewindowtable where cabletemplate = @cabletemplate";

                NpgsqlCommand cmd = new NpgsqlCommand(query, con);
                cmd.Parameters.AddWithValue("@cabletemplate", CoreComboBox.SelectedItem.ToString());
                NpgsqlDataReader reader = cmd.ExecuteReader();
                if (reader.HasRows)
                {
                    dt.Load(reader);
                    cableDataGridView.DataSource = dt;

                }
                else
                {
                    MessageBox.Show("No data found for selected Core");
                }

            }
        }

        private void comboBox1_SelectedIndexChanged(object sender, EventArgs e)
        {
            ShowCoreWise();
        }

        private void textBox1_TextChanged(object sender, EventArgs e)
        {

        }

        private void pictureBox1_Click(object sender, EventArgs e)
        {

        }

        private void label2_Click(object sender, EventArgs e)
        {

        }

        private void label1_Click(object sender, EventArgs e)
        {

        }

        private void label4_Click(object sender, EventArgs e)
        {

        }

        private void label3_Click(object sender, EventArgs e)
        {

        }

        private void listView1_SelectedIndexChanged(object sender, EventArgs e)
        {

        }

        void ShowReportWise()
        {
            DataTable dt = new DataTable();

            using (NpgsqlConnection con = db.GetConnection())
            {
                con.Open();
                string query = @"select report_no as""Report No"",date_ as ""Date"" ,timestamp_ as""Timestamp"",cabletemplate as ""CableTemplate"" from cablewindowtable where report_no = @reportNum";

                NpgsqlCommand cmd = new NpgsqlCommand(query, con);
                cmd.Parameters.AddWithValue("@reportNum", Convert.ToInt32(textBox1.Text));
                NpgsqlDataReader reader = cmd.ExecuteReader();
                if (reader.HasRows)
                {
                    dt.Load(reader);
                }

                cableDataGridView.DataSource = dt;
            }
        }
        private void textBox1_KeyDown(object sender, KeyEventArgs e)
        {
            if (textBox1.Text != "" && int.TryParse(textBox1.Text, out int n) && e.KeyCode == Keys.Enter)
            {
                ShowReportWise();
                
            }
        }

        private void SearchBtn2_MouseDown(object sender, MouseEventArgs e)
        {
            ShowReportWise();
        }
    }
}
