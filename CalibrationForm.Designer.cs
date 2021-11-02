
namespace CablesWindoApp3
{
    partial class CalibrationForm
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
            this.CalbrateBtn = new System.Windows.Forms.Button();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.lblLastCalib = new System.Windows.Forms.Label();
            this.lblCalibParameter = new System.Windows.Forms.Label();
            this.btnSaveCalib = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // CalbrateBtn
            // 
            this.CalbrateBtn.Location = new System.Drawing.Point(52, 110);
            this.CalbrateBtn.Name = "CalbrateBtn";
            this.CalbrateBtn.Size = new System.Drawing.Size(158, 34);
            this.CalbrateBtn.TabIndex = 0;
            this.CalbrateBtn.Text = "Calibrate System";
            this.CalbrateBtn.UseVisualStyleBackColor = true;
            this.CalbrateBtn.Click += new System.EventHandler(this.CalbrateBtn_Click);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(38, 38);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(88, 13);
            this.label1.TabIndex = 1;
            this.label1.Text = "Last Calibration  :";
            this.label1.Click += new System.EventHandler(this.label1_Click);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(13, 73);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(113, 13);
            this.label2.TabIndex = 2;
            this.label2.Text = "Calibration Parameter :";
            // 
            // lblLastCalib
            // 
            this.lblLastCalib.AutoSize = true;
            this.lblLastCalib.Location = new System.Drawing.Point(142, 38);
            this.lblLastCalib.Name = "lblLastCalib";
            this.lblLastCalib.Size = new System.Drawing.Size(22, 13);
            this.lblLastCalib.TabIndex = 3;
            this.lblLastCalib.Text = "NA";
            // 
            // lblCalibParameter
            // 
            this.lblCalibParameter.AutoSize = true;
            this.lblCalibParameter.Location = new System.Drawing.Point(142, 73);
            this.lblCalibParameter.Name = "lblCalibParameter";
            this.lblCalibParameter.Size = new System.Drawing.Size(28, 13);
            this.lblCalibParameter.TabIndex = 4;
            this.lblCalibParameter.Text = "0.00";
            // 
            // btnSaveCalib
            // 
            this.btnSaveCalib.Location = new System.Drawing.Point(52, 150);
            this.btnSaveCalib.Name = "btnSaveCalib";
            this.btnSaveCalib.Size = new System.Drawing.Size(158, 34);
            this.btnSaveCalib.TabIndex = 5;
            this.btnSaveCalib.Text = "Save Parameters";
            this.btnSaveCalib.UseVisualStyleBackColor = true;
            this.btnSaveCalib.Click += new System.EventHandler(this.button1_Click);
            // 
            // CalibrationForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(268, 213);
            this.Controls.Add(this.btnSaveCalib);
            this.Controls.Add(this.lblCalibParameter);
            this.Controls.Add(this.lblLastCalib);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.CalbrateBtn);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.Name = "CalibrationForm";
            this.Text = "System Calibration";
            this.Load += new System.EventHandler(this.CalibrationForm_Load);
            this.Shown += new System.EventHandler(this.CalibrationForm_Shown);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button CalbrateBtn;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label lblLastCalib;
        private System.Windows.Forms.Label lblCalibParameter;
        private System.Windows.Forms.Button btnSaveCalib;
    }
}