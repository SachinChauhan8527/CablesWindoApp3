using algorithmLib;//////////////---------------------cpp
using CablesWindoApp3.Properties;
using Microsoft.Reporting.WinForms;
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
    public partial class BatchForm : Form
    {
        Class1 algo = new Class1();///////////////-----------------------cpp

        public BatchForm()
        {
            InitializeComponent();
        }
        ///----------------------------ReportParmeterCollection-------------------------//
        ReportParameterCollection rptc = new ReportParameterCollection(); 
   
        void rdlcReporttxtFun2()
        {
            string CableImgPath = globalVars.projectDirectory + @"\ReportFolder\{0}\Cable.bmp";
            string ProcessImgpath = globalVars.projectDirectory + @"ReportFolder\{0}\Process.bmp";

            //string CableImgPath = @"E:\CablesWindoApp3\ReportFolder\{0}\Cable.bmp";
            //string ProcessImgpath = @"E:\CablesWindoApp3\ReportFolder\{0}\Process.bmp";

            ReportParameter rdlcCableImg = new ReportParameter("rdlcCableImg", new Uri(string.Format(CableImgPath, Settings.Default.ReportFolderNo.ToString())).AbsoluteUri);
            BatchReportViewer.LocalReport.EnableExternalImages = true;
            BatchReportViewer.LocalReport.SetParameters(new ReportParameter[] { rdlcCableImg });

            ReportParameter rdlcProcessImg = new ReportParameter("rdlcProcessImg", new Uri(string.Format(ProcessImgpath, Settings.Default.ReportFolderNo.ToString())).AbsoluteUri);
            this.BatchReportViewer.LocalReport.EnableExternalImages = true;
            this.BatchReportViewer.LocalReport.SetParameters(new ReportParameter[] { rdlcProcessImg });
            //Me.reportViewer1.LocalReport.SetParameters(New ReportParameter() { Path})

            rptc.Add(new ReportParameter("rdlcTestNo", Convert.ToString(UserInformationForm.TestNo)));
            rptc.Add(new ReportParameter("rdlcTestIdRef", Form1.TestIdRef));
            rptc.Add(new ReportParameter("rdlcCustNa", Form1.CustomerName));
            rptc.Add(new ReportParameter("rdlcCustId", Convert.ToString(Form1.CustomerId)));
            rptc.Add(new ReportParameter("rdlcOperNa", Form1.OperatorName));
            rptc.Add(new ReportParameter("rdlcOperId", Convert.ToString(Form1.OperatorId)));
            rptc.Add(new ReportParameter("rdlcBatchNo", Convert.ToString(UserInformationForm.BatchNo)));
            rptc.Add(new ReportParameter("rdlcReportNo", Convert.ToString(Settings.Default.ReportFolderNo)));
            rptc.Add(new ReportParameter("rdlcDate", Form1.date));
            rptc.Add(new ReportParameter("rdlcTime", Form1.time));
            rptc.Add(new ReportParameter("rdlcTempNa", Form1.selButton));


            this.BatchReportViewer.LocalReport.SetParameters(rptc);
        }

        void CoreFunRdlc()
        {
            ReportDataSource rds = new ReportDataSource();
            List<batch> batchList = new List<batch>();
            batchList.Clear();


            for (int i = 0; i <Form1.ThreeCoreDataGridView.RowCount-1; i++)
            {
                batchList.Add(new batch
                {
                  
                    S_No = int.Parse(Form1.ThreeCoreDataGridView.Rows[i].Cells[0].Value.ToString()),
                    T1 = (Form1.ThreeCoreDataGridView.Rows[i].Cells[1].Value.ToString()),
                    T2 = (Form1.ThreeCoreDataGridView.Rows[i].Cells[2].Value.ToString()),
                    T3 = (Form1.ThreeCoreDataGridView.Rows[i].Cells[3].Value.ToString()),
                    T4 = (Form1.ThreeCoreDataGridView.Rows[i].Cells[4].Value.ToString()),
                    T5 = (Form1.ThreeCoreDataGridView.Rows[i].Cells[5].Value.ToString()),
                    T6 = (Form1.ThreeCoreDataGridView.Rows[i].Cells[6].Value.ToString()),
                    MinThick = (Form1.ThreeCoreDataGridView.Rows[i].Cells[7].Value.ToString()),
                    AvgThick = (Form1.ThreeCoreDataGridView.Rows[i].Cells[8].Value.ToString()),
                    OuterDiameter =(Form1.ThreeCoreDataGridView.Rows[i].Cells[9].Value.ToString()),
                    ConcentriCity = (Form1.ThreeCoreDataGridView.Rows[i].Cells[10].Value.ToString()),
                    Ovality = (Form1.ThreeCoreDataGridView.Rows[i].Cells[11].Value.ToString()),           

                });

            }

            rds.Name = "DataSet1";
            rds.Value = batchList;

            this.BatchReportViewer.LocalReport.DataSources.Clear();
            this.BatchReportViewer.LocalReport.DataSources.Add(rds);
            this.BatchReportViewer.LocalReport.ReportEmbeddedResource = "CablesWindoApp3.BatchReport.rdlc";

            //this.rdlc.LocalReport.DataSources.Clear();
            //this.rdlcReportViewer.LocalReport.DataSources.Add(rds);
            //this.rdlcReportViewer.LocalReport.ReportEmbeddedResource = "CablesWindoApp3.ThreeCore.rdlc";
        }
        void BatchRdlc()
        {
            ReportDataSource rds = new ReportDataSource();
            List<BatchClass2> Blist = new List<BatchClass2>();
            Blist.Clear();


            for (int i = 0; i < Form1.ThreeCoreDataGridView.RowCount - 1; i++)
            {
                Blist.Add(new BatchClass2
                {

                    S_No = int.Parse(Form1.ThreeCoreDataGridView.Rows[i].Cells[0].Value.ToString()),
                    T1 = Convert.ToString(Form1.ThreeCoreDataGridView.Rows[i].Cells[1].Value),
                    T2 = (Form1.ThreeCoreDataGridView.Rows[i].Cells[2].Value.ToString()),
                    T3 = (Form1.ThreeCoreDataGridView.Rows[i].Cells[3].Value.ToString()),
                    T4 = (Form1.ThreeCoreDataGridView.Rows[i].Cells[4].Value.ToString()),
                    T5 = (Form1.ThreeCoreDataGridView.Rows[i].Cells[5].Value.ToString()),
                    T6 = (Form1.ThreeCoreDataGridView.Rows[i].Cells[6].Value.ToString()),
                    MinThick = (Form1.ThreeCoreDataGridView.Rows[i].Cells[7].Value.ToString()),
                    AvgThick = (Form1.ThreeCoreDataGridView.Rows[i].Cells[8].Value.ToString()),
                    OuterDiameter = (Form1.ThreeCoreDataGridView.Rows[i].Cells[9].Value.ToString()),
                    ConcentriCity = (Form1.ThreeCoreDataGridView.Rows[i].Cells[10].Value.ToString()),
                    Ovality = (Form1.ThreeCoreDataGridView.Rows[i].Cells[11].Value.ToString()),

                });

            }

            rds.Name = "DataSet1";
            rds.Value = Blist;

            this.BatchReportViewer.LocalReport.DataSources.Clear();
            this.BatchReportViewer.LocalReport.DataSources.Add(rds);
            this.BatchReportViewer.LocalReport.ReportEmbeddedResource = "CablesWindoApp3.BatchReport.rdlc";

            //this.rdlc.LocalReport.DataSources.Clear();
            //this.rdlcReportViewer.LocalReport.DataSources.Add(rds);
            //this.rdlcReportViewer.LocalReport.ReportEmbeddedResource = "CablesWindoApp3.ThreeCore.rdlc";
        }

        private void BatchForm_Load(object sender, EventArgs e)
        {
            rdlcReporttxtFun2();
            CoreFunRdlc();
            //BatchRdlc();
            this.BatchReportViewer.RefreshReport();
        }
    }
}
