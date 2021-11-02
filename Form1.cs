using System;
using System.Drawing;
using System.Windows.Forms;
using Microsoft.Reporting.WinForms;
using System.Drawing.Imaging;
using System.Data;
using System.Collections.Generic;
using System.Threading.Tasks;
using System.Threading;
using System.Drawing.Drawing2D; //DashStyle Library
using System.IO;
using CablesWindoApp3.Properties;
using algorithmLib;     ////-------------------------CPP
using System.Collections;
using CablesWindoApp3.Classes;
using Newtonsoft.Json;
using Npgsql;

namespace CablesWindoApp3
{
    public partial class Form1 : Form
    {

        //----------toupCamCode-----------//
        private delegate void DelegateEvent(Toupcam.eEVENT[] ev);
        private Toupcam cam_ = null;
        private Bitmap bmp_ = null;
        Bitmap btmp;
        private DelegateEvent ev_ = null;

        //---------------- Variables ----------------------------//
        int OutTempCaptur = 4;
        public static int num;
        public static int SubReportNo = 1;
        public static string selButton = null;
        string selectedTemplate = "";
        public bool but = false;
        bool formClose = false;


        //---------------- Image Variables ----------------------------//
        Bitmap bmpImg;
        Bitmap procImage;
        Bitmap algoImage;

        //-------------XLPE------------------------//
       XP xp = new XP();

        //----------------------------------Table Create -----------------------------------//
        DataTable BatchTable = new DataTable();


        //------------------------------------------------------USERINFO VARIABLES----------
        public static int ReportNo;
        public static int BatchNo;
        public static int TestNo;
        public static string TestIdRef;
        public static string CustomerId;
        public static string OperatorId;
        //public static string TemplateName;
        public static string CustomerName;
        public static string OperatorName;
        public static string date;
        public static string time;


        //--------------------------Repoerts Variables----------------------------------------//
        string PdfpathFolder;
        string ExcelpathFolder;

        //--------------------------------Crop Variables ------------------------------//
        int cropX;
        int cropY;
        int cropWidth;
        int cropHeight;
        //int oCropX;
        //int oCropY;
        public Pen cropPen;
        public DashStyle cropDashStyle = DashStyle.DashDot;

        //-----------------------Ext--------------------------------------//
        private bool process;
        private object ckhMonoColor;

        //------------------Create DataGrid View---------------------------------//
        public static DataGridView ThreeCoreDataGridView = new DataGridView();


        //GlobalVariables _______________________________________________
       
        //-----------------------------Light Variables
        bool topLightState = false;
        bool bottomLightState = false;

        //---------------- Class Objects ------------------------//
        //      Class1 algo = new Class1();////---------------------------------------CPP
        //cameraFns camController = new cameraFns();
        //BaumerCam c0 = new BaumerCam();
        UserInformationClass uic = new UserInformationClass();
        //UserInformationForm uif = new UserInformationForm();
        Report rpt = new Report();
        SearchForm SrForm = new SearchForm();
        BatchForm btf = new BatchForm();
        SetttingFormcs SForm = new SetttingFormcs();
        CableWindowDataBaseClass d = new CableWindowDataBaseClass();
        CalibrationForm clForm = new CalibrationForm();
        ReportParameterCollection rptc = new ReportParameterCollection();

        //-------------------Array--------------------//
        Button[] btnArray;        //-------------------Core Button Array-------------------//
        Label[] LblArray;

        Button button;             // Core Buttons Global Variable-------------//
                                   //------------------------arduino light comm-----------------
        private void connectSerialPort(String portName)
        {
            try
            {
                ser_port_light.PortName = portName;
                if (!(ser_port_light.IsOpen))
                {
                    ser_port_light.Open();

                }
                setLight("all", false);
                enableDisableLightButtons(true);
            }
            catch (Exception Exx)
            {
                MessageBox.Show("Check USB cable");
                enableDisableLightButtons(false);
            }



        }
        bool enableDisableLightButtons(bool connectionStatus)
        {
            if (connectionStatus)
            {
                TopLight.Enabled = true;
                TopLight.Enabled = true;
                return true;
            }
            else
            {
                BottomLight.Enabled = false;
                BottomLight.Enabled = false;
                return false;
            }

        }
        private int setLight(string lightName, bool state) //lightname top bottom all
        {
            try
            {
                if (!(ser_port_light.IsOpen))
                {
                    ser_port_light.Open();
                }
                if (lightName == "top")
                {
                    if (state)
                    {
                        ser_port_light.Write("t_on");
                        ser_port_light.Write("\n");

                    }
                    else
                    {
                        ser_port_light.Write("t_off");
                        ser_port_light.Write("\n");

                    }
                    Console.WriteLine("top " + state.ToString());
                }
                if (lightName == "bottom")
                {
                    if (state)
                    {
                        ser_port_light.Write("b_on");
                        ser_port_light.Write("\n");
                    }
                    else
                    {
                        ser_port_light.Write("b_off");
                        ser_port_light.Write("\n");
                    }
                    Console.WriteLine("bottom " + state.ToString());
                }
                if (lightName == "all")
                {
                    if (state)
                    {
                        ser_port_light.Write("all_on");
                        ser_port_light.Write("\n");
                    }
                    else
                    {
                        ser_port_light.Write("all_off");
                        ser_port_light.Write("\n");
                    }
                    Console.WriteLine("all " + state.ToString());
                }
                return 1;
            }
            catch (Exception exx)
            {
                return 0;

            }
        }

        //--------------------------------------------------------------

        //----------CamEvents-----------//

        private void OnEventError()
        {
            if (cam_ != null)
            {
                cam_.Close();
                cam_ = null;
            }
            CameraStatus.Image = Bitmap.FromFile("E:/CablesWindoApp3/buttonstosizefinal/CameraStatus.png");
            MessageBox.Show("Error");

        }

        private void OnEventDisconnected()
        {
            if (cam_ != null)
            {
                cam_.Close();
                cam_ = null;
            }
            CameraStatus.Image = Bitmap.FromFile("E:/CablesWindoApp3/buttonstosizefinal/CameraStatus.png");
            MessageBox.Show("The camera is disconnected, maybe has been pulled out.");

        }

        private void OnEventExposure()
        {
            if (cam_ != null)
            {
                uint nTime = 0;
                if (cam_.get_ExpoTime(out nTime))
                {
                    //Core5Lbl.Text = (nTime / 1000).ToString() + " ms";
                }
            }
        }

     

        private void OnEventImage()
        {
            if (bmp_ != null)
            {
                BitmapData bmpdata = bmp_.LockBits(new Rectangle(0, 0, bmp_.Width, bmp_.Height), ImageLockMode.WriteOnly, bmp_.PixelFormat);

                Toupcam.FrameInfoV2 info = new Toupcam.FrameInfoV2();
                cam_.PullImageV2(bmpdata.Scan0, 24, out info);

                bmp_.UnlockBits(bmpdata);
                if (process)
                {
                    bmpImg = (Bitmap)bmp_.Clone();
                    bmpImg.Save(DateTime.Now.ToString("DD_MM_mm_ss") + "snap.bmp");
                    process = false;
                    //globalVars.algo.getMeasurements4c(bmpImg);
                    //pictureBox2.Image = bmpImg;
                    //  pictureBox2.Invalidate();
                    //pictureBox2.Refresh();
                    //lblAvgThick.Text = globalVars.algo.avgThicknessProp.ToString("N3");
                    //lblMinthick.Text = globalVars.algo.minThicknessProp.ToString("N3");
                    //lblOuterDia.Text = globalVars.algo.outerDiaProp.ToString("N3");
                    //lblConcentricity.Text = globalVars.algo.concentricityProp.ToString("N3");

                }
                if (formClose == false)
                {
                    PicBox1.Image = bmp_;

                    PicBox1.Invalidate();
                }

            }
        }

        private void OnEventStillImage()
        {
            Toupcam.FrameInfoV2 info = new Toupcam.FrameInfoV2(); ;
            if (cam_.PullStillImageV2(IntPtr.Zero, 24, out info))   /* peek the width and height */
            {
                Bitmap sbmp = new Bitmap((int)info.width, (int)info.height, PixelFormat.Format24bppRgb);

                BitmapData bmpdata = sbmp.LockBits(new Rectangle(0, 0, sbmp.Width, sbmp.Height), ImageLockMode.WriteOnly, sbmp.PixelFormat);
                cam_.PullStillImageV2(bmpdata.Scan0, 24, out info);
                sbmp.UnlockBits(bmpdata);

                sbmp.Save("demowinformcs2.jpg");
            }
        }
        private void OnEventTempTint()
        {
            if (cam_ != null)
            {
                int nTemp = 0, nTint = 0;
                if (cam_.get_TempTint(out nTemp, out nTint))
                {
                    //label2.Text = nTemp.ToString();
                    //label3.Text = nTint.ToString();
                    //trackBar2.Value = nTemp;
                    //trackBar3.Value = nTint;
                }
            }
        }
        //private void ckhMonoColor_CheckedChanged(object sender, EventArgs e)
        //{
        //    if (ckhMonoColor.Checked)
        //    {
        //        cam_.put_Chrome(true);
        //    }
        //    else
        //    {
        //        cam_.put_Chrome(false);
        //    }
        //}

        private void DelegateOnEvent(Toupcam.eEVENT[] ev)
        {
            switch (ev[0])
            {
                case Toupcam.eEVENT.EVENT_ERROR:
                    OnEventError();
                    break;
                case Toupcam.eEVENT.EVENT_DISCONNECTED:
                    OnEventDisconnected();
                    break;
                case Toupcam.eEVENT.EVENT_EXPOSURE:
                    OnEventExposure();
                    break;
                case Toupcam.eEVENT.EVENT_IMAGE:
                    OnEventImage();
                    break;
                case Toupcam.eEVENT.EVENT_STILLIMAGE:
                    OnEventStillImage();
                    break;
                case Toupcam.eEVENT.EVENT_TEMPTINT:
                    OnEventTempTint();
                    break;
            }
        }
        void CameraConnect()
        {
            if (cam_ != null)
                return;

            Toupcam.DeviceV2[] arr = Toupcam.EnumV2();
            if (arr.Length <= 0)
                this.CameraStatus.Image = global::CablesWindoApp3.Properties.Resources.CameraStatus;
            //CameraStatus.Image = Bitmap.FromFile("E:/CablesWindoApp3/buttonstosizefinal/CameraStatus.png");
            else
            //CameraStatus.Image = Bitmap.FromFile("E:/CablesWindoApp3/buttonstosizefinal/CameraStatus.png");
            {
                cam_ = Toupcam.Open(arr[0].id);
                if (cam_ != null)
                {
                    //checkBox1.Enabled = true;
                    //trackBar1.Enabled = true;
                    //numericUpDown1.Enabled = true;
                    //trackBar2.Enabled = true;
                    //trackBar3.Enabled = true;
                    //comboBox1.Enabled = true;
                    //button2.Enabled = true;
                    //button3.Enabled = true;
                    //button2.ContextMenuStrip = null;
                    //InitSnapContextMenuAndExpoTimeRange();

                    //trackBar2.SetRange(2000, 15000);
                    //trackBar3.SetRange(200, 2500);
                    OnEventTempTint();

                    uint resnum = cam_.ResolutionNumber;
                    uint eSize = 0;
                    if (cam_.get_eSize(out eSize))
                    {
                        for (uint i = 0; i < resnum; ++i)
                        {
                            int w = 0, h = 0;
                            //if (cam_.get_Resolution(i, out w, out h))
                            //comboBox1.Items.Add(w.ToString() + "*" + h.ToString());
                        }
                        //comboBox1.SelectedIndex = (int)eSize;

                        int width = 0, height = 0;
                        if (cam_.get_Size(out width, out height))
                        {
                            bmp_ = new Bitmap(width, height, PixelFormat.Format24bppRgb);
                            ev_ = new DelegateEvent(DelegateOnEvent);
                            if (!cam_.StartPullModeWithCallback(new Toupcam.DelegateEventCallback(DelegateOnEventCallback)))
                                MessageBox.Show("failed to start device");
                            else
                            {

                                bool autoexpo = true;
                                cam_.get_AutoExpoEnable(out autoexpo);

                                //checkBox1.Checked = autoexpo;
                                //trackBar1.Enabled = !checkBox1.Checked;
                                //numericUpDown1.Enabled = !checkBox1.Checked;
                            }
                        }
                    }
                }

                this.CameraStatus.Image = global::CablesWindoApp3.Properties.Resources.CameraStatusOn;

                //CameraStatus.Image = Bitmap.FromFile("E:/CablesWindoApp3/buttonstosizefinal/CameraStatusOn.png");
            }

        }
        //--------------------------Camera Settigns Methods ------------------------------//
        public void UpdateExposure(uint exp)
        {
            cam_.put_ExpoTime(exp);
        }
        public void UpdateGain(ushort gain)
        {
            cam_.put_ExpoAGain(gain);
        }
        public void UpdateSaturation(int Sat)
        {
            cam_.put_Saturation(Sat);
        }
        public void UpdateBrightness(int bri)
        {
            cam_.put_Brightness(bri);
        }
        public void UpdateContrast(int Con)
        {
            cam_.put_Contrast(Con);
        }
        void SetImageThreadSafe(PictureBox pb, Image img)
        {
            if (pb.InvokeRequired)
            {
                BeginInvoke((Action)delegate
                {
                    SetImageThreadSafe(pb, img);
                    // cblPitcurBox.Image.Save(@"E:\CablesWindoApp3\Image\temp.bmp");
                });
                return;

            }

            pb.Image?.Dispose();
            pb.Image = img;
        }
        public void SetTextboxValue()
        {

            ReportNotxtbox.Text = Settings.Default.ReportFolderNo.ToString();
            Datetxtbox.Text = DateTime.UtcNow.ToString("dd-MM-yyyy");
            Timetxtbox.Text = DateTime.Now.ToString("hh-mm-ss tt");
        }
        public void SetTextboxValue2()
        {
            ReportNotxtbox.Text = Settings.Default.ReportFolderNo.ToString() + "/" + SubReportNo;
            Datetxtbox.Text = DateTime.UtcNow.ToString("dd-MM-yyyy");
            Timetxtbox.Text = DateTime.Now.ToString("hh-mm-ss tt");

        }


