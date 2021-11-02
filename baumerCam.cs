using System;
using System.Collections.Generic;
using System.Drawing;
using System.Runtime.InteropServices;
using System.Windows.Forms;


namespace CablesWindoApp3
{
    //class BaumerCam
    //{
    //        public BGAPI2.Device cam = null;
    //        public BGAPI2.DataStream mDataStream = null;
    //        public BGAPI2.BufferList bufferList = null;
    //        public BGAPI2.Buffer mBuffer = null;

    //        public BGAPI2.ImageProcessor imgProcessor = new BGAPI2.ImageProcessor();
    //        public byte[] imageBufferCopy;
    //        public byte[] transformImageBufferCopy;

    //        public BGAPI2.Node pixelFormatInfoSelector;
    //        public BGAPI2.Node bytesPerPixel;

    //        public BGAPI2.Image mImage;
    //        public BGAPI2.Buffer mBufferFilled = null;
    //        public BGAPI2.Image mTransformImage = null;

    //        public Bitmap grabbedImage;
    //        public Bitmap grabbedImageColor;
    //        public bool connectionStatus = false;
    //        public bool capturing = false;

    //        public string name = "Default Camera";
    //        public string serialNo = "DIGI2000";

    //        public double exposureTimeUser = 5000.00;// 8500.000000;
    //        public string triggerSource = "Software"; //Set to Software or Line 0

    //        public bool inspectionResult = false;
    //        //camera functions and events
    //        public int getImageOnEvent(BGAPI2.Events.NewBufferEventArgs mDSEvent)
    //        {
    //            // System.Console.WriteLine(" Cam H event trigger**********HHHHHHHHHHHH");
    //            try
    //            {
    //                pixelFormatInfoSelector = imgProcessor.NodeList["PixelFormatInfoSelector"];
    //                bytesPerPixel = imgProcessor.NodeList["BytesPerPixel"];

    //                mImage = imgProcessor.CreateImage();
    //                mBufferFilled = null;
    //                //  System.Console.WriteLine(" Entered Buffer processing 11");
    //                mBufferFilled = mDSEvent.BufferObj;
    //                if (mBufferFilled == null)
    //                {
    //                    //    System.Console.WriteLine(" Entered Buffer processing if");
    //                    System.Console.Write("Error: Buffer Timeout after 1000 msec\r\n");
    //                    return 0;
    //                }
    //                else if (mBufferFilled.IsIncomplete == true)
    //                {
    //                    //  System.Console.WriteLine(" Entered Buffer processing elif");
    //                    System.Console.Write("Error: Image is incomplete\r\n");
    //                    mBufferFilled.QueueBuffer();
    //                    return 0;
    //                }
    //                else
    //                {
    //                    //     System.Console.WriteLine(" Entered Buffer processing22");

    //                    //     System.Console.WriteLine(" Image {0, 5:d} received in memory address {1:XmTransformImage}\r\n", mBufferFilled.FrameID, (ulong)mBufferFilled.MemPtr);

    //                    //create an image object from the filled buffer and convert it
    //                    mTransformImage = null;
    //                    mImage.Init(mBufferFilled);
    //                    //System.Console.Write("  mImageH.Pixelformat:             {0}\r\n", mImageH.PixelFormat);
    //                    //System.Console.Write("  mImageH.Width:                   {0}\r\n", mImageH.Width);
    //                    //System.Console.Write("  mImageH.Height:                  {0}\r\n", mImageH.Height);
    //                    //System.Console.Write("  mImageH.Buffer:                  {0:X8}\r\n", (ulong)mImageH.Buffer);

    //                    pixelFormatInfoSelector.Value = mBufferFilled.PixelFormat;
    //                    double fBytesPerPixel = bytesPerPixel.IsAvailable ? bytesPerPixel.Value.ToDouble() : 0.0;

    //                    //   System.Console.Write("  Bytes per image:                {0}\r\n", (long)((uint)mImage.Width * (uint)mImage.Height * fBytesPerPixel));
    //                    //   System.Console.Write("  Bytes per pixel:                {0}\r\n", fBytesPerPixel);

    //                    //COPY UNMANAGED IMAGEBUFFER TO A MANAGED BYTE ARRAY
    //                    imageBufferCopy = new byte[(uint)((uint)mImage.Width * (uint)mImage.Height * fBytesPerPixel)];
    //                    Marshal.Copy(mImage.Buffer, imageBufferCopy, 0, (int)((int)mImage.Width * (int)mImage.Height * fBytesPerPixel));
    //                    ulong imageBufferAddress = (ulong)mImage.Buffer;

