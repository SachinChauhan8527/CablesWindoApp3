using System;
using System.Collections.Generic;
using System.Data;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using Npgsql;

namespace CablesWindoApp3
{
    public class CableWindowDataBaseClass
    {
        public NpgsqlConnection GetConnection()
        {
            return new NpgsqlConnection(@"Server=localhost; Port = 5432; user Id = postgres; password = 1234; Database = CableWindowDatabase;");
        }
        public void TestConnection()
        {
            using (NpgsqlConnection con = GetConnection())
            {
                con.Open();
                if (con.State != ConnectionState.Open)
                {
                    MessageBox.Show("I'm lost ....");
                }
                else
                {
                    MessageBox.Show("Connected");

                }
            }
        }
        public void InsertRecord(int ReportNo, DateTime Date, string Time, string CableTemplate)
        {
            using (NpgsqlConnection con = GetConnection())
            {

                string query = @"insert into public.CableWindowTable(Report_No,Date_, TimeStamp_, CableTemplate)
                               values(@ReportNo, @Date, @Time, @CableTemplate)";

                NpgsqlCommand cmd = new NpgsqlCommand(query, con);

                //cmd.Parameters.AddWithValue("@Serial_No", Serial_No);
                cmd.Parameters.AddWithValue("@ReportNo", ReportNo);
                cmd.Parameters.AddWithValue("@Date", Date);
                cmd.Parameters.AddWithValue("@Time", Time);
                cmd.Parameters.AddWithValue("@CableTemplate", CableTemplate);

                con.Open();
                int n = cmd.ExecuteNonQuery();
            }
        }
        //public void ReadData()
        //{
        //    using (NpgsqlConnection con = db.GetConnection())
        //    {
        //        con.Open();
        //        string query = @"select _time from chartdata where _date = @date";
        //        NpgsqlCommand cmd = new NpgsqlCommand(query, con);
        //        cmd.Parameters.AddWithValue("@date", Convert.ToDateTime(userDate));

        //        NpgsqlDataReader reader = cmd.ExecuteReader();
        //        while (reader.Read())
        //        {
        //            comboBox1.Items.Add(reader[0]);
        //            comboBox2.Items.Add(reader[0]);
        //        }
        //    }
        //}





    }
}