        void measureFun()
        {
            //string cableimg = @"E:\CablesWindoApp3\xlpeImages\01.bmp";
            string cableimg = globalVars.projectDirectory + @"\ReportFolder\{0}\Cable.bmp";
            //string cableimg = @"E:\CablesWindoApp3\ReportFolder\{0}\Cable.bmp";
            Bitmap algoImage = new Bitmap(string.Format(cableimg, Settings.Default.ReportFolderNo.ToString()));
            //Bitmap algoImage = (Bitmap)Image.FromFile(cableimg);
            //bmp_ = (Bitmap)Image.FromFile(sampleimagePath);

            //-----model sel
            switch (selectedTemplate)
            {
                case "CORE1":
                    globalVars.algo.getMeasurements6c(algoImage); //////////////------------------------CPP                   
                    break;
                case "CORE2":
                    globalVars.algo.getMeasurements2c(algoImage); ////////////--------------CPP                   
                    break;
                case "CORE3":
                    globalVars.algo.getMeasurements3c(algoImage); /////---------------------------CPP                  
                    break;
                case "CORE4":
                    globalVars.algo.getMeasurements4c(algoImage); /////-------------------------CPP                   
                    break;
                case "CORE5":
                    globalVars.algo.getMeasurements5c(algoImage); ////------------------------CPP                   
                    break;
                case "CORE6":
                    globalVars.algo.getMeasurements6c(algoImage); ////--------------------CPP                  
                    break;
                case "XLPE":
                    globalVars.algo.getMeasurements8c(algoImage); //////---------------------------------CPP
                                                                  //string result = File.ReadAllText(@"C:\Users\VisionSystem\Desktop\xlp.txt");
                    xp.xlpes = JsonConvert.DeserializeObject<List<XP>>(globalVars.algo.jsonFileProp);

                    //xp.xlpes[0].minThick = 5;
                    //float k = xp.xlpes[1].minThick;
                    //xp.xlpes[2].minThick;

                    foreach (XP xp in xp.xlpes)
                    {
                        Console.WriteLine("This is avgThick value " + xp.avgThick);
                        Console.WriteLine("This is minThick value " + xp.minThick);
                        Console.WriteLine("This is concentricity value " + xp.concentricity);
                        Console.WriteLine("This is OuterDia value " + xp.OuterDia);
                        //Console.WriteLine("This is xp value " + xp.minThickData);
                    }

                    //UperPanels();
                    break;
                    //case "MULTICORE":
                    //    globalVars.algo.getMeasurementsMultiCore(algoImage);
                    //    UperPanels();
                    //    break;
                    //case "FLATCABLE":
                    //    globalVars.algo.flatCable(algoImage); //////---------------------------------CPP
                    //    UperPanels();
                    //    break;
            }

            //--------


            //Console.WriteLine("imahe ");
            //globalVars.projectDirectory + @"/ProjectButton/"

            string ProcessImgPath = globalVars.projectDirectory + @"\ReportFolder\{0}\Process.bmp"; //-------Main

            //string ProcessImgPath = @"E:\CablesWindoApp3\ReportFolder\{0}\Process.bmp";

            algoImage.Save(string.Format(ProcessImgPath, Settings.Default.ReportFolderNo.ToString()));//-------Main

            //cblPitcurBox.Image.Dispose();


            cblPitcurBox.Image = algoImage; //-------Main
            cblPitcurBox.Refresh();
            //cblPitcurBox.Invalidate();

            //string sampleimagePath = @"E:\CablesWindoApp3\Image\temp.bmp";
            //bmp_ = (Bitmap)Image.FromFile(sampleimagePath);
            //globalVars.algo.getMeasurements7c(bmp_);
            //bmpImg = (Bitmap)bmp_.Clone();
            //Console.WriteLine("Image format" + bmpImg.PixelFormat);
            //cblPitcurBox.Image = bmpImg;

        }
        public int calibrateSys()
        {
            Bitmap image = (Bitmap)bmp_.Clone(new Rectangle(0, 0, bmp_.Width, bmp_.Height), PixelFormat.Format24bppRgb);
            int resp = globalVars.algo.calibUsingTargetCircle(image);
            if (resp == 1)
            {
                MessageBox.Show("System Calibrated");
            }
            else
            { MessageBox.Show("Kindly clean the surface and place the calibration plate."); }
            cblPitcurBox.Image = image;
            return resp;
        }
        public void cameraFun()
        {
            //if (c0.connectionStatus == true)
            //{
            //CableImgPanel.Visible = true;

            //cblPitcurBox.Image = new Bitmap(PicBox1.Image, PicBox1.Image.Width, PicBox1.Image.Height);
            //cblPitcurBox.Image = new Bitmap(PicBox1.Image, PicBox1.Image.Width, PicBox1.Image.Height);

            //pbImage = (Bitmap)PicBox1.Image.Clone(); ////
            Bitmap pbImage = (Bitmap)PicBox1.Image.Clone(); ////
            procImage = (Bitmap)pbImage.Clone(new Rectangle(0, 0, pbImage.Width, pbImage.Height), PixelFormat.Format24bppRgb);


            switch (selButton)
            {
                //case "CORE1":
                //    algoImage = (Bitmap)procImage.Clone();
                //    globalVars.algo.getMeasurements1c(algoImage);
                //    OutDiamet();
                //break;
                case "CORE2":
                    algoImage = (Bitmap)procImage.Clone();
                    globalVars.algo.getMeasurements2c(algoImage); ////------------------CPP
                    OutDiamet();
                    break;
                case "CORE3":
                    algoImage = (Bitmap)procImage.Clone();
                    globalVars.algo.getMeasurements3c(algoImage); ////--------CPP
                    OutDiamet();
                    break;
                case "CORE4":
                    algoImage = (Bitmap)procImage.Clone();
                    globalVars.algo.getMeasurements4c(algoImage); ////----------------CPP
                    OutDiamet();
                    break;
                case "CORE5":
                    algoImage = (Bitmap)procImage.Clone();
                    globalVars.algo.getMeasurements5c(algoImage);////--------------------CPP
                    OutDiamet();
                    break;
                case "CORE6":
                    algoImage = (Bitmap)procImage.Clone();
                    globalVars.algo.getMeasurements6c(algoImage);////-------------------------CPP
                    OutDiamet();
                    break;
                case "MULTICORE":
                    algoImage = (Bitmap)procImage.Clone();
                    globalVars.algo.getMeasurementsMultiCore(algoImage); ////-------------------------CPP
                    OutDiamet();
                    break;
                default:
                    cblPitcurBox.Image = PicBox1.Image;
                    break;
            }



            //Console.WriteLine(OutTempCaptur);
            //ZoomInCableImgPanel2(OutTempCaptur);
            //Core2Zoom();

            //cblPitcurBox.Image = procImage;
            //cblPitcurBox.Refresh();
            ////globalVars.projectDirectory + @"/ProjectButton/"
            //string imgpath = globalVars.projectDirectory + @"\ReportFolder\{0}\Cable.bmp";
            ////string imgpath = @"E:\CablesWindoApp3\ReportFolder\{0}\Cable.bmp";
            //OutDiamet();
            //cblPitcurBox.Image.Save(string.Format(imgpath, Settings.Default.ReportFolderNo.ToString()));




            switch (OutTempCaptur)
            {
                case 3:
                    ZoomInCableImgPanel2(OutTempCaptur);
                    Core2Zoom();
                    break;
                case 6:
                    ZoomInCableImgPanel2(OutTempCaptur);
                    Core2Zoom();
                    break;
                case 8:
                    ZoomInCableImgPanel2(OutTempCaptur);
                    Core2Zoom();
                    break;
                case 2:
                    ZoomInCableImgPanel2(OutTempCaptur);
                    Core2Zoom();
                    break;
                default:
                    cblPitcurBox.Image = procImage;
                    cblPitcurBox.Refresh();
                    //globalVars.projectDirectory + @"/ProjectButton/"
                    string imgpath = globalVars.projectDirectory + @"\ReportFolder\{0}\Cable.bmp";
                    //string imgpath = @"E:\CablesWindoApp3\ReportFolder\{0}\Cable.bmp";
                    OutDiamet();
                    cblPitcurBox.Image.Save(string.Format(imgpath, Settings.Default.ReportFolderNo.ToString()));
                    break;

            }




            //cblPitcurBox.Refresh();
            //string cableimg = @"E:\CablesWindoApp3\ReportFolder\{0}\Cable.bmp";
            //cblPitcurBox.Image = algoImage;
            //string imgpath = @"E:\CablesWindoApp3\ReportFolder\{0}\Cable.bmp";

            //cblPitcurBox.Image.Save(string.Format(imgpath, Settings.Default.ReportFolderNo.ToString()));




            //cblPitcurBox.Image.Save(@"E:\CablesWindoApp3\Image\temp.bmp");


            //}
            //else
            //{
            //    MessageBox.Show("Please connect camera");
            //}
        }
        void CameraClose()
        {
            if (cam_ != null)
            {
                cam_.Close();
                cam_ = null;
            }
        }




        // Date Time variables //

        //List<UserInformationClass> uifclist = new List<UserInformationClass>();

        //Button[14] btn;

        //GloBal Varible For user Info
        //String GParName;
        //String GCustName;
        //String GOperName;
        //String GAddress;
        //String GCusRef;

        //Int Global Varibles
        //int GPartNo;
        //int GOperId;


        //Varibles
        //PictureBox ptb;

        //rdlcImageGVarbile

        //DataTable dt = new DataTable();
        //Coloumns





        public Form1()
        {
            InitializeComponent();

            this.CableImgPanel.MouseWheel += CableImgPanel_MouseWheel;
            this.panel10.MouseWheel += Panel10_MouseWheel;
            //this.EditPickbox.Click += EditPickBox_Second_Click;

            //----------------------------------------Label Array---------------------//
            LblArray = new Label[13] { label5, label14, label9, Core4Lbl, label13, Core5lbl, label8, label10, label7, label4, label6, label3, label11 };

            //this.LiveImgPanel.MouseWheel += LiveImMouseWheel;
            btnArray = new Button[14] {ca1btn,ca2btn,ca3btn,ca4btn,ca5btn,ca6btn,ca7btn,ca8btn,ca9btn,
            ca10btn,ca11btn,ca12btn,ca13btn,ca14btn};
            foreach (Button bt in btnArray)
            {
                bt.Click += Bt_Click;
               
            }

            DateTimeTimer.Enabled = true;
            DateTimeTimer.Interval = 1000;

        }


        private void Panel10_MouseWheel(object sender, MouseEventArgs e)
        {
            if (PicBox1.Image != null)
            {
                // Mouse Wheel is Moved
                if (e.Delta < 0)
                {
                    //PictureBox Dimensions Are range in 15
                    if (PicBox1.Width < (15 * panel10.Width) && (PicBox1.Height < (15 * panel10.Height)))
                    {

                        //Change pictureBox Size and Multiply Zoomfactor
                        PicBox1.Width = (int)(PicBox1.Width * 1.25);
                        PicBox1.Height = (int)(PicBox1.Height * 1.25);

                        //Move Picture box
                        PicBox1.Top = (int)(e.Y - 1.25 * (e.Y - PicBox1.Top));
                        PicBox1.Left = (int)(e.X - 1.25 * (e.X - PicBox1.Left));


                        Console.WriteLine("X={0},y={1}", e.X, e.Y);


                    }
                }


                else if ((cblPitcurBox.Width > panel10.Width) && (cblPitcurBox.Height > panel10.Height))
                {
                    //Change pictureBox Size and Multiply Zoomfactor
                    PicBox1.Width = (int)(PicBox1.Width / 1.25);
                    PicBox1.Height = (int)(PicBox1.Height / 1.25);

                    //Move Picture box
                    PicBox1.Top = (int)(e.Y - 0.80 * (e.Y - PicBox1.Top));
                    PicBox1.Left = (int)(e.X - 0.80 * (e.X - PicBox1.Left));

                }


            }
        }

       
        //-------ToupCam--------------//
        private void DelegateOnEventCallback(Toupcam.eEVENT ev)
        {
            /* this delegate is call by internal thread of toupcam.dll which is NOT the same of UI thread.
             * Why we use BeginInvoke, Please see:
             * http://msdn.microsoft.com/en-us/magazine/cc300429.aspx
             * http://msdn.microsoft.com/en-us/magazine/cc188732.aspx
             * http://stackoverflow.com/questions/1364116/avoiding-the-woes-of-invoke-begininvoke-in-cross-thread-winform-event-handling
            */
            BeginInvoke(ev_, new Toupcam.eEVENT[1] { ev });
        }

      
        private void Form1_Load(object sender, EventArgs e)
        {
            //d.TestConnection();
            globalVars.algo.mmPerPixProp = Settings.Default.mmPerPix;
            Console.WriteLine("calib parameter = " + globalVars.algo.mmPerPixProp.ToString());
            globalVars.algo.mmPerPixTempProp = globalVars.algo.mmPerPixProp;
            CameraConnect();
            connectSerialPort(Settings.Default.SerialPortName);
            Exposure();


            clearLabels();
            BatchPanel.Hide();

            //------------Core Buttons Disabled Method----------//
            CoreButtonsDisabled();
            //--------- Disbled Bottom Buttons Method ------------//
            DisbledBottomButton();

            //--------------------------------DataGridViewColoumns-----------------------------------------//

            ThreeCoreDataGridView.ColumnCount = 12;
            ThreeCoreDataGridView.Columns[0].Name = "No";
            ThreeCoreDataGridView.Columns[1].Name = "T1";
            ThreeCoreDataGridView.Columns[2].Name = "T2";
            ThreeCoreDataGridView.Columns[3].Name = "T3";
            ThreeCoreDataGridView.Columns[4].Name = "T4";
            ThreeCoreDataGridView.Columns[5].Name = "T5";
            ThreeCoreDataGridView.Columns[6].Name = "T6";
            ThreeCoreDataGridView.Columns[7].Name = "MinThick";
            ThreeCoreDataGridView.Columns[8].Name = "AvgThick";
            ThreeCoreDataGridView.Columns[9].Name = "Concentricity";
            ThreeCoreDataGridView.Columns[10].Name = "OuterDiameter";
            ThreeCoreDataGridView.Columns[11].Name = "Ovality";

            //----------------------------------------Label Hide---------------------//
            foreach (Label Lb in LblArray)
            {
                Lb.Visible = false;
            }
            //---------------------------------------------------DatabseConnection----------------------//

            PanelXple.Visible = false;
            WindowState = System.Windows.Forms.FormWindowState.Maximized;
            this.Refresh();
        }




        private void Bt_Click(object sender, EventArgs e)
        {
            if (but == false)
            {
                but = true;
                button = (Button)sender;
                selButton = button.Tag.ToString();
                selectedTemplate = selButton;
                Console.WriteLine("You have seleted core {0}", selButton);
                //this.button.BackgroundImage = global::CablesWindoApp3.Properties.Resources.selButton;
                button.BackgroundImage = Bitmap.FromFile(globalVars.projectDirectory + @"/ProjectButton/" + selButton + "_E.png");
                //button.BackgroundImage = Bitmap.FromFile("E:/CablesWindoApp3/buttonstosizefinal/new70_70/" + selButton + "_E.png");
                //----------------Bottom Buttons Enbled---------------------//
                EnbledBottomButton();
                switch (selectedTemplate) 
                {
                    case "CORE2":
                        TPanels(panelT1, panelT2);
                        break;
                    case "CORE3":
                        TPanels(panelT1,panelT2,panelT3);
                        break;
                    //case "CORE4":
                    //    TPanels(panelT1, panelT2, panelT3);
                    //    break;
                    case "CORE5":
                        TPanels(panelT1, panelT2, panelT3, PanelT4, PanelT5);
                        break;
                    case "MULTICORE":
                        TPanels(panelT1, panelT2, panelT3, PanelT4, PanelT5, PanelT6);
                        break;
                    case "XLPE":
                        PanelXple.Visible = true;
                        break;
                    default :
                        MessageBox.Show("This Core is Not avalible");
                        break;


                }

                //  MessageBox.Show(string.Format("You have Selected {0}", selButton));

                foreach (Button bt in btnArray)
                {
                    if (bt.Name != button.Name)
                    {
                        bt.Enabled = false;
                        UperPanels();
                      
                        //bt.BackgroundImage = Bitmap.FromFile("default path " + selButton + "_d.png");
                        ////rdlcReportbtn.Enabled = true;
                        //savebtn.Enabled = true;
                        // camerabtn.Enabled = true;
                        // button1.Enabled = true;
                        //ImageBtn.Enabled = true;
                        //uif.Show();
                    }


                }

            }
            else
            {
                but = false;
                Button button = (Button)sender;
                selButton = button.Tag.ToString();
                selectedTemplate = selButton;
                Console.WriteLine("You have seleted core {0}", selButton);
                PanelXple.Visible = false;
                button.BackgroundImage = Bitmap.FromFile(globalVars.projectDirectory + @"/ProjectButton/" + selButton + "_D.png");
                //button.BackgroundImage = Bitmap.FromFile("E:/CablesWindoApp3/buttonstosizefinal/new70_70/" + selButton + "_D.png");
                clearLabels();
                //  MessageBox.Show(string.Format("You have Selected {0}", selButton));

                foreach (Button bt in btnArray)
                {
                    if (bt.Name != button.Name)
                    {
                        bt.Enabled = true;
                    }


                }
            }
        }


        private void cblePitcurbox_MouseWheel(object sender, MouseEventArgs e)
        {

        }

        private void CableImgPanel_MouseWheel(object sender, MouseEventArgs e)
        {
            if (cblPitcurBox.Image != null)
            {
                // Mouse Wheel is Moved
                if (e.Delta < 0)
                {
                    //PictureBox Dimensions Are range in 15
                    if (cblPitcurBox.Width < (15 * this.Width) && (cblPitcurBox.Height < (15 * this.Height)))
                    {

                        //Change pictureBox Size and Multiply Zoomfactor
                        cblPitcurBox.Width = (int)(cblPitcurBox.Width * 1.25);
                        cblPitcurBox.Height = (int)(cblPitcurBox.Height * 1.25);

                        //Move Picture box
                        cblPitcurBox.Top = (int)(e.Y - 1.25 * (e.Y - cblPitcurBox.Top));
                        cblPitcurBox.Left = (int)(e.X - 1.25 * (e.X - cblPitcurBox.Left));
                        Console.WriteLine("X={0},y={1}", e.X, e.Y);
                    }
                }


                else if ((cblPitcurBox.Width > CableImgPanel.Width) && (cblPitcurBox.Height > CableImgPanel.Height))/* &&*/
                ////        (cblPitcurBox.Width >= 494) && (cblPitcurBox.Height >= 253))
                //////((cblPitcurBox.Width > (CableImgPanel.Width) && (cblPitcurBox.Height > (CableImgPanel.Height))))))

                // {  //PictureBox Dimensions Are range in 15
                //if (cblPitcurBox.Width > (CableImgPanel.Width) && (cblPitcurBox.Height > (CableImgPanel.Height)))
                {
                    //Change pictureBox Size and Multiply Zoomfactor
                    cblPitcurBox.Width = (int)(cblPitcurBox.Width / 1.25);
                    cblPitcurBox.Height = (int)(cblPitcurBox.Height / 1.25);

                    //Move Picture box
                    cblPitcurBox.Top = (int)(e.Y - 0.80 * (e.Y - cblPitcurBox.Top));
                    cblPitcurBox.Left = (int)(e.X - 0.80 * (e.X - cblPitcurBox.Left));

                }

                //else
                //{
                //    CableImgPanel.Width = cblPitcurBox.Width;
                //    CableImgPanel.Width = cblPitcurBox.Width;
                //}

                // }
            }
        }

      
      
        //baumerCam c1 = new baumerCam();
        //baumerCam c2 = new baumerCam();

        //int connectAllCams()
        //{
        //    camController.listOfCamObs_u.Clear();
        //    //camera 1 settings---
        //    c0.name = "OnlyCamera";
        //    c0.serialNo = "700005751700";// "700006163693";
        //    c0.exposureTimeUser = 8000.0;
        //    c0.triggerSource = "Software";
        //    c0.imageReceived += c0event;

        //    ////camera 2 settings----
        //    //    c1.name = "Second Cam";
        //    //    c1.serialNo = "700006163697";
        //    //    c1.exposureTimeUser = 4000.0;
        //    //    c1.triggerSource = "Software";
        //    //    c1.imageReceived += c1event;