    //                    if ((string)mImage.PixelFormat.Substring(0, 4) == "Mono") // if pixel format starts with "Mono"  if black and white 
    //                    {
    //                        //transform to Mono8
    //                        mTransformImage = imgProcessor.CreateTransformedImage(mImage, "Mono8");
    //                        //System.Console.Write(" Image {0, 5:d} transformed to Mono8\r\n", mBufferFilled.FrameID);
    //                        //System.Console.Write("  mTransformImage.Pixelformat:    {0}\r\n", mTransformImage.PixelFormat);
    //                        //System.Console.Write("  mTransformImage.Width:          {0}\r\n", mTransformImage.Width);
    //                        //System.Console.Write("  mTransformImage.Height:         {0}\r\n", mTransformImage.Height);
    //                        //System.Console.Write("  mTransformImage.Buffer:         {0:X8}\r\n", (ulong)mTransformImage.Buffer);
    //                        //System.Console.Write("  Bytes per image:                {0}\r\n", (long)((uint)mTransformImage.Width * (uint)mTransformImage.Height * 1.0));
    //                        //System.Console.Write("  Bytes per pixel:                {0}\r\n", 1.0);

    //                        transformImageBufferCopy = new byte[(uint)((uint)mTransformImage.Width * (uint)mTransformImage.Height * 1.0)];
    //                        Marshal.Copy(mTransformImage.Buffer, transformImageBufferCopy, 0, (int)((int)mTransformImage.Width * (int)mTransformImage.Height * 1.0));
    //                        ulong transformImageBufferAddress = (ulong)mTransformImage.Buffer;
    //                        try
    //                        {
    //                            grabbedImage = new System.Drawing.Bitmap((int)mTransformImage.Width,
    //                       (int)mTransformImage.Height,
    //                       (int)mTransformImage.Width * 1,
    //                       System.Drawing.Imaging.PixelFormat.Format8bppIndexed,
    //                       mTransformImage.Buffer);
    //                            System.Drawing.Imaging.ColorPalette palette = grabbedImage.Palette;
    //                            int nColors = 256;
    //                            for (int ii = 0; ii < nColors; ii++)
    //                            {
    //                                uint Alpha = 0xFF;
    //                                uint Intensity = (uint)(ii * 0xFF / (nColors - 1));
    //                                palette.Entries[ii] = System.Drawing.Color.FromArgb(
    //                                 (int)Alpha,
    //                                 (int)Intensity,
    //                                 (int)Intensity,
    //                                 (int)Intensity);
    //                            }
    //                            grabbedImage.Palette = palette;

    //                            Rectangle rect = new Rectangle(0, 0, grabbedImage.Width, grabbedImage.Height);
    //                            grabbedImageColor = grabbedImage.Clone(rect, System.Drawing.Imaging.PixelFormat.Format24bppRgb);

    //                        }
    //                        catch (ArgumentException ex)
    //                        {
    //                            Console.WriteLine("exception from camera class mono conversion");
    //                            System.Console.Write(ex);
    //                            return 0;
    //                        }

    //                    }
    //                    else // if color format
    //                    {
    //                        try
    //                        {
    //                            //transform to BGR8
    //                            mTransformImage = imgProcessor.CreateTransformedImage(mImage, "BGR8");
    //                            //System.Console.Write(" Image {0, 5:d} transformed to BGR8\r\n", mBufferFilled.FrameID);
    //                            //System.Console.Write("  mTransformImage.Pixelformat:    {0}\r\n", mTransformImage.PixelFormat);
    //                            //System.Console.Write("  mTransformImage.Width:          {0}\r\n", mTransformImage.Width);
    //                            //System.Console.Write("  mTransformImage.Height:         {0}\r\n", mTransformImage.Height);
    //                            //System.Console.Write("  mTransformImage.Buffer:         {0:X8}\r\n", (ulong)mTransformImage.Buffer);
    //                            //System.Console.Write("  Bytes per image:                {0}\r\n", (long)((uint)mTransformImage.Width * (uint)mTransformImage.Height * 3.0));
    //                            //System.Console.Write("  Bytes per pixel:                {0}\r\n", 3.0);

    //                            transformImageBufferCopy = new byte[(uint)((uint)mTransformImage.Width * (uint)mTransformImage.Height * 3.0)];
    //                            Marshal.Copy(mTransformImage.Buffer, transformImageBufferCopy, 0, (int)((int)mTransformImage.Width * (int)mTransformImage.Height * 3.0));
    //                            ulong transformImageBufferAddress = (ulong)mTransformImage.Buffer;
    //                            // don nothing because we not using color camera 
    //                            grabbedImage = new System.Drawing.Bitmap((int)mTransformImage.Width, (int)mTransformImage.Height, (int)mTransformImage.Width * 3, System.Drawing.Imaging.PixelFormat.Format24bppRgb, mTransformImage.Buffer); //to display use: this.pictureBoxImageDisplay.Image = bitmap; bitmap.Save("image.bmp"); 
    //                                                                                                                                                                                                                                          //   abc = (Bitmap)bitmap.Clone();
    //                                                                                                                                                                                                                                          //abc = new Bitmap(grabbedImage);
    //                                                                                                                                                                                                                                          //  abc.RotateFlip(System.Drawing.RotateFlipType.Rotate270FlipNone);
    //                            Rectangle rect = new Rectangle(0, 0, grabbedImage.Width, grabbedImage.Height);
    //                            grabbedImageColor = grabbedImage.Clone(rect, System.Drawing.Imaging.PixelFormat.Format24bppRgb);

