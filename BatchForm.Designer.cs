
namespace CablesWindoApp3
{
    partial class BatchForm
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.BatchReportViewer = new Microsoft.Reporting.WinForms.ReportViewer();
            this.SuspendLayout();
            // 
            // BatchReportViewer
            // 
            this.BatchReportViewer.LocalReport.ReportEmbeddedResource = "CablesWindoApp3.BatchReport.rdlc";
            this.BatchReportViewer.Location = new System.Drawing.Point(12, 21);
            this.BatchReportViewer.Name = "BatchReportViewer";
            this.BatchReportViewer.ServerReport.BearerToken = null;
            this.BatchReportViewer.Size = new System.Drawing.Size(728, 669);
            this.BatchReportViewer.TabIndex = 0;
            // 
            // BatchForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(752, 703);
            this.Controls.Add(this.BatchReportViewer);
            this.Name = "BatchForm";
            this.Text = "BatchForm";
            this.Load += new System.EventHandler(this.BatchForm_Load);
            this.ResumeLayout(false);

        }

        #endregion

        public Microsoft.Reporting.WinForms.ReportViewer BatchReportViewer;
    }
}