        //    //    //-------camera 3 sett---
        //    //    c2.name = "Third Cam";
        //    //    c2.serialNo = "700006163688";
        //    //    c2.exposureTimeUser = 4000.0;
        //    //    c2.triggerSource = "Software";
        //    //    c2.imageReceived += c2event;


        //    camController.listOfCamObs_u.Add(c0);
        //    //camController.listOfCamObs_u.Add(c1);
        //    //camController.listOfCamObs_u.Add(c2);
        //    //-------------------
        //    //find camera/s
        //    camController.findNconnectCams();
        //    //connect camera/s and allocate buffers
        //    foreach (BaumerCam c in camController.listOfCamObs_u)
        //    {
        //        if (c.cam != null)
        //        {
        //            int resp = camController.assignDataStreamNsetFeatures(c, ref c.cam, ref c.mDataStream);
        //            if (resp == 1)
        //            {
        //                if (camController.assignBuffer(c, ref c.mDataStream, ref c.bufferList, ref c.mBuffer) == 0)
        //                {
        //                    System.Console.Write("Error while assigning Buffer " + c.name);
        //                    return 0;
        //                }
        //            }
        //            else
        //            {
        //                System.Console.Write("Error while assigning datastreams " + c.name);
        //                return 0;
        //            }

        //        }
        //        Console.WriteLine("camera connection status :" + c.name + "  " + c.connectionStatus);
        //    }

        //    return 1;
        //}

        //----------------------------------ToupCamConnect Function---------------------------------//


        //void c0event(object sender, bool success)
        //{
        //    SetImageThreadSafe(PicBox1, c0.grabbedImageColor);
        //    //  MessageBox.Show("Image receive event Occured");
        //    c0.okCnt += 1;
        //    //c0.inspectionResult = true;            

        //    Console.WriteLine("c0 event");

        //}

        private void startbtn_Click(object sender, EventArgs e)
        {

            ////SetImageThreadSafe(picCam0, c0.grabbedImageColor);
            ////MessageBox.Show("Image receive event Occured");
            ////c0.okCnt += 1;
            //c0.inspectionResult = true;
            //Console.WriteLine("c0 event");
            //if (globalVars.inspectionRunning == true)
            //{
            //    foreach (BaumerCam c in camController.listOfCamObs_u)
            //    {
            //        camController.stopCameraAcq(c);
            //        //toolStripStatusLabel2.Text = "Inspection Stopped";
            //        //toolStripStatusLabel2.ForeColor = Color.Red;
            //        //toolStripStatusLabel2.BackColor = Color.Red;
            //        startbtn.Text = "Start";
            //        startbtn.ForeColor = Color.Green;
            //        globalVars.inspectionRunning = false;

            //    }

            //}
            //else
            //{
            //    foreach (BaumerCam c in camController.listOfCamObs_u)
            //    {
            //        camController.startCameraAcq(c);
            //        //toolStripStatusLabel2.Text = "Inspection Running";
            //        //toolStripStatusLabel2.ForeColor = Color.Green;
            //        //toolStripStatusLabel2.BackColor = Color.Green;
            //        startbtn.ForeColor = Color.Red;
            //        startbtn.Text = "Stop";
            //        //startbtn.ForeColor = Color.Red;
            //        globalVars.inspectionRunning = true;
            //    }

            //}

        }

        // TrackBar Method


        //public async Task TrackBarMethod()
        //{
        //    await Task.Delay(500);
        //    ptrackBar.Minimum = 1;
        //    ptrackBar.Maximum = 6;
        //    ptrackBar.SmallChange = 1;
        //    ptrackBar.LargeChange = 1;
        //    ptrackBar.UseWaitCursor = false;

        //    //
        //    this.DoubleBuffered = true;
        //    ptb = new PictureBox();
        //    ptb.Image=cablesPicBox.Image;           

        //}
        // Second Method Of ImgTrackbar


        // Image Method
        //Image ZoomPicture(Image imga, Size sizepic)
        //{
        //    //


        // Camerabtn Variable

        //CameraButton

        //public static algorithmLib.Class1 algo = new algorithmLib.Class1();
        //private Bitmap bmp_ = null;
        //Bitmap bmpImg;
    

        private void camerabtn_Click(object sender, EventArgs e)
        {

            //cameraFun();
        }



        //Constructor

        //Timer 
        private void pictimer_Tick(object sender, EventArgs e)
        {
            //if (camController.listOfCamObs_u[0].connectionStatus == true)
            //{
            //camController.listOfCamObs_u[0].cam.RemoteNodeList["TriggerSoftware"].Execute();
            //    Console.WriteLine("Image Captured");
            //}
        }

        //Play Pause Button Variable
        //bool PlayPausebtn = false;



        //Play Pause Button
        //private void pausebtn_Click(object sender, EventArgs e)
        //{
        //    ////PlayButton Code
        //    //if (PlayPausebtn == false)
        //    //{

        //    //    camerabtn.Enabled = false;
        //    //    PlayPausebtn = true;
        //    //    pausebtn.Image = System.Drawing.Image.FromFile(@"E:\Sachin\AddInnovationsProject\CablesWindoApp3\IconsImages\PauseIcon.png");
        //    //    pictimer.Start();
        //    //}

        //    ////PauseButton Code
        //    //else
        //    //{
        //    //    camerabtn.Enabled = true;
        //    //    pausebtn.Image = System.Drawing.Image.FromFile(@"E:\Sachin\AddInnovationsProject\CablesWindoApp3\IconsImages\Playbtn.png");
        //    //    PlayPausebtn = false;
        //    //    pictimer.Stop();
        //    //}

        //}


        private void camerabtn_Leave(object sender, EventArgs e)
        {

        }

        private void cablesPicBox_Click(object sender, EventArgs e)
        {

        }

        private void ca12btn_Click(object sender, EventArgs e)
        {

        }

        private void MaximizeBtn_Click(object sender, EventArgs e)
        {

            if (WindowState == FormWindowState.Normal)
            {
                WindowState = FormWindowState.Maximized;
                FormBorderStyle = FormBorderStyle.None;
            }
            else
            {
                WindowState = FormWindowState.Normal;
            }
        }

        private void MinimizeBtn_Click(object sender, EventArgs e)
        {
            WindowState = FormWindowState.Minimized;
        }

        private void ca1btn_Click(object sender, EventArgs e)
        {

        }

        private void label5_Click(object sender, EventArgs e)
        {

        }

        //tab variable


        //Get User Value Method
        //void GetUserValue()
        //{
        //    GParName = PartNatxtbox.Text;
        //    GCustName = CusNatxtbox.Text;
        //    GOperName = OperNatxtbox.Text;
        //    GAddress = addresstxtbox.Text;
        //    GCusRef = CusReftxtbox.Text;

        //    //int Varible
        //    GPartNo = Convert.ToInt32(PartNotxtbox.Text);
        //    GOperId = Convert.ToInt32(operIdtxtbox.Text);

        //    //temp part
        //    //PartNatxtbox.Text = "P1";
        //    //PartNotxtbox.Text = Convert.ToString(021);
        //    //CusNatxtbox.Text = "Rohit";
        //    //OperNatxtbox.Text = "Ankit";
        //    //operIdtxtbox.Text = Convert.ToString(045);
        //    //addresstxtbox.Text = "Greater Noida";
        //    //CusReftxtbox.Text = "Mohit";
        //}

        //Show User Value Method
        //void ShowUserValTPanal()
        //{
        //    FPartNaLabel.Text = GParName;
        //    FCusNLabel.Text = GCustName;
        //    FOperNLabel.Text = GOperName;
        //    FAddressLabel.Text = GAddress;
        //    FCusRefLabel.Text = GCusRef;

        //    //Int Varibles
        //    FPartNoLabel.Text = Convert.ToString(GPartNo);
        //    FOperIdLabel.Text = Convert.ToString(GOperId);

        //    //temp
        //    FPartNaLabel.Text = PartNatxtbox.Text;
        //    FPartNoLabel.Text = PartNotxtbox.Text;
        //    FCusNLabel.Text = CusNatxtbox.Text;
        //    FOperNLabel.Text = OperNatxtbox.Text;
        //    FOperIdLabel.Text = operIdtxtbox.Text;
        //    FAddressLabel.Text = addresstxtbox.Text;
        //    FCusRefLabel.Text = CusReftxtbox.Text;
        //}

        //cableImage variable; 

        //public void pdfsave()
        //{
        //    string devicID = "";
        //    string[] streamIds;
        //    Warning[] warnings;
        //    string mimeType = string.Empty;
        //    string encoding = string.Empty;
        //    string extensions = string.Empty;
        //    ReportViewer viewer = new ReportViewer();

        //    //viewer.LocalReport.DataSources(new ReportDataSource("datset1", GetDataset1));
        //}
        private void savebtn_Click(object sender, EventArgs e)
        {
            File.WriteAllText(@"E:\" + Settings.Default.reportNum.ToString(), "Hello Sachin");
            Settings.Default.reportNum++;
            Settings.Default.Save();
        }

        private void Exitbtn_Click(object sender, EventArgs e)
        {
            //    Console.WriteLine("form closing .. closing the cameras");
            //    //deregister events
            //    //c0.imageReceived -= c0event;
            //    //c1.imageReceived -= c1event;
            //    //c2.imageReceived -= c2event;

            //    foreach (BaumerCam c in camController.listOfCamObs_u)
            //    {
            //        if (c.capturing == true && c.cam != null)
            //        {
            //            camController.stopCameraAcq(c);
            //            Console.WriteLine("camera is still captureing closingn camera:: " + c.name);
            //        }
            //        //System.Threading.Thread.Sleep(500);

            //        //camController.disposeCamera(c);
            //        //Console.WriteLine("disposed camera " + c.name);
            //    }
            //     Application.Exit();

        }
       
        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            CameraClose();
            //formClose = true;

            //OnEventDisconnected();
            //Console.WriteLine("form closing .. closing the cameras");
            ////deregister events
            ////c0.imageReceived -= c0event;
            ////c1.imageReceived -= c1event;
            ////c2.imageReceived -= c2event;

            //foreach (BaumerCam c in camController.listOfCamObs_u)
            //{
            //    if (c.capturing == true && c.cam != null)
            //    {
            //        camController.stopCameraAcq(c);
            //        Console.WriteLine("camera is still captureing closingn camera:: " + c.name);
            //    }
            //    //System.Threading.Thread.Sleep(500);

            //    //camController.disposeCamera(c);
            //    //Console.WriteLine("disposed camera " + c.name);

            //}
            ////camController.mInterface.Close();
            ////Console.WriteLine("closed interface ");        

        }

        private void label4_Click(object sender, EventArgs e)
        {

        }

        private void label15_Click(object sender, EventArgs e)
        {

        }

        private void tabPage1_Click(object sender, EventArgs e)
        {

        }

        //private void ptrackBar_Scroll(object sender, EventArgs e)
        //{

        //    //if (ptrackBar.Value != 0)
        //    //{
        //    //    cablesPicBox.Image = null;
        //    //    cablesPicBox.Image = ZoomPicture(ptb.Image, new Size(ptrackBar.Value, ptrackBar.Value));
        //    //}
        //}

        private void cablesPicBox_Click_1(object sender, EventArgs e)
        {

        }



        private void exitepicbox_Click(object sender, EventArgs e)
        {

            Application.Exit();
        }

        // Validtion Method
        //void checkValidation()
        //{
        //    if (PartNatxtbox.Text == "")
        //    {
        //        MessageBox.Show("Pleas Enter Part Name");
        //    }
        //    else if (CusNatxtbox.Text == "")
        //    {
        //        MessageBox.Show("Pleas Enter Customer Name");
        //    }
        //    else if (OperNatxtbox.Text == "")
        //    {
        //        MessageBox.Show("Pleas Enter Operator Name");
        //    }
        //    else if (CusReftxtbox.Text == "")
        //    {
        //        MessageBox.Show("Pleas Enter Customer Reference");
        //    }
        //    else if (addresstxtbox.Text == "")
        //    {
        //        MessageBox.Show("Pleas Enter Address");
        //    }
        //    else if (PartNotxtbox.Text == "")
        //    {
        //        MessageBox.Show("Pleas Enter Part No");
        //    }
        //    else if (operIdtxtbox.Text == "")
        //    {
        //        MessageBox.Show("Pleas Enter Operator Id");
        //    }

        //}

        void datset()
        {

        }
        //private void ResultBtn_Click(object sender, EventArgs e)
        //{
        //    Gmt.Text = globalVars.algo.minThicknessProp.ToString("N3");
        //    Gat.Text = globalVars.algo.avgThicknessProp.ToString("N3");
        //    God.Text = globalVars.algo.outerDiaProp.ToString("N3");
        //    GConcenticity.Text = globalVars.algo.concentricityProp.ToString("N3");

        //    Cabledgv.Rows.Add
        //                     (
        //                      globalVars.algo.getSectorThickness(0).ToString("N3"),
        //                      globalVars.algo.getSectorThickness(1).ToString("N3"),
        //                      globalVars.algo.getSectorThickness(2).ToString("N3")
        //                      //globalVars.algo.getSectorThickness(3).ToString("N3")
        //                      //globalVars.algo.minThicknessProp(4).ToString("N3"),
        //                      //globalVars.algo.avgThicknessProp(5).ToString("N3"),
        //                      //globalVars.algo.outerDiaProp(6).ToString("N3"),
        //                      //globalVars.algo.concentricityProp(7).ToString("N3")
        //                      );

        //    //DataSet ds = new DataSet();

        //    //DataTable dt = new DataTable();
        //    //dt.Columns.Add("CableName", typeof(string));
        //    //dt.Columns.Add("CableType", typeof(string));
        //    //foreach (DataGridView dgv in Cabledgv.Rows)
        //    //{
        //    //    dt.Rows.Add(dgv.[0].Value);
        //    //}
        //    //  checkValidation();
        //    rdlcReportViewer.Visible = false;
        //   

        //    //CablespictureBox2.Image = cblPitcurBox.Image;
        //   // GetUserValue();
        //    // Method Call
        //    // GetUserValue();
        //    //ShowUserValTPanal();

        //    //Dataset

        //}

        private void Homebtn_Click(object sender, EventArgs e)
        {
            //resultpanel.Visible = false;
            //rdlcReportViewer.Visible = false;
            //ImagePanel.Visible = false;
        }


        private void DateTimeTimer_Tick(object sender, EventArgs e)
        {
            //DateTimeLabel.Text = DateTime.Now.ToLongTimeString();
        }

        private void ImageBtn_Click(object sender, EventArgs e)
        {
            //resultpanel.Visible = false;
            //rdlcReportViewer.Visible = false; 

        }

        private void resultpanel_Paint(object sender, PaintEventArgs e)
        {

        }

        private void CablespictureBox2_Click(object sender, EventArgs e)
        {

        }

        private void FPartNaLabel_Click(object sender, EventArgs e)
        {

        }

        private void Cabledgv_CellContentClick(object sender, DataGridViewCellEventArgs e)
        {

        }

        private void dataGridView1_CellContentClick(object sender, DataGridViewCellEventArgs e)
        {

        }


        private void imgsvbtn_Click(object sender, EventArgs e)
        {
            //cblPitcurBox.Image.Save(@"E:\CablesWindoApp3\Image\temp.bmp");
        }

        //int partNum = UserInformationForm.PartNo;/////////_________________________
       
        //void rdlcReporttxtFun()
        //  {          

        //      string CableImgPath = @"E:\CablesWindoApp3\ReportFolder\{0}\Cable.bmp";
        //      string ProcessImgpath = @"E:\CablesWindoApp3\ReportFolder\{0}\Process.bmp";

        //      ReportParameter rdlcCableImg = new ReportParameter("rdlcCableImg", new Uri(string.Format(CableImgPath, Settings.Default.ReportFolderNo.ToString())).AbsoluteUri);
        //      this.rdlcReportViewer.LocalReport.EnableExternalImages = true;
        //      this.rdlcReportViewer.LocalReport.SetParameters(new ReportParameter[] { rdlcCableImg });

        //      ReportParameter rdlcProcessImg = new ReportParameter("rdlcProcessImg", new Uri(string.Format(ProcessImgpath, Settings.Default.ReportFolderNo.ToString())).AbsoluteUri);
        //      this.rdlcReportViewer.LocalReport.EnableExternalImages = true;
        //      this.rdlcReportViewer.LocalReport.SetParameters(new ReportParameter[] { rdlcProcessImg });
        //      //Me.reportViewer1.LocalReport.SetParameters(New ReportParameter() { Path})


        //      rptc.Add(new ReportParameter("rdlcTestNo",Convert.ToString(UserInformationForm.TestNo)));
        //      rptc.Add(new ReportParameter("rdlcTestIdRef", Convert.ToString(UserInformationForm.TestIdRef)));
        //      rptc.Add(new ReportParameter("rdlcCustNa",CustomerName));
        //      rptc.Add(new ReportParameter("rdlcCustId", Convert.ToString(CustomerId)));
        //      rptc.Add(new ReportParameter("rdlcOperNa", OperatorName));
        //      rptc.Add(new ReportParameter("rdlcOperId", Convert.ToString(OperatorId)));
        //      rptc.Add(new ReportParameter("rdlcBatchNo",Convert.ToString(UserInformationForm.BatchNo)));
        //      rptc.Add(new ReportParameter("rdlcReportNo", Convert.ToString(Settings.Default.ReportFolderNo)));
        //      rptc.Add(new ReportParameter("rdlcDate", date));
        //      rptc.Add(new ReportParameter("rdlcTime", time));
        //      rptc.Add(new ReportParameter("rdlcTempNa", selButton));