    //                        }
    //                        catch (Exception eximgConv)
    //                        {
    //                            Console.WriteLine("Exception while image conversion " + eximgConv.Message);
    //                        }
    //                    }

    //                    if (mImage != null) mImage.Release();
    //                    // if (mTransformImage != null) mTransformImage.Release();
    //                    // queue buffer again
    //                    mBufferFilled.QueueBuffer();

    //                }

    //            }
    //            catch (BGAPI2.Exceptions.IException ex)
    //            {
    //                System.Console.Write("ExceptionType:    {0} \r\n", ex.GetType());
    //                System.Console.Write("ErrorDescription: {0} \r\n", ex.GetErrorDescription());
    //                System.Console.Write("in function:      {0} \r\n", ex.GetFunctionName());
    //                return 0;
    //            }
    //            //   }


    //            //  hwBswTrig = false;

    //            return 1;
    //        }

    //        public void mDataStream_NewBufferEvent(object sender, BGAPI2.Events.NewBufferEventArgs mDSEvent)
    //        {
    //            getImageOnEvent(mDSEvent);
    //            totalInspected++;
    //            OnImageReceived(true); //generate event for form
    //        }

    //        public void linkBufferEvent()
    //        {
    //            mDataStream.NewBufferEvent += new BGAPI2.Events.DataStreamEventControl.NewBufferEventHandler(mDataStream_NewBufferEvent);
    //            mDataStream.RegisterNewBufferEvent(BGAPI2.Events.EventMode.EVENT_HANDLER);
    //        }
    //        //event for form
    //        public event EventHandler<bool> imageReceived;
    //        protected virtual void OnImageReceived(bool IsSuccessful)
    //        {
    //            imageReceived?.Invoke(this, IsSuccessful);
    //        }

    //        //--inspection variables
    //        public int totalInspected = 0;
    //        public int okCnt = 0;
    //        public int ngCnt = 0;

    //        public int TotalInspected { get => totalInspected; set => totalInspected = value; }
    //        public int OkCnt { get => okCnt; set => okCnt = value; }
    //        public int NgCnt { get => ngCnt; set => ngCnt = value; }

    //        public void resetCounters()
    //        {
    //            totalInspected = 0;
    //            okCnt = 0;
    //            ngCnt = 0;

    //        }

    //        public float okPercentage()
    //        {
    //            float val = 0;

    //            if (TotalInspected > 0 && okCnt > 0)
    //                val = (float)100.0 * ((float)okCnt / (float)TotalInspected);
    //            else
    //                val = 0;

    //            return val;
    //        }

    //        public float ngPercentage()
    //        {
    //            float val = 0;

    //            if (TotalInspected > 0 && ngCnt > 0)
    //                val = (float)100.0 * ((float)ngCnt / (float)TotalInspected);
    //            else
    //                val = 0;

    //            return val;
    //        }

    //        //UI update fns
    //        public void updateDash(Label total, Label ok, Label ng, Label okPc, Label lblresult, Label connStatus)
    //        {
    //            if (connectionStatus == true)
    //            {
    //                connStatus.Text = "Online";
    //                connStatus.ForeColor = Color.LimeGreen;
    //            }
    //            else
    //            {
    //                connStatus.Text = "Offline";
    //                connStatus.ForeColor = Color.Red;
    //            }
    //            if (inspectionResult)
    //            {
    //                lblresult.Text = "OK";
    //                lblresult.BackColor = Color.ForestGreen;

    //            }
    //            else
    //            {
    //                lblresult.Text = "NG";
    //                lblresult.BackColor = Color.Red;
    //            }

    //            total.Text = TotalInspected.ToString();
    //            ok.Text = OkCnt.ToString();
    //            ng.Text = NgCnt.ToString();
    //            okPc.Text = okPercentage().ToString("0.00");


    //        }



        
    //}
    //class cameraFns
    //{
    //    public List<BaumerCam> listOfCamObs_u = new List<BaumerCam>();
    //    public BGAPI2.Interface mInterface = null;
    //    BGAPI2.System mSystem = null;
    //    //variables
    //    BGAPI2.SystemList systemList = null;

    //    string sSystemID = "";

    //    BGAPI2.InterfaceList interfaceList = null;

    //    string sInterfaceID = "";

    //    BGAPI2.DeviceList deviceList = null;

    //    string sDeviceID = "";

    //    //camera control functions
    //    public void findNconnectCams()
    //    {
    //        mInterface = null;
    //        mSystem = null;
    //        //variables
    //        systemList = null;
    //        sSystemID = "";
    //        interfaceList = null;
    //        sInterfaceID = "";
    //        deviceList = null;
    //        sDeviceID = "";

    //        if (listOfCamObs_u.Count == 0)
    //        {
    //            Console.WriteLine("No camera details added in List listOfCamObs_u //n returnning.");
    //            return;
    //        }
    //        //----- search cameras

