using algorithmLib;//////------------------------------cpp
using CablesWindoApp3.Properties;
using Microsoft.Reporting.WinForms;
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

namespace CablesWindoApp3
{
    public partial class Report : Form
    {
        public Report()
        {
            InitializeComponent();          

        }
        Class1 algo = new Class1();////////////////////------------------------------cppp
        ReportParameterCollection rptc = new ReportParameterCollection();
        //public static bool condition;
        ///----------------------CableTable-------------------------------------------//

       

        void rdlcReporttxtFun()
        {
            //globalVars.projectDirectory +
            //string CableImgPath = @"E:\CablesWindoApp3\ReportFolder\{0}\Cable.bmp";
            //string ProcessImgpath = @"E:\CablesWindoApp3\ReportFolder\{0}\Process.bmp";
            string CableImgPath = globalVars.projectDirectory + @"\ReportFolder\{0}\Cable.bmp";
            string ProcessImgpath = globalVars.projectDirectory + @"ReportFolder\{0}\Process.bmp";

            ReportParameter rdlcCableImg = new ReportParameter("rdlcCableImg", new Uri(string.Format(CableImgPath, Settings.Default.ReportFolderNo.ToString())).AbsoluteUri);
            reportViewer1.LocalReport.EnableExternalImages = true;
            reportViewer1.LocalReport.SetParameters(new ReportParameter[] { rdlcCableImg });

            ReportParameter rdlcProcessImg = new ReportParameter("rdlcProcessImg", new Uri(string.Format(ProcessImgpath, Settings.Default.ReportFolderNo.ToString())).AbsoluteUri);
            this.reportViewer1.LocalReport.EnableExternalImages = true;
            this.reportViewer1.LocalReport.SetParameters(new ReportParameter[] { rdlcProcessImg });
            //Me.reportViewer1.LocalReport.SetParameters(New ReportParameter() { Path})

            rptc.Add(new ReportParameter("rdlcTestNo", Convert.ToString(Settings.Default.TestNo)));
            rptc.Add(new ReportParameter("rdlcTestIdRef",Form1.TestIdRef));
            rptc.Add(new ReportParameter("rdlcCustNa", Form1.CustomerName));
            rptc.Add(new ReportParameter("rdlcCustId", Convert.ToString(Form1.CustomerId)));
            rptc.Add(new ReportParameter("rdlcOperNa", Form1.OperatorName));
            rptc.Add(new ReportParameter("rdlcOperId", Convert.ToString(Form1.OperatorId)));
            rptc.Add(new ReportParameter("rdlcBatchNo", Convert.ToString(UserInformationForm.BatchNo)));
            rptc.Add(new ReportParameter("rdlcReportNo", Convert.ToString(Settings.Default.ReportFolderNo)));
            rptc.Add(new ReportParameter("rdlcDate",Form1.date));
            rptc.Add(new ReportParameter("rdlcTime",Form1.time));
            rptc.Add(new ReportParameter("rdlcTempNa", Form1.selButton));


            this.reportViewer1.LocalReport.SetParameters(rptc);
        }
        void rdlcReporttxtFun2()
        {

            string CableImgPath = globalVars.projectDirectory + @"\ReportFolder\{0}\Cable.bmp";
            string ProcessImgpath = globalVars.projectDirectory + @"ReportFolder\{0}\Process.bmp";

            //string CableImgPath = @"E:\CablesWindoApp3\ReportFolder\{0}\Cable.bmp";
            //string ProcessImgpath = @"E:\CablesWindoApp3\ReportFolder\{0}\Process.bmp";

            ReportParameter rdlcCableImg = new ReportParameter("rdlcCableImg", new Uri(string.Format(CableImgPath, Settings.Default.ReportFolderNo.ToString())).AbsoluteUri);
            reportViewer1.LocalReport.EnableExternalImages = true;
            reportViewer1.LocalReport.SetParameters(new ReportParameter[] { rdlcCableImg });

            ReportParameter rdlcProcessImg = new ReportParameter("rdlcProcessImg", new Uri(string.Format(ProcessImgpath, Settings.Default.ReportFolderNo.ToString())).AbsoluteUri);
            this.reportViewer1.LocalReport.EnableExternalImages = true;
            this.reportViewer1.LocalReport.SetParameters(new ReportParameter[] { rdlcProcessImg });
            //Me.reportViewer1.LocalReport.SetParameters(New ReportParameter() { Path})

            rptc.Add(new ReportParameter("rdlcTestNo", Convert.ToString(UserInformationForm.TestNo)));
            rptc.Add(new ReportParameter("rdlcTestIdRef", Form1.TestIdRef));
            rptc.Add(new ReportParameter("rdlcCustNa", Form1.CustomerName));
            rptc.Add(new ReportParameter("rdlcCustId", Convert.ToString(Form1.CustomerId)));
            rptc.Add(new ReportParameter("rdlcOperNa", Form1.OperatorName));
            rptc.Add(new ReportParameter("rdlcOperId", Convert.ToString(Form1.OperatorId)));
            rptc.Add(new ReportParameter("rdlcBatchNo", Convert.ToString(UserInformationForm.BatchNo)));
            rptc.Add(new ReportParameter("rdlcReportNo", Convert.ToString(Settings.Default.ReportFolderNo+"/"+Form1.SubReportNo)));
            rptc.Add(new ReportParameter("rdlcDate", Form1.date));
            rptc.Add(new ReportParameter("rdlcTime", Form1.time));
            rptc.Add(new ReportParameter("rdlcTempNa", Form1.selButton));


            this.reportViewer1.LocalReport.SetParameters(rptc);
        }