        //      //-----------------------New Code-------------------//
        //      //string data = DateTime.UtcNow.ToString("dd-MM-yyyy");
        //      //string time = DateTime.Now.ToString("hh-mm-ss tt");

        //      ////string imgurl = @"E:\CablesWindoApp3\ReportFolder\{0}\Process.bmp";
        //      ////string testimg = @"E:\CablesWindoApp3\ReportFolder\{0}\Cable.bmp";

        //      ////ReportParameter rdlcCableImg = new ReportParameter("rdlcCableImg", new Uri(string.Format(testimg, Settings.Default.ReportFolderNo.ToString())).AbsoluteUri);
        //      ////rdlcReportViewer.LocalReport.EnableExternalImages = true;
        //      ////rdlcReportViewer.LocalReport.SetParameters(new ReportParameter[] { rdlcCableImg });

        //      ////ReportParameter rdlcProcessImg = new ReportParameter("rdlcProcessImg", new Uri(string.Format(imgurl, Settings.Default.ReportFolderNo.ToString())).AbsoluteUri);
        //      ////rdlcReportViewer.LocalReport.EnableExternalImages = true;
        //      ////rdlcReportViewer.LocalReport.SetParameters(new ReportParameter[] { rdlcProcessImg });

        //      //rptc.Add(new ReportParameter("rdlcPartNa", UserInformationForm.PartName));
        //      //rptc.Add(new ReportParameter("rdlcPartNo", Convert.ToString(UserInformationForm.PartNo)));
        //      //rptc.Add(new ReportParameter("rdlcCustNa", UserInformationForm.CustomerName));
        //      //rptc.Add(new ReportParameter("rdlcCustRef", UserInformationForm.CustomerRef));
        //      //rptc.Add(new ReportParameter("rdlcOperNa", UserInformationForm.OperatorName));
        //      //rptc.Add(new ReportParameter("rdlcOperId", Convert.ToString(UserInformationForm.OperatorId)));
        //      //rptc.Add(new ReportParameter("rdlcAddress", UserInformationForm.Address));
        //      //rptc.Add(new ReportParameter("rdlcReportNo", Convert.ToString(Settings.Default.ReportFolderNo)));
        //      //rptc.Add(new ReportParameter("rdlcDate", data));
        //      //rptc.Add(new ReportParameter("rdlcTime", time));
        //      //rptc.Add(new ReportParameter("rdlcCore", selButton));




        //      ////
        //      ////rptc.Add(new ReportParameter("rdlcPartNo",UserInformationForm.PartNo));
        //      ////rptc.Add(new ReportParameter("rdlcOperId",);

        //      //// //temp
        //      ////rptc.Add(new ReportParameter("RMinThickness", globalVars.algo.minThicknessProp.ToString("N3")));
        //      ////rptc.Add(new ReportParameter("RavrgThickness", globalVars.algo.avgThicknessProp.ToString("N3")));
        //      ////rptc.Add(new ReportParameter("ROuter", globalVars.algo.outerDiaProp.ToString("N3")));
        //      ////rptc.Add(new ReportParameter("RConcentricity", globalVars.algo.concentricityProp.ToString("N3")));


        //      this.rdlcReportViewer.LocalReport.SetParameters(rptc);
        //  }



        //void CoreFunRdlc()
        //  {
        //      ReportDataSource rds = new ReportDataSource();
        //      List<Cable> cblst = new List<Cable>();
        //      cblst.Clear();

        //      //for (int i = 0; i<3; i++)
        //      //{
        //          cblst.Add(new Cable
        //          {

        //              T1 = globalVars.algo.getSectorThickness(0).ToString("N3"),
        //              T2 = globalVars.algo.getSectorThickness(1).ToString("N3"),
        //              //T3 = globalVars.algo.getSectorThickness(2).ToString("N3"),
        //              T4 = globalVars.algo.getSectorThickness(3).ToString("N3"),
        //              T5 = globalVars.algo.getSectorThickness(4).ToString("N3"),
        //              T6 = globalVars.algo.getSectorThickness(5).ToString("N3"),
        //              OutDiam     = globalVars.algo.outerDiaProp.ToString("N3"),
        //              OutDiamMax =  globalVars.algo.outerDiaProp.ToString("N3"),
        //              OutDiamMin =  globalVars.algo.outerDiaProp.ToString("N3"),
        //              MinThick   =  globalVars.algo.minThicknessProp.ToString("N3"),
        //              MaxThick  =   globalVars.algo.minThicknessProp.ToString("N3"),
        //              Concentricity      = globalVars.algo.concentricityProp.ToString("N3"),
        //              Area = globalVars.algo.concentricityProp.ToString("N3"),
        //              TotalLengthY = globalVars.algo.concentricityProp.ToString("N3"),
        //              LengthC = globalVars.algo.concentricityProp.ToString("N3"),
        //              AvgThick = globalVars.algo.avgThicknessProp.ToString("N3"),          

        //  });

        //      //}

        //      rds.Name = "DataSet1";
        //      rds.Value = cblst;

        //      this.rdlcReportViewer.LocalReport.DataSources.Clear();
        //      this.rdlcReportViewer.LocalReport.DataSources.Add(rds);
        //      this.rdlcReportViewer.LocalReport.ReportEmbeddedResource = "CablesWindoApp3.ThreeCore.rdlc";

        //      //this.rdlc.LocalReport.DataSources.Clear();
        //      //this.rdlcReportViewer.LocalReport.DataSources.Add(rds);
        //      //this.rdlcReportViewer.LocalReport.ReportEmbeddedResource = "CablesWindoApp3.ThreeCore.rdlc";
        //  }


        //private void rdlcReportbtn_Click(object sender, EventArgs e)
        //{

        //   //rpt.Show();
        //   CableImgPanel.Visible = false;
        //   rdlcReportViewer.Visible = true;

        //   rdlcReporttxtFun();
        //   CoreFunRdlc();
        //   this.rdlcReportViewer.RefreshReport();
        //}

        private void DateTimeLabel_Click(object sender, EventArgs e)
        {

        }



        private void ca5btn_Click(object sender, EventArgs e)
        {

        }
     
        private void button1_Click(object sender, EventArgs e)
        {
            //measureFun();
            //Thickness();
        }

        private void header1Label_Click(object sender, EventArgs e)
        {

        }

        //private void button1_Click_1(object sender, EventArgs e)
        //{
        //    measureFun();
        //}

        private void button2_Click(object sender, EventArgs e)
        {
            SrForm.Show();
           
        }

        private void MeasureGBox_Enter(object sender, EventArgs e)
        {

        }

        private void cblPitcurBox_DoubleClick(object sender, EventArgs e)
        {

        }

        private void cblPitcurBox_MouseMove(object sender, MouseEventArgs e)
        {
            if (cblPitcurBox.Image == null)
            {
                return;
            }
            if (e.Button == System.Windows.Forms.MouseButtons.Left)
            {
                cblPitcurBox.Refresh();
                cropWidth = e.X - cropX;
                cropHeight = e.Y - cropY;
                Console.WriteLine("AX=" + e.X);
                Console.WriteLine("AY=" + e.Y);
                cblPitcurBox.CreateGraphics().DrawRectangle(cropPen, cropX, cropY, cropWidth, cropHeight);
                Console.WriteLine("CropX={0},cropY={1},cropWidth={2},cropHeight={3}", cropX, cropY, cropWidth, cropHeight);
            }
        }

        //void Crop()
        //{
        //    cropX = 325;
        //    cropY = 322;
        //}
        //void AfterCrop()
        //{
        //    cropWidth = 325 - cropX;
        //    cropHeight = panel10.Height - 322;
        //}

        private void cblPitcurBox_MouseDown(object sender, MouseEventArgs e)
        {

            if (e.Button == System.Windows.Forms.MouseButtons.Left)
            {
                Cursor = Cursors.Cross;
                cropX = e.X;
                cropY = e.Y;
                Console.WriteLine("BX=" + e.X);
                Console.WriteLine("BY=" + e.Y);
                cropPen = new Pen(Color.Black, 2);
                cropPen.DashStyle = DashStyle.DashDotDot;
            }
            cblPitcurBox.Refresh();
        }

          
        private void cblPitcurBox_Click(object sender, EventArgs e)
        {
            //CropImage();
        }

        private void button2_Click_1(object sender, EventArgs e)
        {
            btmp.Save(@"E:\CablesWindoApp3\CropImage\crop.bmp");
        }

        private void rdlcReportViewer_Load(object sender, EventArgs e)
        {

        }

        //object UserInformationForm
        
      
       
        private void cblPanel2_Paint(object sender, PaintEventArgs e)
        {

        }

        private void label1_Click(object sender, EventArgs e)
        {

        }

        private void ca2btn_Click(object sender, EventArgs e)
        {
           
           
        }

        private void toolStripStatusLabel1_Click(object sender, EventArgs e)
        {
            //String path = @"Hello/{0}";
            //Console.WriteLine(string.Format(path, "Babuy"));
        }

        private void ImageBtn_Click_1(object sender, EventArgs e)
        {
            //CableImgPanel.Visible = true;
        }
       
        
        private void Homebtn_Click_1(object sender, EventArgs e)
        {

        }

        private void button3_Click(object sender, EventArgs e)
        {
            //connectAllCams();
        }

        private void ca3btn_Click(object sender, EventArgs e)
        {

        }

        private void ca6btn_Click(object sender, EventArgs e)
        {

        }

        private void ca7btn_Click(object sender, EventArgs e)
        {

        }

        private void ca8btn_Click(object sender, EventArgs e)
        {
           
        }

        private void ca9btn_Click(object sender, EventArgs e)
        {
                    
        }

        private void ca10btn_Click(object sender, EventArgs e)
        {

        }

        private void ca11btn_Click(object sender, EventArgs e)
        {

        }

        private void button3_Click_1(object sender, EventArgs e)
        {
            //foreach (Button btn in btnArray)
            //{
            //    btn.Enabled = true;
            //}
        }

        private void ca13btn_Click(object sender, EventArgs e)
        {

        }

        //private void button2_Click_2(object sender, EventArgs e)
        //{

        //}

      
        private void button4_Click(object sender, EventArgs e)
        {
           
                //if (rdlcReportViewer.Visible == true)
                //{
                //    rdlcReportViewer.Visible = false;
                //}
           
           
           
           
            //var rplist = new List<int>();
            //for (int i=Settings.Default.ReportFolderNo; i < Settings.Default.ReportFolderNo++; i++) 
            //{
            //    rplist.Add(i);
            //    Settings.Default.ReportFolderNo++;
            //}
            //for (int i = Settings.Default.ReportFolderNo; i < Settings.Default.ReportFolderNo++; i++)
            //{
            //    Console.WriteLine(rplist[i]);
            //   Settings.Default.ReportFolderNo++;
            //}

        }

        private void button6_Click(object sender, EventArgs e)
        {

        }

        private void savebtn_MouseHover(object sender, EventArgs e)
        {
            
        }

       
       
      

       
        private void TopLight_Click(object sender, EventArgs e)
        {
            if (setLight("top", !topLightState) == 1)
            {
                topLightState = !topLightState;
               
            }

            if (topLightState == true)
            {
                TopLight.BackgroundImage = Bitmap.FromFile(globalVars.projectDirectory + @"/ProjectButton/TopLightOn.png");
                //TopLight.BackgroundImage = Bitmap.FromFile(globalVars.projectDirectory +  @"/buttonstosizefinal/TopLightOn.png");
            }
            else
            {
                TopLight.BackgroundImage = Bitmap.FromFile(globalVars.projectDirectory + @"/ProjectButton/TopLight.png");
                //TopLight.BackgroundImage = Bitmap.FromFile("E:/CablesWindoApp3/buttonstosizefinal/TopLight.png");
            }
        }
        //private void PDFExport(LocalReport report)
        //{
        //    string[] streamids;
        //    string minetype;
        //    string encod;
        //    string fextension;
        //    string deviceInfo =
        //      "<DeviceInfo>" +
        //      "  <OutputFormat>EMF</OutputFormat>" +
        //      "  <PageWidth>8.5in</PageWidth>" +
        //      "  <PageHeight>11in</PageHeight>" +
        //      "  <MarginTop>0.25in</MarginTop>" +
        //      "  <MarginLeft>0.25in</MarginLeft>" +
        //      "  <MarginRight>0.25in</MarginRight>" +
        //      "  <MarginBottom>0.25in</MarginBottom>" +
        //      "</DeviceInfo>";
        //    Warning[] warnings;
        //    byte[] rpbybe = report.Render("PDF", deviceInfo, out minetype, out encod, out fextension, out streamids,
        //       out warnings);
        //    using (FileStream fs = new FileStream(@"E:\", FileMode.Create))
        //    {
        //        fs.Write(rpbybe, 0, rpbybe.Length);
        //    }
        //}
         private void button7_Click(object sender, EventArgs e)
         {
           
         }
    

        private void button5_Click(object sender, EventArgs e)
        {
        //    string pathFolder = @"E:\CablesWindoApp3\ReportFolder\{0}\{1}.xls";
        //    SaveExl(rdlcReportViewer, string.Format(pathFolder, Settings.Default.ReportFolderNo.ToString(), Settings.Default.rptNo.ToString()));
            
        }

        private void pictureBox1_Click(object sender, EventArgs e)
        {
           
        }

        private void panel1_Paint(object sender, PaintEventArgs e)
        {

        }       

        private void label1_Click_1(object sender, EventArgs e)
        {

        }

        //private void button8_Click(object sender, EventArgs e)
        //{
        //    //// taking full path of a file
        //    //string strPath = "E://baumerCam.cs";

        //    //// initialize the value of filename
        //    //string filename = null;

        //    //// using the method
        //    //filename = Path.GetFileName(strPath);
        //    //Console.WriteLine("Filename = " + filename);


        //}

        private void label1_Click_2(object sender, EventArgs e)
        {
           // Bitmap algoImage = new Bitmap(@"E:\Work1\1.bmp");
           // globalVars.algo.getMeasurements4c(algoImage);
           // Console.WriteLine("imahe ");
           // algoImage.Save(@"E:\CablesWindoApp3\Image\myimage.bmp");
           // cblPitcurBox.Image = algoImage;
           // //PicBox1.Image = algoImage;
           //// Console.WriteLine(algoImage.PixelFormat.ToString());
        }

        private void pictureBox1_Click_1(object sender, EventArgs e)
        {

        }

        //List<abc> lst = new List<abc>();
        private void button8_Click(object sender, EventArgs e)
        {
           // CableImgPanel.Visible = false;
           // rdlcReportViewer.Visible = true;
           //lst.Clear();

           // lst.Add(new abc
           // {
           //    m1=UserInformationForm.PartName,
           // });
           // rptc.Add(new ReportParameter("rdlcPartNa", UserInformationForm.PartName));
           // rds.Name = "DataSet2";
           // rds.Value = lst;
           // this.rdlcReportViewer.LocalReport.DataSources.Clear();
           // this.rdlcReportViewer.LocalReport.DataSources.Add(rds);
           // this.rdlcReportViewer.LocalReport.ReportEmbeddedResource = "CablesWindoApp3.SixCore.rdlc";

            //Microsoft.Reporting.WinForms.ReportDataSource reportDataSource2 = new Microsoft.Reporting.WinForms.ReportDataSource("DataSet2");
            //rdlcReportViewer.LocalReport.DataSources.Clear();
            //rdlcReportViewer.LocalReport.ReportPath = "SixteenCoreReport3.rdlc";
            //rdlcReportViewer.LocalReport.DataSources.Add(reportDataSource2);
            //rdlcReportViewer.RefreshReport();


            ////ArrayList arList = new ArrayList();
            ////arList.Add(Settings.Default.ReportFolderNo);
            ////arList.Add();
            ////arList.Add(false);

            ////for(int i = 0; i < arList.Count; i++)
            ////{
            ////    Console.WriteLine(arList[i]);
            ////}

            ////string folder = new DirectoryInfo(path).Name;
            ////Console.WriteLine(folder);




        }

        private void ca4btn_Click(object sender, EventArgs e)
        {
            
        }

        private void backgroundWorker1_DoWork(object sender, System.ComponentModel.DoWorkEventArgs e)
        {

        }

        private void label6_Click(object sender, EventArgs e)
        {

        }

        private void label1_Click_3(object sender, EventArgs e)
        {

        }

        private void ca10btn_Click_1(object sender, EventArgs e)
        {
            
        }

        private void ca14btn_Click(object sender, EventArgs e)
        {
           
        }

        private void ca3btn_Click_1(object sender, EventArgs e)
        {
          
        }

        private void ca12btn_Click_1(object sender, EventArgs e)
        {
           
        }

        private void ca1btn_Click_1(object sender, EventArgs e)
        {
           
        }

        private void ca14btn_Click_1(object sender, EventArgs e)
        {
            
        }

        private void ca3btn_Click_2(object sender, EventArgs e)
        {
            
        }
        