    //        List<string> sInterfaceID_ls = new List<string>();
    //        sInterfaceID_ls.Clear();
    //        int returnCode = 0;
    //        systemList = null;
    //        interfaceList = null;
    //        deviceList = null;

    //        mSystem = null;
    //        mInterface = null;

    //        System.Console.Write("SYSTEM LIST++++++++++++++++++\r\n");
    //        //COUNTING AVAILABLE SYSTEMS (TL producers)
    //        try
    //        {
    //            systemList = BGAPI2.SystemList.Instance;
    //            systemList.Refresh();
    //            System.Console.Write("5.1.2   Detected systems:  {0}\r\n", systemList.Count);


    //            //SYSTEM DEVICE INFORMATION
    //            foreach (KeyValuePair<string, BGAPI2.System> sys_pair in BGAPI2.SystemList.Instance)
    //            {
    //                System.Console.Write("  5.2.1   System Name:     {0}\r\n", sys_pair.Value.FileName);
    //            }
    //        }
    //        catch (BGAPI2.Exceptions.IException ex)
    //        {
    //            System.Console.Write("ExceptionType:    {0} \r\n", ex.GetType());
    //        }


    //        //OPEN THE FIRST SYSTEM IN THE LIST WITH A CAMERA CONNECTED
    //        try
    //        {
    //            foreach (KeyValuePair<string, BGAPI2.System> sys_pair in BGAPI2.SystemList.Instance)
    //            {
    //                System.Console.Write("SYSTEM++++++++++++++++++++++\r\n");

    //                try
    //                {
    //                    // camStatusLbl.Text = " open systems done ";
    //                    if (sys_pair.Value.IsOpen)
    //                        sys_pair.Value.Close();

    //                    sys_pair.Value.Open();
    //                    System.Console.Write("5.1.3   Open next system \r\n");
    //                    System.Console.Write("  5.2.1   System Name:     {0}\r\n", sys_pair.Value.FileName);
    //                    sSystemID = sys_pair.Key;
    //                    System.Console.Write("        Opened system - NodeList Information \r\n");
    //                    System.Console.Write("          GenTL Version:   {0}.{1}\r\n\r\n", (long)sys_pair.Value.NodeList["GenTLVersionMajor"].Value, (long)sys_pair.Value.NodeList["GenTLVersionMinor"].Value);
    //                    System.Console.Write("INTERFACE LIST+++++++++++++++\r\n");

    //                    try
    //                    {
    //                        interfaceList = sys_pair.Value.Interfaces;
    //                        //  camStatusLbl.Text = " assign interface list done ";
    //                        //COUNT AVAILABLE INTERFACES
    //                        interfaceList.Refresh(100); // timeout of 100 msec
    //                        System.Console.Write("5.1.4   Detected interfaces: {0}\r\n", interfaceList.Count);
    //                        //INTERFACE INFORMATION
    //                        foreach (KeyValuePair<string, BGAPI2.Interface> ifc_pair in interfaceList)
    //                        {
    //                            System.Console.Write("  5.2.2   Interface ID:      {0}\r\n", ifc_pair.Value.Id);
    //                        }
    //                    }
    //                    catch (BGAPI2.Exceptions.IException ex)
    //                    {
    //                        returnCode = (0 == returnCode) ? 1 : returnCode;
    //                        System.Console.Write("ExceptionType:    {0} \r\n", ex.GetType());

    //                    }