        void CoreFunRdlc()
        {
            ReportDataSource rds = new ReportDataSource();
            List<Cable> cblst = new List<Cable>();
            cblst.Clear();


            ////for (int i = 0; i <7; i++)
            //{
            cblst.Add(new Cable
            {
                T1 = algo.getSectorThickness(0).ToString("N3"),////////////////////-------------------------------cpp
                T2 = algo.getSectorThickness(1).ToString("N3"),
                T3 = algo.getSectorThickness(2).ToString("N3"),
                T4 = algo.getSectorThickness(3).ToString("N3"),
                T5 = algo.getSectorThickness(4).ToString("N3"),
                T6 = algo.getSectorThickness(5).ToString("N3"),
                OutDiam = algo.outerDiaProp.ToString("N3"),
                OutDiamMax = algo.outerDiaProp.ToString("N3"),
                OutDiamMin = algo.outerDiaProp.ToString("N3"),
                MinThick = algo.minThicknessProp.ToString("N3"),
                MaxThick = algo.minThicknessProp.ToString("N3"),
                Concentricity = algo.concentricityProp.ToString("N3"),
                Area = algo.concentricityProp.ToString("N3"),
                TotalLengthY = algo.ovalityProp.ToString("N3"),
                LengthC = algo.concentricityProp.ToString("N3"),
                AvgThick = algo.avgThicknessProp.ToString("N3"),
                Ovality = algo.ovalityProp.ToString("N3"),/////////////////-------------------------------cpp

            });

            ////}

            rds.Name = "DataSet1";
            rds.Value = cblst;

            this.reportViewer1.LocalReport.DataSources.Clear();
            this.reportViewer1.LocalReport.DataSources.Add(rds);
            this.reportViewer1.LocalReport.ReportEmbeddedResource = "CablesWindoApp3.ThreeCore.rdlc";

            //this.rdlc.LocalReport.DataSources.Clear();
            //this.rdlcReportViewer.LocalReport.DataSources.Add(rds);
            //this.rdlcReportViewer.LocalReport.ReportEmbeddedResource = "CablesWindoApp3.ThreeCore.rdlc";
        }
     
        void Messagebox()
        {
            string title = "Report Save Form";
            string Message = "Report Saved Successfully={0}" + Settings.Default.ReportFolderNo.ToString() + ".pdf";
            MessageBoxButtons buttons = MessageBoxButtons.YesNo;
            DialogResult result = MessageBox.Show(Message, title, buttons);
            if (result == DialogResult.OK)
            {
                //CreateReportbtn_Click();               
                this.Close();
            }
            else
            {
                this.Close();
            }
        }

        //void Batch()
        //{
        //    List<batch> BatchList = new List<batch>();
        //    BatchList.Clear();
        //    for (int i = 0; i < rdlcDgv.Rows.Count - 1; i++)
        //    {
        //        BatchList.Add(new batch
        //        {
        //            ReportNo = int.Parse(rdlcDgv.Rows[i].Cells[0].Value.ToString()),
        //            T1 = float.Parse(rdlcDgv.Rows[i].Cells[1].Value.ToString()),
        //            T2 = float.Parse(rdlcDgv.Rows[i].Cells[1].Value.ToString()),
        //            T3 = float.Parse(rdlcDgv.Rows[i].Cells[1].Value.ToString()),

        //        });
        //        rds.Name = "DataSet2";
        //        rds.Value = BatchList;
        //        reportViewer1.LocalReport.DataSources.Clear();
        //        reportViewer1.LocalReport.DataSources.Add(rds);
        //        reportViewer1.LocalReport.ReportEmbeddedResource = "WindowsFormsApp1.Report1.rdlc";
        //        reportViewer1.RefreshReport();
        //    }
        //}

        private void Report_Load(object sender, EventArgs e)
        {
           
            if (Form1.SubReportNo <= Form1.num)
            {
                rdlcReporttxtFun2();
                CoreFunRdlc();
            }
            rdlcReporttxtFun();
            CoreFunRdlc();

            this.reportViewer1.RefreshReport();

           
        }

        private void reportViewer1_Load(object sender, EventArgs e)
        {

        }    

        private void button1_Click(object sender, EventArgs e)
        {
          
        }

        private void Report_FormClosed(object sender, FormClosedEventArgs e)
        {
            //Settings.Default.ReportFolderNo++;
            ////Settings.Default.rptNo++;
            //Settings.Default.Save();
            //Settings.Default.TestNo++;
            //Settings.Default.Save();

        }

        private void SavePdf_KeyDown(object sender, KeyEventArgs e)
        {
           
            
        }

        private void SavePdf_MouseDown(object sender, MouseEventArgs e)
        {
            //Messagebox();
            //CreateReportbtn_Click();


        }

        private void SavePdf_Click(object sender, EventArgs e)
        {

        }

        private void button1_Click_1(object sender, EventArgs e)
        {
           
        }
    }
}