        private void ca2btn_Click_1(object sender, EventArgs e)
        {
            //CableImgPanel.Visible = false;
            //rdlcReportViewer.Visible = true;
            ////rdlcReportViewer.Clear();
            //Console.WriteLine("Done");
            ////rptc.Add(new ReportParameter("ReportParameter1", UserInformationForm.PartName));
            ////rdlcReportViewer.LocalReport.SetParameters(rptc);
            ////rdlcReportViewer.LocalReport.ReportEmbeddedResource = "CablesWindoApp3.Report1.rdlc";

            //rdlcReportViewer.Reset();
            //rptc.Add(new ReportParameter("ReportParameter1", UserInformationForm.PartName));
            //rdlcReportViewer.LocalReport.SetParameters(rptc);
            ////rdlcReportViewer.LocalReport.ReportEmbeddedResource.Path=@"Report1.rdlc";
            //rdlcReportViewer.LocalReport.ReportPath = "CablesWindoApp3.Report1.rdlc";

            //rdlcReportViewer.RefreshReport();

            //rdlcReportViewer.Refresh();
        }

        private void ca13btn_Click_1(object sender, EventArgs e)
        {
           
        }

        private void label11_Click(object sender, EventArgs e)
        {

        }

        private void PicBox1_Click(object sender, EventArgs e)
        {

          
            //CropImage2();
           

            MessageBox.Show(string.Format("X: {0} Y: {1}", panel10.Location.X,Cursor.Position.Y));
        }

        private void PicBox1_MouseMove(object sender, MouseEventArgs e)
        {
            //if (PicBox1.Image == null)
            //{
            //    return;
            //}
            //if (e.Button == System.Windows.Forms.MouseButtons.Left)
            //{
            //    PicBox1.Refresh();
            //    cropWidth = e.X - cropX;
            //    cropHeight = e.Y - cropY;
            //    PicBox1.CreateGraphics().DrawRectangle(cropPen, cropX, cropY, cropWidth, cropHeight);
            //}
        }

        private void PicBox1_MouseDown(object sender, MouseEventArgs e)
        {
            //if (e.Button == System.Windows.Forms.MouseButtons.Left)
            //{
            //    Cursor = Cursors.Cross;
            //    cropX = e.X;
            //    cropY = e.Y;
            //    cropPen = new Pen(Color.Black, 2);
            //    cropPen.DashStyle = DashStyle.DashDotDot;
            //}
            //PicBox1.Refresh();
        }

        private void PicBox1_Paint(object sender, PaintEventArgs e)
        {
            //ControlPaint.DrawBorder(e.Graphics, pictureBox1.ClientRectangle, Color.Red, ButtonBorderStyle.Solid);
            e.Graphics.DrawLine(
            new Pen(Color.Red, 2f),
            new Point(PicBox1.Size.Width/2, 0),
            new Point(PicBox1.Size.Width/2, PicBox1.Size.Height));

            e.Graphics.DrawLine(
            new Pen(Color.Red, 2f),
            new Point(0, PicBox1.Size.Height/2),
            new Point(PicBox1.Size.Width, PicBox1.Size.Height/2));
            Pen p = Pens.Red;
            Rectangle r = new Rectangle(23, -70, PicBox1.Width - 45, PicBox1.Height + 160);
            //e.Graphics.DrawEllipse(p, new Rectangle(23, -70, PicBox1.Width - 45, PicBox1.Height + 160));
        }

        private void label16_Click(object sender, EventArgs e)
        {

        }

        private void label16_Click_1(object sender, EventArgs e)
        {

        }

        private void ProcessPicbox_Click(object sender, EventArgs e)
        {

        }

        private void ReportPicBox_Click(object sender, EventArgs e)
        {
            //SubReportNo++;
            //while (num > SubReportNo)
            //{
             
            //}


            //SetTextboxValue();
            //Console.WriteLine(SubReportNo);
           
                //rdlcReporttxtFun();
                //CoreFunRdlc();
                //this.rdlcReportViewer.RefreshReport();
                //CreateReportbtn_Click();
           
         


            //if (button == false)
            //{
            //    button = true;
            //    ReportPicBox.Image = Bitmap.FromFile("E:/CablesWindoApp3/buttonstosizefinal/GenrateReportOn.png");
            //    rpt.Show();

            //}
            //else
            //{
            //    button = false;
            //    ReportPicBox.Image = Bitmap.FromFile("E:/CablesWindoApp3/buttonstosizefinal/GenrateReport.png");
            //    rpt.Hide();
            //}
           



        }

        private void pictureBox7_Click(object sender, EventArgs e)
        {

        }

        private void CapturePicBox_Click(object sender, EventArgs e)
        {
            //if (but == false)
            //{
            //    but = true;
            //    cameraFun();
            //    //CapturePicBox.Image = Bitmap.FromFile("E:/CablesWindoApp3/buttonstosizefinal/CaptureOn.png");
            //}
            //else
            //{
            //    but = false;
            //    //CapturePicBox.Image = Bitmap.FromFile("E:/CablesWindoApp3/buttonstosizefinal/Capture.png");
            //}


        }

        private void ProcessPicBox_Click_1(object sender, EventArgs e)
        {          
              
           
        }

        private void label41_Click(object sender, EventArgs e)
        {

        }

        private void panel14_Paint(object sender, PaintEventArgs e)
        {

        }

        private void panel13_Paint(object sender, PaintEventArgs e)
        {

        }

        private void pictureBox5_Click(object sender, EventArgs e)
        {

        }

        private void ResetPicbox_Click(object sender, EventArgs e)
        {       
                      
        }

        private void Edittextbox_Click(object sender, EventArgs e)
        {
           
        }

      
        private void EditPickbox_Click(object sender, EventArgs e)
        {
            
          

        }

      
        private void SavePicBox_Click(object sender, EventArgs e)
        {       
             
        }

        private void panel17_Paint(object sender, PaintEventArgs e)
        {

        }   
             
            
        private void BottomLight_Click(object sender, EventArgs e)
        {
            if (setLight("bottom", !bottomLightState) == 1)
            {
                bottomLightState = !bottomLightState;
                //if (bottomLightState)
                //    btnBottomLight.ForeColor = Color.Green;
                //else
                //    btnBottomLight.ForeColor = Color.Black;

            }
            if (bottomLightState == true)
            {
                BottomLight.BackgroundImage = Bitmap.FromFile(globalVars.projectDirectory + @"/ProjectButton/BottomLightOn.png");
                //BottomLight.BackgroundImage = Bitmap.FromFile("E:/CablesWindoApp3/buttonstosizefinal/BottomLightOn.png");
            }
            else
            {
                BottomLight.BackgroundImage = Bitmap.FromFile(globalVars.projectDirectory + @"/ProjectButton/BottomLight.png");
                //BottomLight.BackgroundImage = Bitmap.FromFile("E:/CablesWindoApp3/buttonstosizefinal/BottomLight.png");
            }

        }

        private void button6_Click_1(object sender, EventArgs e)
        {

        }

        private void Camera_Click(object sender, EventArgs e)
        {
            if (but == false)
            {
                but = true;
                Camera.BackgroundImage = Bitmap.FromFile(globalVars.projectDirectory+"/buttonstosizefinal/CameraOn.png");
            }
            else
            {
                but = false;
                Camera.BackgroundImage = Bitmap.FromFile(globalVars.projectDirectory+"/buttonstosizefinal/Camera.png");
            }

        }

        private void pictureBox3_Click(object sender, EventArgs e)
        {           
            

        }
        private void ca2btn_Click_2(object sender, EventArgs e)
        {            

        }

        private void ca9btn_Click_1(object sender, EventArgs e)
        {
            //ca
        }

        private void ca1btn_Click_2(object sender, EventArgs e)
        {

        }

        private void ca4btn_Click_1(object sender, EventArgs e)
        {
            //ca4btn.BackgroundImage = Bitmap.FromFile("E:/CablesWindoApp3/buttonstosizefinal/Core_e.png");
        }

        private void ca10btn_Click_2(object sender, EventArgs e)
        {
        }

        private void ca14btn_Click_2(object sender, EventArgs e)
        {

        }

        private void pictureBox1_Click_2(object sender, EventArgs e)
        {
            //string result = File.ReadAllText(@"C:\Users\VisionSystem\Desktop\xlp.txt");
            //xp.xlpes = JsonConvert.DeserializeObject<List<Xlpe.XP>>(result);
            //string r = JsonConvert.SerializeObject(xp.xlpes);

            //Console.WriteLine(globalVars.algo.jsonFileProp); 
        }

        //-------------------------------------Cable MeasureMent Methods---------------------------//
        void GetUSERINFO()
        {
            //ReportNo = Convert.ToInt32(ReportNotxtbox.Text);
            //BatchNo = Convert.ToInt32(.Text);
            //TestNo = Convert.ToInt32(.Text);
            TestIdRef = TestIdReferencetxtbox.Text;
            CustomerId = CustomerIdtxtbox.Text;
            OperatorId = OperatorIdtxtbox.Text;
            //TemplateName = TempNaTxtBox.Text;
            CustomerName = CustomerNametxtbox.Text;
            OperatorName = OperatorNametxtbox.Text;
            date = Datetxtbox.Text;
            time = Timetxtbox.Text;

        }

        //--------------------------Panels mathods------------------//
        void UperPanels()
        {
            PanelMinThick.Visible = true;
            PanelAvgTh.Visible = true;
            PanelConc.Visible = true;
            PanelOD.Visible = true;
            PanelOvality.Visible = true;
            MinThick.Text = 0.000.ToString("N3");
            AvgThick.Text = 0.000.ToString("N3");
            OutDiam.Text = 0.000.ToString("N3");
            Ovality.Text = 0.000.ToString("N3");
            Concent.Text = 0.000.ToString("N3");
        }
        void TPanels(Panel T1, Panel T2)
        {
            T1.Visible = true;
            T2.Visible = true;
            lblT1.Text = 0.000.ToString("N3");
            lblT2.Text = 0.000.ToString("N3");
        }
        void TPanels(Panel T1, Panel T2, Panel T3)
        {
            T1.Visible = true;
            T2.Visible = true;
            T3.Visible = true;
            lblT1.Text = 0.000.ToString("N3");
            lblT2.Text = 0.000.ToString("N3");
            lblT3.Text = 0.000.ToString("N3");
        }
        void TPanels(Panel T1, Panel T2, Panel T3, Panel T4, Panel T5)
        {
            T1.Visible = true;
            T2.Visible = true;
            T3.Visible = true;
            T4.Visible = true;
            T5.Visible = true;
            lblT1.Text = 0.000.ToString("N3");
            lblT2.Text = 0.000.ToString("N3");
            lblT3.Text = 0.000.ToString("N3");
            lblT4.Text = 0.000.ToString("N3");
            lblT5.Text = 0.000.ToString("N3");

        }
        void TPanels(Panel T1, Panel T2, Panel T3, Panel T4, Panel T5, Panel T6)
        {
            T1.Visible = true;
            T2.Visible = true;
            T3.Visible = true;
            T4.Visible = true;
            T5.Visible = true;
            T6.Visible = true;
            lblT1.Text = 0.000.ToString("N3");
            lblT2.Text = 0.000.ToString("N3");
            lblT3.Text = 0.000.ToString("N3");
            lblT4.Text = 0.000.ToString("N3");
            lblT5.Text = 0.000.ToString("N3");
            lblT6.Text = 0.000.ToString("N3");

        }
        void clearLabels()
        {
            PanelMinThick.Visible = false;
            PanelAvgTh.Visible = false;
            PanelConc.Visible = false;
            PanelOD.Visible = false;
            PanelOvality.Visible = false;
            panelT1.Visible = false;
            panelT2.Visible = false;
            panelT3.Visible = false;
            PanelT4.Visible = false;
            PanelT5.Visible = false;
            PanelT6.Visible = false;

        }
        void showUpperLabels(float minTh, float avgTh, float OD, float Conc, float Ovl)
        {
            if (minTh > 0)
            {
                PanelMinThick.Visible = true;
                MinThick.Text = minTh.ToString();
            }
            if (avgTh > 0)
            {
                PanelAvgTh.Visible = true;
                AvgThick.Text = avgTh.ToString();
            }
            if (OD > 0)
            {
                PanelOD.Visible = true;
                OutDiam.Text = OD.ToString();
            }
            if (Conc > 0)
            {
                PanelConc.Visible = true;
                Concent.Text = Conc.ToString();
            }
            if (Ovl > 0)
            {
                PanelOvality.Visible = true;
                Ovality.Text = Ovl.ToString();
            }


        }/*-----------------cpp*/
        void showThicknessLabels(float T1, float T2, float T3, float T4, float T5, float T6)
        {
            if (T1 > 0)
            {
                panelT1.Visible = true;
                lblT1.Text = T1.ToString();
            }
            if (T2 > 0)
            {
                panelT2.Visible = true;
                lblT2.Text = T2.ToString();
            }
            if (T3 > 0)
            {
                panelT3.Visible = true;
                lblT3.Text = T3.ToString();
            }
            if (T4 > 0)
            {
                PanelT4.Visible = true;
                lblT4.Text = T4.ToString();
            }
            if (T5 > 0)
            {
                PanelT5.Visible = true;
                lblT5.Text = T5.ToString();
            }
            if (T6 > 0)
            {
                PanelT6.Visible = true;
                lblT6.Text = T6.ToString();
            }

        }
        void showThicknessLabels(float T1, float T2, float T3)
        {
            panelT1.Visible = true;
            lblT1.Text = T1.ToString();
            panelT2.Visible = true;
            lblT2.Text = T2.ToString();
            panelT3.Visible = true;
            lblT3.Text = T3.ToString();

        }/*----------------cpp*/
        public void Thickness()
        {
            clearLabels();
            switch (selectedTemplate)                          /////---------------------------------CPP
            {// function call  to clear and hide labels
                case "CORE1":
                    showUpperLabels(globalVars.algo.minThicknessProp, globalVars.algo.avgThicknessProp, globalVars.algo.outerDiaProp, globalVars.algo.concentricityProp, globalVars.algo.ovalityProp);
                    showThicknessLabels(globalVars.algo.getSectorThickness(0), globalVars.algo.getSectorThickness(1), globalVars.algo.getSectorThickness(2), globalVars.algo.getSectorThickness(3), globalVars.algo.getSectorThickness(4), globalVars.algo.getSectorThickness(5));
                    break;
                case "CORE2":
                    showUpperLabels(globalVars.algo.minThicknessProp, globalVars.algo.avgThicknessProp, globalVars.algo.outerDiaProp, globalVars.algo.concentricityProp, globalVars.algo.ovalityProp);
                    showThicknessLabels(globalVars.algo.getSectorThickness(0), globalVars.algo.getSectorThickness(1), globalVars.algo.getSectorThickness(2), globalVars.algo.getSectorThickness(3), globalVars.algo.getSectorThickness(4), globalVars.algo.getSectorThickness(5));
                    break;

                case "CORE3":
                    Console.WriteLine("Function core3");
                    showUpperLabels(globalVars.algo.minThicknessProp, globalVars.algo.avgThicknessProp, globalVars.algo.outerDiaProp, globalVars.algo.concentricityProp, globalVars.algo.ovalityProp);
                    showThicknessLabels(globalVars.algo.getSectorThickness(0), globalVars.algo.getSectorThickness(1), globalVars.algo.getSectorThickness(2), globalVars.algo.getSectorThickness(3), globalVars.algo.getSectorThickness(4), globalVars.algo.getSectorThickness(5));
                    break;
                case "CORE4":
                    showUpperLabels(globalVars.algo.minThicknessProp, globalVars.algo.avgThicknessProp, globalVars.algo.outerDiaProp, globalVars.algo.concentricityProp, globalVars.algo.ovalityProp);
                    showThicknessLabels(globalVars.algo.getSectorThickness(0), globalVars.algo.getSectorThickness(1), globalVars.algo.getSectorThickness(2), globalVars.algo.getSectorThickness(3), globalVars.algo.getSectorThickness(4), globalVars.algo.getSectorThickness(5));
                    break;
                case "CORE5":
                    showUpperLabels(globalVars.algo.minThicknessProp, globalVars.algo.avgThicknessProp, globalVars.algo.outerDiaProp, globalVars.algo.concentricityProp, globalVars.algo.ovalityProp);
                    showThicknessLabels(globalVars.algo.getSectorThickness(0), globalVars.algo.getSectorThickness(1), globalVars.algo.getSectorThickness(2), globalVars.algo.getSectorThickness(3), globalVars.algo.getSectorThickness(4), globalVars.algo.getSectorThickness(5));
                    break;
                case "MULTICORE":
                    showUpperLabels(globalVars.algo.minThicknessProp, globalVars.algo.avgThicknessProp, globalVars.algo.outerDiaProp, globalVars.algo.concentricityProp, globalVars.algo.ovalityProp);
                    showThicknessLabels(globalVars.algo.getSectorThickness(0), globalVars.algo.getSectorThickness(1), globalVars.algo.getSectorThickness(2), globalVars.algo.getSectorThickness(3), globalVars.algo.getSectorThickness(4), globalVars.algo.getSectorThickness(5));
                    break;
                case "XLPE":
                    showUpperLabels(globalVars.algo.minThicknessProp, globalVars.algo.avgThicknessProp, globalVars.algo.outerDiaProp, globalVars.algo.concentricityProp, globalVars.algo.ovalityProp);
                    showThicknessLabels(globalVars.algo.getSectorThickness(0), globalVars.algo.getSectorThickness(1), globalVars.algo.getSectorThickness(2), globalVars.algo.getSectorThickness(3), globalVars.algo.getSectorThickness(4), globalVars.algo.getSectorThickness(5));
                    break;
                case "FLATECABLE":
                    showUpperLabels(globalVars.algo.minThicknessProp, globalVars.algo.avgThicknessProp, globalVars.algo.outerDiaProp, globalVars.algo.concentricityProp, globalVars.algo.ovalityProp);
                    showThicknessLabels(globalVars.algo.getSectorThickness(0), globalVars.algo.getSectorThickness(1), globalVars.algo.getSectorThickness(2), globalVars.algo.getSectorThickness(3), globalVars.algo.getSectorThickness(4), globalVars.algo.getSectorThickness(5));
                    break;



                    //    //        //        //default
                    //    //        //        //    MinThick.Text = globalVars.algo.minThicknessProp.ToString("N3");
                    //    //        //        //    OutDiam.Text = globalVars.algo.outerDiaProp.ToString("N3");
                    //    //        //        //    Concentricity.Text = globalVars.algo.concentricityProp.ToString("N3");
                    //    //        //        //    AvgThick.Text = globalVars.algo.avgThicknessProp.ToString("N3");
                    //    //        //        //    //ovality
                    //    //        //        //    // show labels (bool minth, bool outDia, 3, 4, 5)
                    //    //        //        //    //{ 
                    //    //        //        //    //MinThick.Visible = minth;
                    //    //        //        //    // }

                    //    //        //        //    // for thicknell labels (6 args)
                    //    //        //        //    lblT1.Text = globalVars.algo.getSectorThickness(0).ToString("N3");
                    //    //        //        //    lblT2.Text = globalVars.algo.getSectorThickness(1).ToString("N3");
                    //    //        //        //    lblT3.Text = globalVars.algo.getSectorThickness(2).ToString("N3");



            }
            this.Refresh();
        }
        //----------------------------------------cpp