    //                    System.Console.Write("INTERFACE+++++++++++++++++++++++++++\r\n");
    //                    //OPEN THE NEXT INTERFACE IN THE LIST
    //                    try
    //                    {
    //                        foreach (KeyValuePair<string, BGAPI2.Interface> ifc_pair in interfaceList)
    //                        {
    //                            try
    //                            {
    //                                // camStatusLbl.Text = " trying to open the interface  ";
    //                                System.Console.Write("5.1.5   Open interface \r\n");
    //                                ifc_pair.Value.Open();
    //                                //search for any camera is connetced to this interface
    //                                deviceList = ifc_pair.Value.Devices;
    //                                deviceList.Refresh(100);
    //                                if (deviceList.Count == 0)
    //                                {
    //                                    //  camStatusLbl.Text = " deviceList.Count   " + deviceList.Count.ToString();
    //                                    System.Console.Write("5.1.13   Close interface ({0} cameras found) \r\n\r\n", deviceList.Count);
    //                                    sInterfaceID = "";
    //                                    ifc_pair.Value.Close();
    //                                }
    //                                else
    //                                {
    //                                    //  camStatusLbl.Text = " device is detected ";
    //                                    sInterfaceID = ifc_pair.Key;
    //                                    Console.WriteLine("s interface id " + sInterfaceID);
    //                                    System.Console.Write("  \r\n");
    //                                    System.Console.Write("        Opened interface - NodeList Information \r\n");
    //                                    if (ifc_pair.Value.TLType == "GEV")
    //                                    {
    //                                        long iIPAddress = (long)ifc_pair.Value.NodeList["GevInterfaceSubnetIPAddress"].Value;
    //                                        System.Console.Write("          GevInterfaceSubnetIPAddress: {0}.{1}.{2}.{3}\r\n", (iIPAddress & 0xff000000) >> 24,
    //                                                                                                                        (iIPAddress & 0x00ff0000) >> 16,
    //                                                                                                                        (iIPAddress & 0x0000ff00) >> 8,
    //                                                                                                                        (iIPAddress & 0x000000ff));
    //                                        long iSubnetMask = (long)ifc_pair.Value.NodeList["GevInterfaceSubnetMask"].Value;
    //                                        System.Console.Write("          GevInterfaceSubnetMask:      {0}.{1}.{2}.{3}\r\n", (iSubnetMask & 0xff000000) >> 24,
    //                                                                                                                        (iSubnetMask & 0x00ff0000) >> 16,
    //                                                                                                                        (iSubnetMask & 0x0000ff00) >> 8,
    //                                                                                                                        (iSubnetMask & 0x000000ff));
    //                                    }
    //                                    System.Console.Write("  \r\n");
    //                                    //  break;
    //                                }
    //                            }
    //                            catch (BGAPI2.Exceptions.ResourceInUseException ex)
    //                            {
    //                                returnCode = (0 == returnCode) ? 1 : returnCode;
    //                                System.Console.Write(" Interface {0} already opened \r\n", ifc_pair.Key);
    //                                System.Console.Write(" ResourceInUseException {0} \r\n", ex.GetErrorDescription());
    //                            }
    //                        }
    //                    }
    //                    catch (BGAPI2.Exceptions.IException ex)
    //                    {
    //                        returnCode = (0 == returnCode) ? 1 : returnCode;
    //                        System.Console.Write("ExceptionType:    {0} \r\n", ex.GetType());
    //                    }

    //                    // if a camera is connected to the system interface then leave the system loop
    //                    if (sInterfaceID != "")
    //                    {

    //                        // sInterfaceID_ls.Add(sInterfaceID);  ///added k
    //                        // break;

    //                        try
    //                        {
    //                            mInterface = interfaceList[sInterfaceID];
    //                            //COUNTING AVAILABLE CAMERAS
    //                            deviceList = mInterface.Devices;
    //                            deviceList.Refresh(100);
    //                            try
    //                            {
    //                                foreach (KeyValuePair<string, BGAPI2.Device> dev_pair in deviceList)
    //                                {
    //                                    try
    //                                    {
    //                                        System.Console.Write("5.1.7   Open first device \r\n");
    //                                        System.Console.Write("          Device DeviceID:        {0}\r\n", dev_pair.Value.Id);
    //                                        System.Console.Write("          Device SerialNumber:    {0}\r\n", dev_pair.Value.SerialNumber);
    //                                        System.Console.Write("          Device AccessStatus:    {0}\r\n", dev_pair.Value.AccessStatus);
    //                                        dev_pair.Value.Open();
    //                                        sDeviceID = dev_pair.Key;
    //                                        System.Console.Write("        Opened device - RemoteNodeList Information \r\n");
    //                                        System.Console.Write("          Device AccessStatus:    {0}\r\n", dev_pair.Value.AccessStatus);

    //                                        //SERIAL NUMBER

    //                                        System.Console.Write("          DeviceSerialNumber:     {0}\r\n", (string)dev_pair.Value.RemoteNodeList["DeviceSerialNumber"].Value);
    //                                        System.Console.Write("          \r\n");


    //                                        //SERIAL NUMBER
    //                                        if (dev_pair.Value.RemoteNodeList.GetNodePresent("DeviceSerialNumber") == true)
    //                                        {
    //                                            string serialnumber = (string)dev_pair.Value.RemoteNodeList["DeviceSerialNumber"].Value;
    //                                            System.Console.Write("          DeviceSerialNumber:     {0}\r\n", serialnumber);
    //                                            foreach (BaumerCam camOb in listOfCamObs_u) //assign camera to cam ob if camera is found physically
    //                                            {
    //                                                if (camOb.serialNo == serialnumber)
    //                                                {
    //                                                    camOb.cam = deviceList[sDeviceID];
    //                                                    Console.WriteLine("cam-----> " + camOb.name + "  " + camOb.cam.Model + "  " + camOb.serialNo + " connected +++++++ and linked ------------------->");
    //                                                }
    //                                            }
    //                                            //if ("700004233821" == serialnumber)
    //                                            //    camH = deviceList[sDeviceID];
    //                                            //if ("700005000992" == serialnumber)
    //                                            //    camV = deviceList[sDeviceID];  //gige
    //                                            //if ("700004233817" == serialnumber)  // 700005943392  //new camera
    //                                            //    camX = deviceList[sDeviceID];

