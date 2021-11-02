
namespace CablesWindoApp3
{
    partial class SetttingFormcs
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
            this.ExposureTimeSetUpdaown = new System.Windows.Forms.NumericUpDown();
            this.GainUpdown = new System.Windows.Forms.NumericUpDown();
            this.ExposureTimeLbl = new System.Windows.Forms.Label();
            this.GainLbl = new System.Windows.Forms.Label();
            this.SaturationLbl = new System.Windows.Forms.Label();
            this.SaturationUpDown = new System.Windows.Forms.NumericUpDown();
            this.BrightnessLbl = new System.Windows.Forms.Label();
            this.BrightnessUpDown = new System.Windows.Forms.NumericUpDown();
            this.ContrastLbl = new System.Windows.Forms.Label();
            this.ContrastUpdown = new System.Windows.Forms.NumericUpDown();
            this.pictureBox1 = new System.Windows.Forms.PictureBox();
            this.cmbAllSerailPorts = new System.Windows.Forms.ComboBox();
            this.label1 = new System.Windows.Forms.Label();
            ((System.ComponentModel.ISupportInitialize)(this.ExposureTimeSetUpdaown)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.GainUpdown)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.SaturationUpDown)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.BrightnessUpDown)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.ContrastUpdown)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).BeginInit();
            this.SuspendLayout();
            // 
            // ExposureTimeSetUpdaown
            // 
            this.ExposureTimeSetUpdaown.Location = new System.Drawing.Point(23, 107);
            this.ExposureTimeSetUpdaown.Name = "ExposureTimeSetUpdaown";
            this.ExposureTimeSetUpdaown.Size = new System.Drawing.Size(181, 20);
            this.ExposureTimeSetUpdaown.TabIndex = 0;
            this.ExposureTimeSetUpdaown.ValueChanged += new System.EventHandler(this.ExposureTimeSetUpdaown_Click);
            this.ExposureTimeSetUpdaown.Click += new System.EventHandler(this.ExposureTimeSetUpdaown_Click);
            // 
            // GainUpdown
            // 
            this.GainUpdown.Location = new System.Drawing.Point(23, 158);
            this.GainUpdown.Name = "GainUpdown";
            this.GainUpdown.Size = new System.Drawing.Size(181, 20);
            this.GainUpdown.TabIndex = 1;
            this.GainUpdown.ValueChanged += new System.EventHandler(this.GainUpdown_ValueChanged);
            // 
            // ExposureTimeLbl
            // 
            this.ExposureTimeLbl.AutoSize = true;
            this.ExposureTimeLbl.Location = new System.Drawing.Point(20, 92);
            this.ExposureTimeLbl.Name = "ExposureTimeLbl";
            this.ExposureTimeLbl.Size = new System.Drawing.Size(74, 13);
            this.ExposureTimeLbl.TabIndex = 2;
            this.ExposureTimeLbl.Text = "ExposureTime";
            // 
            // GainLbl
            // 
            this.GainLbl.AutoSize = true;
            this.GainLbl.Location = new System.Drawing.Point(22, 143);
            this.GainLbl.Name = "GainLbl";
            this.GainLbl.Size = new System.Drawing.Size(29, 13);
            this.GainLbl.TabIndex = 3;
            this.GainLbl.Text = "Gain";
            this.GainLbl.Click += new System.EventHandler(this.label1_Click);
            // 
            // SaturationLbl
            // 
            this.SaturationLbl.AutoSize = true;
            this.SaturationLbl.Location = new System.Drawing.Point(24, 193);
            this.SaturationLbl.Name = "SaturationLbl";
            this.SaturationLbl.Size = new System.Drawing.Size(55, 13);
            this.SaturationLbl.TabIndex = 5;
            this.SaturationLbl.Text = "Saturation";
            // 
            // SaturationUpDown
            // 
            this.SaturationUpDown.Location = new System.Drawing.Point(25, 208);
            this.SaturationUpDown.Name = "SaturationUpDown";
            this.SaturationUpDown.Size = new System.Drawing.Size(181, 20);
            this.SaturationUpDown.TabIndex = 4;
            this.SaturationUpDown.ValueChanged += new System.EventHandler(this.SaturationUpDown_ValueChanged);
            // 
            // BrightnessLbl
            // 
            this.BrightnessLbl.AutoSize = true;
            this.BrightnessLbl.Location = new System.Drawing.Point(22, 244);
            this.BrightnessLbl.Name = "BrightnessLbl";
            this.BrightnessLbl.Size = new System.Drawing.Size(56, 13);
            this.BrightnessLbl.TabIndex = 7;
            this.BrightnessLbl.Text = "Brightness";
            // 
            // BrightnessUpDown
            // 
            this.BrightnessUpDown.Location = new System.Drawing.Point(23, 259);
            this.BrightnessUpDown.Name = "BrightnessUpDown";
            this.BrightnessUpDown.Size = new System.Drawing.Size(181, 20);
            this.BrightnessUpDown.TabIndex = 6;
            this.BrightnessUpDown.ValueChanged += new System.EventHandler(this.BrightnessUpDown_ValueChanged);
            // 
            // ContrastLbl
            // 
            this.ContrastLbl.AutoSize = true;
            this.ContrastLbl.Location = new System.Drawing.Point(25, 299);
            this.ContrastLbl.Name = "ContrastLbl";
            this.ContrastLbl.Size = new System.Drawing.Size(46, 13);
            this.ContrastLbl.TabIndex = 9;
            this.ContrastLbl.Text = "Contrast";
            // 
            // ContrastUpdown
            // 
            this.ContrastUpdown.Location = new System.Drawing.Point(24, 314);
            this.ContrastUpdown.Name = "ContrastUpdown";
            this.ContrastUpdown.Size = new System.Drawing.Size(181, 20);
            this.ContrastUpdown.TabIndex = 8;
            this.ContrastUpdown.ValueChanged += new System.EventHandler(this.ContrastUpdown_ValueChanged);
            // 
            // pictureBox1
            // 
            this.pictureBox1.Anchor = System.Windows.Forms.AnchorStyles.None;
            this.pictureBox1.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(239)))), ((int)(((byte)(248)))), ((int)(((byte)(253)))));
            this.pictureBox1.Image = global::CablesWindoApp3.Properties.Resources.EyeVision_Logo;
            this.pictureBox1.Location = new System.Drawing.Point(-1, 21);
            this.pictureBox1.Name = "pictureBox1";
            this.pictureBox1.Size = new System.Drawing.Size(259, 53);
            this.pictureBox1.SizeMode = System.Windows.Forms.PictureBoxSizeMode.Zoom;
            this.pictureBox1.TabIndex = 87;
            this.pictureBox1.TabStop = false;
            // 
            // cmbAllSerailPorts
            // 
            this.cmbAllSerailPorts.FormattingEnabled = true;
            this.cmbAllSerailPorts.Location = new System.Drawing.Point(23, 369);
            this.cmbAllSerailPorts.Name = "cmbAllSerailPorts";
            this.cmbAllSerailPorts.Size = new System.Drawing.Size(181, 21);
            this.cmbAllSerailPorts.TabIndex = 88;
            this.cmbAllSerailPorts.SelectedIndexChanged += new System.EventHandler(this.cmbAllSerailPorts_SelectedIndexChanged);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(24, 353);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(101, 13);
            this.label1.TabIndex = 89;
            this.label1.Text = "Communication Port";
            // 
            // SetttingFormcs
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(257, 402);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.cmbAllSerailPorts);
            this.Controls.Add(this.pictureBox1);
            this.Controls.Add(this.ContrastLbl);
            this.Controls.Add(this.ContrastUpdown);
            this.Controls.Add(this.BrightnessLbl);
            this.Controls.Add(this.BrightnessUpDown);
            this.Controls.Add(this.SaturationLbl);
            this.Controls.Add(this.SaturationUpDown);
            this.Controls.Add(this.GainLbl);
            this.Controls.Add(this.ExposureTimeLbl);
            this.Controls.Add(this.GainUpdown);
            this.Controls.Add(this.ExposureTimeSetUpdaown);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedToolWindow;
            this.Name = "SetttingFormcs";
            this.Text = "SetttingFormcs";
            this.Load += new System.EventHandler(this.SetttingFormcs_Load);
            ((System.ComponentModel.ISupportInitialize)(this.ExposureTimeSetUpdaown)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.GainUpdown)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.SaturationUpDown)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.BrightnessUpDown)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.ContrastUpdown)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.NumericUpDown ExposureTimeSetUpdaown;
        private System.Windows.Forms.NumericUpDown GainUpdown;
        private System.Windows.Forms.Label ExposureTimeLbl;
        private System.Windows.Forms.Label GainLbl;
        private System.Windows.Forms.Label SaturationLbl;
        private System.Windows.Forms.NumericUpDown SaturationUpDown;
        private System.Windows.Forms.Label BrightnessLbl;
        private System.Windows.Forms.NumericUpDown BrightnessUpDown;
        private System.Windows.Forms.Label ContrastLbl;
        private System.Windows.Forms.NumericUpDown ContrastUpdown;
        private System.Windows.Forms.PictureBox pictureBox1;
        private System.Windows.Forms.ComboBox cmbAllSerailPorts;
        private System.Windows.Forms.Label label1;
    }
}