        //------------------ZoomIn&Out Mehtod ------------//
        void ZoomInCableImgPanel()
        {
            if (cblPitcurBox.Width < (15 * CableImgPanel.Width) && (cblPitcurBox.Height < (15 * CableImgPanel.Height)))
            {

                //Change pictureBox Size and Multiply Zoomfactor
                cblPitcurBox.Width = (int)(cblPitcurBox.Width * 1.25);
                cblPitcurBox.Height = (int)(cblPitcurBox.Height * 1.25);

                //Move Picture box
                cblPitcurBox.Top = (int)(204 - 1.25 * (204 - cblPitcurBox.Top));
                cblPitcurBox.Left = (int)(302 - 1.25 * (302 - cblPitcurBox.Left));
                //label1.Text = Convert.ToString(e.X);
                //label2.Text = Convert.ToString(e.Y);
            }
        }


        void ZoomInCableImgPanel2(double ZoomFactor)
        {

            if (cblPitcurBox.Width < (15 * CableImgPanel.Width) && (cblPitcurBox.Height < (15 * CableImgPanel.Height)))
            {

                //Change pictureBox Size and Multiply Zoomfactor
                cblPitcurBox.Width = (int)(cblPitcurBox.Width * ZoomFactor);
                cblPitcurBox.Height = (int)(cblPitcurBox.Height * ZoomFactor);

                //Move Picture box
                cblPitcurBox.Top = (int)(204 - ZoomFactor * (204 - cblPitcurBox.Top));
                cblPitcurBox.Left = (int)(302 - ZoomFactor * (302 - cblPitcurBox.Left));
                //label1.Text = Convert.ToString(e.X);
                //label2.Text = Convert.ToString(e.Y);
            }
        }
        //void ZoomInCableImgPanel2(double ZoomFactor)
        //{

        //    if (cblPitcurBox.Width < (15 * CableImgPanel.Width) && (cblPitcurBox.Height < (15 * CableImgPanel.Height)))
        //    {

        //        //Change pictureBox Size and Multiply Zoomfactor
        //        cblPitcurBox.Width = (int)(cblPitcurBox.Width * ZoomFactor);
        //        cblPitcurBox.Height = (int)(cblPitcurBox.Height * ZoomFactor);

        //        //Move Picture box
        //        cblPitcurBox.Top = (int)(204 - ZoomFactor * (204 - cblPitcurBox.Top));
        //        cblPitcurBox.Left = (int)(302 - ZoomFactor * (302 - cblPitcurBox.Left));
        //        //label1.Text = Convert.ToString(e.X);
        //        //label2.Text = Convert.ToString(e.Y);
        //    }
        //}
        void ZoomOutCableImgPanel()
        {
            if ((cblPitcurBox.Width > CableImgPanel.Width) && (cblPitcurBox.Height > CableImgPanel.Height))
            {
                //Change pictureBox Size and Multiply Zoomfactor
                cblPitcurBox.Width = (int)(cblPitcurBox.Width / 1.25);
                cblPitcurBox.Height = (int)(cblPitcurBox.Height / 1.25);

                //Move Picture box
                cblPitcurBox.Top = (int)(204 - 0.80 * (204 - cblPitcurBox.Top));
                cblPitcurBox.Left = (int)(302 - 0.80 * (302 - cblPitcurBox.Left));
            }
        }
        void ZoomOutCableImgPanel2(int zoomin)
        {
            if ((cblPitcurBox.Width > CableImgPanel.Width) && (cblPitcurBox.Height > CableImgPanel.Height))
            {
                //Change pictureBox Size and Multiply Zoomfactor
                cblPitcurBox.Width = (int)(cblPitcurBox.Width / zoomin);
                cblPitcurBox.Height = (int)(cblPitcurBox.Height / zoomin);

                //Move Picture box
                cblPitcurBox.Top = (int)(204 - 0.80 * (204 - cblPitcurBox.Top));
                cblPitcurBox.Left = (int)(302 - 0.80 * (302 - cblPitcurBox.Left));
            }
        }


        void ZoomInPnl10()
        {
            if (PicBox1.Image != null)
            {
                //PictureBox Dimensions Are range in 15
                if (PicBox1.Width < (15 * panel10.Width) && (PicBox1.Height < (15 * panel10.Height)))
                {

                    //Change pictureBox Size and Multiply Zoomfactor
                    PicBox1.Width = (int)(PicBox1.Width * 1.25);
                    PicBox1.Height = (int)(PicBox1.Height * 1.25);

                    //Move Picture box
                    PicBox1.Top = (int)(203 - 1.25 * (203 - PicBox1.Top));
                    PicBox1.Left = (int)(300 - 1.25 * (300 - PicBox1.Left));
                    //label1.Text = Convert.ToString(e.X);
                    //label2.Text = Convert.ToString(e.Y);
                }

            }
        }
        void ZoomOutPnl10()
        {
            if (PicBox1.Image != null)
            {
                if ((PicBox1.Width > panel10.Width) && (PicBox1.Height > panel10.Height))
                {
                    //Change pictureBox Size and Multiply Zoomfactor
                    PicBox1.Width = (int)(PicBox1.Width / 1.25);
                    PicBox1.Height = (int)(PicBox1.Height / 1.25);

                    //Move Picture box
                    PicBox1.Top = (int)(203 - 0.80 * (203 - PicBox1.Top));
                    PicBox1.Left = (int)(300 - 0.80 * (300 - PicBox1.Left));

                }
            }
        }

        void jsonFun()
        {
            //uic.TemplateName = UserInformationForm.TemplateName;
            uic.ReportNo = UserInformationForm.ReportNo;
            uic.opname = UserInformationForm.OperatorName;
            uic.opid = UserInformationForm.OperatorId;
            uic.CustomerName = UserInformationForm.CustomerName;
            uic.CustomerId = UserInformationForm.CustomerId;
            uic.BatchNo = UserInformationForm.BatchNo;
            uic.Testno = UserInformationForm.TestNo;
            uic.TestIdRef = UserInformationForm.TestIdRef;

            string jsonpath = @"E:\CablesWindoApp3\ReportFolder\{0}\{1}.json";
            String json = Newtonsoft.Json.JsonConvert.SerializeObject(uic);
            System.IO.File.WriteAllText(string.Format(jsonpath, Settings.Default.ReportFolderNo.ToString(), Settings.Default.ReportFolderNo.ToString()), json);
            Console.WriteLine("Insert json file");
        }
        public void CoreButtonsEnbled()              //-----Core Buttons Enbled Method-----//
        {
            foreach (Button bt in btnArray)
            {
                bt.Enabled = true;
            }
        }

        public void CheckBatchReportNo()        //--------------Check Batch Report No------------------//
        {
            if (SubReportNo <= num)
            {
                SetTextboxValue2();
                SubReportNo++;
                ResetImage();
                RestPicturBox();
                
            }
            //if (num <= SubReportNo)
            //{

            //    GenrateReportBtn.Enabled = false;
            //    MessageBox.Show("Cross Batch No Limit");
            //}
            //Console.WriteLine(SubReportNo);
        }
         
        public void CoreButtonsDisabled()           //-----Core  Buttons Enbled Method-----//
        {
            foreach (Button bt in btnArray)
            {
                bt.Enabled = false;
            }
        }
        void CropImage()
        //-----Crop Image On MousDown and Mouse Move -----//
        {
            Cursor = Cursors.Default;
            if (cropWidth < 1)
            {
                return;
            }
            Rectangle rect = new Rectangle(cropX, cropY, cropWidth, cropHeight);
            //First we define a rectangle with the help of alredy calculated points
            Bitmap OriginalImage = new Bitmap(cblPitcurBox.Image, cblPitcurBox.Width, cblPitcurBox.Height);
            // Original image
            Bitmap _img = new Bitmap(cropWidth, cropHeight);
            //for cropinf image

            Graphics g = Graphics.FromImage(_img);
            //crete graphics
            g.InterpolationMode = System.Drawing.Drawing2D.InterpolationMode.HighQualityBicubic;
            g.PixelOffsetMode = System.Drawing.Drawing2D.PixelOffsetMode.HighQuality;
            g.CompositingQuality = System.Drawing.Drawing2D.CompositingQuality.HighQuality;
            // set image attributes
            g.DrawImage(OriginalImage, 0, 0, rect, GraphicsUnit.Pixel);
            btmp = _img;
            string imagepath = globalVars.projectDirectory + @"\ReportFolder\{0}\Process.bmp";
            btmp.Save(string.Format(imagepath, Settings.Default.ReportFolderNo.ToString()));
        }
        //int cropWidth;
        void CropImage2()                              //-----Crop Image  OnButtonClick-----//  
        {
            //Cursor = Cursors.Default;
            //if (cropWidth < 1)
            //{
            //    return;
            //}
            //cropX = 325;
            //cropY = 322;

            //AfterCrop();
            //cropWidth = 325 - cropX;
            //cropHeight = PicBox1.Height - cropY;
            Rectangle rect = new Rectangle(325,343, panel10.Width, panel10.Height);
            //First we define a rectangle with the help of alredy calculated points
            Bitmap OriginalImage = new Bitmap(PicBox1.Image, PicBox1.Width, PicBox1.Height);
            // Original image
            Bitmap _img = new Bitmap(panel10.Width,panel10.Height);
            //for cropinf image
            //cblPitcurBox.Image =(Image)_img;
            Graphics g = Graphics.FromImage(_img);
            //crete graphics
            g.InterpolationMode = System.Drawing.Drawing2D.InterpolationMode.HighQualityBicubic;
            g.PixelOffsetMode = System.Drawing.Drawing2D.PixelOffsetMode.HighQuality;
            g.CompositingQuality = System.Drawing.Drawing2D.CompositingQuality.HighQuality;
            // set image attributes
            g.DrawImage(OriginalImage, 0, 0, rect, GraphicsUnit.Pixel);
            cblPitcurBox.Image = _img;
            //string imagepath = @"E:\CablesWindoApp3\ReportFolder\{0}\Process.bmp";
            //btmp.Save(string.Format(imagepath, Settings.Default.ReportFolderNo.ToString()));
        }                       

        void RestPicturBox()                           //-----ResetPictureBox2  Buttons Enbled Method-----//    
        {
            cblPitcurBox.Location = new Point(4,5);
            cblPitcurBox.Width = 595;
            cblPitcurBox.Height = 399;

        }
        public void ResetImage()                    //-----Core  Buttons Enbled Method-----// 
        {
            if (cblPitcurBox.Image != null)
            {
                cblPitcurBox.Image.Dispose();
                cblPitcurBox.Image = null;
            }
        }
        void ClearTextBoxesValue()
        {
            CustomerNametxtbox.Clear();
            CustomerIdtxtbox.Clear();
            OperatorNametxtbox.Clear();
            OperatorIdtxtbox.Clear();
            TestIdReferencetxtbox.Clear();
        } //-----Claer UserValuew-----// 
        void TextBoxesValue()
        {
            ReportNotxtbox.Clear();
            Datetxtbox.Clear();
            Timetxtbox.Clear();
            CustomerNametxtbox.Clear();
            CustomerIdtxtbox.Clear();
            OperatorNametxtbox.Clear();
            OperatorIdtxtbox.Clear();
            TestIdReferencetxtbox.Clear();
        } //-----Claer TextBox Value-----// 