    //                                        }//if statement 
    //                                         //  break;
    //                                    }
    //                                    catch (BGAPI2.Exceptions.ResourceInUseException ex)
    //                                    {
    //                                        returnCode = (0 == returnCode) ? 1 : returnCode;
    //                                        System.Console.Write(" Device {0} already opened \r\n", dev_pair.Key);
    //                                        System.Console.Write(" ResourceInUseException {0} \r\n", ex.GetErrorDescription());
    //                                    }
    //                                    catch (BGAPI2.Exceptions.AccessDeniedException ex)
    //                                    {
    //                                        returnCode = (0 == returnCode) ? 1 : returnCode;
    //                                        System.Console.Write(" Device {0} already opened \r\n", dev_pair.Key);
    //                                        System.Console.Write(" AccessDeniedException {0} \r\n", ex.GetErrorDescription());
    //                                    }
    //                                }
    //                            }
    //                            catch (BGAPI2.Exceptions.IException ex)
    //                            {
    //                                returnCode = (0 == returnCode) ? 1 : returnCode;
    //                                System.Console.Write("ExceptionType:    {0} \r\n", ex.GetType());
    //                                System.Console.Write("ErrorDescription: {0} \r\n", ex.GetErrorDescription());
    //                                System.Console.Write("in function:      {0} \r\n", ex.GetFunctionName());
    //                            }

    //                            System.Console.Write("5.1.6   Detected devices:         {0}\r\n", deviceList.Count);

    //                            //DEVICE INFORMATION BEFORE OPENING
    //                            foreach (KeyValuePair<string, BGAPI2.Device> dev_pair in deviceList)
    //                            {
    //                                System.Console.Write("  5.2.3   Device DeviceID:        {0}\r\n", dev_pair.Key);
    //                                System.Console.Write("          Device SerialNumber:    {0}\r\n", dev_pair.Value.SerialNumber);
    //                                System.Console.Write("          Device AccessStatus:    {0}\r\n", dev_pair.Value.AccessStatus);
    //                                System.Console.Write("          Device UserID:          {0}\r\n\r\n", dev_pair.Value.DisplayName);
    //                            }
    //                        }
    //                        catch (BGAPI2.Exceptions.IException ex)
    //                        {
    //                            returnCode = (0 == returnCode) ? 1 : returnCode;
    //                            System.Console.Write("ExceptionType:    {0} \r\n", ex.GetType());
    //                            System.Console.Write("ErrorDescription: {0} \r\n", ex.GetErrorDescription());
    //                            System.Console.Write("in function:      {0} \r\n", ex.GetFunctionName());
    //                        }//devices end
    //                    }
    //                }
    //                catch (BGAPI2.Exceptions.ResourceInUseException ex)
    //                {
    //                    returnCode = (0 == returnCode) ? 1 : returnCode;
    //                    System.Console.Write(" System {0} already opened \r\n", sys_pair.Key);
    //                    System.Console.Write(" ResourceInUseException {0} \r\n", ex.GetErrorDescription());
    //                }
    //            }
    //        }
    //        catch (BGAPI2.Exceptions.IException ex)
    //        {
    //            returnCode = (0 == returnCode) ? 1 : returnCode;
    //            System.Console.Write("ExceptionType:    {0} \r\n", ex.GetType());
    //        }



    //    }

    //    public int assignDataStreamNsetFeatures(BaumerCam camObj, ref BGAPI2.Device mDevice, ref BGAPI2.DataStream mDataStream)
    //    {

    //        if (mDevice.IsOpen != true)
    //        {
    //            Console.WriteLine(" --------------  MDEVICE IS NOT OPEN. Trying to open");
    //            mDevice.Open();
    //        }

    //        string sDataStreamID = "";
    //        int returnCode = 0;
    //        BGAPI2.DataStreamList datastreamList = null;
    //        try
    //        {   //SET Exposure time
    //            mDevice.RemoteNodeList["ExposureTime"].Value = camObj.exposureTimeUser;
    //            //SET TRIGGER SOURCE "SOFTWARE"
    //            mDevice.RemoteNodeList["TriggerSource"].Value = camObj.triggerSource;
    //            Console.WriteLine("Ext trigger");

    //            //SET TRIGGER MODE OFF (FreeRun)
    //            mDevice.RemoteNodeList["TriggerMode"].Value = "On";
    //            mDevice.RemoteNodeList["TriggerActivation"].Value = "FallingEdge";
    //            mDevice.RemoteNodeList["LineSelector"].Value = "Line0"; //set debounce time in camera trugger input
    //            mDevice.RemoteNodeList["LineDebouncerHighTimeAbs"].Value = 500;
    //            mDevice.RemoteNodeList["LineDebouncerLowTimeAbs"].Value = 500;


    //            //SET output line to userOutput1
    //            mDevice.RemoteNodeList["LineSelector"].Value = "Line3";
    //            mDevice.RemoteNodeList["LineSource"].Value = "UserOutput1";
    //            mDevice.RemoteNodeList["UserOutputSelector"].Value = "UserOutput1";
    //            mDevice.RemoteNodeList["UserOutputValue"].Value = false;
    //            //set exposure
    //            //long heightvalue = 3648;
    //            //long widthval = 5472;
    //            //mDevice.RemoteNodeList["Height"].Value = heightvalue;
    //            //mDevice.RemoteNodeList["Width"].Value = widthval;
    //        }
    //        catch (BGAPI2.Exceptions.IException ex)
    //        {
    //            returnCode = (0 == returnCode) ? 1 : returnCode;
    //            System.Console.Write("ExceptionType:    {0} \r\n", ex.GetType());
    //            System.Console.Write("ErrorDescription: {0} \r\n", ex.GetErrorDescription());
    //            System.Console.Write("in function:      {0} \r\n", ex.GetFunctionName());
    //        }


    //        try
    //        {
    //            //COUNTING AVAILABLE DATASTREAMS
    //            datastreamList = mDevice.DataStreams;
    //            datastreamList.Refresh();
    //            System.Console.Write("              Detected datastreams:     {0}\r\n", datastreamList.Count);
    //            //DATASTREAM INFORMATION BEFORE OPENING
    //            foreach (KeyValuePair<string, BGAPI2.DataStream> dst_pair in datastreamList)
    //            {
    //                System.Console.Write("              DataStream ID:          {0}\r\n\r\n", dst_pair.Key);
    //            }
    //        }
    //        catch (BGAPI2.Exceptions.IException ex)
    //        {
    //            returnCode = (0 == returnCode) ? 1 : returnCode;
    //            System.Console.Write("ExceptionType:    {0} \r\n", ex.GetType());
    //            System.Console.Write("ErrorDescription: {0} \r\n", ex.GetErrorDescription());
    //            System.Console.Write("in function:      {0} \r\n", ex.GetFunctionName());
    //        }



    //        //OPEN THE FIRST DATASTREAM IN THE LIST
    //        try
    //        {
    //            foreach (KeyValuePair<string, BGAPI2.DataStream> dst_pair in datastreamList)
    //            {
    //                //System.Console.Write("5.1.9   Open first datastream \r\n");
    //                //System.Console.Write("          DataStream ID:          {0}\r\n\r\n", dst_pair.Key);
    //                dst_pair.Value.Open();
    //                sDataStreamID = dst_pair.Key;
    //                // System.Console.Write("        Opened datastream - NodeList Information \r\n");
    //                //System.Console.Write("          StreamAnnounceBufferMinimum:  {0}\r\n", dst_pair.Value.NodeList["StreamAnnounceBufferMinimum"].Value);
    //                if (dst_pair.Value.TLType == "GEV")
    //                {
    //                    //System.Console.Write("          StreamDriverModel:            {0}\r\n", dst_pair.Value.NodeList["StreamDriverModel"].Value);
    //                }
    //                System.Console.Write("  \r\n");
    //                break;
    //            }
    //        }
    //        catch (BGAPI2.Exceptions.IException ex)
    //        {
    //            returnCode = (0 == returnCode) ? 1 : returnCode;
    //            System.Console.Write("ExceptionType:    {0} \r\n", ex.GetType());
    //            System.Console.Write("ErrorDescription: {0} \r\n", ex.GetErrorDescription());
    //            System.Console.Write("in function:      {0} \r\n", ex.GetFunctionName());
    //        }

    //        if (sDataStreamID == "")
    //        {
    //            System.Console.Write("              No DataStream found \r\n");
    //            System.Console.Write("\n\nInput any number to close the program:\r\n\n");
    //            // mDevice.Close();
    //            return 0;
    //        }
    //        else
    //        {
    //            mDataStream = datastreamList[sDataStreamID];
    //            if (mDataStream == null)
    //            {
    //                Console.WriteLine("mDataStreamIs null in assignDataStream " + camObj.name);
    //                return 0;
    //            }
    //            else
    //            {
    //                Console.WriteLine("Assigned datastream to camera:" + camObj.name);
    //                return 1;
    //            }

    //        }

    //    }

    //    public int assignBuffer(BaumerCam camObj, ref BGAPI2.DataStream mDataStream, ref BGAPI2.BufferList bufferList, ref BGAPI2.Buffer mBuffer)
    //    {
    //        int returnCode = 0;
    //        try
    //        {
    //            bufferList = mDataStream.BufferList;
    //            for (int i = 0; i < 4; i++)
    //            {
    //                mBuffer = new BGAPI2.Buffer();
    //                bufferList.Add(mBuffer);
    //            }
    //            //mBuffer = new BGAPI2.Buffer();
    //            //bufferList.Add(mBuffer);
    //            //mBuffer.QueueBuffer(); //loop or this
    //        }
    //        catch (BGAPI2.Exceptions.IException ex)
    //        {
    //            returnCode = (0 == returnCode) ? 1 : returnCode;
    //            System.Console.Write("ExceptionType:    {0} \r\n", ex.GetType());
    //            System.Console.Write("ErrorDescription: {0} \r\n", ex.GetErrorDescription());
    //            System.Console.Write("in function:      {0} \r\n", ex.GetFunctionName());
    //        }