        public void EnbledBottomButton()     //--------- Enbled Bottom Buttons Method ------------//
        {
            CaptureBtn.Enabled = true;
            ResetBtn.Enabled = true;
            SnapShotBtn.Enabled = true;
            GenrateReportBtn.Enabled = true;
            ProcessBtn.Enabled = true;

        }
        public void DisbledBottomButton()    //--------- Disbled Bottom Buttons Method ------------//
        {
            CaptureBtn.Enabled = false;
            ResetBtn.Enabled = false;
            SnapShotBtn.Enabled = false;
            GenrateReportBtn.Enabled = false;
            ProcessBtn.Enabled = false;
        }
        public void ReportFormShow()         //--------- Report Form Show Method ------------//
        {
            if (rpt.IsDisposed == true)
            {
                rpt = new Report();
                
            }
            rpt.ShowDialog();           
        }
        public void CalibrationFormShow()         //--------- Report Form Show Method ------------//
        {
            if (clForm.IsDisposed == true)
            {
                clForm = new CalibrationForm();

            }
            clForm.ShowDialog();
        }
        public void SearchFormShow()         //--------- Search Form Show Method ------------//
        {
            if (rpt.IsDisposed == true)
            {
                SrForm = new SearchForm();

            }
            SrForm.ShowDialog();
        }
        public void BatchFormShow()         //--------- Report Form Show Method ------------//
        {
            if (btf.IsDisposed == true)
            {
                btf = new BatchForm();

            }
            btf.Show();
        }
        public void SettingFormShow()         //--------- Setting Form Show Method ------------//
        {
            if (rpt.IsDisposed == true)
            {
                SForm = new SetttingFormcs();

            }
            SForm.ShowDialog();
        }
        void Core2Zoom()                       //--------- Zoom Image On Captur Button ------------//     
        {

            try
            {
                cblPitcurBox.Image = procImage;
                cblPitcurBox.Refresh();
                //globalVars.projectDirectory + @"/ProjectButton/"
                string imgpath = globalVars.projectDirectory + @"\ReportFolder\{0}\Cable.bmp";
                //string imgpath = @"E:\CablesWindoApp3\ReportFolder\{0}\Cable.bmp";
                cblPitcurBox.Image.Save(string.Format(imgpath, Settings.Default.ReportFolderNo.ToString()));

            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
            
        }

        void OutDiamet()                      //Get outerDiameter Value //
        {
            //switch

            OutTempCaptur = Convert.ToInt32(globalVars.algo.outerDiaProp);
            Console.WriteLine("OutDiameter" + OutTempCaptur);
            RestPicturBox();

        }                        
        private void StartNewTestBtn_MouseDown(object sender, MouseEventArgs e)
        {
            Console.WriteLine(selButton);

            //RestPicturBox();
            //------CoreButtons--------//          


            if (selButton != null)
            {
                ResetImage();
                RestPicturBox();
                SetTextboxValue();
                rpt.Close();
                DirectoryInfo di = Directory.CreateDirectory(globalVars.projectDirectory + @"\ReportFolder\" + Settings.Default.ReportFolderNo + "");
                //DirectoryInfo di = Directory.CreateDirectory(@"E:\CablesWindoApp3\ReportFolder\" + Settings.Default.ReportFolderNo + "");
                button.BackgroundImage = Bitmap.FromFile(globalVars.projectDirectory + @"/ProjectButton/" + selButton + "_D.png");
                PanelXple.Visible = false;
                //button.BackgroundImage = Bitmap.FromFile("E:/CablesWindoApp3/buttonstosizefinal/new70_70/" + selButton + "_D.png");
                this.StartNewTestBtn.BackgroundImage = global::CablesWindoApp3.Properties.Resources.START_NEW_TESTON;
                clearLabels();

                //-------------Reset Method--------//  

                //-------------TextBox Value Method--------//


                //StartNewTestBtn.BackgroundImage = Bitmap.FromFile("E:/CablesWindoApp3/buttonstosizefinal/START_NEW_TESTON.png");

            }
            else
            {
                RestPicturBox();
                DirectoryInfo di = Directory.CreateDirectory(globalVars.projectDirectory + @"\ReportFolder\" + Settings.Default.ReportFolderNo + "");
                //DirectoryInfo di = Directory.CreateDirectory(@"E:\CablesWindoApp3\ReportFolder\" + Settings.Default.ReportFolderNo + "");
                //------------Core Buttons Method----------//
                CoreButtonsEnbled();
                //-------------TextBox Value Method--------//
                SetTextboxValue();
                clearLabels();
                BatchPanel.Visible = false;
                this.BatchBtn.BackgroundImage = global::CablesWindoApp3.Properties.Resources.BatchButton;

                this.StartNewTestBtn.BackgroundImage = global::CablesWindoApp3.Properties.Resources.START_NEW_TESTON;

                //StartNewTestBtn.BackgroundImage = Bitmap.FromFile("E:/CablesWindoApp3/buttonstosizefinal/START_NEW_TESTON.png");
            }
            //MessageBox.Show("New Folder"+ Settings.Default.ReportFolderNo);


        }
        private void CreateReportbtn_Click()
        {

            //jsonFun();
            //string pPath =;
            //string fPath = string.Format(pPath, Settings.Default.ReportFolderNo, Settings.Default.rptNo);
            //string FRptno = @"E:\CablesWindoApp3\ReportFolder\" + Settings.Default.ReportFolderNo + "\"+ Settings.Default.rptNo+".pdf";
          
            //SavePDF(rpt.reportViewer1, @"E:\CablesWindoApp3\ReportFolder\" + Settings.Default.ReportFolderNo + "\"" + Settings.Default.rptNo + ".pdf");
           
           


            //string ExlpathFolder = @"E:\CablesWindoApp3\ReportFolder\{0}\{1}.xls";
            //SaveExl(reportViewer1, string.Format(ExlpathFolder, Settings.Default.ReportFolderNo.ToString(), Settings.Default.ReportFolderNo.ToString()));

            //string imagepath = @"E:\CablesWindoApp3\ReportFolder\{0}\{1}.bmp";
            //btmp.Save(string.Format(imagepath, Settings.Default.ReportFolderNo.ToString(), Settings.Default.ReportFolderNo.ToString()));


        }
        public void SavePDF(ReportViewer viewer, string savePath)
        {

            Warning[] warnings;
            string[] streamids;
            string mimeType;
            string encoding;
            string filenameExtension;
            byte[] bytes = viewer.LocalReport.Render(
                "PDF", null, out mimeType, out encoding, out filenameExtension,
                out streamids, out warnings);

            using (FileStream fs = new FileStream(savePath, FileMode.Create))
            {
                fs.Write(bytes, 0, bytes.Length);
            }
        }
        public void SaveExl(ReportViewer viewer, string savePath)
        {

            Warning[] warnings;
            string[] streamids;
            string mimeType;
            string encoding;
            string filenameExtension;


            byte[] bytes = viewer.LocalReport.Render(
                "Excel", null, out mimeType, out encoding, out filenameExtension,
                out streamids, out warnings);

            using (FileStream fs = new FileStream(savePath, FileMode.Create))
            {
                fs.Write(bytes, 0, bytes.Length);
            }
        }
        public static uint m;
        void SetExposure()
        {
            if (cam_ != null)
            {
                ////uint n = (uint)trackBar1.Value;
                //uint n = (uint)numericUpDown1.Value;
                cam_.put_AutoExpoEnable(false);
                cam_.put_ExpoTime(m);
                //cam_.put_ExpoAGain(270);
                //cam_.put_Brightness(24);
                //cam_.put_Contrast(100);

                //label1.Text = (n / 1000).ToString() + " ms";
            }
        }
        void Exposure()
        {
            if (cam_ != null)
            {
                ////uint n = (uint)trackBar1.Value;
                //uint n = (uint)numericUpDown1.Value;
                //cam_.put_AutoExpoEnable(false);
                cam_.put_ExpoTime(2000);
                //cam_.put_Brightness(0);
                //cam_.put_ExpoAGain(270);
                //cam_.put_Brightness(24);
                //cam_.put_Contrast(0);

                //label1.Text = (n / 1000).ToString() + " ms";
            }
        }
        void GetDataGridValue()
        {
            DataGridViewRow newRow = new DataGridViewRow();
            newRow.CreateCells(ThreeCoreDataGridView);
            newRow.Cells[0].Value = SubReportNo;
            /// --------------------------------------------------------------cpp
            newRow.Cells[1].Value = globalVars.algo.getSectorThickness(0).ToString("N3");
            newRow.Cells[2].Value = globalVars.algo.getSectorThickness(1).ToString("N3");
            newRow.Cells[3].Value = globalVars.algo.getSectorThickness(2).ToString("N3");
            newRow.Cells[4].Value = globalVars.algo.getSectorThickness(3).ToString("N3");
            newRow.Cells[5].Value = globalVars.algo.getSectorThickness(4).ToString("N3");
            newRow.Cells[6].Value = globalVars.algo.getSectorThickness(5).ToString("N3");
            newRow.Cells[7].Value = globalVars.algo.minThicknessProp.ToString("N3");
            newRow.Cells[8].Value = globalVars.algo.avgThicknessProp.ToString("N3");
            newRow.Cells[9].Value = globalVars.algo.outerDiaProp.ToString("N3");
            newRow.Cells[10].Value = globalVars.algo.concentricityProp.ToString("N3");
            newRow.Cells[11].Value = globalVars.algo.ovalityProp.ToString("N3");
            //newRow.Cells[12].Value = /*globalVars.algo.ovalityProp;

            ThreeCoreDataGridView.Rows.Add(newRow);
        }

        private void StartNewTestBtn_MouseUp(object sender, MouseEventArgs e)        
        {          
          
            foreach (Button bt in btnArray)
            {
                bt.Enabled = true;
            }
            this.StartNewTestBtn.BackgroundImage = global::CablesWindoApp3.Properties.Resources.START_NEW_TEST;
            //StartNewTestBtn.BackgroundImage = Bitmap.FromFile("E:/CablesWindoApp3/buttonstosizefinal/START_NEW_TEST.png");
        }

        private void StartNewTestBtn_Click(object sender, EventArgs e)
        {
                      
        }

        private void Form1_MouseClick(object sender, MouseEventArgs e)
        {
            Console.WriteLine("x: " + Cursor.Position.X + " y: " + Cursor.Position.Y);
        }

        private void button4_Click_1(object sender, EventArgs e)
        {


            //foreach (DataGridViewRow dgv in dataGridView1.Rows)
            //{
            //    dataGridView1.Rows.Add(
            //                            dgv.Cells[0].Value = globalVars.algo.getSectorThickness(0).ToString("N3"),
            //                            dgv.Cells[1].Value = globalVars.algo.getSectorThickness(1).ToString("N3"),
            //                            dgv.Cells[2].Value = globalVars.algo.getSectorThickness(2).ToString("N3")
            //                            );
            //}
        }

        private void panel12_Paint(object sender, PaintEventArgs e)
        {

        }

        
        private void button3_Click_2(object sender, EventArgs e)
        {
           
          
        }

        private void BatchBtn_MouseDown(object sender, MouseEventArgs e)
        {
            rpt.Close();
            ResetImage();
            RestPicturBox();
            DirectoryInfo di = Directory.CreateDirectory(globalVars.projectDirectory + @"\ReportFolder\" + Settings.Default.ReportFolderNo + "");
            //DirectoryInfo di = Directory.CreateDirectory(@"E:\CablesWindoApp3\ReportFolder\" + Settings.Default.ReportFolderNo + "");
            PanelXple.Visible = false;
            this.BatchBtn.BackgroundImage = global::CablesWindoApp3.Properties.Resources.BatchButtonOn;
            CoreButtonsEnbled();


        }

        private void BatchBtn_MouseUp(object sender, MouseEventArgs e)
        {
           
        }

        private void BatchBtn_MouseLeave(object sender, EventArgs e)
        {
           
        }

        private void Okbtn_Click(object sender, EventArgs e)
        {
            num = Convert.ToInt32(BatchTxtbox.Text);
            //Console.WriteLine("Num="+num);            
            SetTextboxValue2();
            BatchPanel.Hide();
            //MessageBox.Show("Please Select Core");
        }

        private void BatchBtn_Click(object sender, EventArgs e)
        {
            if (selButton != null)
            {
                button.BackgroundImage = Bitmap.FromFile(globalVars.projectDirectory + @"/ProjectButton/" + selButton + "_D.png");
                //button.BackgroundImage = Bitmap.FromFile("E:/CablesWindoApp3/buttonstosizefinal/new70_70/" + selButton + "_D.png");

                //-------------Reset Method--------//  
                ResetImage();
                //-----------------Core Buttons Enbled Method------------------//
                CoreButtonsEnbled();
               
                BatchBtn.BackgroundImage = Bitmap.FromFile(globalVars.projectDirectory + @"/ProjectButton/BatchButtonOn.png");
                BatchPanel.Show();
                rpt.Close();
            }
            BatchPanel.Show();
            //----------------Enabled Core Buttons Method-------------------//
            CoreButtonsEnbled();
             BatchBtn.BackgroundImage = Bitmap.FromFile(globalVars.projectDirectory + @"/ProjectButton/BatchButtonOn.png");
            //BatchBtn.BackgroundImage = Bitmap.FromFile("E:/CablesWindoApp3/buttonstosizefinal/BatchButtonOn.png");
        }

        private void EditBtn_MouseDown(object sender, MouseEventArgs e)
        {                
                          //------- Textbox Value Clear Method --------//
            ClearTextBoxesValue();
            this.EditBtn.BackgroundImage = global::CablesWindoApp3.Properties.Resources.EditOn;
            //EditBtn.BackgroundImage = Bitmap.FromFile("E:/CablesWindoApp3/buttonstosizefinal/EditOn.png");

        }

        private void EditBtn_MouseUp(object sender, MouseEventArgs e)
        {
            this.EditBtn.BackgroundImage = global::CablesWindoApp3.Properties.Resources.Edit;
            //EditBtn.BackgroundImage = Bitmap.FromFile("E:/CablesWindoApp3/buttonstosizefinal/Edit.png");
        }

        private void SaveBtn_MouseDown(object sender, MouseEventArgs e)
        {           
            //--------------- Get TextBox Value Method-------------------//
            GetUSERINFO();
            //--------------- Get TextBox Value Method-------------------//
            CoreButtonsDisabled();
            this.SaveBtn.BackgroundImage = global::CablesWindoApp3.Properties.Resources.SaveOn;
            //SaveBtn.BackgroundImage = Bitmap.FromFile("E:/CablesWindoApp3/buttonstosizefinal/SaveOn.png");
        }

        private void SaveBtn_MouseUp(object sender, MouseEventArgs e)
        {
            this.SaveBtn.BackgroundImage = global::CablesWindoApp3.Properties.Resources.Save;
            //SaveBtn.BackgroundImage = Bitmap.FromFile("E:/CablesWindoApp3/buttonstosizefinal/Save.png");
        }

        private void CaptureBtn_MouseDown(object sender, MouseEventArgs e)
        {
            //-----------Capture Image Method--------------//

            //ZoomInCableImgPanel2();

            cameraFun();
            this.CaptureBtn.BackgroundImage = global::CablesWindoApp3.Properties.Resources.CaptureOn;
            //cblPitcurBox.Image = PicBox1.Image;

            //ZoomInCableImgPanel2(zoomin);
            //cblPitcurBox.Image = PicBox1.Image;
            //zoomin = 0;

            //CaptureBtn.BackgroundImage = Bitmap.FromFile("E:/CablesWindoApp3/buttonstosizefinal/CaptureOn.png");
            //cblPitcurBox.SizeMode = PictureBoxSizeMode.CenterImage;
            //---------------------------------------------------------------Xlpe------------------------------------//
            //xp.xlpes[0].avgThick;
            //xp.xlpes[1].avgThick;
            //label10.Text = xp.xlpes[2].minThick.ToString();

        }

        private void CaptureBtn_MouseUp(object sender, MouseEventArgs e)
        {
            //CaptureBtn.BackgroundImage = Bitmap.FromFile("E:/CablesWindoApp3/buttonstosizefinal/Capture.png");
            this.CaptureBtn.BackgroundImage = global::CablesWindoApp3.Properties.Resources.Capture;
        }

        private void ResetBtn_MouseDown(object sender, MouseEventArgs e)
        {
                   //---------Reset Image Method------------//
            ResetImage();
            this.ResetBtn.BackgroundImage = global::CablesWindoApp3.Properties.Resources.ResetOn;

            //ResetBtn.BackgroundImage = Bitmap.FromFile("E:/CablesWindoApp3/buttonstosizefinal/ResetOn.png");
        }

        private void ResetBtn_MouseUp(object sender, MouseEventArgs e)
        {
            this.ResetBtn.BackgroundImage = global::CablesWindoApp3.Properties.Resources.Reset;
            //ResetBtn.BackgroundImage = Bitmap.FromFile("E:/CablesWindoApp3/buttonstosizefinal/Reset.png");
        }

        private void ProcessBtn_MouseDown(object sender, MouseEventArgs e)
        {

           
            this.ProcessBtn.BackgroundImage = global::CablesWindoApp3.Properties.Resources.ProcessOn;
            ProcessBtn.Refresh();
            //-------------Sample Measurement Mehtod---------------// 
            measureFun();
            //-------------Thickness Mehtod---------------// 

            Thickness();
            OutTempCaptur = Convert.ToInt32(globalVars.algo.outerDiaProp);
            Console.WriteLine("Temp=" + OutTempCaptur);

            
          
            //ProcessBtn.BackgroundImage = Bitmap.FromFile("E:/CablesWindoApp3/buttonstosizefinal/ProcessOn.png");
        }
       
        private void ProcessBtn_MouseUp(object sender, MouseEventArgs e)
        {
            this.ProcessBtn.BackgroundImage = global::CablesWindoApp3.Properties.Resources.Process;
            //ProcessBtn.BackgroundImage = Bitmap.FromFile("E:/CablesWindoApp3/buttonstosizefinal/Process.png");
        }

       
        private void GenrateReportBtn_MouseDown(object sender, MouseEventArgs e)
        {
            //CropImage2();
            //ReportFormShow();
            if (num > 0)
            {
                if (SubReportNo <= num)
                {

                    ReportFormShow();
                    PdfpathFolder = globalVars.projectDirectory + @"\ReportFolder\{0}\{1}.pdf";
                    //PdfpathFolder = @"E:\CablesWindoApp3\ReportFolder\{0}\{1}.pdf";
                    SavePDF(rpt.reportViewer1, string.Format(PdfpathFolder, Settings.Default.ReportFolderNo.ToString(), SubReportNo));
                    ExcelpathFolder = globalVars.projectDirectory + @"\ReportFolder\{0}\{1}.xls";
                    //ExcelpathFolder = @"E:\CablesWindoApp3\ReportFolder\{0}\{1}.xls";
                    SaveExl(rpt.reportViewer1, string.Format(ExcelpathFolder, Settings.Default.ReportFolderNo.ToString(), SubReportNo));

                    GetDataGridValue();
                    MessageBox.Show(@"CableWindoApp3\ReportFolder\");
                    SubReportNo++;
                    //if (SubReportNo <= num)
                    //{

                    //    SetTextboxValue2();
                    //    ReportFormShow();
                    //    ResetImage();
                    //    RestPicturBox();
                    //}

                }
              
                if (SubReportNo > num)
                {
                    rpt.Close();                   
                    SubReportNo = 1;
                    num = 0;
                    BatchFormShow();
                    PdfpathFolder = globalVars.projectDirectory + @"\ReportFolder\{0}\{1}.pdf";
                    //PdfpathFolder = @"E:\CablesWindoApp3\ReportFolder\{0}\{1}.pdf";
                    SavePDF(btf.BatchReportViewer, string.Format(PdfpathFolder, Settings.Default.ReportFolderNo.ToString(), Settings.Default.ReportFolderNo.ToString()));
                    //ExcelpathFolder = @"E:\CablesWindoApp3\ReportFolder\{0}\{1}.xls";
                    ExcelpathFolder = globalVars.projectDirectory + @"\ReportFolder\{0}\{1}.xls";
                    SaveExl(btf.BatchReportViewer, string.Format(ExcelpathFolder, Settings.Default.ReportFolderNo.ToString(), Settings.Default.ReportFolderNo.ToString()));

                    ThreeCoreDataGridView.Refresh();
                    ThreeCoreDataGridView.Rows.Clear();

                    MessageBox.Show(@"CableWindoApp3\ReportFolder\");
                    Settings.Default.ReportFolderNo++;
                    Settings.Default.Save();
                    //SetTextboxValue();
                    ResetImage();
                    RestPicturBox();
                    this.GenrateReportBtn.BackgroundImage = global::CablesWindoApp3.Properties.Resources.GenrateReportOn;
                    DisbledBottomButton();
                    PanelXple.Visible = false;
                    //this.GenrateReportBtn.BackgroundImage = global::CablesWindoApp3.Properties.Resources.GenrateReport;
                    BatchBtn.BackgroundImage = Bitmap.FromFile(globalVars.projectDirectory + @"/ProjectButton/BatchButton.png");
                    //BatchBtn.BackgroundImage = Bitmap.FromFile("E:/CablesWindoApp3/buttonstosizefinal/BatchButton.png");
                }
            }
            else
            {
                this.GenrateReportBtn.BackgroundImage = global::CablesWindoApp3.Properties.Resources.GenrateReportOn;
                ReportFormShow();
                PdfpathFolder = globalVars.projectDirectory + @"\ReportFolder\{0}\{1}.pdf";
                //PdfpathFolder = @"E:\CablesWindoApp3\ReportFolder\{0}\{1}.pdf";
                SavePDF(rpt.reportViewer1, string.Format(PdfpathFolder, Settings.Default.ReportFolderNo.ToString(), Settings.Default.ReportFolderNo.ToString()));
                ExcelpathFolder = globalVars.projectDirectory + @"\ReportFolder\{0}\{1}.xls";
                //ExcelpathFolder = @"E:\CablesWindoApp3\ReportFolder\{0}\{1}.xls";
                SaveExl(rpt.reportViewer1, string.Format(ExcelpathFolder, Settings.Default.ReportFolderNo.ToString(), Settings.Default.ReportFolderNo.ToString()));
                d.InsertRecord(Settings.Default.ReportFolderNo,Convert.ToDateTime(DateTime.Now.ToString("yyyy-MM-dd")), time,selectedTemplate);
                TextBoxesValue();
                BatchTxtbox.Clear();
                MessageBox.Show(@"CableWindoApp3\ReportFolder\");
                Settings.Default.ReportFolderNo++;
                Settings.Default.BatchNo++;
                Settings.Default.TestNo++;
                Settings.Default.Save();
                PanelXple.Visible = false;
                ResetImage();
                RestPicturBox();
                DisbledBottomButton();
                this.GenrateReportBtn.BackgroundImage = global::CablesWindoApp3.Properties.Resources.GenrateReport;              
                //BatchBtn.BackgroundImage = Bitmap.FromFile(globalVars.projectDirectory + @"/ProjectButton/BatchButton.png");
                //this.GenrateReportBtn.BackgroundImage = global::CablesWindoApp3.Properties.Resources.GenrateReport;
                //BatchBtn.BackgroundImage = Bitmap.FromFile("E:/CablesWindoApp3/buttonstosizefinal/BatchButton.png");
            }

                // if (SubReportNo > num)
                //{            

                //        btf.Visible = false;
                //        ReportFormShow();
                //        PdfpathFolder = @"E:\CablesWindoApp3\ReportFolder\{0}\{1}.pdf";
                //        SavePDF(rpt.reportViewer1, string.Format(PdfpathFolder, Settings.Default.ReportFolderNo.ToString(), Settings.Default.ReportFolderNo.ToString()));
                //        Settings.Default.ReportFolderNo++;
                //        SetTextboxValue();
                //        ResetImage();
                //        RestPicturBox();
                //        this.GenrateReportBtn.BackgroundImage = global::CablesWindoApp3.Properties.Resources.GenrateReportOn;

                //    if (SubReportNo == num + 1)
                //    {
                //        rpt.Close();
                //        BatchFormShow();
                //        PdfpathFolder = @"E:\CablesWindoApp3\ReportFolder\{0}\{1}.pdf";
                //        SavePDF(btf.BatchReportViewer, string.Format(PdfpathFolder, Settings.Default.ReportFolderNo.ToString(), Settings.Default.ReportFolderNo.ToString()));
                //        ThreeCoreDataGridView.Refresh();
                //        ThreeCoreDataGridView.Rows.Clear();

                //        SubReportNo = 1;
                //    }





                //-----------------Report Form Show Method------------------//

                //if (SubReportNo <=num)
                //{
                //   SubReportNo++;
                //   SetTextboxValue2();
                //   ResetImage();
                //   RestPicturBox();
                //}

                //if (SubReportNo >=num)
                //{
                //    DisbledBottomButton();
                //    Settings.Default.ReportFolderNo++;
                //    BatchBtn.BackgroundImage = Bitmap.FromFile("E:/CablesWindoApp3/buttonstosizefinal/BatchButton.png");
                //}

                //CheckBatchReportNo();

                //-----------------SetTextBox method2--------------------//

                //GenrateReportBtn.BackgroundImage = Bitmap.FromFile("E:/CablesWindoApp3/buttonstosizefinal/GenrateReportOn.png");
                //-----------------Check Batch Report no ------------------//
                //CheckBatchReportNo();


                ////num++;

                ////BatcReportNo++;           


                ////Settings.Default.BatchNo++;
                ////Settings.Default.TestNo++;
                //Settings.Default.ReportFolderNo++;


            }

        private void GenrateReportBtn_MouseUp(object sender, MouseEventArgs e)
        {
            this.GenrateReportBtn.BackgroundImage = global::CablesWindoApp3.Properties.Resources.GenrateReport;
            //GenrateReportBtn.BackgroundImage = Bitmap.FromFile("E:/CablesWindoApp3/buttonstosizefinal/GenrateReport.png");
            //Settings.Default.ReportFolderNo++;
            //Settings.Default.TestNo++;
            Settings.Default.Save();

        }

        private void GenrateReportBtn_MouseLeave(object sender, EventArgs e)
        {
            //GenrateReportBtn.BackgroundImage = Bitmap.FromFile("E:/CablesWindoApp3/buttonstosizefinal/GenrateReport.png");
        }

        private void GenrateReportBtn_Click(object sender, EventArgs e)
        {
            ////-----------------Report Form Show Method------------------//
            //ReportFormShow();
            ////-----------------SetTextBox method2 --------------------//
            //if (SubReportNo <= num)
            //{
            //    SubReportNo++;
            //    SetTextboxValue2();
            //    ResetImage();
            //}

            //if (SubReportNo >= num)
            //{
            //    DisbledBottomButton();
            //    BatchBtn.BackgroundImage = Bitmap.FromFile("E:/CablesWindoApp3/buttonstosizefinal/BatchButton.png");
            //}
            ////-----------------Check Batch Report no ------------------//
            ////CheckBatchReportNo();


            ////num++;

            ////BatcReportNo++;           


            ////Settings.Default.BatchNo++;
            ////Settings.Default.TestNo++;

            //GenrateReportBtn.BackgroundImage = Bitmap.FromFile("E:/CablesWindoApp3/buttonstosizefinal/GenrateReportOn.png");
        }

        //private void CaptureBtn_Click(object sender, EventArgs e)
        //{
        //    //cameraFun();
        //    //CaptureBtn.BackgroundImage = Bitmap.FromFile("E:/CablesWindoApp3/buttonstosizefinal/CaptureOn.png");
        //}

        private void ProcessBtn_Click(object sender, EventArgs e)
        {
            ////-------------Sample Measurement Mehtod---------------// 
            //measureFun();
            ////-------------Thickness Mehtod---------------// 

            ////Thickness(); 
            //ProcessBtn.BackgroundImage = Bitmap.FromFile("E:/CablesWindoApp3/buttonstosizefinal/ProcessOn.png");
        }

        private void Form1_Shown(object sender, EventArgs e)
        {
          
        }

        private void panel10_Paint(object sender, PaintEventArgs e)
        {

        }

        private void panel11_Paint(object sender, PaintEventArgs e)
        {

        }

        private void ca3btn_Click_3(object sender, EventArgs e)
        {

        }

        private void ca12btn_Click_2(object sender, EventArgs e)
        {

        }

        private void ca6btn_Click_1(object sender, EventArgs e)
        {

        }

        private void ca11btn_Click_1(object sender, EventArgs e)
        {

        }

        private void ca13btn_Click_2(object sender, EventArgs e)
        {

        }

        private void ca5btn_Click_1(object sender, EventArgs e)
        {

        }

        private void ca7btn_Click_1(object sender, EventArgs e)
        {

        }

        private void ca8btn_Click_1(object sender, EventArgs e)
        {

        }

        private void button1_Click_1(object sender, EventArgs e)
        {

        }

        private void label11_Click_1(object sender, EventArgs e)
        {

        }

        private void label3_Click(object sender, EventArgs e)
        {

        }

        private void label4_Click_1(object sender, EventArgs e)
        {

        }

        private void label7_Click(object sender, EventArgs e)
        {

        }

        private void label6_Click_1(object sender, EventArgs e)
        {

        }

        private void label10_Click(object sender, EventArgs e)
        {

        }

        private void label8_Click(object sender, EventArgs e)
        {

        }

        private void Core5lbl_Click(object sender, EventArgs e)
        {

        }

        private void label13_Click(object sender, EventArgs e)
        {

        }

        private void Core4Lbl_Click(object sender, EventArgs e)
        {

        }

        private void label9_Click(object sender, EventArgs e)
        {

        }

        private void label14_Click(object sender, EventArgs e)
        {

        }

        private void label12_Click(object sender, EventArgs e)
        {

        }

        private void label5_Click_1(object sender, EventArgs e)
        {

        }

        private void panel12_Paint_1(object sender, PaintEventArgs e)
        {

        }

        private void label28_Click(object sender, EventArgs e)
        {

        }

        private void SnapShotBtn_Click(object sender, EventArgs e)
        {

        }

        private void pictureBox6_Click(object sender, EventArgs e)
        {
            if (cblPitcurBox.Width < (15 * CableImgPanel.Width) && (cblPitcurBox.Height < (15 * CableImgPanel.Height)))
            {

                //Change pictureBox Size and Multiply Zoomfactor
                cblPitcurBox.Width = (int)(cblPitcurBox.Width * 1.25);
                cblPitcurBox.Height = (int)(cblPitcurBox.Height * 1.25);

                //Move Picture box
                cblPitcurBox.Top = (int)(247 - 1.25 * (247 - cblPitcurBox.Top));
                cblPitcurBox.Left = (int)(286 - 1.25 * (286- cblPitcurBox.Left));
                //label1.Text = Convert.ToString(e.X);
                //label2.Text = Convert.ToString(e.Y);
            }



        }

        private void pictureBox5_Click_1(object sender, EventArgs e)
        {


           
        }

        private void ZoomOutBtn_MouseDown(object sender, MouseEventArgs e)
        {
            ZoomOutCableImgPanel();
            this.ZoomOutBtn.BackgroundImage = global::CablesWindoApp3.Properties.Resources.ZoomOutOn;
        }
        
          

        private void ZoomInbtn_MouseDown(object sender, MouseEventArgs e)
        {
            ZoomInCableImgPanel();
            this.ZoomInbtn.BackgroundImage = global::CablesWindoApp3.Properties.Resources.ZoomInOn;

        }

        private void button4_Click_2(object sender, EventArgs e)
        {

        }

        public int zoomin =0;
        private void ZoomInBtnPanel10_MouseDown(object sender, MouseEventArgs e)
        {
            ZoomInPnl10();
            zoomin++;
            this.ZoomInBtnPanel10.BackgroundImage = global::CablesWindoApp3.Properties.Resources.ZoomInOn;
        }

        private void ZoomOutPanel10_MouseDown(object sender, MouseEventArgs e)
        {
            ZoomOutPnl10();
            zoomin--;
            this.ZoomOutPanel10.BackgroundImage = global::CablesWindoApp3.Properties.Resources.ZoomOutOn;
        }

        private void ZoomInBtnPanel10_MouseUp(object sender, MouseEventArgs e)
        {
            //this.ZoomInBtnPanel10.BackgroundImage = global::CablesWindoApp3.Properties.Resources.ZoomIn;
        }

        private void ZoomInBtnPanel10_Click(object sender, EventArgs e)
        {

        }

        private void ZoomOutPanel10_MouseUp(object sender, MouseEventArgs e)
        {
            this.ZoomOutPanel10.BackgroundImage = global::CablesWindoApp3.Properties.Resources.ZoomOut;
        }

        private void ZoomInbtn_MouseUp(object sender, MouseEventArgs e)
        {
            //this.ZoomInbtn.BackgroundImage = global::CablesWindoApp3.Properties.Resources.ZoomIn;
        }

        private void ZoomOutBtn_MouseUp(object sender, MouseEventArgs e)
        {
            this.ZoomOutBtn.BackgroundImage = global::CablesWindoApp3.Properties.Resources.ZoomOut;
        }

        private void button3_Click_3(object sender, EventArgs e)
        {
           
        }

        private void button1_Click_2(object sender, EventArgs e)
        {
            CoreButtonsEnbled();
        }

        private void pictureBox2_Click(object sender, EventArgs e)
        {
            //DataTable dt = new DataTable();

            //for (int i = 0; i < xp.xlpes.Count; i++)
            //{
                
            //    dt.Columns.Add(string.Format("T{0}", i + 1), typeof(float));
            //    globalVars.algo.getSectorThickness(0);
                
            //}

            //dataGridView1.DataSource = dt;

        }
      

        private void dataGridView1_CellContentClick_1(object sender, DataGridViewCellEventArgs e)
        {

        }
      

        private void button1_Click_3(object sender, EventArgs e)
        {
           
        }
       
        private void button4_Click_3(object sender, EventArgs e)
        {
           
        }

        private void CableImgPanel_Paint(object sender, PaintEventArgs e)
        {

        }

        private void CameraStatus_Click(object sender, EventArgs e)
        {

        }

        private void CaptureBtn_Click(object sender, EventArgs e)
        {

        }

        private void ResetBtn_Click(object sender, EventArgs e)
        {

        }

        private void ZoomOutPanel10_Click(object sender, EventArgs e)
        {

        }

        private void button1_Click_4(object sender, EventArgs e)
        {
            //Console.WriteLine("Picture width=" + .Width);
            //Console.WriteLine("Picture Height=" + cblPitcurBox.Height);
            Console.WriteLine("Picture x=" + panel10.Location.X);
            Console.WriteLine("Picture y=" + panel10.Location.Y);
        }

        private void ZoomInbtn_Click(object sender, EventArgs e)
        {

        }

        private void button5_MouseDown(object sender, MouseEventArgs e)
        {
            SForm.Show();
        }

        private void button1_Click_5(object sender, EventArgs e)
        {
            SetExposure();
        }

        private void SettingBtn_MouseDown(object sender, MouseEventArgs e)
        {
                       
            this.SettingBtn.BackgroundImage = global::CablesWindoApp3.Properties.Resources.SettingsOn;
            SettingBtn.Refresh();
            SettingFormShow();
           
        }

        private void SettingBtn_MouseUp(object sender, MouseEventArgs e)
        {
            
            this.SettingBtn.BackgroundImage = global::CablesWindoApp3.Properties.Resources.Settings;
            this.Refresh();


        }

        private void ReportBtn_MouseDown(object sender, MouseEventArgs e)
        {            
            this.ReportBtn.BackgroundImage = global::CablesWindoApp3.Properties.Resources.ReportsOn;
            ReportBtn.Refresh();
            SearchFormShow();
        }

        private void ReportBtn_MouseUp(object sender, MouseEventArgs e)
        {
            this.ReportBtn.BackgroundImage = global::CablesWindoApp3.Properties.Resources.Reports;
        }

        private void SettingBtn_MouseLeave(object sender, EventArgs e)
        {
            
            this.ReportBtn.BackgroundImage = global::CablesWindoApp3.Properties.Resources.Reports;
        }

        private void SettingBtn_MouseEnter(object sender, EventArgs e)
        {
            this.SettingBtn.BackgroundImage = global::CablesWindoApp3.Properties.Resources.SettingsOn;

        }

        private void ReportBtn2_MouseDown(object sender, MouseEventArgs e)
        {
            ReportFormShow();
        }

        private void CalibrationBtn_MouseDown(object sender, MouseEventArgs e)
        {
            CalibrationFormShow();
            //cameraFun();
        }

        private void PanelT5_Paint(object sender, PaintEventArgs e)
        {

        }

        private void SettingBtn_Click(object sender, EventArgs e)
        {

        }

        private void button1_Click_6(object sender, EventArgs e)
        {
            Console.WriteLine(globalVars.projectDirectory);
        }

        private void button1_Click_7(object sender, EventArgs e)
        {
            Console.WriteLine("b Panel 10 X=" + panel10.Location.X);
            Console.WriteLine("b Panel 10 y=" + panel10.Location.Y);
            CropImage2();
            Console.WriteLine("A Panel 10 X=" + panel10.Location.X);
            Console.WriteLine("A Panel 10 y=" + panel10.Location.Y);
        }
    }

    // -----------------------------Reports Classes----------------------//
    public class Cable
    {
        public string partName { get; set;}
        public string T1 { get; set; }
        public string T2 { get; set; }
        public string T3 { get; set; }
        public string T4 { get; set; }
        public string T5 { get; set; }
        public string T6 { get; set; }
        public string OutDiam { get; set; }
        public string OutDiamMax { get; set; }
        public string OutDiamMin { get; set; }
        public string MinThick { get; set; }
        public string MaxThick { get; set; }
        public string Concentricity { get; set; }
        public string Area { get; set; }
        public string TotalLengthY { get; set; }
        public string LengthC { get; set; }
        public string AvgThick { get; set;}
        public string Ovality { get; set; }

    }
    public class batch
    {
        public int S_No { get; set; }
        public string T1 { get; set; }
        public string T2 { get; set; }
        public string T3 { get; set; }
        public string T4{ get; set; }
        public string T5 { get; set; }
        public string T6 { get; set; }       
        public string MinThick { get; set; }
        public string AvgThick { get; set; }
        public string OuterDiameter { get; set; }
        public string ConcentriCity { get; set; }
        public string Ovality { get; set; }
    }
   
  


}