    //        try
    //        {
    //            foreach (KeyValuePair<string, BGAPI2.Buffer> buf_pair in bufferList)
    //            {
    //                buf_pair.Value.QueueBuffer();
    //            }
    //            System.Console.Write("5.1.11   Queued buffers:          {0}\r\n", bufferList.QueuedCount);
    //        }
    //        catch (BGAPI2.Exceptions.IException ex)
    //        {
    //            returnCode = (0 == returnCode) ? 1 : returnCode;
    //            System.Console.Write("ExceptionType:    {0} \r\n", ex.GetType());
    //            System.Console.Write("ErrorDescription: {0} \r\n", ex.GetErrorDescription());
    //            System.Console.Write("in function:      {0} \r\n", ex.GetFunctionName());
    //        }
    //        System.Console.Write("Buffer queueing done  ----\r\n");
    //        //defining buffer event


    //        try
    //        {
    //            camObj.linkBufferEvent();
    //            System.Console.Write("        Register Event Mode to:   {0}\r\n\r\n", mDataStream.EventMode.ToString());
    //        }
    //        catch (BGAPI2.Exceptions.IException ex)
    //        {
    //            returnCode = (0 == returnCode) ? 1 : returnCode;
    //            System.Console.Write("ExceptionType:    {0} \r\n", ex.GetType());
    //            System.Console.Write("ErrorDescription: {0} \r\n", ex.GetErrorDescription());
    //            System.Console.Write("in function:      {0} \r\n", ex.GetFunctionName());
    //        }
    //        ////---------------------

    //        try
    //        {
    //            camObj.mDataStream.StartAcquisition();
    //            //  camObj.cam.RemoteNodeList["AcquisitionStart"].Execute();
    //            System.Console.Write("CAMERA START " + camObj.name + "\r\n ");
    //            camObj.connectionStatus = true;
    //        }
    //        catch (BGAPI2.Exceptions.IException ex)
    //        {
    //            System.Console.Write("ErrorDescription: {0} \r\n", ex.GetErrorDescription());
    //            System.Console.Write("in function:      {0} \r\n", ex.GetFunctionName());
    //        }
    //        return 1;
    //    }
    //    public void startCameraAcq(BaumerCam c)
    //    {
    //        if (c.connectionStatus == true && c.capturing == false)
    //        {
    //            try
    //            {
    //                c.cam.RemoteNodeList["AcquisitionStart"].Execute();
    //                c.capturing = true;
    //                Console.WriteLine("Started capture camera : " + c.name);
    //            }
    //            catch (Exception ee)
    //            {
    //                Console.WriteLine("Exception in Started capture camera : " + c.name);
    //            }
    //        }
    //    }

    //    public void stopCameraAcq(BaumerCam c)
    //    {

    //        if (c.connectionStatus == true && c.cam != null && c.capturing == true)
    //        {
    //            try
    //            {
    //                c.cam.RemoteNodeList["AcquisitionAbort"].Execute();
    //                c.cam.RemoteNodeList["AcquisitionStop"].Execute();
    //                c.capturing = false;
    //                Console.WriteLine("Stopped capture camera : " + c.name);
    //            }
    //            catch (Exception ee)
    //            {
    //                Console.WriteLine("Exception in Stopped capture camera : " + c.name);
    //            }
    //        }
    //    }

    //    public void disposeCamera(BaumerCam c)
    //    {

    //        if (c.connectionStatus == true && c.cam != null && c.capturing == false)
    //        {
    //            try
    //            {

    //                c.capturing = false;
    //                c.connectionStatus = false;
    //                c.mDataStream.StopAcquisition();
    //                c.cam.Close();
    //                c.cam = null;
    //                Console.WriteLine("closed camera : " + c.name);
    //            }
    //            catch (Exception ee)
    //            {
    //                Console.WriteLine("Exception in Dispose camera : " + c.name);
    //            }

    //        }
    //    }

    //    public void setExposure(ref BaumerCam c, double fExposure)
    //    {
    //        if (c.cam.IsOpen == true)
    //        {
    //            c.cam.RemoteNodeList["ExposureTime"].Value = fExposure;
    //            c.exposureTimeUser = fExposure;
    //            Console.WriteLine(c.name + " ExposureTIme modified");
    //        }

    //    }


    //    void setCameraOP(BaumerCam c, bool state)
    //    {
    //        c.cam.RemoteNodeList["UserOutputSelector"].Value = "UserOutput1";
    //        c.cam.RemoteNodeList["UserOutputValue"].Value = state;

    //    }

    //    void setCameraOPPulse(BaumerCam c, bool state, int widthMs)
    //    {
    //        c.cam.RemoteNodeList["UserOutputSelector"].Value = "UserOutput1";
    //        c.cam.RemoteNodeList["UserOutputValue"].Value = state;
    //        System.Threading.Thread.Sleep(widthMs);
    //        c.cam.RemoteNodeList["UserOutputValue"].Value = !state;
    //        System.Threading.Thread.Sleep(widthMs / 2);//excess delay to keep consicutive pulses separate
    //    }

    //}
}
