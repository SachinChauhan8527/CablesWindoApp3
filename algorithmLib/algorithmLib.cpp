// This is the main DLL file.

#include "stdafx.h"
#include <algorithm>
#include <cstdlib>
#include "nlohmann/json.hpp"
#include "algorithmLib.h"
#include <vector>
using namespace System;
using namespace cv;
using namespace std;
using json = nlohmann::json;
//2413,1028
Mat image00;
Mat imageGrey;
Mat imgROI;
Mat draw;
Mat baseColorSel;
Mat maskImg;

Mat imgBGR,imgHSV;
Point2f P1;
Point2f P2;
string path;
Point2f ptTopUI=Point2f(0,0);
Point2f ptBottomUI=Point2f(0,0);

//machineTypeProp  0 ---bottom fix   1 --- top fix
	// sampleMarkContrastProp  0 black mark 1 white mark
System::Drawing::Bitmap^ MatToBitmap(Mat inputImage)
{
	return gcnew System::Drawing::Bitmap(inputImage.cols, inputImage.rows, 4 * inputImage.rows, System::Drawing::Imaging::PixelFormat::Format24bppRgb, IntPtr(inputImage.data));
}
void resShow(string name, Mat img, float scale = 0.25)
{
	Mat res;
	//scale = 0.2;
	resize(img, res, Size(), scale, scale);
	imshow(name, res);
	waitKey(0);
}
Mat BitmapToMat(System::Drawing::Bitmap^ bitmap)
{

	System::Drawing::Rectangle blank = System::Drawing::Rectangle(0, 0, bitmap->Width, bitmap->Height);
	System::Drawing::Imaging::BitmapData^ bmpdata = bitmap->LockBits(blank, System::Drawing::Imaging::ImageLockMode::ReadWrite, bitmap->PixelFormat);
	if (bitmap->PixelFormat == System::Drawing::Imaging::PixelFormat::Format8bppIndexed)
	{
		//tmp = cvCreateImage(cvSize(bitmap->Width, bitmap->Height), IPL_DEPTH_8U, 1);
		//tmp->imageData = (char*)bmData->Scan0.ToPointer();
		cv::Mat thisimage(cv::Size(bitmap->Width, bitmap->Height), CV_8UC1, bmpdata->Scan0.ToPointer(), cv::Mat::AUTO_STEP);
		bitmap->UnlockBits(bmpdata);
		return thisimage;
	}

	else if (bitmap->PixelFormat == System::Drawing::Imaging::PixelFormat::Format24bppRgb)
	{

		cv::Mat thisimage(cv::Size(bitmap->Width, bitmap->Height), CV_8UC3, bmpdata->Scan0.ToPointer(), cv::Mat::AUTO_STEP);
		bitmap->UnlockBits(bmpdata);
		return thisimage;

	}

	Mat returnMat = (Mat::zeros(640, 480, CV_8UC1));
	//   bitmap->UnlockBits(bmData);
	return returnMat;
	//   return cvarrToMat(tmp);
}
void Morph(const cv::Mat &src, cv::Mat &dst, int operation, int kernel_type, int size)
{
    cv::Point anchor = cv::Point(size, size);
    cv::Mat element = getStructuringElement(kernel_type, cv::Size(2 * size + 1, 2 * size + 1), anchor);
    morphologyEx(src, dst, operation, element, anchor);
}

//----------------------color Processing-----------------------------------------------
int hueRngProp=42;
int satRngProp=50;

struct HSVval {
double Hue;
double Saturation;
double Value;
};

static Mat colorPkImg;
static vector<HSVval> colorVectorTop;
static vector<HSVval> colorVectorBott;
static vector<HSVval> colorVectorDumbell;
static vector<HSVval> colorVectorBackGr;
static vector<HSVval> colorVectorRead;//clearColorVectors
int algorithmLib::Class1::clearColorVectors()
{colorVectorTop.clear();
colorVectorBackGr.clear();
colorVectorBott.clear();
colorVectorDumbell.clear();
return 1;
}
int algorithmLib::Class1::updateColorTop(float x1,float y1)
{
	Rect ROI = Rect(Point(P1.x,P1.y), Point(P2.x,P2.y));
	//Mat inImg = imgBGR;


double Hue =imgHSV(ROI).at<Vec3b>(y1,x1)[0];
double Sat =imgHSV(ROI).at<Vec3b>(y1,x1)[1];
double Val =imgHSV(ROI).at<Vec3b>(y1,x1)[2];
//Mat show;
//cvtColor(imgHSV(ROI),show,CV_HSV2BGR);
// circle(show,Point(x1,y1),15,Scalar(0,255,0),-1);
// imshow("colorHSV",show);
// waitKey(5);


HSVval color;
color.Hue=Hue;
color.Saturation=Sat ;
color.Value=Val ;
colorVectorTop.push_back(color);
ptTopUI=Point2f(x1,y1);
return 1;
}
int algorithmLib::Class1::updateColorBottom(float x1,float y1)
{
		Rect ROI = Rect(Point(P1.x,P1.y), Point(P2.x,P2.y));
	//Mat inImg = imgBGR;


double Hue =imgHSV(ROI).at<Vec3b>(y1,x1)[0];
double Sat =imgHSV(ROI).at<Vec3b>(y1,x1)[1];
double Val =imgHSV(ROI).at<Vec3b>(y1,x1)[2];
HSVval color;
color.Hue=Hue;
color.Saturation=Sat ;
color.Value=Val ;
colorVectorBott.push_back(color);
ptBottomUI=Point2f(x1,y1);
return 1;
}
int algorithmLib::Class1::updateColorDumbell(float x1,float y1)
{
	Rect ROI = Rect(Point(P1.x,P1.y), Point(P2.x,P2.y));
	//Mat inImg = imgBGR;


double Hue =imgHSV(ROI).at<Vec3b>(y1,x1)[0];
double Sat =imgHSV(ROI).at<Vec3b>(y1,x1)[1];
double Val =imgHSV(ROI).at<Vec3b>(y1,x1)[2];
HSVval color;
color.Hue=Hue;
color.Saturation=Sat ;
color.Value=Val ;
colorVectorDumbell.push_back(color);
return 1;
}
int algorithmLib::Class1::updateColorBackGround(float x1,float y1)
{
	Rect ROI = Rect(Point(P1.x,P1.y), Point(P2.x,P2.y));
	//Mat inImg = imgBGR;


double Hue =imgHSV(ROI).at<Vec3b>(y1,x1)[0];
double Sat =imgHSV(ROI).at<Vec3b>(y1,x1)[1];
double Val =imgHSV(ROI).at<Vec3b>(y1,x1)[2];
HSVval color;
color.Hue=Hue;
color.Saturation=Sat ;
color.Value=Val ;
colorVectorBackGr.push_back(color);
return 1;
}
 System::Drawing::Bitmap^ algorithmLib::Class1::processColorVector(int code)
{
//System::Drawing::Bitmap^ colorPickerFn::functions::fillColor(System::Drawing::Bitmap^ inImage,int x, int y)
//{
	vector<HSVval> colorVectorTmp;
	Rect ROI = Rect(Point(P1.x,P1.y), Point(P2.x,P2.y));
	Mat inImg = imgBGR.clone();
	Vec3b colorDraw;

	if (code==0)  ///top
	{for (int i =0;i<colorVectorTop.size();i++)
		{
			colorVectorTmp.push_back(colorVectorTop[i]);
			colorDraw=Vec3b(0,150,150);
		}
	
	}
	else if (code==1) //bottom
	{for (int i =0;i<colorVectorBott.size();i++)
		{
			colorVectorTmp.push_back(colorVectorBott[i]);
			colorDraw=Vec3b(180,0,0);
		}
	
	}
	else  if (code==2) //Dumbell
	{for (int i =0;i<colorVectorDumbell.size();i++)
		{
			colorVectorTmp.push_back(colorVectorDumbell[i]);
		colorDraw=Vec3b(0,0,190);
		}
	
	}
	else if (code==3)//background
		{for (int i =0;i<colorVectorBackGr.size();i++)
		{
			colorVectorTmp.push_back(colorVectorBackGr[i]);
			colorDraw=Vec3b(0,150,0);
		}
	
	}







//double Hue =imgHSV.at<Vec3b>(y,x)[0];
//double Sat =imgHSV.at<Vec3b>(y,x)[1];
//double Val =imgHSV.at<Vec3b>(y,x)[2];
HSVval color;
//color.Hue=Hue;
//color.Saturation=Sat ;
//color.Value=Val ;


//imshow("imgHSV00",imgHSV(ROI));
for(int k=0; k <colorVectorTmp.size();k++)
{
	color=colorVectorTmp[k];
cout<<"vectorSize::"<<colorVectorTmp.size()<<"H::"<<color.Hue <<" _Sat::"<<color.Saturation<<endl;

for (int i = 0;i<imgHSV(ROI).rows;i++)
{
	for (int j=0;j<imgHSV(ROI).cols;j++)
	{
	if ((imgHSV(ROI).at<Vec3b>(i,j)[0]>=(color.Hue-hueRngProp)) && (imgHSV(ROI).at<Vec3b>(i,j)[0]<=(color.Hue+hueRngProp)) &&( imgHSV(ROI).at<Vec3b>(i,j)[1]>=(color.Saturation -satRngProp))&&( imgHSV(ROI).at<Vec3b>(i,j)[1]<=(color.Saturation +satRngProp))&&( imgHSV(ROI).at<Vec3b>(i,j)[2]>=(color.Value -satRngProp))&&( imgHSV(ROI).at<Vec3b>(i,j)[2]<=(color.Value +satRngProp)) )// && imgHSV.at<Vec3b>(i,j)[1]==S && imgHSV.at<Vec3b>(i,j)[2]==V)  ////  
	{//cout<<"inside loop::"<<i<<","<<j<<endl;
	//inImg(ROI).at<Vec3b>(i,j)[0]=0;
	//inImg(ROI).at<Vec3b>(i,j)[1]=255;
	//inImg(ROI).at<Vec3b>(i,j)[2]=0;
		inImg(ROI).at<Vec3b>(i,j)=colorDraw ;

	}



	}

}
}
//imshow("imgHSV00",imgHSV(ROI));
//Mat imgShow=colorPkImg.clone();
	//cout<<"funcCall::"<<endl;
//return inImg;

System::Drawing::Bitmap^ dst = MatToBitmap(inImg(ROI).clone());
	return dst;
//}



}
Mat processColorVectorMask(Mat inImg,Mat mask,int code)
{

	vector<HSVval> colorVectorTmp;
	//Rect ROI = Rect(Point(P1.x,P1.y), Point(P2.x,P2.y));
	//Mat inImg = imgBGR.clone();
	//Vec3b colorDraw;

	if (code==0)  ///top
	{for (int i =0;i<colorVectorTop.size();i++)
		{
			colorVectorTmp.push_back(colorVectorTop[i]);
		
		}
	
	}
	else if (code==1) //bottom
	{for (int i =0;i<colorVectorBott.size();i++)
		{
			colorVectorTmp.push_back(colorVectorBott[i]);
		
		}
	
	}
	else  if (code==2) //Dumbell
	{for (int i =0;i<colorVectorDumbell.size();i++)
		{
			colorVectorTmp.push_back(colorVectorDumbell[i]);
		
		}
	
	}
	else if (code==3)//background
		{for (int i =0;i<colorVectorBackGr.size();i++)
		{
			colorVectorTmp.push_back(colorVectorBackGr[i]);
		
		}
	
	}

HSVval color;

//imshow("imgHSV00",imgHSV(ROI));
for(int k=0; k <colorVectorTmp.size();k++)
{
	color=colorVectorTmp[k];
cout<<"vectorSize::"<<colorVectorTmp.size()<<"H::"<<color.Hue <<" _Sat::"<<color.Saturation<<endl;

for (int i = 0;i<inImg.rows;i++)
{
	for (int j=0;j<inImg.cols;j++)
	{
	if ((inImg.at<Vec3b>(i,j)[0]>=(color.Hue-hueRngProp)) && (inImg.at<Vec3b>(i,j)[0]<=(color.Hue+hueRngProp)) &&( inImg.at<Vec3b>(i,j)[1]>=(color.Saturation -satRngProp))&&(inImg.at<Vec3b>(i,j)[1]<=(color.Saturation +satRngProp))&&(inImg.at<Vec3b>(i,j)[2]>=(color.Value -satRngProp))&&( inImg.at<Vec3b>(i,j)[2]<=(color.Value +satRngProp)) )// && imgHSV.at<Vec3b>(i,j)[1]==S && imgHSV.at<Vec3b>(i,j)[2]==V)  ////  
	{//cout<<"inside loop::"<<i<<","<<j<<endl;
	//inImg(ROI).at<Vec3b>(i,j)[0]=0;
	//inImg(ROI).at<Vec3b>(i,j)[1]=255;
	//inImg(ROI).at<Vec3b>(i,j)[2]=0;
		mask.at<uchar>(i,j)=255;

	}



	}

}
}
//imshow("imgHSVmask",mask);
//Mat imgShow=colorPkImg.clone();
	//cout<<"funcCall::"<<endl;
//return inImg;

//System::Drawing::Bitmap^ dst = MatToBitmap(inImg(ROI).clone());
	return mask;
//}



}


//---------------------------------------------------------------------

//other funcs
float distancePoint(Point2f p, Point2f q) {
	Point2f diff = p - q;
	return cv::sqrt(diff.x*diff.x + diff.y*diff.y);
}
int algorithmLib::Class1::pathReader() {

	string str1 = "C:/Add Innovations/trainPath.txt", strOut;

	ifstream infileF(str1.c_str());

	if (infileF.is_open())
	{
		getline(infileF, strOut);
		replace(strOut.begin(), strOut.end(), '\\', '/');
		infileF.close();
	}

	path = strOut;
	cout << "path::" << path << endl;
	return 1;
}
//img Processing

float targetEncCircDist = 140.7;
int algorithmLib::Class1::calibUsingTargetCircle(System::Drawing::Bitmap^ bitmap)
{
	int returnCode = -1;
	try {
		//targetEncCircDist = 140.65;
		Mat imageIn = BitmapToMat(bitmap);
		Mat readImage = imageIn.clone();
		Mat res = imageIn.clone();
		SimpleBlobDetector::Params params;

		// Change thresholds
		params.minThreshold = 0;
		params.maxThreshold = 140;

		// Filter by Area.
		params.filterByArea = true;
		params.minArea = 50;
		params.maxArea = 110;
		// Filter by Circularity
		params.filterByCircularity = true;
		params.minCircularity = 0.7;

		// Filter by Convexity
		params.filterByConvexity = true;
		params.minConvexity = 0.87;

		// Filter by Inertia
		params.filterByInertia = true;
		params.minInertiaRatio = 0.7;
		//-----------------------
		//SimpleBlobDetector detector(params);

		Ptr<SimpleBlobDetector> detector = SimpleBlobDetector::create(params);
		// Detect blobs.
		std::vector<KeyPoint> keypoints;
		detector->detect(readImage, keypoints);
		cout << "keypoints size ::" << keypoints.size() << endl; //5041
		// Draw detected blobs as red circles.
		// DrawMatchesFlags::DRAW_RICH_KEYPOINTS flag ensures the size of the circle corresponds to the size of blob
		Mat im_with_keypoints;
		drawKeypoints(readImage, keypoints, im_with_keypoints, Scalar(0, 200, 0), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
		vector<Point2f> vec_centers;
		for (int i = 0; i < keypoints.size(); i++)
		{
			circle(im_with_keypoints, keypoints[i].pt, 5, Scalar(0, 0, 200), -1);
			vec_centers.push_back(keypoints[i].pt);
		}
		if (keypoints.size() > 0)
		{
			cout << "size of detected circle in Pixels" << keypoints[0].size << endl;
		}
		RotatedRect rr = minAreaRect(vec_centers);
		Point2f vert[4];
		float dists[4] = { 1,10,100,1000 };
		rr.points(vert);

		for (int i = 0; i < 4; i++)
		{
			dists[i] = distancePoint(vert[i], vert[(i + 1) % 4]);
			//	putText(im_with_keypoints, to_string(dists[i]), vert[i], FONT_HERSHEY_PLAIN, 4, Scalar(0, 120, 0), 2);
			putText(im_with_keypoints, cv::format("%5.2f", dists[i]), vert[i], FONT_HERSHEY_PLAIN, 4, Scalar(0, 120, 0), 2);
			cout << "side of square::" + to_string(i) + "  :" << dists[i] << endl;
			line(im_with_keypoints, vert[i], vert[(i + 1) % 4], Scalar(200, 0, 0), 2);
			if (i > 0)
			{
				if (abs(dists[i] - dists[0]) < keypoints[0].size)
					dists[i] = 0;
			}
		}
		if (dists[1] == 0 && dists[2] == 0 && dists[3] == 0)
		{
			putText(im_with_keypoints, "Square pattern detected", Point(20, 180), FONT_HERSHEY_PLAIN, 4, Scalar(0, 220, 0), 2);
			cout << "side of square::" << dists[0] << endl;
			float actualSpacingMM = 0.50 * 70.0;
			cout << "actual Spacing :" << actualSpacingMM << endl;
			cout << "mmPer pix::" << actualSpacingMM / dists[0] << endl;
			mmPerPixTempProp = actualSpacingMM / dists[0];
			returnCode = 1;
		}
		else
		{
			putText(im_with_keypoints, "mismatch in square sides", Point(20, 180), FONT_HERSHEY_PLAIN, 4, Scalar(0, 220, 0), 2);
			returnCode = 0;
		}

		if (vec_centers.size() == 5041)
		{
			putText(im_with_keypoints, "Target detected", Point(20, 80), FONT_HERSHEY_PLAIN, 4, Scalar(0, 220, 0), 2);
			returnCode = 1;
		}
		else
		{
			putText(im_with_keypoints, "Missed some points", Point(20, 80), FONT_HERSHEY_PLAIN, 4, Scalar(0, 0, 220), 2);
			returnCode = 1;
		}

		//rectangle(im_with_keypoints, br, Scalar(200, 0, 0), 2);
		// Show blobs
		//resShow("keypoints", im_with_keypoints, 0.3);
		//waitKey(0);
		//imshow("calibImage", res);
		//waitKey(100);
		//tagetFoundProp = 1;
		mmPerPixProp = mmPerPix;

		//System::Drawing::Bitmap^ dst = MatToBitmap(color);
		//imageIn

		resize(im_with_keypoints, imageIn, imageIn.size());
		return returnCode;
	}
	catch (exception exx)
	{
		mmPerPixProp = 1;
		tagetFoundProp = 0;
		return returnCode;

	}

}
//int algorithmLib::Class1::calibUsingTargetCircle(System::Drawing::Bitmap^ bitmap)
//{
//	try {
//		//targetEncCircDist = 140.65;
//		Mat imageIn = BitmapToMat(bitmap);
//		vector<Point2f> pointBuf;
//		vector<Point2f> subPixCorners;
//		//Mat imageIn = imread("C:/Users/krish/OneDrive/Pictures/Baumer Image Records/IMG0124.bmp");
//		Mat res = imageIn.clone();
//		//resize(imageIn, res, Size(), 0.5, 0.5);
//
//		Mat imgTmp = res.clone();
//		bool found = findCirclesGrid(imgTmp, Size(4, 11), pointBuf, CALIB_CB_ASYMMETRIC_GRID);
//		if (found == false || !(pointBuf.size() == 44))
//		{
//			mmPerPixProp = 1;
//				tagetFoundProp = 0;
//				return 0;
//		}
//		std::cout << "pointBuf size::" << pointBuf.size() << endl;
//		Mat resGray;
//		cv::cvtColor(res, resGray, COLOR_BGR2GRAY);
//
//		//cornerSubPix(resGray, subPixCorners,Size(11,11),Size(-1,-1), TermCriteria(TermCriteria::EPS + TermCriteria::COUNT, 30, 0.0001));
//	  //  cout << "subPixCorners size::" << subPixCorners.size() << endl;
//		float distmin = 5000;
//		for (int i = 0; i < pointBuf.size(); i++)
//		{
//			cv::circle(res, pointBuf[i], 20, Scalar(0, 255, 0), 2);
//			cv::circle(res, pointBuf[i], 2, Scalar(0, 0, 255), 2);
//			float dist = 0;
//			for (int j = 0; j < pointBuf.size(); i = j++)
//			{
//				dist = distancePoint(pointBuf[i], pointBuf[j]);
//				if (dist > 0 && dist < distmin)
//				{
//					distmin = dist;
//				}
//			}
//		}
//		std::cout << "distMin::" << distmin << endl;
//		/*  for (int i = 0; i < subPixCorners.size(); i++)
//		  {
//			  circle(res, subPixCorners[i], 5, Scalar(0, 0,255), 2);
//		  }*/
//		RotatedRect rr = minAreaRect(pointBuf);
//		Point2f vertices[4];
//		rr.points(vertices);
//		vector <Point2f> widthPoints;
//		for (int i = 0; i < 4; i++)
//		{
//			line(res, vertices[i], vertices[(i + 1) % 4], Scalar(0, 0, 255), 1);
//			float dist;
//			for (int j = 0; j < pointBuf.size(); j++)
//			{
//				dist = distancePoint(vertices[i], pointBuf[j]);
//				if (dist < (distmin - 0.7 * distmin))
//				{
//					widthPoints.push_back(pointBuf[j]);
//				}
//			}
//
//		}
//
//		std::cout << "widthCorners size::" << widthPoints.size() << endl;
//		float widthIn_mm = targetEncCircDist;// 176.50;// 140.7;
//		float widthIn_pixels = widthIn_mm;
//		if (widthPoints.size() == 2)
//		{
//			for (int i = 0; i < widthPoints.size(); i++)
//			{
//				cv::circle(res, widthPoints[i], 10, Scalar(255, 0, 0), 2);
//
//			}
//			line(res, widthPoints[0], widthPoints[1], Scalar(0, 120, 120), 2);
//			widthIn_pixels = distancePoint(widthPoints[0], widthPoints[1]);
//		}
//		else {std:: cout << "widthPixels Not Found" << endl;
//		
//		
//			mmPerPixProp = 1;
//			tagetFoundProp = 0;
//			return 0;
//		
//		
//		}
//
//		std::cout << "input distance::" << targetEncCircDist << endl;
//
//		float mmPerPix = widthIn_mm / widthIn_pixels;
//
//		std::cout << "mmPerPix::" << mmPerPix << endl;
//		std::cout << "found::" << found << endl;
//		//imshow("imageRead", res);
//
//
//		
//		tagetFoundProp = 1;
//		mmPerPixProp = mmPerPix;
//
//		//System::Drawing::Bitmap^ dst = MatToBitmap(color);
//		//imageIn
//		resize(res, imageIn, imageIn.size());
//		return 1;
//	}
//catch (exception exx)
//{
//	mmPerPixProp = 1;
//	tagetFoundProp = 0;
//	return 0;
//
//}
//
//}
System::Drawing::Bitmap^ algorithmLib::Class1::calibUsingTarget(System::Drawing::Bitmap^ bitmap)
{float mmPerPixTarg=0;
		//Mat imageIn = imread("D:/calibTarget.bmp");
	try {
	Mat imageIn=image00 = BitmapToMat(bitmap);
	if (imageIn.channels()>2)
	{
	cvtColor(imageIn,imageIn,COLOR_BGR2GRAY );
	}
//	Mat rot = getRotationMatrix2D(Point(imageIn.cols/2,imageIn.rows/2), 90, 1.0);
//	Rect bbox = RotatedRect(Point(imageIn.cols/2,imageIn.rows/2), imageIn.size(), 90).boundingRect();
//
//// adjust transformation matrix
//rot.at<double>(0, 2) += bbox.width / 2.0 - imageIn.cols/2;
//rot.at<double>(1, 2) += bbox.height / 2.0 - imageIn.rows/2;


//warpAffine(imageIn, imageIn, rot, bbox.size());
	Mat color;
	cvtColor(imageIn,color,COLOR_GRAY2BGR );
	Mat imgThresh;
	
	threshold(imageIn,imgThresh,80,255,THRESH_BINARY_INV);
	imwrite("threshImg.bmp",imgThresh);
	Mat threshInv=255-imgThresh ;
	Mat R;
//	resize(imgThresh,R,Size(),0.4,0.4);
	//	imshow("imgThresh",R );

	vector<vector<cv::Point> > contours;
	vector<vector<cv::Point> > contoursValid;
	vector<Vec4i> hierarchy;
	vector<RotatedRect> rectsValid;
	Mat cont =imgThresh.clone();
	findContours(imgThresh, contours, hierarchy, RETR_CCOMP, CHAIN_APPROX_SIMPLE);
	RotatedRect rect;
	RotatedRect target;
	//Rect temp;
	if (contours.size()==0)
	{
	return bitmap;
	}
	for (int i = 0; i < contours.size(); i++)
	{
		//drawContours(color, contours, i, Scalar(255,0,0), 2, 8, hierarchy);

		rect=minAreaRect(contours[i]);
	//	temp=boundingRect(contours[i]);
		//cout<<"rect h:"<<rect.size.height <<" rect w: "<<rect.size.width<<endl;
		//cout<<"w/h::"<<rect.size.width /rect.size.height<<endl;
		//cout<<"h/w::"<<rect.size.height/rect.size.width <<endl;
	if (rect.size.height>100 && rect.size.width>100 && rect.size.height <2000 && rect.size.width<2000)
	{//0.72--0.78
		if (((rect.size.width /rect.size.height)>0.65 && (rect.size.width /rect.size.height)<0.85 )||((rect.size.height/rect.size.width )>0.65 && ( rect.size.height/rect.size.width)<0.85 ) )
		{
		drawContours(color, contours, i, Scalar(0,255,0), 2, 8, hierarchy);
		contoursValid.push_back(contours[i]);
	rectsValid.push_back(rect);
		}
		//waitKey();
	}
	/*Mat res;
	resize(color,res,Size(),0.3,0.3);
		imshow("thresh",res);*/
	}
	//validate ----calib target------------------
	bool s1,s2,s3,s4,s5;
	int cnz=0;
		if (contoursValid.size()==0)
	{
	return bitmap;
	}
	for (int j = 0; j < contoursValid.size(); j++)
	{s1=false;
	s2=false;
	s3=false;
	s4=false;
	s5=false;
	cnz=0;
//	Rect r1=Rect(rectsValid[j].center
	circle(color,rectsValid[j].center,4, Scalar(0,0,255));
	Point C1=rectsValid[j].center;
	int width=min(rectsValid[j].size.width,rectsValid[j].size.height);
	int height=max(rectsValid[j].size.height,rectsValid[j].size.height);

	Rect r1=Rect(C1.x+(width/24),C1.y-(width/24),width/12,width/12);//wt
	cnz=countNonZero(threshInv(r1));
	if (cnz> (0.8*(r1.width*r1.height)))
	{	s1=true;
	rectangle(color,r1,Scalar(0,125,125),-1);}

	 r1=Rect(C1.x+(width/24)+width/6,C1.y-(width/24),width/12,width/12);//blk
		cnz=countNonZero(imgThresh(r1));
	if (cnz>(0.8*(r1.width*r1.height)))
		{s2=true;
	rectangle(color,r1,Scalar(0,125,125),2);}
	r1=Rect(C1.x+(width/24)+width/6,C1.y-(width/24)-(width/6),width/12,width/12);//wt
			cnz=countNonZero(threshInv(r1));
	if (cnz>(0.8*(r1.width*r1.height)))
	{	s3=true;
	rectangle(color,r1,Scalar(0,125,125),-1);}
	r1=Rect(C1.x-(width/24)-(width/12),C1.y-(width/12)-(width/6)-(width/24),width/12,width/12);//blk
			cnz=countNonZero(imgThresh(r1));
	if (cnz>(0.8*(r1.width*r1.height)))
	{	s4=true;
	rectangle(color,r1,Scalar(0,125,125),2);
	}
 r1=Rect(C1.x+(width/6),C1.y+2*(width/6),width/12,width/12);//wt
 			cnz=countNonZero(threshInv(r1));
	if (cnz>(0.8*(r1.width*r1.height)))
		{s5=true;
	rectangle(color,r1,Scalar(0,125,125),-1);
	}
	if (s1 && s2 && s3 && s4 && s5)
		target=rectsValid[j];
	}
	
	rectangle(color,target.boundingRect() ,Scalar(255,255,0),1);

	
	Mat crop = threshInv(target.boundingRect());
	Mat cropC;
	cvtColor(crop,cropC,COLOR_GRAY2BGR);
	findContours(crop, contours, hierarchy, RETR_CCOMP, CHAIN_APPROX_SIMPLE);
	int width=min(target.size.width,target.size.height);
	Point lowC=Point(5000,5000);
	Point highC=Point(0,0);
	if (contours.size()==0)
	{
	return bitmap;
	}
	for (int i = 0; i < contours.size(); i++)
	{
		//drawContours(cropC, contours, i, Scalar(255,0,0), 2, 8, hierarchy);
		
		
		rect=minAreaRect(contours[i]);
		Rect rectR=boundingRect(contours[i]);
		//cout<<"rect again h:"<<rect.size.height <<" rect again w: "<<rect.size.width<<endl;
		//cout<<"w/h::"<<rect.size.width /rect.size.height<<endl;
		//cout<<"h/w::"<<rect.size.height/rect.size.width <<endl;
		//0.95
		if (((rect.size.width /rect.size.height)>0.90 && (rect.size.width /rect.size.height)<1.2 )||((rect.size.height/rect.size.width )>0.90 && ( rect.size.height/rect.size.width)<1.2 ) )
		{//cout<<"rect again _______h:"<<rect.size.height <<" rect again________ w: "<<rect.size.width<<endl;
			//cout<<"lower B::"<<((2*width/6)+(width/6)*0.2)<<endl;
	//cout<<"upper B::"<<((2*width/6)-(width/6)*0.2) <<endl;
		if ((max(rect.size.height,rect.size.width )>((2*width/6)-(width/6)*0.3)) && (max(rect.size.height,rect.size.width )<((2*width/6)+(width/6)*0.3) ))
		{drawContours(cropC, contours, i, Scalar(0,255,0), 2, 8, hierarchy);
		
		if (rect.center.y<lowC.y)
		{
			lowC=Point(rectR.x,rectR.y);

		}
			if (rect.center.y>highC.y)
		{	
			highC=Point(rectR.x,rectR.y) ;
			
		}
		
		
		
		}
		}
	
		
	
		//waitKey();

		
	}
		//circle(cropC,lowC,4,Scalar(0,255,0),2);
		//circle(cropC,highC,4,Scalar(0,255,0),2);
		cout<<"lowC::"<<lowC<<endl;
		cout<<"highC::"<<highC<<endl;
		float dist=distancePoint(lowC,highC);
		 mmPerPixTarg=45/dist;
		cout<<"mmperPixTarg:;"<<mmPerPixTarg<<endl;
		
		circle(color,Point(lowC.x + target.boundingRect().x,lowC.y + target.boundingRect().y),8,Scalar(0,255,0),-1);
		circle(color,Point(highC.x+target.boundingRect().x,highC.y+target.boundingRect().y),8,Scalar(0,255,0),-1);
		imwrite("imageCalib.bmp",color);
		//----------------rotate back
//		 rot = getRotationMatrix2D(Point(imageIn.cols/2,imageIn.rows/2), -90, 1.0);
//	 bbox = RotatedRect(Point(imageIn.cols/2,imageIn.rows/2), imageIn.size(), -90).boundingRect();
//
//// adjust transformation matrix
//rot.at<double>(0, 2) += bbox.width / 2.0 - imageIn.cols/2;
//rot.at<double>(1, 2) += bbox.height / 2.0 - imageIn.rows/2;
 

//warpAffine(color, color, rot, bbox.size());
//------------------------------------------
//------------------------------------------
	//imshow("cropC",cropC);		
//	imshow("color",color );
	//waitKey();
//	return 1;
if ( mmPerPixTarg==0)
{
	tagetFoundProp=0;
}
else
{
	tagetFoundProp=1;
	mmPerPixProp=mmPerPixTarg;
}
		//System::Drawing::Bitmap^ dst = MatToBitmap(color);
		//imageIn
		resize(color, imageIn, imageIn.size());
	return bitmap;
	}
	catch (exception exx)
	{
		tagetFoundProp=0;
	return bitmap;
	
	}

}
int  algorithmLib::Class1::initialPoint(float x1,float y1,float x2,float y2)
{
	//P1=Point2f(x1*3.8,y1*3.8);
	//P2=Point2f(x2*3.8,y2*3.8);

		P1=Point2f(x1,y1);
	P2=Point2f(x2,y2);
	//	P1=Point2f(2343.2,962.8);
	//P2=Point2f(2392.5,1020.8);
	
	
	return 1;
}
System::Drawing::Bitmap^ algorithmLib::Class1::copy_BaseImage(System::Drawing::Bitmap^ bitmap,float x1,float y1,float x2,float y2)
{
	baseColorSel= BitmapToMat(bitmap).clone();
	P1=Point2f(x1,y1);
	P2=Point2f(x2,y2);
	Point PL=P1;
	Point PR=P2;
	imgBGR=baseColorSel.clone();
	cvtColor(imgBGR,imgHSV,COLOR_BGR2HSV);
	//Rect ROI = Rect(Point(744,1529), Point(2559,1956));
	colorVectorTop.clear();
	colorVectorBott.clear();
	colorVectorDumbell.clear();
	colorVectorBackGr.clear();
	Rect ROI = Rect(Point(PL.x,PL.y), Point(PR.x,PR.y));
	

System::Drawing::Bitmap^ dst = MatToBitmap(baseColorSel(ROI).clone());
	return dst;

	
}


System::Drawing::Bitmap^ algorithmLib::Class1::clickResp(float x1,float y1,float x2,float y2,float mouseLocX,float mouseLocY)
{
	
	
	P1=Point2f(x1,y1);
	P2=Point2f(x2,y2);
	Point PL=P1;
	Point PR=P2;
	//Rect ROI = Rect(Point(744,1529), Point(2559,1956));
	
	Rect ROI = Rect(Point(PL.x,PL.y), Point(PR.x,PR.y));
	//2413,1028
	cout<<"P1::"<<PL.x<<"__"<<PL.y<<endl;
	cout<<"P2::"<<PR.x<<"__"<<PR.y<<endl;
	cout<<"rect::"<<ROI<<endl;
// imgROI = imageIn(ROI).clone();
 circle(baseColorSel(ROI),Point(mouseLocX,mouseLocY),15,Scalar(0,255,0),-1);
 Mat z;
resize(baseColorSel,z,Size(),0.2,0.2);
imshow("color",z);
System::Drawing::Bitmap^ dst = MatToBitmap(baseColorSel(ROI).clone());
	return dst;


}
System::Drawing::Bitmap^ algorithmLib::Class1::detectPointsHSV(System::Drawing::Bitmap^ bitmap,int mode) 
{//return bitmap;

	//machineTypeProp  0 ---bottom fix   1 --- top fix
	// sampleMarkContrastProp  0 black mark 1 white mark

	int defCount = 0;
	 image00 = BitmapToMat(bitmap).clone();
	// System::Drawing::Bitmap^ dst= MatToBitmap(image00);
	//return dst;
	//Mat image00 = imread(path);
	cvtColor(image00,imgHSV,COLOR_BGR2HSV);



	cvtColor(image00, imageGrey, COLOR_BGR2GRAY);
	//Rect ROI = Rect(Point(744,1529), Point(2559,1956));
	
	Rect ROI = Rect(Point(P1.x,P1.y), Point(P2.x,P2.y));
	//2413,1028
	cout<<"P1::"<<P1.x<<"__"<<P1.y<<endl;
	cout<<"P2::"<<P2.x<<"__"<<P2.y<<endl;
	cout<<"rect::"<<ROI<<endl;
 imgROI = imageGrey(ROI).clone();
 Mat imgHSVcrop=imgHSV(ROI);
 
 
 	maskImg=Mat::zeros(imgHSVcrop.size(),CV_8UC1);
 
	maskImg=processColorVectorMask(imgHSVcrop,maskImg,0);
	maskImg=processColorVectorMask(imgHSVcrop,maskImg,1);
	//maskImg=processColorVectorMask(imgHSVcrop,maskImg,2);
	//cout << "findCont OK" << endl;
	//cvtColor(imgROI, draw, COLOR_GRAY2BGR);


		draw=image00(ROI).clone();	
 /*if (sampleMarkContrast==1)
	{
	threshold(imgROI, imgROI, 180, 255, CV_THRESH_BINARY);
	}
	else
	{
		threshold(imgROI, imgROI, 180, 255, CV_THRESH_BINARY_INV);
	}*/

	Morph(maskImg,maskImg,MORPH_DILATE,MORPH_ELLIPSE,2);
		Morph(maskImg,maskImg,MORPH_ERODE,MORPH_ELLIPSE,1);
	//cout << "threshold OK" << endl;
	vector< vector<Point> > contours;
	vector<Vec4i> hierarchy;
	Mat imgConts = maskImg.clone();
	//cout << "clone OK" << endl;
	findContours(imgConts, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_NONE);
	Point center1, center2;
	if (contours.size()<2)
	{
		distanceProp=0;
	return bitmap;
	}

	Rect bRect;
	bool state = false;
	for (int i = 0; i < (int)contours.size(); i++) {
		//approxPolyDP(Mat(contours[i]), contours_poly[i], 3, true);

		if (contourArea(contours[i])>10 && contourArea(contours[i])<5000)
		{
			//cout << "inside Loop" << endl;
			drawContours(draw, contours, i, Scalar(0, 0, 255), -1);
			bRect = boundingRect(contours[i]);
			if (state == false){
				center1 = Point(bRect.x + bRect.width / 2, bRect.y + bRect.height / 2);
				state = true;
			}
			else
				center2 = Point(bRect.x + bRect.width / 2, bRect.y + bRect.height / 2);
		}
	}
	//circle(draw, center1, 6, Scalar(0, 255, 0), 2);
	//circle(draw, center2, 6, Scalar(0, 255, 0), 2);
	line(draw,center1,center2,Scalar(0,125,125),2);
	Point2f ptUp,ptDn;
if (center1.y>center2.y)
{
	circle(draw, center1, 6, Scalar(180,0,0), 2);
circle(draw, center2, 6, Scalar(0, 180, 0), 2);
ptUp= Point2f(center2.x,center2.y);
ptDn=Point2f(center1.x,center1.y);
}
else
{
	circle(draw, center1, 6, Scalar(0, 180, 0), 2);
circle(draw, center2, 6, Scalar(180, 0, 0), 2);
 ptUp = Point2f(center1.x,center1.y);
 ptDn= Point2f(center2.x,center2.y);
}
if (machineTypeProp==0)
{
	if(P1.y+ ptUp.y -40>0)
	{
	P1=Point2f( P1.x,P1.y+ ptUp.y -40);
	}
}
else
{   cout<<"machine type 1"<<endl;
	if(P1.y+ ptDn.y +40<image00.rows )
	{
	P2=Point2f( P2.x,P1.y+ptDn.y +40);
	 cout<<"P2 new val"<<P2<<endl;
	}
}
	float dist = distancePoint(center1, center2);
	distanceProp = dist*mmPerPixProp;
	//cout << "distance::" << distanceProp << endl;

	//draw.copyTo(image00(ROI));
	//putText(draw, cv::format("Distance= %3.3f", distanceProp), Point(50, 40), CV_FONT_HERSHEY_PLAIN, 4, Scalar(0, 0, 255), 4);
	//Mat res;
	//resize(image00, res, Size(), 0.3, 0.3);

	//cout << "contours Found::" << contours.size() << endl;
	//imshow("thresh", imgROI);
	//waitKey(5);
	//imshow("conts", draw);

	//Mat imageGrey;
	//cvtColor(image00, imageGrey, COLOR_BGR2GRAY);
	//Rect ROI = Rect(Point(1597, 29), Point(1913, 1973));
	//Mat imgROI = imageGrey(ROI).clone();
	//threshold(imgROI, imgROI, 100, 255, CV_THRESH_BINARY);
	//vector< vector<Point> > contours;
	//vector<Vec4i> hierarchy;
	//findContours(imageGrey, contours, hierarchy, RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);

	//for (int i = 0; i < (int)contours.size(); i++) {
	//	//approxPolyDP(Mat(contours[i]), contours_poly[i], 3, true);
	//	drawContours(image00, contours, i, Scalar(0, 0, 255), 1);
	//		if (contourArea(contours[i])>2 && contourArea(contours[i])<50)
	//		{
	//			drawContours(image00, contours, i, Scalar(0, 0, 255), 1);
	//		}
	//}
	//	// cout<<"area::"<<contourArea(contours[i])<<endl;
	//	/*if (contourArea(contours[i])>2000 && contourArea(contours[i])<1600000000)
	//	{

	//	}*/
	//}
	//if (image0.channels()<3)
	//cvtColor(imageGrey, imageGrey, COLOR_GRAY2BGR);
	//// the code you wish to time goes here

	//cout << "imageSize::" << image00.size() << "___channels::" << image00.channels()<<endl;
	//if (defCount>0){
	//	putText(imageGrey, "NG", Point(50, 40), CV_FONT_HERSHEY_PLAIN, 4, Scalar(0, 0, 255), 4);
	//	outCodeProp = 0;
	//}
	//else{
	//	putText(imageGrey, "OK", Point(50, 40), CV_FONT_HERSHEY_PLAIN, 4, Scalar(0, 255, 0), 4);
	//	outCodeProp = 1;
	//}

	//----------------------------------------------------------------------
	//waitKey();
	// 	imshow("ROI",image0);


	System::Drawing::Bitmap^ dst = MatToBitmap(draw);
	return dst;
	//return bitmap ;
} //fuel

System::Drawing::Bitmap^ algorithmLib::Class1::detectPoints(System::Drawing::Bitmap^ bitmap,int mode) 
{//return bitmap;

	//machineTypeProp  0 ---bottom fix   1 --- top fix
	// sampleMarkContrastProp  0 black mark 1 white mark

	int defCount = 0;
	 image00 = BitmapToMat(bitmap).clone();
	// System::Drawing::Bitmap^ dst= MatToBitmap(image00);
	//return dst;
	//Mat image00 = imread(path);
	
	cvtColor(image00, imageGrey, COLOR_BGR2GRAY);
	//Rect ROI = Rect(Point(744,1529), Point(2559,1956));
	
	Rect ROI = Rect(Point(P1.x,P1.y), Point(P2.x,P2.y));
	//2413,1028
	cout<<"P1::"<<P1.x<<"__"<<P1.y<<endl;
	cout<<"P2::"<<P2.x<<"__"<<P2.y<<endl;
	cout<<"rect::"<<ROI<<endl;
 imgROI = imageGrey(ROI).clone();
 	
	//cout << "findCont OK" << endl;
	//cvtColor(imgROI, draw, COLOR_GRAY2BGR);
		draw=image00(ROI).clone();	
 if (sampleMarkContrast==1)
	{
	threshold(imgROI, imgROI, 180, 255, THRESH_BINARY);
	}
	else
	{
		threshold(imgROI, imgROI, 180, 255, THRESH_BINARY_INV);
	}

	Morph(imgROI,imgROI,MORPH_DILATE,MORPH_ELLIPSE,2);
	//cout << "threshold OK" << endl;
	vector< vector<Point> > contours;
	vector<Vec4i> hierarchy;
	Mat imgConts = imgROI.clone();
	//cout << "clone OK" << endl;
	findContours(imgConts, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_NONE);
	Point center1, center2;
	if (contours.size()<2)
	{
		distanceProp=0;
	return bitmap;
	}

	Rect bRect;
	bool state = false;
	for (int i = 0; i < (int)contours.size(); i++) {
		//approxPolyDP(Mat(contours[i]), contours_poly[i], 3, true);

		if (contourArea(contours[i])>10 && contourArea(contours[i])<5000)
		{
			//cout << "inside Loop" << endl;
			drawContours(draw, contours, i, Scalar(0, 0, 255), -1);
			bRect = boundingRect(contours[i]);
			if (state == false){
				center1 = Point(bRect.x + bRect.width / 2, bRect.y + bRect.height / 2);
				state = true;
			}
			else
				center2 = Point(bRect.x + bRect.width / 2, bRect.y + bRect.height / 2);
		}
	}
	//circle(draw, center1, 6, Scalar(0, 255, 0), 2);
	//circle(draw, center2, 6, Scalar(0, 255, 0), 2);
	line(draw,center1,center2,Scalar(0,125,125),2);
	Point2f ptUp,ptDn;
if (center1.y>center2.y)
{
	circle(draw, center1, 6, Scalar(180,0,0), 2);
circle(draw, center2, 6, Scalar(0, 180, 0), 2);
ptUp= Point2f(center2.x,center2.y);
ptDn=Point2f(center1.x,center1.y);
}
else
{
	circle(draw, center1, 6, Scalar(0, 180, 0), 2);
circle(draw, center2, 6, Scalar(180, 0, 0), 2);
 ptUp = Point2f(center1.x,center1.y);
 ptDn= Point2f(center2.x,center2.y);
}
if (machineTypeProp==0)
{
	if(P1.y+ ptUp.y -20>0)
	{
	P1=Point2f( P1.x,P1.y+ ptUp.y -20);
	}
}
else
{   cout<<"machine type 1"<<endl;
	if(P1.y+ ptDn.y +20<image00.rows )
	{
	P2=Point2f( P2.x,P1.y+ptDn.y +20);
	 cout<<"P2 new val"<<P2<<endl;
	}
}
	float dist = distancePoint(center1, center2);
	distanceProp = dist*mmPerPixProp;
	//cout << "distance::" << distanceProp << endl;

	//draw.copyTo(image00(ROI));
	//putText(draw, cv::format("Distance= %3.3f", distanceProp), Point(50, 40), CV_FONT_HERSHEY_PLAIN, 4, Scalar(0, 0, 255), 4);
	//Mat res;
	//resize(image00, res, Size(), 0.3, 0.3);

	//cout << "contours Found::" << contours.size() << endl;
	imshow("thresh", imgROI);
	waitKey(5);
	//imshow("conts", draw);

	//Mat imageGrey;
	//cvtColor(image00, imageGrey, COLOR_BGR2GRAY);
	//Rect ROI = Rect(Point(1597, 29), Point(1913, 1973));
	//Mat imgROI = imageGrey(ROI).clone();
	//threshold(imgROI, imgROI, 100, 255, CV_THRESH_BINARY);
	//vector< vector<Point> > contours;
	//vector<Vec4i> hierarchy;
	//findContours(imageGrey, contours, hierarchy, RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);

	//for (int i = 0; i < (int)contours.size(); i++) {
	//	//approxPolyDP(Mat(contours[i]), contours_poly[i], 3, true);
	//	drawContours(image00, contours, i, Scalar(0, 0, 255), 1);
	//		if (contourArea(contours[i])>2 && contourArea(contours[i])<50)
	//		{
	//			drawContours(image00, contours, i, Scalar(0, 0, 255), 1);
	//		}
	//}
	//	// cout<<"area::"<<contourArea(contours[i])<<endl;
	//	/*if (contourArea(contours[i])>2000 && contourArea(contours[i])<1600000000)
	//	{

	//	}*/
	//}
	//if (image0.channels()<3)
	//cvtColor(imageGrey, imageGrey, COLOR_GRAY2BGR);
	//// the code you wish to time goes here

	//cout << "imageSize::" << image00.size() << "___channels::" << image00.channels()<<endl;
	//if (defCount>0){
	//	putText(imageGrey, "NG", Point(50, 40), CV_FONT_HERSHEY_PLAIN, 4, Scalar(0, 0, 255), 4);
	//	outCodeProp = 0;
	//}
	//else{
	//	putText(imageGrey, "OK", Point(50, 40), CV_FONT_HERSHEY_PLAIN, 4, Scalar(0, 255, 0), 4);
	//	outCodeProp = 1;
	//}

	//----------------------------------------------------------------------
	//waitKey();
	// 	imshow("ROI",image0);


	System::Drawing::Bitmap^ dst = MatToBitmap(draw);
	return dst;
	//return bitmap ;
} //fuel


//new markers
vector <Point> markerPt_v = { Point(0,0),Point(0,0), Point(0,0), Point(0,0) };
Rect r_init = Rect(0, 0, 10, 10);
Rect rs_init = Rect(0, 0, 20, 20);
vector <Rect> templR_v = { r_init ,r_init ,r_init,r_init };
vector <Rect> searchR_v = { rs_init ,rs_init ,rs_init ,rs_init };
vector <Mat> markerTmpl_v;
Point M1P1 = Point(579, 1371);
Point M1P2 = Point(578, 1561);
Rect r_M11, r_M12;
Rect rs_M11, rs_M12;
Mat M1P1Template;
Mat M1P2Template;
Mat M2P1Template;
Mat M2P2Template;
RNG rng(12345);
Mat baseImage;
int algorithmLib::Class1::setMarker(int x, int y, int markerCode)
{
	if (markerCode < 4)
	{markerPt_v[markerCode]= Point(x, y);
	cout << "marker_" << markerCode << "::" << markerPt_v[markerCode] << endl;
	}
	return 1;
}
int algorithmLib::Class1::setMarkerP1(int x, int y)
{
	M1P1 = Point(x, y);
	//int markerSizeb2 = markerSizeProp / 2;
	//r_M11 = Rect(M1P1.x - markerSizeb2, M1P1.y - markerSizeb2, markerSizeProp, markerSizeProp);
	return 1;
}

int algorithmLib::Class1::setMarkerP2(int x, int y)
{
	M1P2 = Point(x, y);
	//int markerSizeb2 = markerSizeProp / 2;
	//r_M12 = Rect(M1P2.x - markerSizeb2, M1P2.y - markerSizeb2, markerSizeProp, markerSizeProp);

	return 1;
}
Mat imageIn;

int scale = 1;
int templateScale = 4.0;
int algorithmLib::Class1::setBaseImage(System::Drawing::Bitmap^ rawImg)
{
	baseImage = BitmapToMat(rawImg).clone();
	return 1;
}
Mat imageBase;
Rect imageROI = Rect(0, 0, 100, 100);
Rect getSearchArea(Rect templateRect, Size searchImgSz, int marginPc)
{
	Rect searchRect = templateRect;
	float marginePcFlt = marginPc / 100.0;
	if ((templateRect.x - (marginePcFlt * (float)templateRect.width)) >= 0)
	{
		searchRect.x = templateRect.x - (marginePcFlt * (float)templateRect.width);
	}
	else
	{
		searchRect.x = 0;
	}
	if ((templateRect.y - (marginePcFlt * (float)templateRect.height)) >= 0)
	{
		searchRect.y = templateRect.y - (marginePcFlt * (float)templateRect.height);
	}
	else
	{
		searchRect.y = 0;
	}
	if (((templateRect.x + templateRect.width + (marginePcFlt * (float)templateRect.width)) < searchImgSz.width) )
	{
		searchRect = Rect(Point(searchRect.x, searchRect.y), Point((templateRect.x + templateRect.width + (marginePcFlt * (float)templateRect.width)), searchRect.y + searchRect.height-1));
	}
	else
	{
		searchRect = Rect(Point(searchRect.x, searchRect.y), Point(searchImgSz.width - 1, searchRect.y+ searchRect.height-1));

	}
	if ( ((templateRect.y + templateRect.height + (marginePcFlt * (float)templateRect.height)) < searchImgSz.height))
	{
		searchRect = Rect(Point(searchRect.x, searchRect.y), Point(searchRect.x + searchRect.width - 1, (templateRect.y + templateRect.height + (marginePcFlt * (float)templateRect.height))));
	}
	else
	{
		searchRect = Rect(Point(searchRect.x, searchRect.y), Point(searchRect.x+ searchRect.width-1, searchImgSz.height - 1));

	}
	cout << "searchRect :" << searchRect << endl;
	return searchRect;
}

int algorithmLib::Class1::cropTemplatesBoth(System::Drawing::Bitmap^ rawImg, int code)
{
	imageROI = Rect(0, 0, 100, 100);
	imageIn = BitmapToMat(rawImg).clone();
	Size imageInWoScale = imageIn.size();
	imageROI.height = imageIn.rows;
	if (scale > 1)
		resize(imageIn, imageIn, Size(), scale, scale);
	imageBase = imageIn.clone();
	//cout << "imageSize::" << imageIn.size() << endl;
	//imshow("imageIn", imageIn);
	//waitKey();
	int markerSizeb2 = markerSizeProp / 2;

	if (code == 0) //sample crops
	{
		templR_v[0] = Rect(scale * (markerPt_v[0].x - (W_templS/2)), scale * (markerPt_v[0].y - (H_templS/2)), scale * W_templS, scale * H_templS);
		templR_v[1] = Rect(scale * (markerPt_v[1].x - (W_templS / 2)), scale * (markerPt_v[1].y - (H_templS / 2)), scale * W_templS, scale * H_templS);

		searchR_v[0] = Rect(scale * (markerPt_v[0].x - (W_tmpl_searchArF/2.0) * W_templS), scale * (markerPt_v[0].y - (H_tmpl_searchArF /2.0) * H_templS), scale * W_tmpl_searchArF * W_templS, scale * H_tmpl_searchArF * H_templS);
		searchR_v[1] = Rect(scale * (markerPt_v[1].x - (W_tmpl_searchArF/2.0) * W_templS), scale * (markerPt_v[1].y - (H_tmpl_searchArF/2.0 ) * H_templS), scale * W_tmpl_searchArF * W_templS, scale * H_tmpl_searchArF * H_templS);
		
		//markerTmpl_v[0] = imageIn(templR_v[0]).clone();

		//markerTmpl_v[1] = imageIn(templR_v[1]).clone();

		vector <Point> pts;
		pts.push_back(markerPt_v[0]);
		pts.push_back(markerPt_v[1]);
		Rect br=boundingRect(pts);

		if ((br.x -(4* W_templS)) > 0)
		{
			imageROI.x = br.x -(4* W_templS);
		}
		else
		{
			imageROI.x = 0;
		}
		if ((br.x + br.width +(4* W_templS)) < imageInWoScale.width)
		{
			imageROI = Rect(Point(imageROI.x, 0), Point(br.x + br.width + (4 * W_templS), imageInWoScale.height - 1));
		}
		else
		{
			imageROI = Rect(Point(imageROI.x, 0), Point(imageInWoScale.width-1, imageInWoScale.height - 1));
		
		}



		M1P1Template = imageIn(templR_v[0]).clone();
		M1P2Template = imageIn(templR_v[1]).clone();

	/*	imshow("M1_1", M1P1Template);
		imshow("M1_2", M1P2Template);*/
		//Rect searchAr = getSearchArea(br, imageIn.size(), 50);
		cout << "templateRect:" << br << endl;
		cout << "imageIn size:" << imageIn.size() << endl;
		//rectangle(imageIn, searchAr, Scalar(0, 255, 0), 8);
		rectangle(imageIn, imageROI, Scalar(0, 255, 0), 8);
		rectangle(imageIn, br, Scalar(0, 0, 255), 8);
		Mat res;
		//resize(imageIn, res, Size(), 0.3, 0.3);
		//imshow("Samp", res);
		Mat cropSR = imageIn(imageROI);
		//resize(cropSR, res, Size(), 0.3, 0.3);
		//imshow("SampICr", res);

	}
	else if (code == 1) //grip crops
	{
		templR_v[2] = Rect(scale * (markerPt_v[2].x - (W_templG/2)), scale * (markerPt_v[2].y - (H_templG / 2)), scale * W_templG, scale * H_templG);
		templR_v[3] = Rect(scale * (markerPt_v[3].x - (W_templG / 2)), scale * (markerPt_v[3].y - (H_templG / 2)), scale * W_templG, scale * H_templG);

		searchR_v[2] = Rect(scale * (markerPt_v[2].x - (W_tmpl_searchArF/2.0) * W_templG), scale * (markerPt_v[2].y - (H_tmpl_searchArF/2.0) * H_templG), scale * W_tmpl_searchArF * W_templG, scale * H_tmpl_searchArF * H_templG);
		searchR_v[3] = Rect(scale * (markerPt_v[3].x - (W_tmpl_searchArF/2.0) * W_templG), scale * (markerPt_v[3].y -(H_tmpl_searchArF/2.0)* H_templG), scale * W_tmpl_searchArF * W_templG, scale * H_tmpl_searchArF * H_templG);
		//markerTmpl_v[2] = imageIn(templR_v[2]).clone();
		//markerTmpl_v[3] = imageIn(templR_v[3]).clone();
	
		
		Rect br = boundingRect(markerPt_v);

		if ((br.x - (3 * searchR_v[3].width)) > 0)
		{
			imageROI.x = br.x - (3 * searchR_v[3].width);
		}
		else
		{
			imageROI.x = 0;
		}
		if ((br.x + br.width + (3 * searchR_v[3].width)) < imageInWoScale.width)
		{
			imageROI = Rect(Point(imageROI.x, 0), Point(br.x + br.width + (3 * searchR_v[3].width), imageInWoScale.height - 1));
		}
		else
		{
			imageROI = Rect(Point(imageROI.x, 0), Point(imageInWoScale.width - 1, imageInWoScale.height - 1));
	
		}
	/*	rectangle(imageIn, imageROI, Scalar(0, 255, 0), 8);
		rectangle(imageIn, br, Scalar(0,0, 255), 8);
		Mat res;
		resize(imageIn, res, Size(), 0.3, 0.3);
		imshow("both", res);
		Mat cropSR = imageIn(imageROI);
		resize(cropSR, res, Size(), 0.3, 0.3);
		imshow("bothCr", res);*/



		//M1P1Template = imageIn(templR_v[0]).clone();
		//M1P2Template = imageIn(templR_v[1]).clone();

		M2P1Template = imageIn(templR_v[2]).clone();
		M2P2Template = imageIn(templR_v[3]).clone();
		//imshow("M2_1", M2P1Template);
		//imshow("M2_2", M2P2Template);


	}
	cout << "imageSize::" << imageIn.size() << endl;



	


	/*rectangle(imageIn, imageROI, Scalar(0, 255, 0), 8);
	Mat res;
	resize(imageIn, res, Size(), 0.3, 0.3);
	imshow("imageROI", res);
	Mat cropSR = imageIn(imageROI);
	resize(cropSR, res, Size(), 0.3, 0.3);
	imshow("imageROICr", res);*/
	//waitKey(5);

	//waitKey(5);
	return 1;
}
int algorithmLib::Class1::updateProcessROI()
{
	templR_v[0].x = templR_v[0].x- (imageROI.x * scale);
templR_v[1].x = templR_v[1].x- (imageROI.x * scale) ;
searchR_v[0].x =searchR_v[0].x - (imageROI.x * scale) ;
searchR_v[1].x = searchR_v[1].x- (imageROI.x * scale);


	templR_v[2].x = templR_v[2].x - (imageROI.x * scale);
	templR_v[3].x = templR_v[3].x - (imageROI.x * scale);
	searchR_v[2].x = searchR_v[2].x - (imageROI.x * scale);
	searchR_v[3].x = searchR_v[3].x - (imageROI.x * scale);
	return 1;
}

vector <int> updateTempl = { false, false,  false ,false };
vector<int> templateLost = { false, false,  false ,false };
int algorithmLib::Class1::cropTemplates(System::Drawing::Bitmap^ rawImg)
{
	
	imageIn = BitmapToMat(rawImg).clone();
	if (scale > 1)
		resize(imageIn, imageIn, Size(), scale, scale);
	imageBase = imageIn.clone();
	//imshow("imageIn", imageIn);
	//waitKey();
	int markerSizeb2 = markerSizeProp / 2;
	r_M11 = Rect(scale * (M1P1.x - markerSizeb2), scale * (M1P1.y - markerSizeb2), scale * markerSizeProp, scale * markerSizeProp);
	r_M12 = Rect(scale * (M1P2.x - markerSizeb2), scale * (M1P2.y - markerSizeb2), scale * markerSizeProp, scale * markerSizeProp);

	//rs_M11 = Rect(M1P1.x - 1.5 * markerSizeProp, M1P1.y - 2 * markerSizeProp, 3 * markerSizeProp, 5 * markerSizeProp);
	//rs_M12 = Rect(M1P2.x - 1.5 * markerSizeProp, M1P2.y - 2 * markerSizeProp, 3 * markerSizeProp, 5 * markerSizeProp);

	rs_M11 = Rect(scale * (M1P1.x - 0.75 * markerSizeProp), scale * (M1P1.y - 2.5 * markerSizeProp), scale * 1.5 * markerSizeProp, scale * 5 * markerSizeProp);
	rs_M12 = Rect(scale * (M1P2.x - 0.75 * markerSizeProp), scale * (M1P2.y - 2.5 * markerSizeProp), scale * 1.5 * markerSizeProp, scale * 5 * markerSizeProp);

	/*	rs_M11 = Rect(M1P1.x - 1 * markerSizeProp, M1P1.y - 0.5* markerSizeProp,2* markerSizeProp, 2 * markerSizeProp);
		rs_M12 = Rect(M1P2.x - 1 * markerSizeProp, M1P2.y - 0.5 * markerSizeProp,2 * markerSizeProp, 2 * markerSizeProp);*/
		//cvtColor(imageIn, imageIn, COLOR_BGR2GRAY);
		//imageIn = getSobel(imageIn);

	M1P1Template = imageIn(r_M11).clone();
	M1P2Template = imageIn(r_M12).clone();



	/*	rectangle(imageIn, r_M11, Scalar(0, 255, 0), 2);
		rectangle(imageIn, r_M12, Scalar(0, 255, 0), 2);

		rectangle(imageIn, rs_M11, Scalar(255, 0, 0), 1, 4);
		rectangle(imageIn, rs_M12, Scalar(255, 0, 0), 1, 4);*/

		//Console.WriteLine("imageSize {0}___x___{1}", imageIn.cols, imageIn.rows);
	//cout << "imageSize::" << imageIn.size() << endl;


	//imshow("M1", M1P1Template);
	//imshow("M2", M1P2Template);
	//waitKey(5);
	return 1;
}
Mat getSobel(Mat img)
{
	Mat grad_x, grad_y;
	Mat abs_grad_x, abs_grad_y;
	Mat ret;
	int ddepth = CV_16S;
	int scale = 4;
	int delta = 0;
	/// Gradient X
	//Scharr( src_gray, grad_x, ddepth, 1, 0, scale, delta, BORDER_DEFAULT );
	Sobel(img, grad_x, ddepth, 1, 0, 3, scale, delta, BORDER_DEFAULT);
	convertScaleAbs(grad_x, abs_grad_x);

	/// Gradient Y
	//Scharr( src_gray, grad_y, ddepth, 0, 1, scale, delta, BORDER_DEFAULT );
	Sobel(img, grad_y, ddepth, 0, 1, 3, scale, delta, BORDER_DEFAULT);
	convertScaleAbs(grad_y, abs_grad_y);

	/// Total Gradient (approximate)
	addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, ret);
	return ret;
}
Mat result;
int match_method = TM_CCORR_NORMED;// 4;//4

float templateLostTh = 0.92;
float templateUpdateTh = 0.95;
float mmPpLocal=1;
int algorithmLib::Class1::updateSettings()
{
	scale = imageScaleFProp;
	templateScale = templScaleFProp;
	templateLostTh = templLostThProp;
	templateUpdateTh = templUpdateThProp;
	targetEncCircDist = circleGridC2CmmProp;
	mmPpLocal = mmPerPixProp;
	return 1;
}


int algorithmLib::Class1::resetTest()
{
	updateTempl = { false, false,  false ,false };
	templateLost = { false, false,  false ,false };
	sampleMarkerLost = 0;
	return 1;
}


Mat imageMatch(Mat img, Mat templ, Point2f* matchLocRet, int* updateTemplFlag, int* templateLostFlag,Point2f distMM)
{
	Mat img_display;
	if (templateScale > 1) {
		resize(img, img, Size(), templateScale, templateScale, INTER_AREA);
		resize(templ, templ, Size(), templateScale, templateScale, INTER_AREA);
	}
	img.copyTo(img_display);

	/// Create the result matrix
	int result_cols = img.cols - templ.cols + 1;
	int result_rows = img.rows - templ.rows + 1;

	result.create(result_rows, result_cols, CV_32FC1);

	/// Do the Matching and Normalize
	//cout << "imageAnd template channels::" << img.channels() << "templChaans : " << templ.channels() << endl;
	//Mat imgGr, templGr;
	//cvtColor(img, imgGr, COLOR_BGR2GRAY);
	//cvtColor(templ, templGr, COLOR_BGR2GRAY);
	matchTemplate(img, templ, result, match_method);
	//normalize(result, result, 0, 1, NORM_MINMAX, -1, Mat());

	/// Localizing the best match with minMaxLoc
	double minVal; double maxVal; Point minLoc; Point maxLoc;
	Point2f matchLoc;
	float correlationF;
	minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc, Mat());

	/// For SQDIFF and SQDIFF_NORMED, the best matches are lower values. For all the other methods, the higher the better

	if (match_method == TM_SQDIFF || match_method == TM_SQDIFF_NORMED)
	{
		matchLoc = minLoc;
		correlationF = minVal;

	}
	else
	{
		matchLoc = maxLoc;
		correlationF = maxVal;

	}
	//cout << "corrFactor M1::" << correlationF << endl;

	//cout << "templateLostTh "<< templateLostTh <<endl;
	//cout << "templateUpdateTh  "<< templateUpdateTh <<endl;
	cout << "templateScale "<< templateScale <<endl;

	if (correlationF < templateLostTh)
		*templateLostFlag = 1;
	if (correlationF >= templateLostTh && correlationF < templateUpdateTh)
		*updateTemplFlag = 1;

	/// Show me what you got
	//circle(img_display, matchLoc, 2, Scalar(0, 0, 255), -1);
	//rectangle(img_display, matchLoc, Point(matchLoc.x + templ.cols, matchLoc.y + templ.rows), Scalar(0, 255, 0), 1, 8, 0);
	//rectangle(result, matchLoc, Point(matchLoc.x + templ.cols, matchLoc.y + templ.rows), Scalar::all(0), 2, 8, 0);
	//cout << "loc P1_tmpl int:" << matchLoc << endl;
	matchLoc = matchLoc + Point2f(templ.cols / 2, templ.rows / 2);
	//cout << "loc P1_tmpl int++:" << matchLoc << endl;
	if (templateScale > 1)
	{
		*matchLocRet = Point2f(float(matchLoc.x) / templateScale, float(matchLoc.y) / templateScale);
		
	}
	else
	{
		*matchLocRet = matchLoc;
	}
	distMM = Point2f(float(matchLoc.x), float(matchLoc.y)) * (mmPpLocal / (float)templateScale);
	//cout << "loc P1_tmpl flt:" << *matchLocRet << endl;
	//cout << "loc P2__:" << locRetM2 << endl;
	//resize(img_display, img_display, Size(), 0.5, 0.5, INTER_AREA);
	//imshow("displayImgM1", img_display);
	//imshow("templ", templ);
	//waitKey(5);

	return result;
}
Mat imageMatchM2(Mat img, Mat templ, Point2f* matchLocRet, int* updateTemplFlag, int* templateLostFlag)
{
	Mat img_display;
	if (templateScale > 1)
	{
		resize(img, img, Size(), templateScale, templateScale, INTER_AREA);
		resize(templ, templ, Size(), templateScale, templateScale, INTER_AREA);
	}
	img.copyTo(img_display);

	/// Create the result matrix
	int result_cols = img.cols - templ.cols + 1;
	int result_rows = img.rows - templ.rows + 1;

	result.create(result_rows, result_cols, CV_32FC1);

	/// Do the Matching and Normalize
/*	Mat imgGr, templGr;
	cvtColor(img, imgGr, COLOR_BGR2GRAY);
	cvtColor(templ, templGr, COLOR_BGR2GRAY);*/
	matchTemplate(img, templ, result, match_method);
	//	matchTemplate(img, templ, result, match_method);
	//	normalize(result, result, 0, 1, NORM_MINMAX, -1, Mat());

		/// Localizing the best match with minMaxLoc
	double minVal; double maxVal; Point minLoc; Point maxLoc;
	Point2f matchLoc;
	float correlationF;
	minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc, Mat());

	/// For SQDIFF and SQDIFF_NORMED, the best matches are lower values. For all the other methods, the higher the better

	if (match_method == TM_SQDIFF || match_method == TM_SQDIFF_NORMED)
	{
		matchLoc = minLoc;
		correlationF = minVal;

	}
	else
	{
		matchLoc = maxLoc;
		correlationF = maxVal;

	}
	cout << "corrFactor M2::" << correlationF << endl;

	if (correlationF < 0.92)
		*templateLostFlag = 1;
	if (correlationF >= 0.92 && correlationF < 0.95)
		*updateTemplFlag = 1;

	/// Show me what you got
	circle(img_display, matchLoc, 2, Scalar(0, 0, 255), -1);
	rectangle(img_display, matchLoc, Point(matchLoc.x + templ.cols, matchLoc.y + templ.rows), Scalar(0, 255, 0), 1, 8, 0);
	//rectangle(result, matchLoc, Point(matchLoc.x + templ.cols, matchLoc.y + templ.rows), Scalar::all(0), 2, 8, 0);
	//*matchLocRet = (matchLoc + Point(templ.cols / 2, templ.rows / 2)) / 10;
	matchLoc = matchLoc + Point2f(templ.cols / 2, templ.rows / 2);
	if (templateScale > 1)
		*matchLocRet = Point2f(float(matchLoc.x) / templateScale, float(matchLoc.y) / templateScale);
	else
		*matchLocRet = matchLoc;
	//*matchLocRet = Point2f(float(matchLoc.x) / 10.0, float(matchLoc.y) / 10.0);
	resize(img_display, img_display, Size(), 0.5, 0.5, INTER_AREA);
	imshow("displayImgM2", img_display);
	imshow("templ", templ);
	//waitKey(5);
	return result;
}
float correlationImg(cv::Mat& image1, cv::Mat& image2)
{
	cv::Mat corr;
	cv::matchTemplate(image1, image2, corr, cv::TM_CCORR_NORMED);
	return corr.at<float>(0, 0);  // corr only has one pixel
}

Mat transMat;
float corrCoeff = 0;
Rect finalPos;
Rect roiCurr;
Mat finalTransform;
Mat imgClone;

Mat imageMatchIn, imgMatchInCpy;
Mat res;

Point2f centerM1P1curr, centerM1P2curr;
Point2f centerM2P1curr, centerM2P2curr;
Point2f mmcenterM1P1curr, mmcenterM1P2curr;
Point2f mmcenterM2P1curr, mmcenterM2P2curr;
Point2f locRetM1P1, locRetM1P2;
Point2f locRetM2P1, locRetM2P2;
Point2f mmLocM1P1, mmlocM1P2;
Point2f mmLocM2P1,mmlocM2P2;

float getAngle(Point l1s, Point l1e, Point l2s, Point l2e)
{
	float ang1 = atan2(l1s.y - l1e.y, l1s.x - l1e.x);
	float ang2 = atan2(l2s.y - l2e.y, l2s.x - l2e.x);
	float ang = (ang2 - ang1) * 180 / (3.14);
	if (ang < 0)
	{
		ang += 360;
	}
	return ang;
}
struct thicknessPt
{
	Point2f innerPt;
	Point2f outerPt;
	float distance;
};
Mat tempImg;
Mat image;
vector <thicknessPt> returnMinT;
struct formData {
	float minThick;
	float avgThick;
	float maxThick;
	float outerDia;
	float concentricity;
	//float ovality;
	vector<float> minThickData;
};
void to_json(json& j, const formData& t)
{
	j = json{ {"minThick", t.minThick}, {"avgThick", t.avgThick},{"maxtThick", t.maxThick}, {"OuterDia", t.outerDia}, {"concentricity", t.concentricity}, {"minThickData", t.minThickData} };
}

void drawContours_1(Mat& imageC, vector<Contour> contours) {

	Mat draw; //= imageC.clone();
	for (int i = 0; i < contours.size(); i++) {
		draw = imageC.clone();
		for (int j = 0; j < contours[i].points.size(); j++) {

			circle(draw, contours[i].points[j], 10, Scalar(0, 0, 123), 1);
		}


		resShow("image", draw);

	}

}
void drawContours_1(Mat& imageC, vector<Contour> contours, int num) {
	//Mat draw; //= imageC.clone();
	//for (int i = 0; i < contours.size(); i++) {
		///draw = imageC.clone();
	for (int j = 0; j < contours[num].points.size(); j++) {

		circle(imageC, contours[num].points[j], 10, Scalar(0, 0, 123), 1);
	}

	resShow("image", imageC);


	//}

}

int low = 40;
int high = 90;
double alpha = 1.0;
int mode = 1;
//
//int getMaxAreaContour(vector <vector<cv::Point>> contours) {
//	double maxArea = 0;
//	//double areaLimit = 80000;
//	int maxAreaContourNo = -1;
//	for (int j = 0; j < contours.size(); j++) {
//		double newArea = cv::contourArea(contours.at(j));
//		if (newArea > maxArea) {
//			maxArea = newArea;
//			maxAreaContourNo = j;
//		}
//	}
//	return maxAreaContourNo;
//}

//int getMaxAreaContour(vector<Contour> contours) {
//	double maxArea = 0;
//	int maxAreaContNo = -1;
//	for (int j = 0; j < contours.size(); j++) {
//		double newArea = contourArea(contours.at(j).points);
//		if (newArea > maxArea) {
//			maxArea = newArea;
//			maxAreaContNo = j;
//		}
//
//	}
//	return maxAreaContNo;
//}

int get_ovallity(vector<Contour> contour, int max, Point2f center) {
	float maxDist{};
	float minDist{};
	for (int i = 0; i < contour[max].points.size(); i++) {
		float dist = distancePoint(center, contour[max].points[i]);
		if (!i) {
			maxDist = dist;
			minDist = dist;
		}
		if (maxDist < dist) { maxDist = dist;}
		if (minDist > dist) { minDist = dist; }


	}
	return maxDist - minDist;

}

void mergeContour(Mat image, vector<Contour>& contours, int limit, vector<Contour>& myContour, vector<int>& isCopied) {

	int minDist = 20;
	//Point2f center;
	//float radius;

	if (!limit) {

		return;
	}

	for (size_t i = 0; i < contours.size() - 1; i++) {



		int count = i + 1;

		//	minEnclosingCircle(contours[i].points, center, radius);
		for (size_t j = 0; j < contours[i].points.size(); j++) {


			while (count < contours.size()) {


				for (size_t z = 0; z < contours[count].points.size(); ++z) {

					if (isCopied.at(count) == 1) {
						break;
					}

					int dist = distancePoint(contours[i].points[j], contours[count].points[z]);

					if (dist < minDist) {

						RotatedRect brect = minAreaRect(contours[i].points);

						//for (size_t x = 0; x < contours[count].points.size(); ++x) {
						//	contours[i].points.push_back(contours[count].points[x]);
						//	//isCopied.at(i) = 3;
						//}

						for (int y = 0; y < contours[i].points.size(); ++y) {
							myContour[0].points.push_back(contours[i].points[y]);
						}
						for (size_t x = 0; x < contours[count].points.size(); x++) {
							myContour[0].points.push_back(contours[count].points[x]);
						}
						//	isCopied.at(i) = 2;
						isCopied.at(count) = 1;
						break;

					}
				}
				++count;
			}
		}
	}
	--limit;

	mergeContour(image, contours, limit, myContour, isCopied);
}
float drawDiameter(float dia, float defaultVal = 10.00) {
	if (dia < 0.5) {
		return 0.5;

	}
	else if (dia < 14.00) {
		return dia / defaultVal;

	}
	else if (dia / defaultVal > 2.00 && dia / defaultVal < 4.00)
		return 2;
	else if (dia / defaultVal > 4.00 && dia / defaultVal < 6.00)
		return 3;
	else
		return 4;
}
int maxAreaContour(vector<Contour> xyz) {


	double maxArea = 0;
	//double areaLimit = 80000;
	int maxAreaContourNo = -1;
	for (int i = 0; i < xyz.size(); i++) {

		double newArea = cv::contourArea(xyz[i].points);
		if (newArea > maxArea) {
			maxArea = newArea;
			maxAreaContourNo = i;
		}

	}

	return maxAreaContourNo;
}

float calculate_ovality(vector<Contour> contours, int max, Point2f center, float OD, float mmPerixProp) {
	float ovality_1{};

	float maxDist{};
	float minDist{};
	for (int i = 0; i < contours[max].points.size(); i++) {

		float dist = distancePoint(contours[max].points[i], center);
		if (i == 0) {
			maxDist = dist;
			minDist = dist;
		}
		if (dist > maxDist) { maxDist = dist; }

		if (dist < minDist) { minDist = dist; }

	}
	maxDist *= 2;
	minDist *= 2;
	maxDist *= mmPerixProp;
	minDist *= mmPerixProp;
	cout << "max Dist " << maxDist * 0.0188627 << endl;
	cout << "Min Dist " << minDist * 0.0188627 << endl;
	cout << "OD " << OD << endl;
	ovality_1 = ((maxDist - minDist) / OD) * 100;
	return ovality_1;
}
void circleDivision(vector<vector<Point2f>>& circlePoints, Contour& usableUniqueConts, vector<Point2f> centerPoint, float minDist) {
	minDist += 5;

	cout << "center Point size" << centerPoint.size() << endl;
	cout << "usable Unique Conts" << usableUniqueConts.points.size() << endl;

	for (size_t i = 0; i < centerPoint.size(); i++) {

		for (size_t y = 0; y < usableUniqueConts.points.size(); ++y) {
			float dist = distancePoint(centerPoint[i], usableUniqueConts.points[y]);
			if (dist < minDist) {

				circlePoints[i].push_back(usableUniqueConts.points[y]);
			}
		}

	}
}

void sectorDivision(Point2f center, const vector<Point2f>& circlePoints, vector<vector<Point2f>>& sectorInner, float dist, vector<vector<Point2f>>& sectorsOuter, const  Contour& contOuter, Point extremePoint) {
	//cout << "SectorInner size " << sectorInner.size() << endl;

	for (size_t i = 0; i < circlePoints.size(); i++) {
		int sector{};
		float curAngle = getAngle(center, extremePoint, center, circlePoints.at(i));

		if (curAngle >= 315 || curAngle < 45)
			sector = 0;
		else
			sector = (int)(((curAngle - 45) / 90) + 1);

		sectorInner[sector].push_back(circlePoints[i]);
	}
	for (size_t y = 0; y < contOuter.points.size(); y++) {
		int sector{};
		float curAngle = getAngle(center, extremePoint, center, contOuter.points.at(y));
		float minDist = distancePoint(center, contOuter.points.at(y));
		if (curAngle >= 315 || curAngle < 45)
			sector = 0;
		else
			sector = (int)(((curAngle - 45) / 90) + 1);
		if (minDist < dist) {
			sectorsOuter[sector].push_back(contOuter.points[y]);
			//	std:: cout << "loop entered" << endl;
		}
	}

}
void drawSector(Mat& imageC, Point2f center, vector<vector<Point2f>>& sectorInner) {


	for (int sec = 0; sec < sectorInner.size(); sec++) {
		Scalar randomColor(

			(double)std::rand() / RAND_MAX * 225,
			(double)std::rand() / RAND_MAX * 225,
			(double)std::rand() / RAND_MAX * 225
		);
		for (int inn = 0; inn < sectorInner[sec].size(); inn++) {
			if (sec == 0)
				circle(imageC, sectorInner[sec][inn], 2, randomColor, -1);
			else if (sec == 1)
				circle(imageC, sectorInner[sec][inn], 2, randomColor, -1);
			else if (sec == 2)
				circle(imageC, sectorInner[sec][inn], 2, randomColor, -1);
			else
				circle(imageC, sectorInner[sec][inn], 2, randomColor, -1);


		}
	}
}
int ptToVector(Point2i p, Point2i q, Point2f(&returnVec)[2])
{
	//returnVec[2]----[0] element contains direction [1] contains point
	Point2f diff = p - q;
	float magV = sqrt(diff.x * diff.x + diff.y * diff.y);
	Mat img(640, 480, CV_8UC3, Scalar(0, 0, 0));
	Point2f U = Point2f(diff.x / magV, diff.y / magV);
	//	char H[100];
	//sprintf_s(H,"diff.x=%3.2f  diff.y = %3.2f",diff.x,diff.y);
	////Point2f returnVec[2];
	//	char T[100];
	//sprintf_s(T,"U.x=%3.2f  U.y = %3.2f",U.x,U.y);
	//putText(img, H, Point(20, 40), CV_FONT_HERSHEY_COMPLEX_SMALL, 1, CV_RGB(155, 0, 0), 2, 4, false);
	//putText(img, T, Point(20, 120), CV_FONT_HERSHEY_COMPLEX_SMALL, 1, CV_RGB(155, 0, 0), 2, 4, false);
	//namedWindow( "circle_and_lines", WINDOW_AUTOSIZE );
	//  imshow("circle_and_lines",img);
	//  waitKey(0);
	returnVec[0] = Point2f(U.x, U.y);
	returnVec[1] = Point2f(p.x, p.y);
	return 1;
}

Point2f  getPointAtDist(Point2f returnVec[2], int distance, int position) {
	//returnVec[2]----[0] element contains direction [1] contains point
	Point2f returnPoint;
	Point2f swappedVec;
	swappedVec.x = returnVec[0].y;
	swappedVec.y = returnVec[0].x;
	if (position == 1) //opposite direction
	{
		returnPoint = returnVec[1] - distance * returnVec[0];
	}
	else if (position == 2) //90 CW invert Y
	{
		returnPoint = returnVec[1] + distance * Point2f(swappedVec.x, -1 * swappedVec.y);
	}
	else if (position == 3) // 90  CCW invert x
	{
		returnPoint = returnVec[1] + distance * Point2f(-1 * swappedVec.x, swappedVec.y);
	}
	else //same direction
	{
		returnPoint = returnVec[1] + distance * returnVec[0];
	}
	return returnPoint;
}


int algorithmLib::Class1::getMeasurements2c(System::Drawing::Bitmap^ rawImg) {
	float ovality_1;
	int maxId{};
	Mat img = BitmapToMat(rawImg);

	if (img.empty()) {

		cout << "Image not found" << endl;
		return -1;
	}


	cv::Mat mask = cv::Mat::zeros(img.size(), img.type());

	//Define your destination image
	cv::Mat dstImage = cv::Mat::zeros(img.size(), CV_8UC1);
	dstImage = 255 - dstImage;
	cvtColor(dstImage, dstImage, COLOR_GRAY2BGR);
	cv::circle(mask, cv::Point(mask.cols / 2, mask.rows / 2), 2000, cv::Scalar(255, 255, 255), -1, 8, 0);
	img.copyTo(dstImage, mask);


	
	returnMinT.clear();

	Mat image = dstImage.clone();

	if (image.channels() > 2) {
		cvtColor(image, image, COLOR_BGR2GRAY);
	}

	Mat imageC;
	vector<vector<Point>> contours_1;
	vector<Contour> contours;
	vector<Contour> uniqueContours;
	vector<Contour> usableUniqueConts;
	vector<Vec4i> hierarchy;

	int64  t0 = getCPUTickCount();
	EdgesSubPix(image, alpha, low, high, contours, hierarchy, mode, contours_1);
	int64 t1 = getCPUTickCount();
	cvtColor(image, imageC, COLOR_GRAY2BGR);
	//findContours(image, contours_1,hierarchy,1, RETR_TREE);
	drawContours(imageC, contours_1, -1, Scalar(0, 0, 255), 2);
	cout << "Contours_1 size:: " << contours_1.size() << endl;

	vector<vector<Point2f>> conts;
	vector<RotatedRect> finalConts;
	vector<RotatedRect> finalContsWoBig;

	for (size_t i = 0; i < contours.size(); i++) {
		RotatedRect brect = minAreaRect(contours[i].points);

		float heightR = max(brect.size.height, brect.size.width);
		float widthR = min(brect.size.height, brect.size.width);

		if (i == 0) {
			finalConts.push_back(brect);
			uniqueContours.push_back(contours[i]);

		}
		bool unique = true;
		for (int j = 0; j < finalConts.size(); j++) {

			if ((abs(finalConts[j].center.x - brect.center.x) < 5) && (abs(finalConts[j].center.y - brect.center.y) < 5))
			{
				float height = max(finalConts[j].size.height, finalConts[j].size.width);
				float width = min(finalConts[j].size.height, finalConts[j].size.width);
				float hdiff = abs(height - heightR);
				float wdiff = abs(width - widthR);
				if (hdiff < 8 && wdiff < 8)
				{
					if (height < heightR && width < widthR)
					{
						finalConts.at(j) = brect;

					}
					//std::cout << "cont not unique ::" << i << std::endl;
					unique = false;
					break;
				}

			}

		}
		if (unique == true) {
			finalConts.push_back(brect);
			uniqueContours.push_back(contours[i]);
		}

	}
	Point2f vertices[4];

	int Contvalid = 0;
	Contour contInner;
	Contour contOuter;

	usableUniqueConts.clear();


	cout << "UniqueContours :: " << uniqueContours.size() << endl;
	for (int uq = 0; uq < uniqueContours.size(); uq++) {
		RotatedRect brect = minAreaRect(uniqueContours[uq].points);
		cout << "height" << brect.size.height << endl;
		cout << "Width " << brect.size.width << endl;
		if (brect.size.height > 38 && brect.size.width > 38 && brect.size.height < 3000 && brect.size.width < 3000)
		{
			for (int cntIn = 0; cntIn < uniqueContours[uq].points.size(); cntIn += 10) {
				cv::circle(imageC, (Point)uniqueContours[uq].points[cntIn], 5, Scalar(0, 160, 10), 1);
			}

			usableUniqueConts.push_back(uniqueContours[uq]);
		}
	}
	if (usableUniqueConts.size() != 2) {
		cout << "Contours not found properly" << endl;
		return -1;
	}
	cout << "Usable Unique Contours :: " << usableUniqueConts.size() << endl;
	for (int oi = 0; oi < usableUniqueConts.size(); oi++) {
		if (oi == 0) {
			contInner = usableUniqueConts[oi];
			contOuter = usableUniqueConts[oi];
		}

		RotatedRect brect = minAreaRect(contInner.points);
		float heightInner = max(brect.size.height, brect.size.width);
		float widthInner = min(brect.size.height, brect.size.width);
		/*cout << "Height Inner " << heightInner << endl;
		cout << "Width Inner " << widthInner << endl;*/

		brect = minAreaRect(contOuter.points);
		float heightOuter = max(brect.size.height, brect.size.width);
		float widthOuter = min(brect.size.height, brect.size.width);
	/*	cout << "Height Outer " << heightOuter << endl;
		cout << "Width Outer " << widthOuter << endl;*/

		brect = minAreaRect(usableUniqueConts[oi].points);
		float heightR = max(brect.size.height, brect.size.width);
		float widthR = min(brect.size.height, brect.size.width);
	/*	cout << "Height R " << heightR << endl;
		cout << "Width R " << widthR << endl;*/

		if (heightOuter < heightR && widthOuter < widthR) {
			contOuter = usableUniqueConts[oi];
		}

		if (heightR < heightInner && widthR < widthInner) {
			contInner = usableUniqueConts[oi];
		}
	}

	/*cout << "Contour Inner :: " << contInner.points.size() << endl;
	cout << "Contour Outer :: " << contOuter.points.size() << endl;*/

	Point2f minThickPtIn = Point(0, 0);
	Point2f minThickPtOut = Point(0, 0);
	float minthick = 10000;
	vector<Point2f> minDistMatch;
	for (int cntIn = 0; cntIn < contInner.points.size(); cntIn++) {
		float minD{ 0 };
		Point2f minDpt = Point2f(0, 0);
		for (int cntOut = 0; cntOut < contOuter.points.size(); cntOut++) {
			float dist = distancePoint(contInner.points[cntIn], contOuter.points[cntOut]);
			if (cntOut == 0) {
				minD = dist;
				minDpt = contOuter.points[cntOut];
			}
			if (dist < minD) {
				minD = dist;
				minDpt = contOuter.points[cntOut];
			}
		}
		if (minD < minthick) {
			minthick = minD;
			minThickPtIn = contInner.points[cntIn];
			minThickPtOut = minDpt;
		}
		minDistMatch.push_back(minDpt);
	}

	line(imageC, (Point)minThickPtIn, (Point)minThickPtOut, Scalar(0, 180, 0), 4);
	cout << "mmperpixProp :: " << mmPerPixProp << endl;
	float minthickness = distancePoint(minThickPtIn, minThickPtOut) * mmPerPixProp;
	//std::cout << " Distance Point : " << distancePoint(minThickPtIn, minThickPtOut) << endl;
	std::cout << "MinThickness : " << minthickness << endl;
	minThicknessProp = minthickness;
	//-------get center of inner contour
	vector <Point2f> tempPoints;
	std::vector<vector<cv::Point2f>> convex_hull(2);
	cv::approxPolyDP(cv::Mat(contInner.points), tempPoints, 2, true);

	// convex hull
	cv::convexHull(tempPoints, convex_hull[0], false);
	if (convex_hull[0].size() < 3) return false;

	// center of gravity
	cv::Moments mo = cv::moments(convex_hull[0]);
	Point2f centInner = cv::Point2f(mo.m10 / mo.m00, mo.m01 / mo.m00);
	cv::circle(imageC, centInner, 10, Scalar(180, 0, 0), 1);
	//-------center of inner cont end
	tempPoints.clear();
	cv::approxPolyDP(cv::Mat(contOuter.points), tempPoints, 2, true);

	// Convex hull
	cv::convexHull(tempPoints, convex_hull[1], false);
	if (convex_hull[1].size() < 3) return false;
	//	cout << "Convex hull size : small ::" << convex_hull[0] << endl;
	//	cout << "Convex hull size : outer ::" << convex_hull[1] << endl;
		// center of gravity
	mo = cv::moments(convex_hull[1]);
	Point2f centOuter = cv::Point2f(mo.m10 / mo.m00, mo.m01 / mo.m00);
	cv::circle(imageC, centOuter, 10, Scalar(180, 0, 0), 1);

	//for(int cntIn=0; cntIn<convex_hull[0].size(); cntIn+=1)
	   //	{  circle(imageC,(Point)convex_hull[0][cntIn],10,Scalar(180,0,0),4);}

	//	 for(int cntIn=0; cntIn<convex_hull[1].size(); cntIn+=1)
	   //	{  circle(imageC,(Point)convex_hull[1][cntIn],10,Scalar(180,0,0),4);}
///---center outer end

	RotatedRect rrOuter = fitEllipse(contOuter.points);
	std::cout << "cable dia" << rrOuter.size << endl;
	//mmPerPix = 20.818/ ((rrOuter.size.height + rrOuter.size.width) / 2);
	std::cout << "mmperPix= " << mmPerPixProp << endl;
	float cableDiamm = ((rrOuter.size.height + rrOuter.size.width) / 2) * mmPerPixProp;
	outerDiaProp = cableDiamm;
	std::cout << "cable dia mm" << cableDiamm << endl;
	cout << "minthickness mm :: " << minthick * mmPerPixProp << endl;
	
	vector <vector<Point2f>> sectorsInner(6);
	vector <vector<Point2f>> sectorsOuter(6);
	//cout << "inner Size" << contInner.points.size() << endl;
	//cout << "outer Size" << contOuter.points.size() << endl;
	float currAngle = 0;
	for (int cntIn = 0; cntIn < contInner.points.size(); cntIn += 1)
	{

		currAngle = getAngle(centInner, minThickPtIn, centInner, contInner.points[cntIn]);
		//line(imageC, (Point)contInner.points[cntIn], centInner, Scalar(0, 180, 0), 1);
		//cout<<"angle::"<<currAngle<<endl;
		int sector = 0;
		if (currAngle >= 270 || currAngle < 90)
		{
			sector = 0;
		}
		else
		{
			sector = (((int)currAngle - 90) / 180) + 1;
		}
		//cout<<"sector   ::"<<sector<<endl;
		sectorsInner[sector].push_back(contInner.points[cntIn]);
	}

	for (int cntIn = 0; cntIn < contOuter.points.size(); cntIn += 1)
	{

		currAngle = getAngle(centInner, minThickPtIn, centInner, contOuter.points[cntIn]);
		//line(imageC,(Point)contInner.points[cntIn],centInner,Scalar(0,180,0),1);
		//cout<<"angle::"<<currAngle<<endl;
		int sector = 0;
		if (currAngle >= 300 || currAngle < 60)
		{
			sector = 0;
		}
		else
		{
			sector = (((int)currAngle - 60) / 180) + 1;
		}
		//cout<<"sector   ::"<<sector<<endl;
		sectorsOuter[sector].push_back(contOuter.points[cntIn]);
	}

	vector <thicknessPt> minThickData(sectorsInner.size());
	srand(time(0));
	//draw Sectors ---
	for (int sec = 0; sec < sectorsInner.size(); sec++)
	{
		Scalar randomColor(
			(double)rand() / RAND_MAX * 225,
			(double)rand() / RAND_MAX * 225,
			(double)rand() / RAND_MAX * 225
		);
		float mindist = 100000;
		for (int inn = 0; inn < sectorsInner[sec].size(); inn++)

		{
			if (sec == 0)
				circle(imageC, sectorsInner[sec][inn], 5, randomColor, -1);
			else if (sec == 1)
				circle(imageC, sectorsInner[sec][inn], 5, randomColor, -1);
		

			for (int outr = 0; outr < sectorsOuter[sec].size(); outr++)
			{
				float currDist = distancePoint(sectorsInner[sec][inn], sectorsOuter[sec][outr]);
				if (currDist < mindist)
				{
					mindist = currDist;
					minThickData[sec].distance = mindist;
					minThickData[sec].innerPt = sectorsInner[sec][inn];
					minThickData[sec].outerPt = sectorsOuter[sec][outr];

				}

			}


		}
		line(imageC, minThickData[sec].innerPt, minThickData[sec].outerPt, Scalar(0, 60, 250), 3);
		cv::rectangle(imageC, Point(minThickData[sec].outerPt.x, minThickData[sec].outerPt.y + 3), Point(minThickData[sec].outerPt.x + (drawDiameter(cableDiamm) * 149), minThickData[sec].outerPt.y - (drawDiameter(cableDiamm) * 32)), Scalar(0, 150, 255), -1);

		
		cv::putText(imageC, cv::format("d=%3.3f", minThickData[sec].distance * mmPerPix), minThickData[sec].outerPt, FONT_HERSHEY_COMPLEX, drawDiameter(cableDiamm), Scalar(0, 0, 0), 1);
		
	}
	cv::rectangle(imageC, Point(centOuter.x, centOuter.y + 5), Point(centOuter.x + (drawDiameter(cableDiamm) * 179), centOuter.y - (drawDiameter(cableDiamm) * 38)), Scalar(0, 150, 255), -1);

	cv::putText(imageC, cv::format("OD=%3.3f", cableDiamm), centOuter, FONT_HERSHEY_COMPLEX, drawDiameter(cableDiamm), Scalar(0, 0, 0), 1);


	//float avgThickness = 0;
	float maxThick = 0;
	for (int ac = 0; ac < minThickData.size(); ac++)
	{
		avgThickness += minThickData[ac].distance * mmPerPixProp;
		std::cout << "minthick dist  :: " << minThickData[ac].distance * mmPerPixProp << endl;
		if (minThickData[ac].distance * mmPerPixProp > maxThick)
		{
			maxThick = minThickData[ac].distance * mmPerPixProp;
		}
		returnMinT.push_back(minThickData[ac]);
	}
	sectorCountProp = returnMinT.size();
	std::cout << "return thickness vec size" << returnMinT.size() << endl;
	concentricityProp = (minThicknessProp / maxThick) * 100;
	std::cout << "concentricity::" << concentricityProp << endl;
	avgThicknessProp = avgThickness / 4.0;
	std::cout << "minthick sum  :: " << avgThickness << endl;
	std::cout << "minthick avg :: " << avgThicknessProp << endl;

	maxId = maxAreaContour(usableUniqueConts);
	ovality_1 = calculate_ovality(usableUniqueConts, maxId, centOuter, cableDiamm, mmPerPixProp)  ;
	cout << "Ovallity_1 " << ovality_1 << "%" << endl;
	ovalityProp = ovality_1;

	jsonFileProp = "Hello";



	//cout << "contvalid :" << contValid << endl;
	/*cv::imwrite("resullt4core.bmp", imageC);*/


	//namedWindow("Display frame", WINDOW_NORMAL);
	//
	//maxId = getMaxAreaContour(usableUniqueCont);
	//Rect boundRect_1 = boundingRect(usableUniqueCont[maxId]);//enclose in Rect

	//Mat roi;
	//boundRect_1 = Rect(boundRect_1.tl().x - 50, boundRect_1.tl().y - 50, boundRect_1.width + 200, boundRect_1.height + 200);
	//roi = imageC(boundRect_1);

	//resShow("Display", imageC);
	//imshow("Display frame", roi);
	//waitKey(0);

	resize(imageC, img, img.size());


	return 0;

}

int algorithmLib::Class1::getMeasurements3c(System::Drawing::Bitmap^ rawImg)

{
	float ovality_1{};
	int maxId{};
	Mat img = BitmapToMat(rawImg);
	if (img.empty()) {

		cout << "Image not found" << endl;
		return -1;
	}


	cv::Mat mask = cv::Mat::zeros(img.size(), img.type());

	//Define your destination image
	cv::Mat dstImage = cv::Mat::zeros(img.size(), CV_8UC1);
	dstImage = 255 - dstImage;
	cvtColor(dstImage, dstImage, COLOR_GRAY2BGR);
	cv::circle(mask, cv::Point(mask.cols / 2, mask.rows / 2), 2200, cv::Scalar(255, 255, 255), -1, 8, 0);
	img.copyTo(dstImage, mask);


	//vector <thicknessPt> returnMinT;
	returnMinT.clear();
	//String imageFile = "D:/CV/7sept vapi/algo/SHIV-4-MM-NEW -17nov8pp_1mm.bmp";
//String outputFile = parser.get<String>("@output");

	Mat imageIn = dstImage;
	Mat image = imageIn.clone();

	if (image.channels() > 2)
	{
		cvtColor(image, image, COLOR_BGR2GRAY);
	}
	//threshold(image,image,120,255,CV_THRESH_TOZERO);
	Mat imageC;


	//imshow("inimg",image);
	//waitKey();
	vector<vector<Point>> contours_1;
	vector<Contour> contours;
	vector<Contour> uniqueContours;
	vector<Contour> usableUniqueConts;
	vector<Vec4i> hierarchy;
	int64 t0 = getCPUTickCount();
    EdgesSubPix(image, alpha, low, high, contours, hierarchy, mode, contours_1);
	int64 t1 = getCPUTickCount();
	cvtColor(image, imageC, COLOR_GRAY2BGR);
	cout << "Contours_1 size : " << contours_1.size() << endl;





	//cout << "execution time is " << (t1 - t0) / (double)getTickFrequency() << " seconds" << endl;
	//cout << "contours size:" << contours.size() << endl;
	/*	vector < vector < cv::Point2f > > contours2;
		for (int i=0;i<contours[0].points.size();i++)
		{
		contours2[0].push_back(cv::Point2f(contours[0].points[i].x,contours[0].points[i].y));
		}
		cout<<"cont point size()"<<contours2[0].size()<<endl;
		drawContours(img,contours2 ,0 , Scalar(0, 255, 0), 5, 8);
		waitKey();*/
	vector<vector<cv::Point2f> > conts;

	vector<RotatedRect > finalConts;
	vector<RotatedRect > finalContsWoBig;

	Mat rr;
	//std::cout << "Contours Size : " << contours.size() << endl;
	for (int i = 0; i < contours.size(); i++)
	{
		RotatedRect  brect = minAreaRect(contours[i].points);

		//	cout << "center:" << i << ":" << brect.center << endl;
			//cout<<"height_"<<i<<"=="<<brect.size.height*0.25<<endl;
				//cout<<"width_"<<i<<"=="<<brect.size.width*0.25<<endl;
		float heightR = max(brect.size.height, brect.size.width);
		float widthR = min(brect.size.height, brect.size.width);
		if (i == 0)
		{
			finalConts.push_back(brect);
			uniqueContours.push_back(contours[i]);

			//conts.push_back(contours[i].points);
			/*circle(imageC,brect.center,5,Scalar(255,255,0),4);


				(imageC,cv::format("%3.2f",0.25*max(brect.size.height,brect.size.width)),brect.center,CV_FONT_HERSHEY_COMPLEX,1,Scalar(0,0,255));
				putText(imageC,cv::format("%3.2f",0.25*min(brect.size.height,brect.size.width)),Point(brect.center.x,brect.center.y+60),CV_FONT_HERSHEY_COMPLEX,1,Scalar(0,0,255));*/
		}
		//resize(imageC,rr,Size(),0.35,0.35);
//	imshow("rr",rr);
//	waitKey();
		bool unique = true;
		for (int j = 0; j < finalConts.size(); j++)
		{
			if ((abs(finalConts[j].center.x - brect.center.x) < 5) && (abs(finalConts[j].center.y - brect.center.y) < 5))
			{
				float height = max(finalConts[j].size.height, finalConts[j].size.width);
				float width = min(finalConts[j].size.height, finalConts[j].size.width);
				float hdiff = abs(height - heightR);
				float wdiff = abs(width - widthR);
				if (hdiff < 8 && wdiff < 8)
				{
					if (height < heightR && width < widthR)
					{
						finalConts.at(j) = brect;

					}
					//std::cout << "cont not unique ::" << i << std::endl;
					unique = false;
					break;
				}

			}


		}

		if (unique == true)
		{
			finalConts.push_back(brect);
			uniqueContours.push_back(contours[i]);
		}

	}
	//	cout << "finalConts size_1:" << finalConts.size() << endl;
	//	cout << "UniqueConts size_1:" << uniqueContours.size() << endl;
	Point2f vertices[4];

	int contValid = 0;
	Contour contInner;
	Contour contOuter;
	//float heightMax, widthMax;
	//float heightMin, widthMin;
	//-----print all conts 1 by one
	usableUniqueConts.clear();
	std::cout << "Unique Contours : " << uniqueContours.size() << endl;
	for (int uq = 0; uq < uniqueContours.size(); uq++)
	{//Mat imgSh=imageC.clone();
		RotatedRect  brect = minAreaRect(uniqueContours[uq].points);
		//cout << "rectSize ::" << brect.size << endl;
		cout << "height" << brect.size.height << endl;
		cout << "width" << brect.size.width << endl;
		if (brect.size.height > 140 && brect.size.width > 140 && brect.size.height < 3000 && brect.size.width < 3000)
		{
			//rectangle(imageC, brect.boundingRect(), Scalar(0, 0, 255), 2);
			for (int cntIn = 0; cntIn < uniqueContours[uq].points.size(); cntIn += 10)
			{
				cv::circle(imageC, (Point)uniqueContours[uq].points[cntIn], 7, Scalar(0, 160, 10), 1);
				//cv::drawContours(imageC, contours_1, -1, Scalar(0, 160, 10), 1);
			}

			//resize(imageC, res, Size(), 0.3, 0.3);
			//imshow("imageCont",res);
			usableUniqueConts.push_back(uniqueContours[uq]);
			//waitKey(5);

		}
	}
	std::cout << "UsableUniqueConts " << usableUniqueConts.size() << endl;
	if (usableUniqueConts.size() != 2) {
		cout << "Contours not found properly" << endl;
		return -1;
	}
	

	
	for (int oi = 0; oi < usableUniqueConts.size(); oi++)
	{
		if (oi == 0)
		{
			contInner = usableUniqueConts[oi];
			contOuter = usableUniqueConts[oi];
		}
		RotatedRect  brect = minAreaRect(contInner.points);
		float heightInner = max(brect.size.height, brect.size.width);
		float widthInner = min(brect.size.height, brect.size.width);
		std::cout << "Height Inner " << heightInner << endl;
		std::cout << "Width Inner " << widthInner << endl;

		brect = minAreaRect(contOuter.points);
		float heightOuter = max(brect.size.height, brect.size.width);
		float widthOuter = min(brect.size.height, brect.size.width);
		/*std::cout << "height outer " << heightOuter << endl;
		std::cout << "Width outer " << widthOuter << endl;*/

		brect = minAreaRect(usableUniqueConts[oi].points);
		float heightR = max(brect.size.height, brect.size.width);
		float widthR = min(brect.size.height, brect.size.width);
		/*std::cout << "Hright R " << heightR << endl;
		std::cout << "Width R " << widthR << endl;*/

		if (heightR > heightOuter && widthR > widthOuter)
			contOuter = usableUniqueConts[oi];


		if (heightR < heightInner && widthR < widthInner)
			contInner = usableUniqueConts[oi];
	}
	//std::cout << "contInner size initial:" << contInner.points.size() << endl;
	//std::cout << "contOuter size initial:" << contOuter.points.size() << endl;

	//----------------get minimum thickness points
	Point2f minThickPtIn = Point(0, 0);
	Point2f minThickPtOut = Point(0, 0);
	float minthick = 10000;
	vector <Point2f> minDistMatch;//(contInner.points.size());
	for (int cntIn = 0; cntIn < contInner.points.size(); cntIn += 1)
	{ // circle(imageC,(Point)contInner.points[cntIn],10,Scalar(180,0,0),4);
		float minD = 0;
		Point2f minDpt = Point2f(0, 0);
		for (int cntOut = 0; cntOut < contOuter.points.size(); cntOut++)
		{
			float dist = distancePoint(contInner.points[cntIn], contOuter.points[cntOut]);
			//cout << " Distance " << dist << endl;
			if (cntOut == 0)
			{
				minD = dist;
				minDpt = contOuter.points[cntOut];

			}
			if (dist < minD)
			{
				minD = dist;
				minDpt = contOuter.points[cntOut];

			}
			//circle(imageC,contOuter.points[cntOut],1,Scalar(0,0,180),1);


		}
		if (minD < minthick)
		{
			minthick = minD;
			minThickPtIn = contInner.points[cntIn];
			minThickPtOut = minDpt;
			//	std::cout << "MinThickPTin " << minThickPtIn << endl;
			//	std::cout << "MinThickPTOut " << minThickPtOut << endl;

		}
		//cout<<"minPt"<<cntIn<<"::"<<minDpt<<"___dist::"<<minD<<endl;
		//circle(imageC,(Point)minDpt,10,Scalar(0,0,180),4);
		minDistMatch.push_back(minDpt);
		//putText(imageC,cv::format("%3.2f",minD),minDpt,CV_FONT_HERSHEY_COMPLEX ,2,Scalar(0,125,180));
	//line(imageC,(Point)contInner.points[cntIn],(Point)minDpt,Scalar(0,180,0),10);

	}
	//	cout << "minthickness pix:: " << minthick << endl;

	cout << "mmperpixProp :: " << mmPerPixProp << endl;
	float minthickness = distancePoint(minThickPtIn, minThickPtOut) * mmPerPixProp;
	//std::cout << " Distance Point : " << distancePoint(minThickPtIn, minThickPtOut) << endl;
	std::cout << "MinThickness : " << minthickness << endl;
	minThicknessProp = minthickness;
	//-------get center of inner contour
	vector <Point2f> tempPoints;
	std::vector<vector<cv::Point2f>> convex_hull(2);
	cv::approxPolyDP(cv::Mat(contInner.points), tempPoints, 2, true);

	// convex hull
	cv::convexHull(tempPoints, convex_hull[0], false);
	if (convex_hull[0].size() < 3) return false;

	// center of gravity
	cv::Moments mo = cv::moments(convex_hull[0]);
	Point2f centInner = cv::Point2f(mo.m10 / mo.m00, mo.m01 / mo.m00);
	cv::circle(imageC, centInner, 10, Scalar(180, 0, 0), 1);
	//-------center of inner cont end
	tempPoints.clear();
	cv::approxPolyDP(cv::Mat(contOuter.points), tempPoints, 2, true);

	// Convex hull
	cv::convexHull(tempPoints, convex_hull[1], false);
	if (convex_hull[1].size() < 3) return false;
	//	cout << "Convex hull size : small ::" << convex_hull[0] << endl;
	//	cout << "Convex hull size : outer ::" << convex_hull[1] << endl;
		// center of gravity
	mo = cv::moments(convex_hull[1]);
	Point2f centOuter = cv::Point2f(mo.m10 / mo.m00, mo.m01 / mo.m00);
	cv::circle(imageC, centOuter, 10, Scalar(180, 0, 0), 1);

	//for(int cntIn=0; cntIn<convex_hull[0].size(); cntIn+=1)
	   //	{  circle(imageC,(Point)convex_hull[0][cntIn],10,Scalar(180,0,0),4);}

	//	 for(int cntIn=0; cntIn<convex_hull[1].size(); cntIn+=1)
	   //	{  circle(imageC,(Point)convex_hull[1][cntIn],10,Scalar(180,0,0),4);}
///---center outer end

	RotatedRect rrOuter = fitEllipse(contOuter.points);
	std::cout << "cable dia 1 " << rrOuter.size << endl;
	//mmPerPix = 20.818/ ((rrOuter.size.height + rrOuter.size.width) / 2);
	std::cout << "mmperPix= " << mmPerPixProp << endl;
	std::cout << "CD::" << (rrOuter.size.height + rrOuter.size.width) / 2 << endl;;
	float cableDiamm = ((rrOuter.size.height + rrOuter.size.width) / 2) * mmPerPixProp;
	outerDiaProp = cableDiamm;
	std::cout << "cable dia mm" << cableDiamm << endl;
	cout << "minthickness mm :: " << minthick * mmPerPixProp << endl;
	
	vector <vector<Point2f>> sectorsInner(6);
	vector <vector<Point2f>> sectorsOuter(6);
	//cout << "inner Size" << contInner.points.size() << endl;
	//cout << "outer Size" << contOuter.points.size() << endl;
	float currAngle = 0;
	for (int cntIn = 0; cntIn < contInner.points.size(); cntIn += 1)
	{

		currAngle = getAngle(centInner, minThickPtIn, centInner, contInner.points[cntIn]);
		//line(imageC, (Point)contInner.points[cntIn], centInner, Scalar(0, 180, 0), 1);
		//cout<<"angle::"<<currAngle<<endl;
		int sector = 0;
		if (currAngle >= 300 || currAngle < 60)
		{
			sector = 0;
		}
		else
		{
			sector = (((int)currAngle - 60) / 120) + 1;
		}
		//cout<<"sector   ::"<<sector<<endl;
		sectorsInner[sector].push_back(contInner.points[cntIn]);
	}

	//cout << "No of Inner Sectors :: " << sectorsInner.size() << endl;
	for (int cntIn = 0; cntIn < contOuter.points.size(); cntIn += 1)
	{

		currAngle = getAngle(centInner, minThickPtIn, centInner, contOuter.points[cntIn]);
		//line(imageC,(Point)contInner.points[cntIn],centInner,Scalar(0,180,0),1);
		//cout<<"angle::"<<currAngle<<endl;
		int sector = 0;
		if (currAngle >= 300 || currAngle < 60)
		{
			sector = 0;
		}
		else
		{
			sector = (((int)currAngle - 60) / 120) + 1;
		}
		//cout<<"sector   ::"<<sector<<endl;
		sectorsOuter[sector].push_back(contOuter.points[cntIn]);
	}

	//cout << "No of Outer Sectors :: " << sectorsOuter.size() << endl;
	vector <thicknessPt> minThickData(sectorsInner.size());
	//draw Sectors ---
	srand(time(0));
	for (int sec = 0; sec < sectorsInner.size(); sec++)
	{
		float mindist = 100000;
		Scalar randomColor(
			(double)rand() / RAND_MAX * 225,
			(double)rand() / RAND_MAX * 225,
			(double)rand() / RAND_MAX * 225
		);
		for (int inn = 0; inn < sectorsInner[sec].size(); inn++)

		{
			if (sec == 0)
				circle(imageC, sectorsInner[sec][inn], 5, randomColor, -1);
			else if (sec == 1)
				circle(imageC, sectorsInner[sec][inn], 5, randomColor, -1);
			else if (sec == 2)
				circle(imageC, sectorsInner[sec][inn], 5, randomColor, -1);

			for (int outr = 0; outr < sectorsOuter[sec].size(); outr++)
			{
				float currDist = distancePoint(sectorsInner[sec][inn], sectorsOuter[sec][outr]);
				if (currDist < mindist)
				{
					mindist = currDist;
					minThickData[sec].distance = mindist;
					minThickData[sec].innerPt = sectorsInner[sec][inn];
					minThickData[sec].outerPt = sectorsOuter[sec][outr];

				}

			}


		}
		line(imageC, minThickData[sec].innerPt, minThickData[sec].outerPt, Scalar(0, 0, 255), 3);
		cv::rectangle(imageC, Point(minThickData[sec].outerPt.x, minThickData[sec].outerPt.y + 3), Point(minThickData[sec].outerPt.x + (drawDiameter(cableDiamm) * 149), minThickData[sec].outerPt.y - (drawDiameter(cableDiamm) * 32)), Scalar(0, 150, 255), -1);


		
		cv::putText(imageC, cv::format("d=%3.3f", minThickData[sec].distance * mmPerPix), minThickData[sec].outerPt, FONT_HERSHEY_COMPLEX, drawDiameter(cableDiamm), Scalar(0, 0, 0), 1);
		
	}
	
	cv::rectangle(imageC, Point(centOuter.x, centOuter.y + 5), Point(centOuter.x + (drawDiameter(cableDiamm) * 170), centOuter.y - (drawDiameter(cableDiamm) * 38)), Scalar(0, 150, 255), -1);


	cv::putText(imageC, cv::format("OD=%3.3f", cableDiamm), centOuter, FONT_HERSHEY_COMPLEX, drawDiameter(cableDiamm), Scalar(0, 0, 0), 1);

	//float avgThickness = 0;
	float maxThick = 0;
	for (int ac = 0; ac < minThickData.size(); ac++)
	{
		avgThickness += minThickData[ac].distance * mmPerPixProp;
		std::cout << "minthick dist  :: " << minThickData[ac].distance * mmPerPixProp << endl;
		if (minThickData[ac].distance * mmPerPixProp > maxThick)
		{
			maxThick = minThickData[ac].distance * mmPerPixProp;
		}
		returnMinT.push_back(minThickData[ac]);
	}
	sectorCountProp = returnMinT.size();
	std::cout << "return thickness vec size" << returnMinT.size() << endl;
	concentricityProp = (minThicknessProp / maxThick) * 100;
	std::cout << "concentricity::" << concentricityProp << endl;
	avgThicknessProp = avgThickness / 4.0;
	std::cout << "minthick sum  :: " << avgThickness << endl;
	std::cout << "minthick avg :: " << avgThicknessProp << endl;
	std::cout << " " << endl;

	ovality_1 = calculate_ovality(usableUniqueConts, maxId, centOuter, cableDiamm , mmPerPixProp)  ;
	cout << "Ovallity_1 " << ovality_1 << "%" << endl;
	ovalityProp = ovality_1;




	//putText(imageC, "Cable profile by AI", Point(80, 120), FONT_HERSHEY_COMPLEX, 3, Scalar(0, 0, 255), 1);

	//cout << "contvalid :" << contValid << endl;
	cv::imwrite("resullt4core.bmp", imageC);

	//cv::resize(imageC, imageIn, imageIn.size());
	//namedWindow("Display frame", WINDOW_NORMAL);
	//maxId = getMaxAreaContour(contours_1);
	//Mat roi;
	//Rect boundingRect_1 = boundingRect(contours_1[maxId]);

	//boundingRect_1 = Rect(boundingRect_1.tl().x - 90, boundingRect_1.tl().y - 100, boundingRect_1.width + 250, boundingRect_1.height + 200);
	//roi = imageC(boundingRect_1);


	//resShow("Display frame", imageC);
	//imshow("Display frame", roi);
	//waitKey(0);
	resize(imageC, img, img.size());
	return 0;

}

int algorithmLib::Class1::getMeasurements4c(System::Drawing::Bitmap^ rawImg)
{
	float ovality_1{};
	int maxId{};
	Mat img = BitmapToMat(rawImg);
	returnMinT.clear();
	//String imageFile = "D:/CV/7sept vapi/algo/SHIV-4-MM-NEW -17nov8pp_1mm.bmp";
//String outputFile = parser.get<String>("@output");

	Mat imageIn = BitmapToMat(rawImg);
	image = imageIn.clone();

	if (image.channels() > 2)
	{
		cvtColor(image, image, COLOR_BGR2GRAY);
	}
	//threshold(image,image,120,255,CV_THRESH_TOZERO);
	Mat imageC;
	cvtColor(image, imageC, COLOR_GRAY2BGR);
	//imshow("inimg",image);
	//waitKey();
	vector<vector<Point>> contours_1;
	vector<Contour> contours;
	vector<Contour> uniqueContours;
	vector<Contour> usableUniqueConts;
	vector<Vec4i> hierarchy;
	int64 t0 = getCPUTickCount();
	EdgesSubPix(image, alpha, low, high, contours, hierarchy, mode, contours_1);
	int64 t1 = getCPUTickCount();
//	cout << "execution time is " << (t1 - t0) / (double)getTickFrequency() << " seconds" << endl;
	//cout << "contours size:" << contours.size() << endl;
	/*vector < vector < cv::Point2f > > contours2;
	for (int i=0;i<contours[0].points.size();i++)
	{
	contours2[0].push_back(cv::Point2f(contours[0].points[i].x,contours[0].points[i].y));
	}
	cout<<"cont point size()"<<contours2[0].size()<<endl;
	drawContours(imageC,contours2 ,0 , Scalar(0, 255, 0), 5, 8);
	waitKey();*/
	vector<vector<cv::Point2f> > conts;

	vector<RotatedRect > finalConts;
	vector<RotatedRect > finalContsWoBig;

	Mat rr;
	for (int i = 0; i < contours.size(); i++)
	{
		RotatedRect  brect = minAreaRect(contours[i].points);

	//	cout << "center:" << i << ":" << brect.center << endl;
		//cout<<"height_"<<i<<"=="<<brect.size.height*0.25<<endl;
			//cout<<"width_"<<i<<"=="<<brect.size.width*0.25<<endl;
		float heightR = max(brect.size.height, brect.size.width);
		float widthR = min(brect.size.height, brect.size.width);
		if (i == 0)
		{
			finalConts.push_back(brect);
			//uniqueContours.push_back(contours[i]);

	//conts.push_back(contours[i].points);
	/*circle(imageC,brect.center,5,Scalar(255,255,0),4);

		putText(imageC,cv::format("%3.2f",0.25*max(brect.size.height,brect.size.width)),brect.center,CV_FONT_HERSHEY_COMPLEX,1,Scalar(0,0,255));
		putText(imageC,cv::format("%3.2f",0.25*min(brect.size.height,brect.size.width)),Point(brect.center.x,brect.center.y+60),CV_FONT_HERSHEY_COMPLEX,1,Scalar(0,0,255));*/
		}
		//resize(imageC,rr,Size(),0.35,0.35);
//	imshow("rr",rr);
//	waitKey();
		bool unique = true;
		for (int j = 0; j < finalConts.size(); j++)
		{
			if ((abs(finalConts[j].center.x - brect.center.x) < 5) && (abs(finalConts[j].center.y - brect.center.y) < 5))
			{
				float height = max(finalConts[j].size.height, finalConts[j].size.width);
				float width = min(finalConts[j].size.height, finalConts[j].size.width);
				float hdiff = abs(height - heightR);
				float wdiff = abs(width - widthR);
				if (hdiff < 8 && wdiff < 8)
				{
					if (height < heightR && width < widthR)
					{
						finalConts.at(j) = brect;

					}
					unique = false;
					break;
				}




			}


		}

		if (unique == true)
		{
			finalConts.push_back(brect);
			uniqueContours.push_back(contours[i]);
		}

	}
//	cout << "finalConts size:" << finalConts.size() << endl;
//	cout << "UniqueConts size:" << uniqueContours.size() << endl;
	Point2f vertices[4];

	int contValid = 0;
	Contour contInner;
	Contour contOuter;
	float heightMax, widthMax;
	float heightMin, widthMin;
	//-----print all conts 1 by one
	usableUniqueConts.clear();
	for (int uq = 0; uq < uniqueContours.size(); uq++)
	{//Mat imgSh=imageC.clone();
		RotatedRect  brect = minAreaRect(uniqueContours[uq].points);
	//	cout << "rectSize ::" << brect.size << endl;
		if (brect.size.height > 200 && brect.size.width > 200 && brect.size.height < 3000 && brect.size.width < 3000)
		{
			//rectangle(imageC, brect.boundingRect(), Scalar(0, 0, 255), 2);
			for (int cntIn = 0; cntIn < uniqueContours[uq].points.size(); cntIn += 10)
			{
				circle(imageC, (Point)uniqueContours[uq].points[cntIn], 10, Scalar(0, 160, 10), 1);
			}

			resize(imageC, res, Size(), 0.3, 0.3);
			//imshow("imageCont",res);
			usableUniqueConts.push_back(uniqueContours[uq]);
			//waitKey(5);

		}
	}
 	cout << "usableUniqueConts size:" << usableUniqueConts.size() << endl;
	if (usableUniqueConts.size() != 2) {
		cout << "Contours not found properly" << endl;
		return -1;
	}
	//------------printing conts---------end
	//get outer and inner conts
	for (int oi = 0; oi < usableUniqueConts.size(); oi++)
	{
		if (oi == 0)
		{
			contInner = usableUniqueConts[oi];
			contOuter = usableUniqueConts[oi];
		}
		RotatedRect  brect = minAreaRect(contInner.points);
		float heightInner = max(brect.size.height, brect.size.width);
		float widthInner = min(brect.size.height, brect.size.width);

		brect = minAreaRect(contOuter.points);
		float heightOuter = max(brect.size.height, brect.size.width);
		float widthOuter = min(brect.size.height, brect.size.width);


		brect = minAreaRect(usableUniqueConts[oi].points);
		float heightR = max(brect.size.height, brect.size.width);
		float widthR = min(brect.size.height, brect.size.width);

		if (heightR > heightOuter&& widthR > widthOuter)
			contOuter = usableUniqueConts[oi];


		if (heightR < heightInner && widthR < widthInner)
			contInner = usableUniqueConts[oi];
	}
	cout << "contInner size initial:" << contInner.points.size() << endl;

	//----------------get minimum thickness points
	Point2f minThickPtIn = Point(0, 0);
	Point2f minThickPtOut = Point(0, 0);
	float minthick = 10000;
	vector <Point2f> minDistMatch;//(contInner.points.size());
	for (int cntIn = 0; cntIn < contInner.points.size(); cntIn += 1)
	{ // circle(imageC,(Point)contInner.points[cntIn],10,Scalar(180,0,0),4);
		float minD = 0;
		Point2f minDpt = Point2f(0, 0);
		for (int cntOut = 0; cntOut < contOuter.points.size(); cntOut++)
		{
			float dist = distancePoint(contInner.points[cntIn], contOuter.points[cntOut]);
			if (cntOut == 0)
			{
				minD = dist;
				minDpt = contOuter.points[cntOut];
			}
			if (dist < minD)
			{
				minD = dist;
				minDpt = contOuter.points[cntOut];
			}
			//circle(imageC,contOuter.points[cntOut],10,Scalar(0,0,180),1);


		}
		if (minD < minthick)
		{
			minthick = minD;
			minThickPtIn = contInner.points[cntIn];
			minThickPtOut = minDpt;
		}
		//cout<<"minPt"<<cntIn<<"::"<<minDpt<<"___dist::"<<minD<<endl;
		//circle(imageC,(Point)minDpt,10,Scalar(0,0,180),4);
		minDistMatch.push_back(minDpt);
		//putText(imageC,cv::format("%3.2f",minD),minDpt,CV_FONT_HERSHEY_COMPLEX ,2,Scalar(0,125,180));
	//line(imageC,(Point)contInner.points[cntIn],(Point)minDpt,Scalar(0,180,0),1);

	}
//	cout << "minthickness pix:: " << minthick << endl;

	line(imageC, (Point)minThickPtIn, (Point)minThickPtOut, Scalar(0, 180, 0), 4);
	float minthickness = distancePoint(minThickPtIn, minThickPtOut) * mmPerPixProp;
	minThicknessProp = minthickness;
	//-------get center of inner contour
	vector <Point2f> tempPoints;
	std::vector<vector<cv::Point2f>> convex_hull(2);
	cv::approxPolyDP(cv::Mat(contInner.points), tempPoints, 2, true);

	// convex hull
	cv::convexHull(tempPoints, convex_hull[0], false);
	if (convex_hull[0].size() < 3) return false;

	// center of gravity
	cv::Moments mo = cv::moments(convex_hull[0]);
	Point2f centInner = cv::Point2f(mo.m10 / mo.m00, mo.m01 / mo.m00);
	circle(imageC, centInner, 10, Scalar(180, 0, 0), 1);
	//-------center of inner cont end
	tempPoints.clear();
	cv::approxPolyDP(cv::Mat(contOuter.points), tempPoints, 2, true);

	// convex hull
	cv::convexHull(tempPoints, convex_hull[1], false);
	if (convex_hull[1].size() < 3) return false;
//	cout << "Convex hull size : small ::" << convex_hull[0] << endl;
//	cout << "Convex hull size : outer ::" << convex_hull[1] << endl;
	// center of gravity
	mo = cv::moments(convex_hull[1]);
	Point2f centOuter = cv::Point2f(mo.m10 / mo.m00, mo.m01 / mo.m00);
	circle(imageC, centOuter, 10, Scalar(180, 0, 0), 1);

	//for(int cntIn=0; cntIn<convex_hull[0].size(); cntIn+=1)
	   //	{  circle(imageC,(Point)convex_hull[0][cntIn],10,Scalar(180,0,0),4);}

	//	 for(int cntIn=0; cntIn<convex_hull[1].size(); cntIn+=1)
	   //	{  circle(imageC,(Point)convex_hull[1][cntIn],10,Scalar(180,0,0),4);}
///---center outer end

	RotatedRect rrOuter = fitEllipse(contOuter.points);
	//cout << "cable dia" << rrOuter.size << endl;
	//mmPerPix = 20.818/ ((rrOuter.size.height + rrOuter.size.width) / 2);
	cout << "mmperPix=" << mmPerPixProp << endl;
	float cableDiamm = ((rrOuter.size.height + rrOuter.size.width) / 2) * mmPerPixProp;
	outerDiaProp = cableDiamm;
//	cout << "cable dia mm" << cableDiamm << endl;
//	cout << "minthickness mm :: " << minthick* mmPerPixProp << endl;
	vector <vector<Point2f>> sectorsInner(6);
	vector <vector<Point2f>> sectorsOuter(6);
	//cout << "inner Size" << contInner.points.size() << endl;
	//cout << "outer Size" << contOuter.points.size() << endl;
	float currAngle = 0;
	for (int cntIn = 0; cntIn < contInner.points.size(); cntIn += 1)
	{

		currAngle = getAngle(centInner, minThickPtIn, centInner, contInner.points[cntIn]);
		//line(imageC, (Point)contInner.points[cntIn], centInner, Scalar(0, 180, 0), 1);
		//cout<<"angle::"<<currAngle<<endl;
		int sector = 0;
		if (currAngle >= 315 || currAngle < 45)
		{
			sector = 0;
		}
		else
		{
			sector = (((int)currAngle - 45) / 90) + 1;
		}
		//cout<<"sector   ::"<<sector<<endl;
		sectorsInner[sector].push_back(contInner.points[cntIn]);
	}

	for (int cntIn = 0; cntIn < contOuter.points.size(); cntIn += 1)
	{

		currAngle = getAngle(centInner, minThickPtIn, centInner, contOuter.points[cntIn]);
		//line(imageC,(Point)contInner.points[cntIn],centInner,Scalar(0,180,0),1);
		//cout<<"angle::"<<currAngle<<endl;
		int sector = 0;
		if (currAngle >= 315 || currAngle < 45)
		{
			sector = 0;
		}
		else
		{
			sector = (((int)currAngle - 45) / 90) + 1;
		}
		//cout<<"sector   ::"<<sector<<endl;
		sectorsOuter[sector].push_back(contOuter.points[cntIn]);
	}

	vector <thicknessPt> minThickData(sectorsInner.size());
	//draw Sectors ---
	for (int sec = 0; sec < sectorsInner.size(); sec++)
	{
		float mindist = 100000;
		for (int inn = 0; inn < sectorsInner[sec].size(); inn++)
		{
			for (int outr = 0; outr < sectorsOuter[sec].size(); outr++)
			{
				float currDist = distancePoint(sectorsInner[sec][inn], sectorsOuter[sec][outr]);
				if (currDist < mindist)
				{
					mindist = currDist;
					minThickData[sec].distance = mindist;
					minThickData[sec].innerPt = sectorsInner[sec][inn];
					minThickData[sec].outerPt = sectorsOuter[sec][outr];
			
				}

			}
			
			
		}
		line(imageC, minThickData[sec].innerPt, minThickData[sec].outerPt, Scalar(0, 60, 250), 4);
		cv::rectangle(imageC, Point(minThickData[sec].outerPt.x, minThickData[sec].outerPt.y + 3), Point(minThickData[sec].outerPt.x + (drawDiameter(cableDiamm) * 149), minThickData[sec].outerPt.y - (drawDiameter(cableDiamm) * 32)), Scalar(0, 150, 160), -1);

		/*if (drawDiameter(cableDiamm) < 1) {
			cv::rectangle(imageC, Point(minThickData[sec].outerPt.x, minThickData[sec].outerPt.y + 3), Point(minThickData[sec].outerPt.x + 72, minThickData[sec].outerPt.y - 20), Scalar(0, 150, 255), -1);
		}
		else if (drawDiameter(cableDiamm) >= 1 && drawDiameter(cableDiamm) < 2) {

			cv::rectangle(imageC, Point(minThickData[sec].outerPt.x, minThickData[sec].outerPt.y + 3), Point(minThickData[sec].outerPt.x + 155, minThickData[sec].outerPt.y - 28), Scalar(0, 150, 255), -1);

		}
		else if (drawDiameter(cableDiamm) >= 2 && drawDiameter(cableDiamm) < 3) {
			cv::rectangle(imageC, Point(minThickData[sec].outerPt.x, minThickData[sec].outerPt.y + 5), Point(minThickData[sec].outerPt.x + 280, minThickData[sec].outerPt.y - 50), Scalar(0, 150, 255), -1);
		}
		else if (drawDiameter(cableDiamm) >= 3 && drawDiameter(cableDiamm) <= 4) {
			rectangle(imageC, Point(minThickData[sec].outerPt.x, minThickData[sec].outerPt.y + 3), Point(minThickData[sec].outerPt.x + 525, minThickData[sec].outerPt.y - 88), Scalar(0, 150, 255), -1);
		}*/
		cv::putText(imageC, cv::format("d=%3.3f", minThickData[sec].distance * mmPerPix), minThickData[sec].outerPt, FONT_HERSHEY_COMPLEX, drawDiameter(cableDiamm), Scalar(0, 0, 0), 2);
		//cv::putText(imageC, cv::format("d=%3.3f", minThickData[sec].distance * mmPerPix), minThickData[sec].outerPt, FONT_HERSHEY_COMPLEX, 1, Scalar(0, 0, 255), 1);
		cout << "  " << minThickData[sec].distance * mmPerPix;
		//imshow("sectorImage ",res);
		//waitKey();
	}
	/*if (drawDiameter(cableDiamm) < 1) {
		cv::rectangle(imageC, Point(centOuter.x, centOuter.y + 5), Point(centOuter.x + 85, centOuter.y - 28), Scalar(0, 150, 255), -1);
	}
	else if (drawDiameter(cableDiamm) >= 1 && drawDiameter(cableDiamm) < 2) {

		cv::rectangle(imageC, Point(centOuter.x, centOuter.y + 5), Point(centOuter.x + 175, centOuter.y - 28), Scalar(0, 150, 255), -1);

	}
	else if (drawDiameter(cableDiamm) >= 2 && drawDiameter(cableDiamm) < 3) {
		cv::rectangle(imageC, Point(centOuter.x, centOuter.y + 5), Point(centOuter.x + 355, centOuter.y - 50), Scalar(0, 150, 255), -1);
	}
	else if (drawDiameter(cableDiamm) >= 3 && drawDiameter(cableDiamm) <= 4) {
		rectangle(imageC, Point(centOuter.x, centOuter.y + 5), Point(centOuter.x + 150, centOuter.y - 28), Scalar(0, 150, 255), -1);
	}*/
	cv::rectangle(imageC, Point(centOuter.x, centOuter.y + 5), Point(centOuter.x + (drawDiameter(cableDiamm) * 179), centOuter.y - (drawDiameter(cableDiamm) * 38)), Scalar(0, 150, 255), -1);

	cv::putText(imageC, cv::format("OD=%3.3f", cableDiamm), centOuter, FONT_HERSHEY_COMPLEX, drawDiameter(cableDiamm), Scalar(0, 0, 0), 2);
	
	float avgThickness = 0;
	float maxThick = 0;
	for (int ac = 0; ac < minThickData.size(); ac++)
	{
		avgThickness += minThickData[ac].distance * mmPerPix;
		cout << "minthick dist  :: " << minThickData[ac].distance * mmPerPix << endl;
		if (minThickData[ac].distance * mmPerPix > maxThick)
		{
			maxThick = minThickData[ac].distance * mmPerPix;
		}
		returnMinT.push_back(minThickData[ac]);
	}
	sectorCountProp = returnMinT.size();
	cout << "return thickness vec size"<< returnMinT.size() << endl;
	concentricityProp = (minThicknessProp / maxThick) * 100;
	cout << "concentricity::" << concentricityProp<<endl;
	avgThicknessProp = avgThickness / 4.0;
	cout << "minthick sum  :: " << avgThickness << endl;
	cout << "minthick avg :: " << avgThicknessProp << endl;
	cout << " " << endl;
	//draw Sectors ---

	
	maxId = maxAreaContour(usableUniqueConts);
	ovality_1 = calculate_ovality(usableUniqueConts, maxId, centOuter, cableDiamm, mmPerPixProp)  ;
	cout << "Ovallity_1 " << ovality_1 << "%" << endl;
	ovalityProp = ovality_1;


	putText(imageC, "Cable profile by AI", Point(80, 120),FONT_HERSHEY_COMPLEX, 4, Scalar(0, 0, 255), 2);

	//cout << "contvalid :" << contValid << endl;
	imwrite("resullt4core.bmp", imageC);
	/*namedWindow("Display Window", WINDOW_NORMAL);
	imshow("Display Window", imageC);
	waitKey(0);*/
	resize(imageC, img, img.size());
	return 0;

}

int algorithmLib::Class1::getMeasurements5c(System::Drawing::Bitmap^ rawImg) {
	float ovality_1{};
	int maxId{};
	Mat img = BitmapToMat(rawImg);
	returnMinT.clear();
	if (img.empty()) {
		cout << "Img not found " << endl;
		return -1;
	}

	Mat mask = Mat::zeros(img.size(), img.type());

	//definiton of your mask images 
	Mat dstImage = Mat::zeros(img.size(), CV_8UC1);
	dstImage = 255 - dstImage;
	cvtColor(dstImage, dstImage, COLOR_GRAY2BGR);
	circle(mask, Point(mask.rows / 2, mask.cols / 2), 1000, Scalar(255, 255, 255), 1, -1);
	img.copyTo(dstImage, mask);

	//	Mat imageIn = dstImage;
	Mat imageIn = img;
	Mat image = imageIn.clone();

	if (image.channels() > 2) {
		cvtColor(image, image, COLOR_BGR2GRAY);
	}

	Mat imageC;
	cvtColor(image, imageC, COLOR_GRAY2BGR);

	vector < vector<Point>> contours_1;
	vector<Contour> contours;
	vector<Contour> uniqueContours;
	vector<Contour> usableUniqueConts;
	vector<Vec4i> hierarchy;

  EdgesSubPix(image, alpha, low, high, contours, hierarchy, mode, contours_1);

	//	drawContours_1(imageC, contours);
	vector<RotatedRect> finalConts;
	//imshow("iamgeC", imageC);

	for (size_t i = 0; i < contours.size(); i++) {
		RotatedRect brect = minAreaRect(contours[i].points);

		float heightR = max(brect.size.height, brect.size.width);
		float widthR = min(brect.size.height, brect.size.width);

		if (i == 0) {
			finalConts.push_back(brect);
			uniqueContours.push_back(contours[i]);
		}
		bool unique{ true };
		for (size_t j = 0; j < finalConts.size(); j++) {
			if ((abs(finalConts[j].center.x - brect.center.x) < 5) && (abs(finalConts[j].center.y - brect.center.y) < 5))
			{
				float height = max(finalConts[j].size.height, finalConts[j].size.width);
				float width = min(finalConts[j].size.height, finalConts[j].size.width);
				float hdiff = abs(height - heightR);
				float wdiff = abs(width - widthR);
				if (hdiff < 8 && wdiff < 8)
				{
					if (height < heightR && width < widthR)
					{
						finalConts.at(j) = brect;

					}
					//std::cout << "cont not unique ::" << i << std::endl;
					unique = false;
					break;
				}

			}


		}
		if (unique == true) {
			finalConts.push_back(brect);
			uniqueContours.push_back(contours[i]);

		}
	}

	Point2f vertices[4];
	int contValid{};
	Contour contInner{};
	Contour contOuter{};
	cout << "Unique Contours" << uniqueContours.size() << endl;
	usableUniqueConts.clear();
	for (size_t i = 0; i < uniqueContours.size(); i++) {
		RotatedRect brect = minAreaRect(uniqueContours[i].points);
		if (brect.size.height > 50 && brect.size.width > 50 && brect.size.height < 3000 && brect.size.width < 3000) {
			for (size_t cntIn = 0; cntIn < uniqueContours[i].points.size(); cntIn += 10) {
				//	circle(imageC, uniqueContours[i].points[cntIn], 8, Scalar(0, 255, 0), 2);
			}
			usableUniqueConts.push_back(uniqueContours[i]);
		}
	}
	cout << "Usable Unique Conts " << usableUniqueConts.size() << endl;
	if (usableUniqueConts.size() != 2) {
		cout << "Contours not found properly" << endl;
		return -1;
	}
	for (size_t i = 0; i < usableUniqueConts.size(); i++) {

		if (i == 0) {
			contInner = usableUniqueConts[i];
			contOuter = usableUniqueConts[i];
		}
		RotatedRect brect = minAreaRect(contInner.points);
		float  heightInner = max(brect.size.height, brect.size.width);
		float widthInner = min(brect.size.height, brect.size.width);

		brect = minAreaRect(contOuter.points);
		float heightOuter = max(brect.size.height, brect.size.width);
		float widthOuter = min(brect.size.height, brect.size.width);

		brect = minAreaRect(usableUniqueConts[i].points);
		float heightR = max(brect.size.height, brect.size.width);
		float widthR = min(brect.size.height, brect.size.width);

		if (heightR > heightOuter && widthR > widthOuter) { contOuter = usableUniqueConts[i]; }
		if (heightR < heightInner && widthR < widthInner) { contInner = usableUniqueConts[i]; }
	}

	Point minThickPtIn = Point(0, 0);
	Point minThickPtOut = Point(0, 0);
	float minthick = 10000;
	vector <Point2f> minDistMatch;

	for (size_t cntIn = 0; cntIn < contInner.points.size(); cntIn++) {
		float minD = 0.0;
		Point2f minDpt = Point(0, 0);
		for (size_t cntOut = 0; cntOut < contOuter.points.size(); cntOut++) {
			float dist = distancePoint(contInner.points[cntIn], contOuter.points[cntOut]);
			if (cntOut == 0) {
				minD = dist;
				minDpt = contOuter.points[cntOut];
			}
			if (dist < minD) {
				minD = dist;
				minDpt = contOuter.points[cntOut];
			}
			if (minD < minthick) {
				minthick = minD;
				minThickPtIn = contInner.points[cntIn];
				minThickPtOut = contOuter.points[cntOut];
			}
			minDistMatch.push_back(minDpt);
		}

	}
	line(imageC, minThickPtIn, minThickPtOut, Scalar(0, 0, 255), 3);
	cout << "Distance Point " << distancePoint(minThickPtIn, minThickPtOut);
	float minthickness = distancePoint(minThickPtIn, minThickPtOut) * mmPerPixProp;
	cout << "Minimum Thickness :: " << minthickness << endl;
	minThicknessProp = minthickness;

	vector<Point2f> tempPoints;
	vector<vector<Point2f>> convex_hull(2);
	approxPolyDP(Mat(contInner.points), tempPoints, 2, true);

	convexHull(tempPoints, convex_hull[0], false);
	if (convex_hull[0].size() < 3) return false;

	cv::Moments mo = cv::moments(convex_hull[0]);
	Point2f centInner = cv::Point2f(mo.m10 / mo.m00, mo.m01 / mo.m00);
	tempPoints.clear();

	circle(imageC, centInner, 5, Scalar(255, 0, 0), 2);
	approxPolyDP(Mat(contOuter.points), tempPoints, 2, true);
	convexHull(tempPoints, convex_hull[1], false);
	if (convex_hull[1].size() < 3) return false;

	mo = moments(convex_hull[1]);
	Point2f centOuter = Point2f(mo.m10 / mo.m00, mo.m01 / mo.m00);

	circle(imageC, centOuter, 7, Scalar(0, 0, 255), 2);

	RotatedRect rrOuter = fitEllipse(contOuter.points);
	cout << "Cable Dia meter -> " << rrOuter.size << endl;

	float cableDiamm = ((rrOuter.size.height + rrOuter.size.width) / 2) * mmPerPixProp;
	cout << "Cable Dia in MM " << cableDiamm << endl;
	outerDiaProp = cableDiamm;

	vector<vector<Point2f>> sectorsInner(5);
	vector<vector<Point2f>> sectorsOuter(5);

	float currAngle{};

	for (size_t cntIn = 0; cntIn < contInner.points.size(); cntIn++) {
		currAngle = getAngle(centInner, minThickPtIn, centInner, contInner.points[cntIn]);
		int sector = 0;

		if (currAngle >= 324 || currAngle < 36) {
			sector = 0;
		}
		else {
			sector = (((int)currAngle - 36) / 72) + 1;
		}
		sectorsInner[sector].push_back(contInner.points[cntIn]);
	}

	for (size_t cntOut = 0; cntOut < contOuter.points.size(); cntOut++) {
		currAngle = getAngle(centInner, minThickPtIn, centInner, contOuter.points[cntOut]);
		int sector = 0;
		if (currAngle >= 324 || currAngle < 36) {
			sector = 0;
		}
		else {
			sector = (((int)currAngle - 36) / 72) + 1;
		}
		sectorsOuter[sector].push_back(contOuter.points[cntOut]);
	}

	vector<thicknessPt> minThickData(sectorsInner.size());
	srand(time(0));
	for (size_t sec = 0; sec < sectorsInner.size(); sec++)
	{
		float mindist = 100000;
		Scalar randomColor(
			(double)rand() / RAND_MAX * 255,
			(double)rand() / RAND_MAX * 255,
			(double)rand() / RAND_MAX * 255
		);
		for (size_t cntIn = 0; cntIn < sectorsInner[sec].size(); cntIn++) {
			if (sec == 0)
				circle(imageC, sectorsInner[sec][cntIn], 3, randomColor, -1);
			else if (sec == 1)
				circle(imageC, sectorsInner[sec][cntIn], 3, randomColor, -1);
			else if (sec == 2)
				circle(imageC, sectorsInner[sec][cntIn], 3, randomColor, -1);
			else if (sec == 3)
				circle(imageC, sectorsInner[sec][cntIn], 3, randomColor, -1);
			else
				circle(imageC, sectorsInner[sec][cntIn], 3, randomColor, -1);


			for (size_t cntOut = 0; cntOut < sectorsOuter[sec].size(); cntOut++) {
				/*	if (sec == 0)
						circle(imageC, sectorsOuter[sec][cntOut], 3, randomColor, -1);
					else if (sec == 1)
						circle(imageC, sectorsOuter[sec][cntOut], 3, randomColor, -1);
					else if (sec == 2)
						circle(imageC, sectorsOuter[sec][cntOut], 3, randomColor, -1);
					else if (sec == 3)
						circle(imageC, sectorsOuter[sec][cntOut], 3, randomColor, -1);
					else
						circle(imageC, sectorsOuter[sec][cntOut], 3, randomColor, -1);*/
				float currDist = distancePoint(sectorsInner[sec][cntIn], sectorsOuter[sec][cntOut]);
				if (currDist < mindist) {
					mindist = currDist;
					minThickData[sec].distance = mindist;
					minThickData[sec].innerPt = sectorsInner[sec][cntIn];
					minThickData[sec].outerPt = sectorsOuter[sec][cntOut];
				}
			}
		}
		line(imageC, minThickData[sec].innerPt, minThickData[sec].outerPt, Scalar(0, 0, 255), 4);
		cv::rectangle(imageC, Point(minThickData[sec].outerPt.x, minThickData[sec].outerPt.y + 3), Point(minThickData[sec].outerPt.x + (drawDiameter(cableDiamm) * 149), minThickData[sec].outerPt.y - (drawDiameter(cableDiamm) * 32)), Scalar(0, 150, 160), -1);
		cout << "minThickdata[" << sec << "]->" << minThickData[sec].distance << endl;
		/*if (drawDiameter(cableDiamm) < 1) {
			cv::rectangle(imageC, Point(minThickData[sec].outerPt.x, minThickData[sec].outerPt.y + 3), Point(minThickData[sec].outerPt.x + 72, minThickData[sec].outerPt.y - 20), Scalar(0, 150, 255), -1);
		}
		else if (drawDiameter(cableDiamm) >= 1 && drawDiameter(cableDiamm) < 2) {

			cv::rectangle(imageC, Point(minThickData[sec].outerPt.x, minThickData[sec].outerPt.y + 3), Point(minThickData[sec].outerPt.x + 155, minThickData[sec].outerPt.y - 28), Scalar(0, 150, 255), -1);

		}
		else if (drawDiameter(cableDiamm) >= 2 && drawDiameter(cableDiamm) < 3) {
			cv::rectangle(imageC, Point(minThickData[sec].outerPt.x, minThickData[sec].outerPt.y + 5), Point(minThickData[sec].outerPt.x + 280, minThickData[sec].outerPt.y - 50), Scalar(0, 150, 255), -1);
		}
		else if (drawDiameter(cableDiamm) >= 3 && drawDiameter(cableDiamm) <= 4) {
			rectangle(imageC, Point(minThickData[sec].outerPt.x, minThickData[sec].outerPt.y + 3), Point(minThickData[sec].outerPt.x + 525, minThickData[sec].outerPt.y - 88), Scalar(0, 150, 255), -1);
		}*/

		cv::putText(imageC, cv::format("d=%3.3f", minThickData[sec].distance * mmPerPix), minThickData[sec].outerPt, FONT_HERSHEY_COMPLEX, drawDiameter(cableDiamm), Scalar(0, 0, 0), 1);
	}
	/*if (drawDiameter(cableDiamm) < 1) {
		cv::rectangle(imageC, Point(centOuter.x, centOuter.y + 5), Point(centOuter.x + 85, centOuter.y - 28), Scalar(0, 150, 255), -1);
	}
	else if (drawDiameter(cableDiamm) >= 1 && drawDiameter(cableDiamm) < 2) {

		cv::rectangle(imageC, Point(centOuter.x, centOuter.y + 5), Point(centOuter.x + 175, centOuter.y - 28), Scalar(0, 150, 255), -1);

	}
	else if (drawDiameter(cableDiamm) >= 2 && drawDiameter(cableDiamm) < 3) {
		cv::rectangle(imageC, Point(centOuter.x, centOuter.y + 5), Point(centOuter.x + 355, centOuter.y - 50), Scalar(0, 150, 255), -1);
	}
	else if (drawDiameter(cableDiamm) >= 3 && drawDiameter(cableDiamm) <= 4) {
		rectangle(imageC, Point(centOuter.x, centOuter.y + 5), Point(centOuter.x + 150, centOuter.y - 28), Scalar(0, 150, 255), -1);
	}*/
	cv::rectangle(imageC, Point(centOuter.x, centOuter.y + 5), Point(centOuter.x + (drawDiameter(cableDiamm) * 179), centOuter.y - (drawDiameter(cableDiamm) * 38)), Scalar(0, 150, 255), -1);

	cv::putText(imageC, cv::format("OD=%3.3f", cableDiamm), centOuter, FONT_HERSHEY_COMPLEX, drawDiameter(cableDiamm), Scalar(0, 0, 0), 1);

	
	returnMinT.clear();

	float maxThick = 0;
	cout << "MinThickDta---------------------- " << minThickData.size() << endl;
	for (int ac = 0; ac < minThickData.size(); ac++)
	{
		avgThickness += minThickData[ac].distance * mmPerPixProp;
		std::cout << "minthick dist  :: " << minThickData[ac].distance *mmPerPixProp << endl;
		if (minThickData[ac].distance * mmPerPix > maxThick)
		{
			maxThick = minThickData[ac].distance * mmPerPixProp;
		}
		returnMinT.push_back(minThickData[ac]);
	}

	sectorCountProp = returnMinT.size();
	std::cout << "return thickness vec size" << returnMinT.size() << endl;
	concentricityProp = (minThicknessProp / maxThick) * 100;
	std::cout << "concentricity::" << concentricityProp << endl;
	avgThicknessProp = avgThickness / 4.0;
	std::cout << "minthick sum  :: " << avgThickness << endl;
	std::cout << "minthick avg :: " << avgThicknessProp << endl;
	std::cout << " " << endl;

	cv::imwrite("resullt4core.bmp", imageC);

	//cv::resize(imageC, imageIn, imageIn.size());

	//namedWindow("Display frame", WINDOW_NORMAL);

	//	maxId = getMaxAreaContour(contours_1);
	maxId = maxAreaContour(usableUniqueConts);
	ovality_1 = calculate_ovality(usableUniqueConts, maxId, centOuter, cableDiamm, mmPerPixProp)  ;
	cout << "Ovallity_1 " << ovality_1 << "%" << endl;
	ovalityProp = ovality_1;

	//Rect boundRect_1 = boundingRect(usableUniqueConts[maxId].points);//enclose in Rect

	//Mat roi;
	//boundRect_1 = Rect(boundRect_1.tl().x - 75, boundRect_1.tl().y - 75, boundRect_1.width + 300, boundRect_1.height + 300);
	//roi = imageC(boundRect_1);

	//imshow("Display frame", roi);
	//waitKey(0);
	resize(imageC, img, img.size());
	return 0;

}

int algorithmLib::Class1::getMeasurements6c(System::Drawing::Bitmap^ rawImg) {
	int maxId{};
	float ovality_1{};
	Mat img = BitmapToMat(rawImg);
	returnMinT.clear();
	if (img.empty()) {
		cout << "Image not found" << endl;
		return -1;
	}
	Mat imgIn = img.clone();
	Mat image = imgIn;

	if (image.channels() > 2) {
		cvtColor(image, image, COLOR_BGR2GRAY);
	}

	Mat imageC;
	cvtColor(image, imageC, COLOR_GRAY2BGR);

	vector<vector<Point>> contours_1;
	vector<Contour> contours;
	vector<Contour> uniqueContours;
	vector<Contour> usableUniqueConts;
	vector<Vec4i> hierarchy;

    EdgesSubPix(image, alpha, low, high, contours, hierarchy, mode, contours_1);
	cout << "Contours size" << contours.size() << endl;
	cout << "Contours_1 size " << contours_1.size() << endl;

	vector<RotatedRect> finalConts;


	for (size_t i = 0; i < contours.size(); i++) {
		RotatedRect brect = minAreaRect(contours[i].points);

		float heightR = max(brect.size.height, brect.size.width);
		float widthR = min(brect.size.height, brect.size.width);
		if (i == 0) {
			finalConts.push_back(brect);
			uniqueContours.push_back(contours[i]);
		}
		bool unique = true;
		for (size_t j = 0; j < finalConts.size(); j++) {
			if ((abs(finalConts[j].center.x - brect.center.x) < 5) && (abs(finalConts[j].center.y - brect.center.y) < 5))
			{
				float height = max(finalConts[j].size.height, finalConts[j].size.width);
				float width = min(finalConts[j].size.height, finalConts[j].size.width);
				float hdiff = abs(height - heightR);
				float wdiff = abs(width - widthR);
				if (hdiff < 8 && wdiff < 8)
				{
					if (height < heightR && width < widthR)
					{
						finalConts.at(j) = brect;

					}

					unique = false;
					break;
				}

			}


		}
		if (unique == true) {
			finalConts.push_back(brect);
			uniqueContours.push_back(contours[i]);

		}
	}
	cout << "Unique Contour Size" << uniqueContours.size() << endl;

	Point2f vertices[4];
	Contour contInner{};
	Contour contOuter{};
	for (size_t uq = 0; uq < uniqueContours.size(); uq++) {
		RotatedRect brect = minAreaRect(uniqueContours[uq].points);
		if (brect.size.height > 50 && brect.size.width > 50 && brect.size.height < 3000 && brect.size.width < 3000) {
			for (size_t cntIn = 0; cntIn < uniqueContours[uq].points.size(); cntIn += 5) {
				circle(imageC, uniqueContours[uq].points[cntIn], 5, Scalar(0, 0, 255), 1);
			}
			usableUniqueConts.push_back(uniqueContours[uq]);
		}
	}
	cout << "Usable Unique Contours" << usableUniqueConts.size() << endl;
	if (usableUniqueConts.size() != 2) {
		cout << "Contours not found properly" << endl;
		return -1;
	}
	for (int i = 0; i < usableUniqueConts.size(); i++) {
		if (i == 0) {
			contInner = usableUniqueConts[i];
			contOuter = usableUniqueConts[i];
		}
		RotatedRect brect = minAreaRect(contInner.points);
		float heightInner = max(brect.size.height, brect.size.width);
		float widthInner = min(brect.size.height, brect.size.width);

		brect = minAreaRect(contOuter.points);
		float heightOuter = max(brect.size.height, brect.size.width);
		float widthOuter = min(brect.size.height, brect.size.width);

		brect = minAreaRect(usableUniqueConts[i].points);
		float heightR = max(brect.size.height, brect.size.width);
		float widthR = min(brect.size.height, brect.size.width);

		if (heightR < heightInner && widthR < widthInner) { contInner = usableUniqueConts[i]; }
		if (heightR > heightOuter && widthR > widthOuter) { contOuter = usableUniqueConts[i]; }

	}
	Point2f minThickPtIn = Point(0, 0);
	Point2f minThickPtOut = Point(0, 0);
	float minthick = 10000;
	vector <Point2f> minDistMatch;

	for (size_t cntIn = 0; cntIn < contInner.points.size(); cntIn++) {
		float minD{};
		Point2f minDpt{};
		for (size_t cntOut = 0; cntOut < contOuter.points.size(); cntOut++) {
			float dist = distancePoint(contInner.points[cntIn], contOuter.points[cntOut]);
			if (cntOut == 0) {
				minD = dist;
				minDpt = contOuter.points[cntOut];
			}
			if (dist < minD) {
				minD = dist;
				minDpt = contOuter.points[cntOut];
			}
			if (minD < minthick) {
				minthick = minD;
				minThickPtIn = contInner.points[cntIn];
				minThickPtOut = minDpt;
			}
			minDistMatch.push_back(minDpt);
		}
	}
	line(imageC, minThickPtIn, minThickPtOut, Scalar(0, 0, 255), 2);
	float minthickness = distancePoint(minThickPtIn, minThickPtOut) * mmPerPixProp;
	cout << "Minthickness" << minthickness << endl;
	minThicknessProp = minthickness;
	imshow("imageC", imageC);
	waitKey(0);
	vector<Point2f> tempPoints;
	vector<vector<Point2f>> convex_hull(2);

	cv::approxPolyDP(cv::Mat(contInner.points), tempPoints, 2, true);

	// convex hull
	cv::convexHull(tempPoints, convex_hull[0], false);
	if (convex_hull[0].size() < 3) return false;


	Moments mo = moments(convex_hull[0]);
	Point2f centInner = cv::Point2f(mo.m10 / mo.m00, mo.m01 / mo.m00);
	circle(imageC, centInner, 5, Scalar(0, 0, 255), 1);

	tempPoints.clear();
	approxPolyDP(Mat(contOuter.points), tempPoints, 2, true);
	convexHull(tempPoints, convex_hull[1], false);
	if (convex_hull[1].size() < 3) return false;

	mo = moments(convex_hull[1]);
	Point2f centOuter = cv::Point2f(mo.m10 / mo.m00, mo.m01 / mo.m00);
	circle(imageC, centOuter, 6, Scalar(255, 0, 0), 1);

	RotatedRect rrOuter = fitEllipse(contOuter.points);
	cout << "Rrouter" << rrOuter.size << endl;
	float cableDiamm = ((rrOuter.size.height + rrOuter.size.width) / 2) * mmPerPixProp;
	cout << "Cable Dia in MM" << cableDiamm << endl;

	vector<vector<Point2f>> sectorsInner(6);
	vector<vector<Point2f>> sectorsOuter(6);
	float curAngle{};

	for (size_t cntIn = 0; cntIn < contInner.points.size(); cntIn++) {

		int sector{};
		curAngle = getAngle(centInner, minThickPtIn, centInner, contInner.points[cntIn]);
		if (curAngle >= 330 || curAngle < 30) {
			sector = 0;
		}
		else
		{
			sector = (((int)curAngle - 30) / 60) + 1;
		}
		sectorsInner[sector].push_back(contInner.points[cntIn]);
	}

	for (size_t cntOut = 0; cntOut < contOuter.points.size(); cntOut++) {
		int sector{};
		curAngle = getAngle(centInner, minThickPtIn, centInner, contOuter.points[cntOut]);
		if (curAngle >= 330 || curAngle < 30) {
			sector = 0;
		}
		else {
			sector = (((int)curAngle - 30) / 60) + 1;
		}
		sectorsOuter[sector].push_back(contOuter.points[cntOut]);
	}

	srand(time(0));
	vector<thicknessPt> minThickData(sectorsInner.size());

	for (size_t sec = 0; sec < sectorsInner.size(); sec++) {

		float minDist = 10000000;
		Scalar radomColor(
			(double)rand() / RAND_MAX * 255,
			(double)rand() / RAND_MAX * 255,
			(double)rand() / RAND_MAX * 255
		);
		for (size_t inn = 0; inn < sectorsInner[sec].size(); inn++) {

			/*if (sec == 0)
				circle(imageC, sectorsInner[sec][inn], 5, radomColor,  -1);
			else if (sec == 1)
				circle(imageC, sectorsInner[sec][inn], 5, radomColor,  -1);
			else if (sec == 2)
				circle(imageC, sectorsInner[sec][inn], 5, radomColor,  -1);
			else if (sec == 3)
				circle(imageC, sectorsInner[sec][inn], 5, radomColor,  -1);
			else if (sec == 4)
				circle(imageC, sectorsInner[sec][inn], 5, radomColor,  -1);
			else if (sec == 5)
				circle(imageC, sectorsInner[sec][inn], 5, radomColor,  -1);*/

			for (size_t out = 0; out < sectorsOuter[sec].size(); out++) {

				if (sec == 0)
					circle(imageC, sectorsOuter[sec][out], 5, radomColor, -1);
				else if (sec == 1)
					circle(imageC, sectorsOuter[sec][out], 5, radomColor, -1);
				else if (sec == 2)
					circle(imageC, sectorsOuter[sec][out], 5, radomColor, -1);
				else if (sec == 3)
					circle(imageC, sectorsOuter[sec][out], 5, radomColor, -1);
				else if (sec == 4)
					circle(imageC, sectorsOuter[sec][out], 5, radomColor, -1);
				else if (sec == 5)
					circle(imageC, sectorsOuter[sec][out], 5, radomColor, -1);

				float currDist = distancePoint(sectorsInner[sec][inn], sectorsOuter[sec][out]);
				if (currDist < minDist) {
					minDist = currDist;
					minThickData[sec].distance = minDist;
					minThickData[sec].innerPt = sectorsInner[sec][inn];
					minThickData[sec].outerPt = sectorsOuter[sec][out];
				}

			}
		}
		line(imageC, minThickData[sec].innerPt, minThickData[sec].outerPt, Scalar(0, 60, 250), 2);
		cv::rectangle(imageC, Point(minThickData[sec].outerPt.x, minThickData[sec].outerPt.y + 3), Point(minThickData[sec].outerPt.x + (drawDiameter(cableDiamm) * 149), minThickData[sec].outerPt.y - (drawDiameter(cableDiamm) * 32)), Scalar(0, 150, 160), -1);
		cv::putText(imageC, cv::format("d=%3.3f", minThickData[sec].distance * mmPerPix), minThickData[sec].outerPt, FONT_HERSHEY_COMPLEX, drawDiameter(cableDiamm), Scalar(0, 0, 0), 1);


	}
	cv::rectangle(imageC, Point(centOuter.x, centOuter.y + 5), Point(centOuter.x + (drawDiameter(cableDiamm) * 179), centOuter.y - (drawDiameter(cableDiamm) * 38)), Scalar(0, 150, 255), -1);
		cv::putText(imageC, cv::format("OD=%3.3f", cableDiamm), centOuter, FONT_HERSHEY_COMPLEX, drawDiameter(cableDiamm), Scalar(0, 0, 0), 1);
	float maxThick = 0;
	vector<thicknessPt> returnMinT;
	for (int ac = 0; ac < minThickData.size(); ac++)
	{
		avgThickness += minThickData[ac].distance * mmPerPixProp;
		std::cout << "minthick dist  :: " << minThickData[ac].distance * mmPerPixProp << endl;
		if (minThickData[ac].distance * mmPerPixProp > maxThick)
		{
			maxThick = minThickData[ac].distance * mmPerPixProp;
		}
		returnMinT.push_back(minThickData[ac]); 
	}

	sectorCountProp = returnMinT.size();
	std::cout << "return thickness vec size" << returnMinT.size() << endl;
	concentricityProp = (minThicknessProp / maxThick) * 100;
	std::cout << "concentricity::" << concentricityProp << endl;
	avgThicknessProp = avgThickness / 4.0;
	std::cout << "minthick sum  :: " << avgThickness << endl;
	std::cout << "minthick avg :: " << avgThicknessProp << endl;
	std::cout << " " << endl;

	cv::imwrite("resullt4core.bmp", imageC);

	//cv::resize(imageC, imageIn, imageIn.size());



	//	maxId = getMaxAreaContour(contours_1);
	maxId = maxAreaContour(usableUniqueConts);
	ovality_1 = calculate_ovality(usableUniqueConts, maxId, centOuter, cableDiamm, mmPerPixProp)  ;
	cout << "Ovallity_1 " << ovality_1 << "%" << endl;
	ovalityProp = ovality_1;

	//Rect boundRect_1 = boundingRect(usableUniqueConts[maxId].points);//enclose in Rect

	//Mat roi;
	//boundRect_1 = Rect(boundRect_1.tl().x - 75, boundRect_1.tl().y - 75, boundRect_1.width + 300, boundRect_1.height + 300);
	//roi = imageC(boundRect_1);

	resize(imageC, img, img.size());
	
	return 0;
}

int algorithmLib::Class1::getMeasurementsMultiCore(System::Drawing::Bitmap^ rawImg) {
	float ovality_1{};
	Mat img = BitmapToMat(rawImg);
	int maxId{};
	if (img.empty()) {

		cout << "Image not found" << endl;
		return -1;
	}


	cv::Mat mask = cv::Mat::zeros(img.size(), img.type());

	//Define your destination image
	cv::Mat dstImage = cv::Mat::zeros(img.size(), CV_8UC1);
	dstImage = 255 - dstImage;
	cvtColor(dstImage, dstImage, COLOR_GRAY2BGR);
	cv::circle(mask, cv::Point(mask.cols / 2, mask.rows / 2), 2000, cv::Scalar(255, 255, 255), -1, 8, 0);
	img.copyTo(dstImage, mask);


	
	returnMinT.clear();

	Mat imageIn = dstImage;
	Mat image = imageIn.clone();

	if (image.channels() > 2) {
		cvtColor(image, image, COLOR_BGR2GRAY);
	}

	Mat imageC;
	cvtColor(image, imageC, COLOR_GRAY2BGR);

	vector<vector<Point>> contours_1;
	vector<Contour> contours;
	vector<Contour> uniqueContours;
	vector<Contour> usableUniqueConts;
	vector<Vec4i> hierarchy;

	int64 t0 = getCPUTickCount();
	EdgesSubPix(image, alpha, low, high, contours, hierarchy, mode, contours_1);
	int64 t1 = getCPUTickCount();
	cout << "Time to execute EdgesSubPix : " << t1 - t0 << endl;
	//	drawContours(imageC, contours_1, -1, Scalar(255, 0, 0), 2);
	cout << "Contours_1 size :: " << contours_1.size() << endl;


	vector < vector<cv::Point2f>> conts;
	vector<RotatedRect> finalConts;
	vector<RotatedRect> finalContsWoBig;

	for (size_t i = 0; i < contours.size(); i++) {
		RotatedRect  brect = minAreaRect(contours[i].points);

		float heightR = max(brect.size.height, brect.size.width);
		float widthR = min(brect.size.height, brect.size.width);
		if (i == 0) {
			finalConts.push_back(brect);
			uniqueContours.push_back(contours[i]);
		}

		bool unique = true;
		for (size_t j = 0; j < finalConts.size(); j++) {
			if ((abs(finalConts[j].center.x - brect.center.x) < 5) && (abs(finalConts[j].center.y - brect.center.y) < 5))
			{
				float height = max(finalConts[j].size.height, finalConts[j].size.width);
				float width = min(finalConts[j].size.height, finalConts[j].size.width);
				float hdiff = abs(height - heightR);
				float wdiff = abs(width - widthR);
				if (hdiff < 8 && wdiff < 8)
				{
					if (height < heightR && width < widthR)
					{
						finalConts.at(j) = brect;

					}
					//std::cout << "cont not unique ::" << i << std::endl;
					unique = false;
					break;
				}

			}


		}
		if (unique == true) {
			finalConts.push_back(brect);
			uniqueContours.push_back(contours[i]);

		}


	}
	cout << "Final Conts size :: " << finalConts.size() << endl;
	cout << "Unique Contours size  :: " << uniqueContours.size() << endl;

	Point2f vertices[4];
	int contValid = 0;
	Contour contInner;
	Contour contOuter;
	usableUniqueConts.clear();
	for (size_t uq = 0; uq < uniqueContours.size(); uq++) {

		RotatedRect brect = minAreaRect(uniqueContours[uq].points);

		if (brect.size.height > 50 && brect.size.width > 50 && brect.size.height < 3000 && brect.size.width < 3000)
		{
			//rectangle(img, brect.boundingRect(), Scalar(0, 0, 255), 2);
			for (int cntIn = 0; cntIn < uniqueContours[uq].points.size(); cntIn += 10) {

				circle(imageC, (Point)uniqueContours[uq].points[cntIn], 7, Scalar(0, 160, 10), 1);

			}

			usableUniqueConts.push_back(uniqueContours[uq]);

		}
	}
	cout << " Usable Unique Conts size :: " << usableUniqueConts.size() << endl;
	for (int oi = 0; oi < usableUniqueConts.size(); oi++) {

		if (oi == 0) {
			contInner = usableUniqueConts[oi];
			contOuter = usableUniqueConts[oi];
		}

		RotatedRect brect = minAreaRect(contInner.points);
		float heigthInner = max(brect.size.height, brect.size.width);
		float widthInner = min(brect.size.height, brect.size.width);

		brect = minAreaRect(contOuter.points);
		float heightOuter = max(brect.size.height, brect.size.width);
		float widthOuter = min(brect.size.height, brect.size.width);

		brect = minAreaRect(usableUniqueConts[oi].points);

		float heightR = max(brect.size.height, brect.size.width);
		float widthR = min(brect.size.height, brect.size.width);

		if (heightR > heightOuter && widthR > widthOuter) {
			contOuter = usableUniqueConts[oi];
		}

		if (heightR < heigthInner && widthR < widthInner) {
			contInner = usableUniqueConts[oi];
		}


	}
	/*cout << "Cont Inner Initial ::" << contInner.points.size() << endl;
	cout << "Cont Outer Initial :: " << contOuter.points.size() << endl;*/

	Point2f minThickPtIn = Point(0, 0);
	Point2f minThickPtOut = Point(0, 0);
	float minthick = 10000;
	vector <Point2f> minDistMatch;


	for (int cntIn = 0; cntIn < contInner.points.size(); cntIn++) {
		float minD = 0.0;
		Point2f minDpt = Point2f(0, 0);
		for (int cntOut = 0; cntOut < contOuter.points.size(); cntOut++) {
			float dist = distancePoint(contInner.points[cntIn], contOuter.points[cntOut]);
			if (cntOut == 0)
			{
				minD = dist;
				minDpt = contOuter.points[cntOut];

			}
			if (dist < minD)
			{
				minD = dist;
				minDpt = contOuter.points[cntOut];

			}
			if (minD < minthick)
			{
				minthick = minD;
				minThickPtIn = contInner.points[cntIn];
				minThickPtOut = minDpt;
				//	std::cout << "MinThickPTin " << minThickPtIn << endl;
				//	std::cout << "MinThickPTOut " << minThickPtOut << endl;

			}
			//cout<<"minPt"<<cntIn<<"::"<<minDpt<<"___dist::"<<minD<<endl;
			//circle(imageC,(Point)minDpt,10,Scalar(0,0,180),4);
			minDistMatch.push_back(minDpt);

		}
	}

	line(imageC, (Point)minThickPtIn, (Point)minThickPtOut, Scalar(0, 180, 0), 1);
	float minthickness = distancePoint(minThickPtIn, minThickPtOut) * mmPerPixProp;
	cout << "Minimum Thickness :: " << minThickness << endl;
	minThicknessProp = minthickness;

	// ----------------------get center of inner contour------------------

	vector <Point2f> tempPoints;
	std::vector<vector<cv::Point2f>> convex_hull(2);
	cv::approxPolyDP(cv::Mat(contInner.points), tempPoints, 2, true);

	// convex hull
	cv::convexHull(tempPoints, convex_hull[0], false);
	if (convex_hull[0].size() < 3) return false;

	// center of gravity
	cv::Moments mo = cv::moments(convex_hull[0]);
	Point2f centInner = cv::Point2f(mo.m10 / mo.m00, mo.m01 / mo.m00);
	cv::circle(imageC, centInner, 7, Scalar(180, 0, 0), 1);

	//----------------------center of inner cont end---------------------
	tempPoints.clear();
	cv::approxPolyDP(cv::Mat(contOuter.points), tempPoints, 2, true);

	// Convex hull
	cv::convexHull(tempPoints, convex_hull[1], false);
	if (convex_hull[1].size() < 3) return false;
	//	cout << "Convex hull size : small ::" << convex_hull[0] << endl;
	//	cout << "Convex hull size : outer ::" << convex_hull[1] << endl;
		// center of gravity
	mo = cv::moments(convex_hull[1]);
	Point2f centOuter = cv::Point2f(mo.m10 / mo.m00, mo.m01 / mo.m00);
	cv::circle(imageC, centOuter, 7, Scalar(180, 0, 0), 2);

	RotatedRect rrOuter = fitEllipse(contOuter.points);
	std::cout << "cable dia" << rrOuter.size << endl;
	//mmPerPix = 20.818/ ((rrOuter.size.height + rrOuter.size.width) / 2);
	std::cout << "mmperPix= " << mmPerPixProp << endl;
	float cableDiamm = ((rrOuter.size.height + rrOuter.size.width) / 2) * mmPerPixProp;
	outerDiaProp = cableDiamm;
	std::cout << "cable dia mm " << cableDiamm << endl;
	cout << "minthickness mm :: " << minthick * mmPerPixProp << endl;


	vector <vector<Point2f>> sectorsInner(6);
	vector <vector<Point2f>> sectorsOuter(6);
	/*cout << "inner Size" << contInner.points.size() << endl;
	cout << "outer Size" << contOuter.points.size() << endl;*/
	float currAngle = 0;
	for (int cntIn = 0; cntIn < contInner.points.size(); cntIn += 1) {
		currAngle = getAngle(centInner, minThickPtIn, centInner, contInner.points[cntIn]);
		int sector = 0;
		if (currAngle >= 330 || currAngle < 30)
		{
			sector = 0;
		}
		else
		{
			sector = (((int)currAngle - 30) / 60) + 1;
			//line(imageC, contOuter.points[cntIn], centInner, Scalar(0, 56, 225), 1);
		}
		sectorsInner[sector].push_back(contInner.points[cntIn]);
	}



	for (int cntOut = 0; cntOut < contOuter.points.size(); cntOut++) {
		currAngle = getAngle(centInner, minThickPtIn, centInner, contOuter.points[cntOut]);
		int sector = 0;
		if (currAngle >= 330 || currAngle < 30)
		{
			sector = 0;
			//  line(imageC, contOuter.points[cntOut], centInner, Scalar(0, 123, 116), 1);
		}
		else
		{
			sector = (((int)currAngle - 30) / 60) + 1;
		}
		//cout<<"sector   ::"<<sector<<endl;
		sectorsOuter[sector].push_back(contOuter.points[cntOut]);
	}


	//cout << "Sector " << sectorsOuter.size() << endl;

	//---------------Draw Sector-------------------------
	vector <thicknessPt> minThickData(sectorsInner.size());
	srand(time(0));
	for (int sec = 0; sec < sectorsInner.size(); sec++)
	{
		float mindist = 100000;
		Scalar randomColor(

			(double)std::rand() / RAND_MAX * 225,
			(double)std::rand() / RAND_MAX * 225,
			(double)std::rand() / RAND_MAX * 225
		);
		for (int inn = 0; inn < sectorsInner[sec].size(); inn++)
		{
			/*if (sec == 0)
				circle(imageC, sectorsInner[sec][inn], 5, randomColor, -1);
			else if (sec == 1)
				circle(imageC, sectorsInner[sec][inn], 5, randomColor, -1);
			else if (sec == 2)
				circle(imageC, sectorsInner[sec][inn], 5, randomColor, -1);
			else if (sec == 3)
				circle(imageC, sectorsInner[sec][inn], 5, randomColor, -1);
			else if (sec == 4)
				circle(imageC, sectorsInner[sec][inn], 5, randomColor, -1);
			else if (sec == 5)
				circle(imageC, sectorsInner[sec][inn], 5, randomColor, -1);*/

			for (int outr = 0; outr < sectorsOuter[sec].size(); outr++)

			{


				float currDist = distancePoint(sectorsInner[sec][inn], sectorsOuter[sec][outr]);
				if (currDist < mindist)
				{
					mindist = currDist;
					minThickData[sec].distance = mindist;
					minThickData[sec].innerPt = sectorsInner[sec][inn];
					minThickData[sec].outerPt = sectorsOuter[sec][outr];

				}

			}


		}
		line(imageC, minThickData[sec].innerPt, minThickData[sec].outerPt, Scalar(0, 60, 250), 2);
		/*if (drawDiameter(cableDiamm) < 1) {
			cv::rectangle(imageC, Point(minThickData[sec].outerPt.x, minThickData[sec].outerPt.y + 3), Point(minThickData[sec].outerPt.x + 72, minThickData[sec].outerPt.y - 20), Scalar(0, 150, 255), -1);
		}
		else if (drawDiameter(cableDiamm) >= 1 && drawDiameter(cableDiamm) < 2) {

			cv::rectangle(imageC, Point(minThickData[sec].outerPt.x, minThickData[sec].outerPt.y + 3), Point(minThickData[sec].outerPt.x + 155, minThickData[sec].outerPt.y - 28), Scalar(0, 150, 255), -1);

		}
		else if (drawDiameter(cableDiamm) >= 2 && drawDiameter(cableDiamm) < 3) {
			cv::rectangle(imageC, Point(minThickData[sec].outerPt.x, minThickData[sec].outerPt.y + 5), Point(minThickData[sec].outerPt.x + 280, minThickData[sec].outerPt.y - 50), Scalar(0, 150, 255), -1);
		}
		else if (drawDiameter(cableDiamm) >= 3 && drawDiameter(cableDiamm) <= 4) {
			rectangle(imageC, Point(minThickData[sec].outerPt.x, minThickData[sec].outerPt.y + 3), Point(minThickData[sec].outerPt.x + 525, minThickData[sec].outerPt.y - 88), Scalar(0, 150, 255), -1);
		}*/
		cv::rectangle(imageC, Point(minThickData[sec].outerPt.x, minThickData[sec].outerPt.y + 3), Point(minThickData[sec].outerPt.x + (drawDiameter(cableDiamm) * 149), minThickData[sec].outerPt.y - (drawDiameter(cableDiamm) * 32)), Scalar(0, 150, 255), -1);

		cv::putText(imageC, cv::format("d=%3.3f", minThickData[sec].distance * mmPerPix), minThickData[sec].outerPt, FONT_HERSHEY_COMPLEX, drawDiameter(cableDiamm), Scalar(0, 0, 0), 1);
	}
	/*if (drawDiameter(cableDiamm) < 1) {
		cv::rectangle(imageC, Point(centOuter.x, centOuter.y + 5), Point(centOuter.x + 85, centOuter.y - 28), Scalar(0, 150, 255), -1);
	}
	else if (drawDiameter(cableDiamm) >= 1 && drawDiameter(cableDiamm) < 2) {

		cv::rectangle(imageC, Point(centOuter.x, centOuter.y + 5), Point(centOuter.x + 175, centOuter.y - 28), Scalar(0, 150, 255), -1);

	}
	else if (drawDiameter(cableDiamm) >= 2 && drawDiameter(cableDiamm) < 3) {
		cv::rectangle(imageC, Point(centOuter.x, centOuter.y + 5), Point(centOuter.x + 355, centOuter.y - 50), Scalar(0, 150, 255), -1);
	}
	else if (drawDiameter(cableDiamm) >= 3 && drawDiameter(cableDiamm) <= 4) {
		rectangle(imageC, Point(centOuter.x, centOuter.y + 5), Point(centOuter.x + 150, centOuter.y - 28), Scalar(0, 150, 255), -1);
	}*/
	cv::rectangle(imageC, Point(centOuter.x, centOuter.y + 5), Point(centOuter.x + (drawDiameter(cableDiamm) * 179), centOuter.y - (drawDiameter(cableDiamm) * 38)), Scalar(0, 150, 255), -1);

	cv::putText(imageC, cv::format("OD=%3.3f", cableDiamm), centOuter, FONT_HERSHEY_COMPLEX, drawDiameter(cableDiamm), Scalar(0, 0, 0), 1);
	float maxThick = 0;
	for (int ac = 0; ac < minThickData.size(); ac++)
	{
		avgThickness += minThickData[ac].distance * mmPerPixProp;
		std::cout << "minthick dist  :: " << minThickData[ac].distance * mmPerPixProp << endl;
		if (minThickData[ac].distance * mmPerPixProp > maxThick)
		{
			maxThick = minThickData[ac].distance * mmPerPixProp;
		}
		returnMinT.push_back(minThickData[ac]);
	}

	sectorCountProp = returnMinT.size();
	std::cout << "return thickness vec size" << returnMinT.size() << endl;
	concentricityProp = (minThicknessProp / maxThick) * 100;
	std::cout << "concentricity::" << concentricityProp << endl;
	avgThicknessProp = avgThickness / 4.0;
	std::cout << "minthick sum  :: " << avgThickness << endl;
	std::cout << "minthick avg :: " << avgThicknessProp << endl;
	std::cout << " " << endl;

	cv::imwrite("resullt4core.bmp", imageC);

	//cv::resize(imageC, imageIn, imageIn.size());

	//namedWindow("Display frame", WINDOW_NORMAL);

	//maxId = getMaxAreaContour(contours_1);
	maxId = maxAreaContour(usableUniqueConts);
	ovality_1 = calculate_ovality(usableUniqueConts, maxId, centOuter, cableDiamm, mmPerPixProp)  ;
	cout << "Ovallity_1 " << ovality_1 << "%" << endl;
	ovalityProp = ovality_1;

	//Rect boundRect_1 = boundingRect(contours_1[maxId]);//enclose in Rect

	//Mat roi;
	//boundRect_1 = Rect(boundRect_1.tl().x - 75, boundRect_1.tl().y - 75, boundRect_1.width + 300, boundRect_1.height + 300);
	//roi = imageC(boundRect_1);


	//resShow("Image C", imageC);
	resize(imageC, img, img.size());

	return 0;
}

int algorithmLib::Class1::getMeasurements7c(System::Drawing::Bitmap^ rawImg) {

	Mat img = BitmapToMat(rawImg);
	int maxId{};
	if (img.empty()) {

		cout << "Image not found" << endl;
		return -1;
	}


	cv::Mat mask = cv::Mat::zeros(img.size(), img.type());

	//Define your destination image
	cv::Mat dstImage = cv::Mat::zeros(img.size(), CV_8UC1);
	dstImage = 255 - dstImage;
	cvtColor(dstImage, dstImage, COLOR_GRAY2BGR);
	cv::circle(mask, cv::Point(mask.cols / 2, mask.rows / 2), 2000, cv::Scalar(255, 255, 255), -1, 8, 0);
	img.copyTo(dstImage, mask);



	returnMinT.clear();

	Mat imageIn = dstImage;
	Mat image = imageIn.clone();

	if (image.channels() > 2) {
		cvtColor(image, image, COLOR_BGR2GRAY);
	}

	Mat imageC;
	cvtColor(image, imageC, COLOR_GRAY2BGR);

	vector<vector<Point>> contours_1;
	vector<Contour> contours;
	vector<Contour> uniqueContours;
	vector<Contour> usableUniqueConts;
	vector<Vec4i> hierarchy;

	int64 t0 = getCPUTickCount();
    EdgesSubPix(image, alpha, low, high, contours, hierarchy, mode, contours_1);
	int64 t1 = getCPUTickCount();
	cout << "Time to execute EdgesSubPix : " << t1 - t0 << endl;
	drawContours(imageC, contours_1, -1, Scalar(255, 0, 0), 2);
	cout << "Contours_1 size :: " << contours_1.size() << endl;


	vector < vector<cv::Point2f>> conts;
	vector<RotatedRect> finalConts;
	vector<RotatedRect> finalContsWoBig;

	for (size_t i = 0; i < contours.size(); i++) {
		RotatedRect  brect = minAreaRect(contours[i].points);

		float heightR = max(brect.size.height, brect.size.width);
		float widthR = min(brect.size.height, brect.size.width);
		if (i == 0) {
			finalConts.push_back(brect);
			uniqueContours.push_back(contours[i]);
		}

		bool unique = true;
		for (size_t j = 0; j < finalConts.size(); j++) {
			if ((abs(finalConts[j].center.x - brect.center.x) < 5) && (abs(finalConts[j].center.y - brect.center.y) < 5))
			{
				float height = max(finalConts[j].size.height, finalConts[j].size.width);
				float width = min(finalConts[j].size.height, finalConts[j].size.width);
				float hdiff = abs(height - heightR);
				float wdiff = abs(width - widthR);
				if (hdiff < 8 && wdiff < 8)
				{
					if (height < heightR && width < widthR)
					{
						finalConts.at(j) = brect;

					}

					unique = false;
					break;
				}

			}


		}
		if (unique == true) {
			finalConts.push_back(brect);
			uniqueContours.push_back(contours[i]);

		}


	}
	cout << "Final Conts size :: " << finalConts.size() << endl;
	cout << "Unique Contours size  :: " << uniqueContours.size() << endl;

	Point2f vertices[4];
	int contValid = 0;
	Contour contInner;
	Contour contOuter;
	usableUniqueConts.clear();
	for (size_t uq = 0; uq < uniqueContours.size(); uq++) {

		RotatedRect brect = minAreaRect(uniqueContours[uq].points);
		///*cout << "Brect height " << brect.size.height << endl;
		//cout << "Brect Width " << brect.size.width << endl;*/
		if (brect.size.height > 500 && brect.size.width > 500 && brect.size.height < 3000 && brect.size.width < 3000)
		{
			//rectangle(imageC, brect.boundingRect(), Scalar(0, 0, 255), 2);
			for (int cntIn = 0; cntIn < uniqueContours[uq].points.size(); cntIn += 10) {

				circle(imageC, (Point)uniqueContours[uq].points[cntIn], 10, Scalar(0, 160, 10), 1);

			}

			usableUniqueConts.push_back(uniqueContours[uq]);

		}
	}
	cout << " Usable Unique Conts size :: " << usableUniqueConts.size() << endl;
	if (usableUniqueConts.size() != 2) {
		cout << "Contours not found properly" << endl;
		return -1;
	}
	for (int oi = 0; oi < usableUniqueConts.size(); oi++) {

		if (oi == 0) {
			contInner = usableUniqueConts[oi];
			contOuter = usableUniqueConts[oi];
		}

		RotatedRect brect = minAreaRect(contInner.points);
		float heigthInner = max(brect.size.height, brect.size.width);
		float widthInner = min(brect.size.height, brect.size.width);

		brect = minAreaRect(contOuter.points);
		float heightOuter = max(brect.size.height, brect.size.width);
		float widthOuter = min(brect.size.height, brect.size.width);

		brect = minAreaRect(usableUniqueConts[oi].points);

		float heightR = max(brect.size.height, brect.size.width);
		float widthR = min(brect.size.height, brect.size.width);

		if (heightR > heightOuter && widthR > widthOuter) {
			contOuter = usableUniqueConts[oi];
		}

		if (heightR < heigthInner && widthR < widthInner) {
			contInner = usableUniqueConts[oi];
		}


	}
	//cout << "Cont Inner Initial ::" << contInner.points.size() << endl;
	//cout << "Cont Outer Initial :: " << contOuter.points.size() << endl;

	Point2f minThickPtIn = Point(0, 0);
	Point2f minThickPtOut = Point(0, 0);
	float minthick = 10000;
	vector <Point2f> minDistMatch;


	for (int cntIn = 0; cntIn < contInner.points.size(); cntIn++) {
		float minD = 0.0;
		Point2f minDpt = Point2f(0, 0);
		for (int cntOut = 0; cntOut < contOuter.points.size(); cntOut++) {
			float dist = distancePoint(contInner.points[cntIn], contOuter.points[cntOut]);
			if (cntOut == 0)
			{
				minD = dist;
				minDpt = contOuter.points[cntOut];

			}
			if (dist < minD)
			{
				minD = dist;
				minDpt = contOuter.points[cntOut];

			}
			if (minD < minthick)
			{
				minthick = minD;
				minThickPtIn = contInner.points[cntIn];
				minThickPtOut = minDpt;
				//	std::cout << "MinThickPTin " << minThickPtIn << endl;
				//	std::cout << "MinThickPTOut " << minThickPtOut << endl;

			}
			//cout<<"minPt"<<cntIn<<"::"<<minDpt<<"___dist::"<<minD<<endl;
			//circle(imageC,(Point)minDpt,10,Scalar(0,0,180),4);
			minDistMatch.push_back(minDpt);

		}
	}

	line(imageC, (Point)minThickPtIn, (Point)minThickPtOut, Scalar(0, 180, 0), 1);
	float minthickness = distancePoint(minThickPtIn, minThickPtOut) * mmPerPixProp;
	cout << "Minimum Thickness :: " << minThickness << endl;
	minThicknessProp = minthickness;

	// ----------------------get center of inner contour------------------

	vector <Point2f> tempPoints;
	std::vector<vector<cv::Point2f>> convex_hull(2);
	cv::approxPolyDP(cv::Mat(contInner.points), tempPoints, 2, true);

	// convex hull
	cv::convexHull(tempPoints, convex_hull[0], false);
	if (convex_hull[0].size() < 3) return false;

	// center of gravity
	cv::Moments mo = cv::moments(convex_hull[0]);
	Point2f centInner = cv::Point2f(mo.m10 / mo.m00, mo.m01 / mo.m00);
	cv::circle(imageC, centInner, 7, Scalar(180, 0, 0), 1);

	//----------------------center of inner cont end---------------------
	tempPoints.clear();
	cv::approxPolyDP(cv::Mat(contOuter.points), tempPoints, 2, true);

	// Convex hull
	cv::convexHull(tempPoints, convex_hull[1], false);
	if (convex_hull[1].size() < 3) return false;
	//	cout << "Convex hull size : small ::" << convex_hull[0] << endl;
	//	cout << "Convex hull size : outer ::" << convex_hull[1] << endl;
		// center of gravity
	mo = cv::moments(convex_hull[1]);
	Point2f centOuter = cv::Point2f(mo.m10 / mo.m00, mo.m01 / mo.m00);
	cv::circle(imageC, centOuter, 7, Scalar(180, 0, 0), 2);

	RotatedRect rrOuter = fitEllipse(contOuter.points);
	std::cout << "cable dia" << rrOuter.size << endl;
	//mmPerPix = 20.818/ ((rrOuter.size.height + rrOuter.size.width) / 2);
	std::cout << "mmperPix= " << mmPerPixProp << endl;
	float cableDiamm = ((rrOuter.size.height + rrOuter.size.width) / 2) * mmPerPixProp;
	outerDiaProp = cableDiamm;
	std::cout << "cable dia mm " << cableDiamm << endl;
	cout << "minthickness mm :: " << minthick * mmPerPixProp << endl;
	

	vector <vector<Point2f>> sectorsInner(6);
	vector <vector<Point2f>> sectorsOuter(6);
	cout << "inner Size" << contInner.points.size() << endl;
	cout << "outer Size" << contOuter.points.size() << endl;
	float currAngle = 0;
	for (int cntIn = 0; cntIn < contInner.points.size(); cntIn += 1) {
		currAngle = getAngle(centInner, minThickPtIn, centInner, contInner.points[cntIn]);
		int sector = 0;
		if (currAngle >= 330 || currAngle < 30)
		{
			sector = 0;
		}
		else
		{
			sector = (((int)currAngle - 30) / 60) + 1;
			//line(imageC, contOuter.points[cntIn], centInner, Scalar(0, 56, 225), 1);
		}
		sectorsInner[sector].push_back(contInner.points[cntIn]);
	}



	for (int cntOut = 0; cntOut < contOuter.points.size(); cntOut++) {
		currAngle = getAngle(centInner, minThickPtIn, centInner, contOuter.points[cntOut]);
		int sector = 0;
		if (currAngle >= 330 || currAngle < 30)
		{
			sector = 0;
			//  line(imageC, contOuter.points[cntOut], centInner, Scalar(0, 123, 116), 1);
		}
		else
		{
			sector = (((int)currAngle - 30) / 60) + 1;
		}
		//cout<<"sector   ::"<<sector<<endl;
		sectorsOuter[sector].push_back(contOuter.points[cntOut]);
	}


	//---------------Draw Sector-------------------------
	vector <thicknessPt> minThickData(sectorsInner.size());
	srand(time(0));
	for (int sec = 0; sec < sectorsInner.size(); sec++)
	{
		float mindist = 100000;
		Scalar randomColor(

			(double)std::rand() / RAND_MAX * 225,
			(double)std::rand() / RAND_MAX * 225,
			(double)std::rand() / RAND_MAX * 225
		);
		for (int inn = 0; inn < sectorsInner[sec].size(); inn++)
		{
			if (sec == 0)
				circle(imageC, sectorsInner[sec][inn], 5, randomColor, -1);
			else if (sec == 1)
				circle(imageC, sectorsInner[sec][inn], 5, randomColor, -1);
			else if (sec == 2)
				circle(imageC, sectorsInner[sec][inn], 5, randomColor, -1);
			else if (sec == 3)
				circle(imageC, sectorsInner[sec][inn], 5, randomColor, -1);
			else if (sec == 4)
				circle(imageC, sectorsInner[sec][inn], 5, randomColor, -1);
			else if (sec == 5)
				circle(imageC, sectorsInner[sec][inn], 5, randomColor, -1);

			for (int outr = 0; outr < sectorsOuter[sec].size(); outr++)

			{


				float currDist = distancePoint(sectorsInner[sec][inn], sectorsOuter[sec][outr]);
				if (currDist < mindist)
				{
					mindist = currDist;
					minThickData[sec].distance = mindist;
					minThickData[sec].innerPt = sectorsInner[sec][inn];
					minThickData[sec].outerPt = sectorsOuter[sec][outr];

				}

			}


		}
		line(imageC, minThickData[sec].innerPt, minThickData[sec].outerPt, Scalar(0, 60, 250), 3);
		cv::rectangle(imageC, Point(minThickData[sec].outerPt.x, minThickData[sec].outerPt.y + 3), Point(minThickData[sec].outerPt.x + (drawDiameter(cableDiamm) * 149), minThickData[sec].outerPt.y - (drawDiameter(cableDiamm) * 32)), Scalar(0, 150, 160), -1);
		/*if (drawDiameter(cableDiamm) < 1) {
			cv::rectangle(imageC, Point(minThickData[sec].outerPt.x, minThickData[sec].outerPt.y + 3), Point(minThickData[sec].outerPt.x + 72, minThickData[sec].outerPt.y - 20), Scalar(0, 150, 255), -1);
		}
		else if (drawDiameter(cableDiamm) >= 1 && drawDiameter(cableDiamm) < 2) {

			cv::rectangle(imageC, Point(minThickData[sec].outerPt.x, minThickData[sec].outerPt.y + 3), Point(minThickData[sec].outerPt.x + 155, minThickData[sec].outerPt.y - 28), Scalar(0, 150, 255), -1);

		}
		else if (drawDiameter(cableDiamm) >= 2 && drawDiameter(cableDiamm) < 3) {
			cv::rectangle(imageC, Point(minThickData[sec].outerPt.x, minThickData[sec].outerPt.y + 5), Point(minThickData[sec].outerPt.x + 280, minThickData[sec].outerPt.y - 50), Scalar(0, 150, 255), -1);
		}
		else if (drawDiameter(cableDiamm) >= 3 && drawDiameter(cableDiamm) <= 4) {
			rectangle(imageC, Point(minThickData[sec].outerPt.x, minThickData[sec].outerPt.y + 3), Point(minThickData[sec].outerPt.x + 525, minThickData[sec].outerPt.y - 88), Scalar(0, 150, 255), -1);
		}*/
		cv::putText(imageC, cv::format("d=%3.3f", minThickData[sec].distance * mmPerPix), minThickData[sec].outerPt, FONT_HERSHEY_COMPLEX, drawDiameter(cableDiamm), Scalar(0, 0, 0), 2);
	}
	/*if(drawDiameter(cableDiamm) < 1) {
		cv::rectangle(imageC, Point(centOuter.x, centOuter.y + 5), Point(centOuter.x + 85, centOuter.y - 28), Scalar(0, 150, 255), -1);
	}
	else if (drawDiameter(cableDiamm) >= 1 && drawDiameter(cableDiamm) < 2) {

	cv::rectangle(imageC, Point(centOuter.x, centOuter.y + 5), Point(centOuter.x + 175, centOuter.y - 28), Scalar(0, 150, 255), -1);

	}
	else if (drawDiameter(cableDiamm) >= 2 && drawDiameter(cableDiamm) < 3) {
	cv::rectangle(imageC, Point(centOuter.x, centOuter.y + 5), Point(centOuter.x + 355, centOuter.y - 50), Scalar(0, 150, 255), -1);
	}
	else if (drawDiameter(cableDiamm) >= 3 && drawDiameter(cableDiamm) <= 4) {
	rectangle(imageC, Point(centOuter.x, centOuter.y + 5), Point(centOuter.x + 150, centOuter.y - 28), Scalar(0, 150, 255), -1);
	}*/
	cv::rectangle(imageC, Point(centOuter.x, centOuter.y + 5), Point(centOuter.x + (drawDiameter(cableDiamm) * 179), centOuter.y - (drawDiameter(cableDiamm) * 38)), Scalar(0, 150, 255), -1);

	cv::putText(imageC, cv::format("OD=%3.3f", cableDiamm), centOuter, FONT_HERSHEY_COMPLEX, drawDiameter(cableDiamm), Scalar(0, 0, 255), 2);

	float maxThick = 0;
	for (int ac = 0; ac < minThickData.size(); ac++)
	{
		avgThickness += minThickData[ac].distance * mmPerPixProp;
		std::cout << "minthick dist  :: " << minThickData[ac].distance * mmPerPixProp << endl;
		if (minThickData[ac].distance * mmPerPixProp > maxThick)
		{
			maxThick = minThickData[ac].distance * mmPerPixProp;
		}
		returnMinT.push_back(minThickData[ac]);
	}

	sectorCountProp = returnMinT.size();
	std::cout << "return thickness vec size" << returnMinT.size() << endl;
	concentricityProp = (minThicknessProp / maxThick) * 100;
	std::cout << "concentricity::" << concentricityProp << endl;
	avgThicknessProp = avgThickness / 4.0;
	std::cout << "minthick sum  :: " << avgThickness << endl;
	std::cout << "minthick avg :: " << avgThicknessProp << endl;
	std::cout << " " << endl;

	cv::imwrite("resullt4core.bmp", imageC);

	//cv::resize(imageC, imageIn, imageIn.size());

//	namedWindow("Display frame", WINDOW_NORMAL);

	//maxId = getMaxAreaContour(contours_1);
	//Rect boundRect_1 = boundingRect(contours_1[maxId]);   //enclose in Rect

	//Mat roi;
	//boundRect_1 = Rect(boundRect_1.tl().x - 70, boundRect_1.tl().y - 100, boundRect_1.width + 280, boundRect_1.height + 200);
	//roi = imageC(boundRect_1);

	/*imshow("Display frame", roi);
	waitKey(0);

	resShow("Image C", imageC);*/
	resize(imageC, img, img.size());
	return 0;
}

int algorithmLib::Class1::getMeasurements8c(System::Drawing::Bitmap^ rawImg) {

	int maxId{};
	float ovality_1{};
	Mat img = BitmapToMat(rawImg);
	vector<thicknessPt> returnMinT0(6);
	vector<thicknessPt> returnMinT1(6);
	vector<thicknessPt> returnMinT2(6);
	returnMinT0.clear();
	returnMinT1.clear();
	returnMinT2.clear();

	if (img.empty()) {

		cout << "Image not found" << endl;
		return -1;
	}


	cv::Mat mask = cv::Mat::zeros(img.size(), img.type());

	//Define your destination image
	cv::Mat dstImage = cv::Mat::zeros(img.size(), CV_8UC1);
	dstImage = 255 - dstImage;
	cvtColor(dstImage, dstImage, COLOR_GRAY2BGR);
	cv::circle(mask, cv::Point(mask.cols / 2, mask.rows / 2), 2500, cv::Scalar(255, 255, 255), -1, 8, 0);
	img.copyTo(dstImage, mask);
	/*
	  namedWindow("Display frame", WINDOW_AUTOSIZE);
	  imshow("Display frame", img);
	  waitKey(0);
	  imshow("Display frame", mask);
	  waitKey(0);
	  imshow("Display frame", dstImage);
	  waitKey(0);*/

	Mat image = dstImage.clone();

	if (image.channels() > 2) {
		cvtColor(image, image, COLOR_BGR2GRAY);
	}

	Mat imageC;
	cvtColor(image, imageC, COLOR_GRAY2BGR);

	vector<vector<Point>> contours_1;
	vector<Contour> contours;
	vector<Contour> uniqueContours;
	vector<Contour> usableUniqueConts;
	vector<Vec4i> hierarchy;

	int64 t0 = getCPUTickCount();
	EdgesSubPix(image, alpha, low, high, contours, hierarchy, mode, contours_1);
	int64 t1 = getCPUTickCount();

	cout << "Contours_1 size " << contours_1.size() << endl;

	vector<vector<Point2f>> conts;
	vector<RotatedRect> finalConts;
	vector<RotatedRect> finalContsWoBig;

	for (int i = 0; i < contours.size(); i++) {
		RotatedRect brect = minAreaRect(contours[i].points);

		float heightR = max(brect.size.height, brect.size.width);
		float widthR = min(brect.size.height, brect.size.width);
		if (i == 0) {
			finalConts.push_back(brect);
			uniqueContours.push_back(contours[i]);
		}
		bool unique = true;
		for (size_t j = 0; j < finalConts.size(); j++) {
			if ((abs(finalConts[j].center.x - brect.center.x) < 5) && (abs(finalConts[j].center.y - brect.center.y) < 5))
			{

				float height = max(finalConts[j].size.height, finalConts[j].size.width);
				float width = min(finalConts[j].size.height, finalConts[j].size.width);
				float hdiff = abs(height - heightR);
				float wdiff = abs(width - widthR);
				if (hdiff < 8 && wdiff < 8)
				{
					if (height < heightR && width < widthR)
					{
						finalConts.at(j) = brect;

					}

					unique = false;
					break;
				}

			}


		}
		if (unique == true) {
			finalConts.push_back(brect);
			uniqueContours.push_back(contours[i]);

		}

	}
	cout << "final Conts " << finalConts.size() << endl;
	cout << "Unique Contours " << uniqueContours.size() << endl;



	//int minDist = 10 ;
	vector<Contour> myContour(uniqueContours.size());
	std::vector<int> isCopied(uniqueContours.size());

	for (int i = 0; i < uniqueContours.size(); i++) {

		cout << endl;
		cout << "UniqueContours[" << i << "] size :: " << uniqueContours[i].points.size() << endl;

	}



	//for (size_t i = 0; i < uniqueContours.size() - 1; ++i) {
	//	
	//	/*if (isCopied.at(i) == 2 || isCopied.at(i) == 1) {
	//		continue;
	//	}
	//*/
	//	isCopied.at(i) = 2;
	//	
	//	int count = i+1;

	//	for (size_t j = 0; j < uniqueContours[i].points.size(); ++j) {

	//		while(count < uniqueContours.size()) {

	//			        for (int z = 0; z < uniqueContours[count].points.size(); ++z) {
	//			        	int dist = static_cast<int>(distancePoint(uniqueContours.at(i).points.at(j), uniqueContours.at(count).points.at(z)));
	//			        	if (dist < minDist) {
	//			        		for (int x = 0; x < uniqueContours[count].points.size(); ++x) {
	//			        			uniqueContours[i].points.push_back(uniqueContours[count].points[x]);
	//								
	//			        		}
	//							for (int y = 0; y < uniqueContours[i].points.size(); ++y) {
	//								myContour[0].points.push_back(uniqueContours[i].points[y]);
	//							}
	//							//isCopied.at(i) = 2;
	//							isCopied.at(count) = 1;
	//			        		
	//							
	//			        		break;
	//			        	}
	//			        	
	//			        		
	//			        }
	//			      
	//			        
	//			count++;
	//		}
	//		/*if (copied == true)
	//			break;*/

	//	}
	//	

	//}
	//drawContours_1(imageC, uniqueContours);
	mergeContour(imageC, uniqueContours, 3, myContour, isCopied);
	//drawContours_1(imageC, myContour, 0);
	/*for (int i = 0; i < uniqueContours.size(); i++) {

		cout << endl;
		cout << "UniqueContours[" << i << "] size ->" << uniqueContours[i].points.size() << endl;

	}*/
	//for (int i = 0; i < isCopied.size(); i++) {
	//	if (isCopied.at(i) == 1) {
	//	//	uniqueContours.erase(uniqueContours.begin() + i);
	//	}
	//}

	//for (auto x : Copied)
	//	cout << x << endl;


  //drawContours_1(imageC, myContour);
	//struct myClass {
	//	bool operator() (cv::Point pt1, cv::Point pt2) { return (pt1.y < pt2.y); }
	//} myObject;
	//cout << "myContour " << myContour[0].points.size() << endl;

	//std::sort(myContour[0].points.begin(), myContour[0].points.end(), myObject);

	//myContour[0].points.erase(std::unique(myContour[0].points.begin(), myContour[0].points.end()), myContour[0].points.end());

	//myContour[0].points.resize(std::distance(myContour[0].points.begin(), std::unique(myContour[0].points.begin(), myContour[0].points.end())));

	cout << "myContour " << myContour[0].points.size() << endl;
	usableUniqueConts.clear();

	usableUniqueConts.push_back(myContour[0]);



	//resShow("Display", imageC);

	cout << "UniqueContours " << uniqueContours.size() << endl;
	cout << "usable Unique Contours  " << usableUniqueConts.size() << endl;


	for (size_t uq = 0; uq < uniqueContours.size(); uq++)
	{
		RotatedRect brect = minAreaRect(uniqueContours[uq].points);

		if (brect.size.height > 800 && brect.size.width > 800 && brect.size.height < 3000 && brect.size.width < 3000) {

			//rectangle(imageC, brect.boundingRect(), Scalar(255, 0, 0), 3);

		 //   for (size_t inn = 0; inn < uniqueContours[uq].points.size(); inn += 10) {

			   //    // circle(imageC, (Point)uniqueContours[uq].points[inn], 10, Scalar(255, 0, 0), 2);
			//}
	  //     // resShow("Display Image", imageC);
			usableUniqueConts.push_back(uniqueContours[uq]);

		}

	}
	cout << "Usable Unique conts " << usableUniqueConts.size() << endl;
	if (usableUniqueConts.size() != 4) {
		cout << "Contours not found properly" << endl;
		return -1;
	}


	for (size_t uq = 0; uq < usableUniqueConts.size(); uq++) {

		RotatedRect brect = minAreaRect(usableUniqueConts[uq].points);

		if (brect.size.height > 300 && brect.size.width > 300 && brect.size.height < 3000 && brect.size.width < 3000) {
			for (size_t inn = 0; inn < usableUniqueConts[uq].points.size(); inn += 10) {
				circle(imageC, (Point)usableUniqueConts[uq].points[inn], 5, Scalar(255, 0, 0), 2);
			}
		}
	}

	cout << "Usable Unique Contours " << usableUniqueConts.size() << endl;
	Point2f vertices[4];
	int contValid = 0;
	Contour contInner;
	Contour contOuter;


	for (size_t oi = 0; oi < usableUniqueConts.size(); oi++) {
		if (oi == 0) {
			contInner = usableUniqueConts[oi];
			contOuter = usableUniqueConts[oi];
		}
		RotatedRect brect = minAreaRect(contInner.points);
		float heightInner = max(brect.size.height, brect.size.width);
		float widthInner = min(brect.size.height, brect.size.width);
		//	rectangle(imageC, brect.boundingRect(), Scalar(0, 255, 0), 2);
		brect = minAreaRect(contOuter.points);
		float heightOuter = max(brect.size.height, brect.size.width);
		float widthOuter = min(brect.size.height, brect.size.width);
		//rectangle(imageC, brect.boundingRect(), Scalar(0, 0, 255), 2);
		brect = minAreaRect(usableUniqueConts[oi].points);
		float heightR = max(brect.size.height, brect.size.width);
		float widthR = min(brect.size.height, brect.size.width);

		if (heightR > heightOuter && widthR > widthOuter) {
			contOuter = usableUniqueConts[oi];
		}
		if (heightR < heightInner && widthR < widthInner) {
			contInner = usableUniqueConts[oi];
		}

	}




	//Rect brect = boundingRect(contOuter.points);
	//rectangle(imageC, brect.tl(), brect.br(), Scalar(255, 0, 0), 5);

	float minthick = 10000;
	vector<Point2f> innerPoints;
	vector<Point2f> outerPoints;

	vector <Point2f> minDistMatch;//(contInner.points.size());
	for (size_t i = 0; i < usableUniqueConts.size() - 1; i++) {

		if (i == 1) {
			minthick = 20000;
		}
		int count = i + 1;

		Point2f minThickPtIn = Point(0, 0);
		Point2f minThickPtOut = Point(0, 0);

		for (int cntIn = 0; cntIn < usableUniqueConts[i].points.size(); cntIn += 1)
		{ // circle(imageC,(Point)contInner.points[cntIn],10,Scalar(180,0,0),4);
			float minD = 0;
			Point2f minDpt = Point2f(0, 0);
			for (int cntOut = 0; cntOut < usableUniqueConts[count].points.size(); cntOut++)
			{
				float dist = distancePoint(usableUniqueConts[i].points[cntIn], usableUniqueConts[count].points[cntOut]);
				//cout << " Distance " << dist << endl;
				if (cntOut == 0)
				{
					minD = dist;
					minDpt = usableUniqueConts[count].points[cntOut];

				}
				if (dist < minD)
				{
					minD = dist;
					minDpt = usableUniqueConts[count].points[cntOut];

				}
				//circle(imageC,contOuter.points[cntOut],1,Scalar(0,0,180),1);


			}
			if (minD < minthick)
			{
				minthick = minD;
				minThickPtIn = usableUniqueConts[i].points[cntIn];
				minThickPtOut = minDpt;
				//	std::cout << "MinThickPTin " << minThickPtIn << endl;
				//	std::cout << "MinThickPTOut " << minThickPtOut << endl;

			}
			//cout<<"minPt"<<cntIn<<"::"<<minDpt<<"___dist::"<<minD<<endl;
			//circle(imageC,(Point)minDpt,10,Scalar(0,0,180),4);
			minDistMatch.push_back(minDpt);
			//putText(imageC,cv::format("%3.2f",minD),minDpt,CV_FONT_HERSHEY_COMPLEX ,2,Scalar(0,125,180));
		//line(imageC,(Point)contInner.points[cntIn],(Point)minDpt,Scalar(0,180,0),10);

		}
		//	cout << "minthickness pix:: " << minthick << endl;
		cout << "Minthick Pt " << minThickPtIn << endl;
		cout << "MInThick pt out " << minThickPtOut << endl;
		innerPoints.push_back(minThickPtIn);
		outerPoints.push_back(minThickPtOut);
		line(imageC, (Point)minThickPtIn, (Point)minThickPtOut, Scalar(0, 180, 0), 4);
	}
	//	resShow("ImageC", imageC);

	vector <Point2f> tempPoints;
	std::vector<vector<cv::Point2f>> convex_hull(2);
	cv::approxPolyDP(cv::Mat(contInner.points), tempPoints, 2, true);

	// convex hull
	cv::convexHull(tempPoints, convex_hull[0], false);
	if (convex_hull[0].size() < 3) return false;

	// center of gravity
	cv::Moments mo = cv::moments(convex_hull[0]);
	Point2f centInner = cv::Point2f(mo.m10 / mo.m00, mo.m01 / mo.m00);
	cv::circle(imageC, centInner, 10, Scalar(180, 0, 0), 3);
	cout << "Cent Inner " << centInner << endl;
	//-------center of inner cont end
	tempPoints.clear();
	cv::approxPolyDP(cv::Mat(contOuter.points), tempPoints, 2, true);

	// Convex hull
	cv::convexHull(tempPoints, convex_hull[1], false);
	if (convex_hull[1].size() < 3) return false;
	//	cout << "Convex hull size : small ::" << convex_hull[0] << endl;
	//	cout << "Convex hull size : outer ::" << convex_hull[1] << endl;
	//	 center of gravity

	mo = cv::moments(convex_hull[1]);
	Point2f centOuter = cv::Point2f(mo.m10 / mo.m00, mo.m01 / mo.m00);
	cv::circle(imageC, centOuter, 10, Scalar(0, 0, 180), 3);
	cout << "Cent Outer " << centOuter << endl;
	/*for(int cntIn=0; cntIn<convex_hull[0].size(); cntIn+=1)
		{  circle(imageC,(Point)convex_hull[0][cntIn],10,Scalar(180,0,0),4);}

	for(int cntIn=0; cntIn<convex_hull[1].size(); cntIn+=1)
		{  circle(imageC,(Point)convex_hull[1][cntIn],10,Scalar(180,0,0),4);}*/
		//---center outer end

	RotatedRect rrOuter = fitEllipse(contOuter.points);
	std::cout << "cable dia" << rrOuter.size << endl;
	//mmPerPix = 20.818/ ((rrOuter.size.height + rrOuter.size.width) / 2);
	std::cout << "mmperPix= " << mmPerPixProp << endl;
	float cableDiamm = ((rrOuter.size.height + rrOuter.size.width) / 2) * mmPerPixProp;
	outerDiaProp = cableDiamm;
	std::cout << "cable dia mm" << cableDiamm << endl;
	//cout << "minthickness mm :: " << minthick * mmPerPixProp << endl;

//	resShow("ImageC", imageC);

	// vector <vector<Point2f>> sectorsInner(6);
	//vector <vector<Point2f>> sectorsOuter(6);
	//cout << "inner Size" << contInner.points.size() << endl;
	//cout << "outer Size" << contOuter.points.size() << endl;
	//float currAngle = 0;
	//for (int cntIn = 0; cntIn < contInner.points.size(); cntIn += 1) {
	//	currAngle = getAngle(centInner, minThickPtIn, centInner, contInner.points[cntIn]);
	//	int sector = 0;
	//	if (currAngle >= 330 || currAngle < 30)
	//	{
	//		sector = 0;
	//	}
	//	else
	//	{
	//		sector = (((int)currAngle - 30) / 60) + 1;
	//		//line(imageC, contOuter.points[cntIn], centInner, Scalar(0, 56, 225), 1);
	//	}
	//	sectorsInner[sector].push_back(contInner.points[cntIn]);
	//}
	vector<vector<Point2f>> sectorsInner_1(6);
	vector<vector<Point2f>> sectorsInner_2(6);
	vector<vector<Point2f>> sectorsInner_3(6);

	vector<vector<Point2f>> sectorsOuter_1(6);
	vector<vector<Point2f>> sectorsOuter_2(6);
	vector<vector<Point2f>> sectorsOuter_3(6);

	float curAngle = 0;

	for (int i = 0; i < usableUniqueConts.size() - 1; ++i) {
		int count = i + 1;
		vector <Point2f> tempPoints_1;
		vector<vector<Point2f>> convex_null_1(2);
		Point inner{};

		if (i == 0)
			inner = innerPoints.at(0);
		else if (i == 1) {
			inner = innerPoints.at(1);
			approxPolyDP(usableUniqueConts[i].points, tempPoints_1, 2, true);
			convexHull(tempPoints_1, convex_null_1[0], true);
			if (convex_null_1[0].size() < 3) return false;
			cv::Moments mo = cv::moments(convex_null_1[0]);
			centInner = cv::Point2f(mo.m10 / mo.m00, mo.m01 / mo.m00);
		}

		else {
			inner = innerPoints.at(2);
			approxPolyDP(usableUniqueConts[2].points, tempPoints_1, 2, true);
			convexHull(tempPoints_1, convex_null_1[1], true);
			if (convex_null_1[1].size() < 3) return false;
			cv::Moments mo = cv::moments(convex_null_1[1]);
			centInner = cv::Point2f(mo.m10 / mo.m00, mo.m01 / mo.m00);
		}
		for (int j = 0; j < usableUniqueConts[i].points.size(); ++j) {
			curAngle = getAngle(centInner, inner, centInner, usableUniqueConts[i].points[j]);
			int sector = 0;
			if (curAngle >= 330 || curAngle < 30) {
				sector = 0;

			}
			else {
				sector = (((int)curAngle - 30) / 60) + 1;
			}
			if (i == 0)
				sectorsInner_1[sector].push_back(usableUniqueConts[i].points[j]);
			else if (i == 1)
				sectorsInner_2[sector].push_back(usableUniqueConts[i].points[j]);
			else
				sectorsInner_3[sector].push_back(usableUniqueConts[i].points[j]);
		}




		for (int x = 0; x < usableUniqueConts[count].points.size(); ++x) {
			curAngle = getAngle(centInner, inner, centInner, usableUniqueConts[count].points[x]);
			int sector = 0;
			if (curAngle >= 330 || curAngle < 30) {
				sector = 0;
			}
			else {
				sector = (((int)curAngle - 30) / 60) + 1;
			}
			if (i == 0)
				sectorsOuter_1[sector].push_back(usableUniqueConts[count].points[x]);
			else if (i == 1)
				sectorsOuter_2[sector].push_back(usableUniqueConts[count].points[x]);
			else
				sectorsOuter_3[sector].push_back(usableUniqueConts[count].points[x]);
		}
		tempPoints_1.clear();
	}




	//for (int cntOut = 0; cntOut < contOuter.points.size(); cntOut++) {
	//	currAngle = getAngle(centInner, minThickPtIn, centInner, contOuter.points[cntOut]);
	//	int sector = 0;
	//	if (currAngle >= 330 || currAngle < 30)
	//	{
	//		sector = 0;
	//		//  line(imageC, contOuter.points[cntOut], centInner, Scalar(0, 123, 116), 1);
	//	}
	//	else
	//	{
	//		sector = (((int)currAngle - 30) / 60) + 1;
	//	}
	//	//cout<<"sector   ::"<<sector<<endl;
	//	sectorsOuter[sector].push_back(contOuter.points[cntOut]);
	//}

	//
	////---------------Draw Sector-------------------------
	vector <thicknessPt> minThickData_1(sectorsInner_1.size());
	srand(time(0));

	for (int sec = 0; sec < sectorsInner_1.size(); sec++)
	{
		float mindist = 100000;
		Scalar randomColor(

			(double)std::rand() / RAND_MAX * 225,
			(double)std::rand() / RAND_MAX * 225,
			(double)std::rand() / RAND_MAX * 225
		);
		for (int inn = 0; inn < sectorsInner_1[sec].size(); inn++)
		{
			if (sec == 0)
				circle(imageC, sectorsInner_1[sec][inn], 5, randomColor, -1);
			else if (sec == 1)
				circle(imageC, sectorsInner_1[sec][inn], 5, randomColor, -1);
			else if (sec == 2)
				circle(imageC, sectorsInner_1[sec][inn], 5, randomColor, -1);
			else if (sec == 3)
				circle(imageC, sectorsInner_1[sec][inn], 5, randomColor, -1);
			else if (sec == 4)
				circle(imageC, sectorsInner_1[sec][inn], 5, randomColor, -1);
			else if (sec == 5)
				circle(imageC, sectorsInner_1[sec][inn], 5, randomColor, -1);

			for (int outr = 0; outr < sectorsOuter_1[sec].size(); outr++)

			{


				float currDist = distancePoint(sectorsInner_1[sec][inn], sectorsOuter_1[sec][outr]);
				if (currDist < mindist)
				{
					mindist = currDist;
					minThickData_1[sec].distance = mindist;
					minThickData_1[sec].innerPt = sectorsInner_1[sec][inn];
					minThickData_1[sec].outerPt = sectorsOuter_1[sec][outr];

				}

			}


		}
		line(imageC, minThickData_1[sec].innerPt, minThickData_1[sec].outerPt, Scalar(0, 60, 250), 7);

		//cv::putText(imageC, cv::format("d=%3.3f", minThickData0[sec].distance * mmPerPix), minThickData0[sec].outerPt, FONT_HERSHEY_COMPLEX, 3, Scalar(0, 0, 255),1);
	}
	cv::putText(imageC, cv::format("OD=%3.3f", cableDiamm), centOuter, FONT_HERSHEY_COMPLEX, 3, Scalar(0, 0, 255), 4);

	vector <thicknessPt> minThickData_2(sectorsInner_2.size());


	for (int sec = 0; sec < sectorsInner_2.size(); sec++)
	{
		float mindist = 100000;
		Scalar randomColor(

			(double)std::rand() / RAND_MAX * 225,
			(double)std::rand() / RAND_MAX * 225,
			(double)std::rand() / RAND_MAX * 225
		);
		for (int inn = 0; inn < sectorsInner_2[sec].size(); inn++)
		{
			if (sec == 0)
				circle(imageC, sectorsInner_2[sec][inn], 5, randomColor, -1);
			else if (sec == 1)
				circle(imageC, sectorsInner_2[sec][inn], 5, randomColor, -1);
			else if (sec == 2)
				circle(imageC, sectorsInner_2[sec][inn], 5, randomColor, -1);
			else if (sec == 3)
				circle(imageC, sectorsInner_2[sec][inn], 5, randomColor, -1);
			else if (sec == 4)
				circle(imageC, sectorsInner_2[sec][inn], 5, randomColor, -1);
			else if (sec == 5)
				circle(imageC, sectorsInner_2[sec][inn], 5, randomColor, -1);

			for (int outr = 0; outr < sectorsOuter_2[sec].size(); outr++)

			{


				float currDist = distancePoint(sectorsInner_2[sec][inn], sectorsOuter_2[sec][outr]);
				if (currDist < mindist)
				{
					mindist = currDist;
					minThickData_2[sec].distance = mindist;
					minThickData_2[sec].innerPt = sectorsInner_2[sec][inn];
					minThickData_2[sec].outerPt = sectorsOuter_2[sec][outr];

				}

			}


		}
		line(imageC, minThickData_2[sec].innerPt, minThickData_2[sec].outerPt, Scalar(0, 60, 250), 7);

		//cv::putText(imageC, cv::format("d=%3.3f", minThickData1[sec].distance * mmPerPix), minThickData1[sec].outerPt, FONT_HERSHEY_COMPLEX, 3, Scalar(0, 0, 255), 2);
	}

	vector <thicknessPt> minThickData_3(sectorsInner_3.size());


	for (int sec = 0; sec < sectorsInner_3.size(); sec++)
	{
		float mindist = 100000;
		Scalar randomColor(

			(double)std::rand() / RAND_MAX * 225,
			(double)std::rand() / RAND_MAX * 225,
			(double)std::rand() / RAND_MAX * 225
		);
		for (int inn = 0; inn < sectorsInner_3[sec].size(); inn++)
		{
			if (sec == 0)
				circle(imageC, sectorsInner_3[sec][inn], 5, randomColor, -1);
			else if (sec == 1)
				circle(imageC, sectorsInner_3[sec][inn], 5, randomColor, -1);
			else if (sec == 2)
				circle(imageC, sectorsInner_3[sec][inn], 5, randomColor, -1);
			else if (sec == 3)
				circle(imageC, sectorsInner_3[sec][inn], 5, randomColor, -1);
			else if (sec == 4)
				circle(imageC, sectorsInner_3[sec][inn], 5, randomColor, -1);
			else if (sec == 5)
				circle(imageC, sectorsInner_3[sec][inn], 5, randomColor, -1);

			for (int outr = 0; outr < sectorsOuter_3[sec].size(); outr++)

			{


				float currDist = distancePoint(sectorsInner_3[sec][inn], sectorsOuter_3[sec][outr]);
				if (currDist < mindist)
				{
					mindist = currDist;
					minThickData_3[sec].distance = mindist;
					minThickData_3[sec].innerPt = sectorsInner_3[sec][inn];
					minThickData_3[sec].outerPt = sectorsOuter_3[sec][outr];

				}

			}


		}
		line(imageC, minThickData_3[sec].innerPt, minThickData_3[sec].outerPt, Scalar(0, 60, 250), 7);

		//cv::putText(imageC, cv::format("d=%3.3f", minThickData2[sec].distance * mmPerPix), minThickData2[sec].outerPt, FONT_HERSHEY_COMPLEX, 3, Scalar(0, 0, 255), 2);
	}
	for (auto& c : minThickData_3) {
		cout << "c" << c.distance << endl;
	}
	
	float maxThick_1 = 0;
	float avgThickness_1{};
	for (int ac = 0; ac < minThickData_1.size(); ac++)
	{
		avgThickness_1 += minThickData_1[ac].distance * mmPerPixProp;
		std::cout << "minthick dist  :: " << minThickData_1[ac].distance * mmPerPixProp << endl;
		if (minThickData_1[ac].distance * mmPerPixProp > maxThick_1)
		{
			maxThick_1 = minThickData_1[ac].distance * mmPerPixProp;
		}
		returnMinT0.push_back(minThickData_1[ac]);
	}
	float maxThick_2 = 0;
	float avgThickness_2{};


	for (size_t t = 0; t < minThickData_2.size(); t++) {
		avgThickness_2 += minThickData_2[t].distance * mmPerPixProp;
		if (minThickData_2[t].distance * mmPerPixProp > maxThick_2) {
			maxThick_2 = minThickData_2[t].distance * mmPerPixProp;
		}
		returnMinT1.push_back(minThickData_2[t]);
	}

	float maxThick_3{};
	float avgThickness_3{};
	for (size_t t = 0; t < minThickData_3.size(); t++) {
		avgThickness_3 += minThickData_3[t].distance * mmPerPixProp;
		if (minThickData_3[t].distance * mmPerPixProp > maxThick_3)
			maxThick_3 = minThickData_3[t].distance * mmPerPixProp;

		returnMinT2.push_back(minThickData_3[t]);
	}
	for (int i = 0; i < minThickData_3.size(); i++) {
		cout << "m" << minThickData_3[i].distance << endl;
	}
	cout << "MIn thickdata size " << minThickData_3.size() << endl;
	vector < vector<thicknessPt>> returnMint;
	returnMint.push_back(returnMinT0);
	returnMint.push_back(returnMinT1);
	returnMint.push_back(returnMinT2);






	//for (int i = 0; i < returnmint.size(); i++) {
	//	for (int k = 0; k < returnmint[i].size(); k++) {
	//		cout << "min distance " << returnmint[i][k].distance << endl;
	//	}
	//}
	//drawContours_1(imageC, usableUniqueConts);
	float minThick_1{0};
	float minThick_2{0};
	float minThick_3{0};
	for (auto& c : minThickData_3) {
		minThick_3 = minThickData_3[0].distance;
		if (c.distance < minThick_3)
			minThick_3 = c.distance;
	}
	for (auto& c : minThickData_1) {
		minThick_1 = minThickData_1[0].distance;
		if (c.distance < minThick_1)
			minThick_1 = c.distance;
	}

	for (auto& c : minThickData_2) {
		cout << "This is loop " << minThick_2 << endl;
		minThick_2 = minThickData_2[0].distance;
		if (c.distance < minThick_2)
			minThick_2 = c.distance;
		
	}
	

	vector<float> concentricity_1(3);
	/*float concentricity_1{};
	float concentricity_2{};
	float concentricity_3{};*/
	sectorCountProp = returnMint.size();
	//std::cout << "return thickness vec size" << returnMint.size() << endl;
//	concentricityProp = (minThicknessProp / maxThick) * 100;
	concentricity_1[0] = (minThick_1 / maxThick_1) * 100;
	concentricity_1[1] = (minThick_2 / maxThick_2) * 100;
	concentricity_1[2] = (minThick_3 / maxThick_3) * 100;
	std::cout << "concentricity::" << concentricityProp << endl;
	avgThicknessProp = avgThickness / 4.0;

	std::cout << "minthick sum  :: " << avgThickness << endl;
	std::cout << "minthick avg :: " << avgThicknessProp << endl;
	std::cout << " " << endl;

	maxId = maxAreaContour(usableUniqueConts);
	ovality_1 = calculate_ovality(usableUniqueConts, maxId, centOuter, cableDiamm, mmPerPixProp)  ;
	cout << "Ovallity_1 " << ovality_1 << "%" << endl;
	ovalityProp = ovality_1;
	vector<float> distance;
	for (size_t i = 0; i < returnMinT0.size(); i++) {
		distance.push_back(returnMinT0[i].distance);
	}
	json j;
	struct formData values_1;
	values_1.minThick = minThick_1;
	values_1.avgThick = avgThickness_1;
	values_1.maxThick = maxThick_1;
	values_1.outerDia = cableDiamm;
	values_1.concentricity = concentricity_1[0];
	values_1.minThickData = distance;
	cout << "MaxThick_1 " << values_1.maxThick << endl;
	cout << "MaxThick_1 " << maxThick_1 << endl;
	j.push_back(values_1);
	distance.clear();
	for (size_t i = 0; i < returnMinT1.size(); i++) {
		distance.push_back(returnMinT1[i].distance);
	}
	values_1.minThick = minThick_2;
	values_1.avgThick = avgThickness_2;
	values_1.maxThick = maxThick_2;
	values_1.outerDia = cableDiamm;
	values_1.concentricity = concentricity_1[1];
	values_1.minThickData = distance;
	cout << "MaxThick_1 " << values_1.maxThick << endl;
	cout << "MaxThick_1 " << maxThick_2 << endl;
	j.push_back(values_1);
	distance.clear();
	for (size_t i = 0; i < returnMinT2.size(); i++) {
		distance.push_back(returnMinT2[i].distance);
	}
	values_1.minThick = minThick_3;
	values_1.avgThick = avgThickness_3;
	values_1.maxThick = maxThick_3;
	values_1.outerDia = cableDiamm;
	values_1.concentricity = concentricity_1[2];
	values_1.minThickData = distance;
	cout << "MaxThick_1 " << values_1.maxThick << endl;
	cout << "MaxThick_1 " << maxThick_3 << endl;
	j.push_back(values_1);
	cout << j << endl;
	const auto s = j.dump();
	System::String^ file = gcnew System::String(s.data());
	jsonFileProp = file;
	cv::imwrite("resullt4core.bmp", imageC);
//	resShow("ImageC", imageC);
	resize(imageC, img, img.size());
	return 0;
}

int algorithmLib::Class1::flatCable(System::Drawing::Bitmap^ rawImg) {
	srand(time(0));
	int maxId{};
	Mat img = BitmapToMat(rawImg);
	if (img.empty()) {
		cerr << "Image not found " << endl;
		return -1;
	}
	int choice{};
	cout << "Enter the number of circles in the image " << endl;

	cin >> choice;
	//======================== Masking our image ==============================
	Mat mask = Mat::zeros(img.size(), img.type());
	Mat dstImage = Mat::zeros(img.size(), CV_8UC1);
	dstImage = 255 - dstImage;
	cvtColor(dstImage, dstImage, COLOR_GRAY2BGR);
	circle(mask, Point(mask.cols / 2, mask.rows / 2), 2200, Scalar(255, 255, 255), -1, 8, 0);
	//resShow("mask", mask);
	img.copyTo(dstImage, mask);
	//resShow("dstImage", dstImage);

	Mat imageIn = dstImage;
	Mat image = imageIn.clone();
	if (image.channels() > 2) {
		cvtColor(image, image, COLOR_BGR2GRAY);
	}
	Mat imageC;
	cvtColor(image, imageC, COLOR_GRAY2BGR);


	vector<vector<Point>> contours_1;
	vector<Contour> contours;
	vector<Contour> uniqueContours;
	vector<Contour> usableUniqueConts;
	vector<Vec4i> hierarchy;
	EdgesSubPix(image, alpha, low, high, contours, hierarchy, mode, contours_1);
	cout << "contours_1 size " << contours_1.size() << endl;
	cout << "contours size " << contours.size() << endl;


	//drawContours(imageC, contours_1, -1, Scalar(0, 255, 0), 5);

	vector < vector<cv::Point2f>> conts;
	vector<RotatedRect> finalConts;
	vector<RotatedRect> finalContsWoBig;

	for (size_t i = 0; i < contours.size(); i++) {
		RotatedRect brect = minAreaRect(contours[i].points);

		float heightR = max(brect.size.height, brect.size.width);
		float widthR = min(brect.size.height, brect.size.width);

		if (i == 0) {
			finalConts.push_back(brect);
			uniqueContours.push_back(contours[i]);
		}
		bool unique = true;
		for (size_t j = 0; j < finalConts.size(); j++) {
			if ((abs(finalConts[j].center.x - brect.center.x) < 5) && (abs(finalConts[j].center.y - brect.center.y) < 5))
			{
				float height = max(finalConts[j].size.height, finalConts[j].size.width);
				float width = min(finalConts[j].size.height, finalConts[j].size.width);
				float hdiff = abs(height - heightR);
				float wdiff = abs(width - widthR);
				if (hdiff < 8 && wdiff < 8)
				{
					if (height < heightR && width < widthR)
					{
						finalConts.at(j) = brect;

					}

					unique = false;
					break;
				}

			}


		}
		if (unique == true) {
			finalConts.push_back(brect);
			uniqueContours.push_back(contours[i]);

		}
	}
	Point2f vertices[4];
	int contValid = 0;
	vector<Contour> myContour(uniqueContours.size());
	std::vector<int> isCopied(uniqueContours.size());

	//mergeContour(imageC, uniqueContours, 6, myContour, isCopied, 30);
	usableUniqueConts.clear();
	//usableUniqueConts.push_back(myContour[0]);

	for (size_t j = 0; j < uniqueContours.size(); j++) {
		RotatedRect brect = minAreaRect(uniqueContours[j].points);
		float width = max(brect.size.height, brect.size.width);
		float height = min(brect.size.height, brect.size.width);
		cout << "brect height " << height << endl;
		cout << " brect width " << width << endl;

		if (height > 110 && width > 440 && height < 3000 && width < 3000) {
			for (size_t uq = 0; uq < uniqueContours[j].points.size(); uq += 5)
			{
				//	circle(imageC, uniqueContours[j].points[uq], 5, Scalar(0, 255, 0), 5);
			}
			usableUniqueConts.push_back(uniqueContours[j]);
		}

	}
	//	drawContours_1(imageC, usableUniqueConts);
	cout << "UniqueContour" << usableUniqueConts.size() << endl;
	if (usableUniqueConts.size() != 2) {
		cout << "Contours not found properly" << endl;
		return -1;
	}
	Contour contInner;
	Contour contOuter;
	for (size_t uq = 0; uq < usableUniqueConts.size(); uq++) {
		if (uq == 0) {
			contInner = usableUniqueConts[uq];
			contOuter = usableUniqueConts[uq];

		}
		RotatedRect brect = minAreaRect(contInner.points);
		float heightInner = min(brect.size.height, brect.size.width);
		float widthInner = max(brect.size.height, brect.size.width);

		brect = minAreaRect(contOuter.points);
		float heightOuter = min(brect.size.height, brect.size.width);
		float widthOuter = max(brect.size.height, brect.size.width);

		brect = minAreaRect(usableUniqueConts[uq].points);
		float heightR = min(brect.size.height, brect.size.width);
		float widthR = max(brect.size.height, brect.size.width);

		if (heightOuter < heightR && widthOuter < widthR) {
			contOuter = usableUniqueConts[uq];
		}

		if (heightR < heightInner && widthR < widthInner) {
			contInner = usableUniqueConts[uq];
		}

	}


	RotatedRect brect = minAreaRect(contOuter.points);
	//rectangle(imageC, brect.boundingRect(), Scalar(0, 0, 255), 3);
	float heightOuter = min(brect.size.height, brect.size.width);
	float widthOuter = max(brect.size.height, brect.size.width);
	Point2f centerOuter = brect.center;
	brect.points(vertices);
	float dividingHeight{};
	dividingHeight = heightOuter * 0.60;


	Point2f vertices_1[4];

	for (int i = 0; i < 4; i++) {

		vertices_1[i].x = (vertices[i].x + vertices[(i + 1) % 4].x) / 2;
		vertices_1[i].y = (vertices[i].y + vertices[(i + 1) % 4].y) / 2;

	}

	//for (int i = 0; i < 4; i++) {
	//	line(imageC, vertices_1[i], vertices_1[(i + 1) % 4], Scalar(0, 0, 255), 4);
	//	cout << "vertices " << vertices_1[i] << endl;
	//}
	brect = minAreaRect(contInner.points);
	brect.points(vertices);
	float widthInner = max(brect.size.height, brect.size.width);
	float heightInner = min(brect.size.height, brect.size.width);

	//for (int i = 0; i < 4; i++) {
	//	line(imageC, vertices[i], vertices[(i + 1) % 4], Scalar(0, 255, 0), 4);
	//}
	Point2f centerInner = brect.center;
	Point2f vertices_2[4];
	for (int i = 0; i < 4; ++i) {
		vertices_2[i].x = (vertices[i].x + vertices[(i + 1) % 4].x) / 2;
		vertices_2[i].y = (vertices[i].y + vertices[(i + 1) % 4].y) / 2;
	}
	float dividingDist;

	dividingDist = widthInner / 4.0;
	cout << "dividingDist " << dividingDist << endl;

	vector<Point> extremePoint;
	for (size_t y = 0; y < 4; ++y) {
		//	line(imageC, vertices_2[y], vertices_2[(y + 1) % 4], Scalar(255, 0, 0), 4);
		int dist = distancePoint(centerInner, vertices_2[y]);
		cout << "Distance " << dist << endl;
		if (dist >= 200) {
			extremePoint.push_back(vertices_2[y]);
		}
	}



	Point2f returnVecl1[2];
	ptToVector(extremePoint[0], extremePoint[1], returnVecl1);
	vector<Point2f> centerPoint;
	for (int i = 0; i < choice; ++i) {
		Point2f point;
		if (i == 0) {
			point = getPointAtDist(returnVecl1, dividingDist / 2.0, 1);
		}
		else {
			point = getPointAtDist(returnVecl1, dividingDist * (i + 0.5), 1);
		}
		centerPoint.push_back(point);


		line(imageC, extremePoint[0], point, Scalar(0, 0, 255), 6);
	}
	for (auto c : centerPoint) {
		cout << c << endl;
	}
	vector<vector<Point2f>> circlePoints(choice);
	cout << "Circle Point size " << circlePoints.size() << endl;

	circleDivision(circlePoints, usableUniqueConts[0], centerPoint, dividingDist / 2);


	for (int i = 0; i < choice; i++) {
		vector < vector<Point2f>> sectorInner(4);
		vector < vector<Point2f>> sectorOuter(4);
		sectorDivision(centerPoint[i], circlePoints[i], sectorInner, dividingHeight, sectorOuter, contOuter, extremePoint[0]);
		/*cout << "Sector Outer " << sectorOuter.size() << endl;*/
		drawSector(imageC, centerPoint[i], sectorOuter);
		drawSector(imageC, centerPoint[i], sectorInner);

		vector<thicknessPt> minThickData(sectorInner.size());


		for (size_t sec = 0; sec < sectorInner.size(); sec++) {

			float minDist = 10000;

			if (i == 0 && sec == 2)
				continue;
			else if (i == 1 && sec % 2 == 0)
				continue;
			else if (i == (choice - 1) && sec == 0)
				continue;
			for (size_t x = 0; x < sectorInner[sec].size(); x++) {

				for (size_t y = 0; y < sectorOuter[sec].size(); y++) {


					float dist = distancePoint(sectorInner[sec][x], sectorOuter[sec][y]);

					if (dist < minDist) {
						minDist = dist;
						//cout << x << endl;
						minThickData[sec].distance = minDist;
						minThickData[sec].innerPt = sectorInner[sec][x];
						minThickData[sec].outerPt = sectorOuter[sec][y];
					}

				}

			}
			line(imageC, minThickData[sec].innerPt, minThickData[sec].outerPt, Scalar(0, 160, 80), 2);
			cv::putText(imageC, cv::format("d=%3.3f", minThickData[sec].distance * mmPerPixProp), minThickData[sec].outerPt, FONT_HERSHEY_COMPLEX, 0.5, Scalar(0, 0, 255), 2);


		}

		sectorInner.erase(sectorInner.begin(), sectorInner.end());
		sectorOuter.erase(sectorOuter.begin(), sectorOuter.end());
		minThickData.clear();

		/*cout << "sectorInner" << sectorInner.size();
		cout << "sectorOuter" << sectorOuter.size();*/

	}

	resize(imageC, img, img.size());
	
	

	return 0;
}
float algorithmLib::Class1::getSectorThickness(int sectorNum)

{
	return returnMinT[sectorNum].distance*mmPerPixProp;
}



int algorithmLib::Class1::trackPositions(System::Drawing::Bitmap^ rawImg)
{/*	cv::estimateAffine2D()*/

	//cout << "W_tmpl_searchArF "<< W_tmpl_searchArF <<endl;
	//cout << "H_tmpl_searchArF "<< H_tmpl_searchArF <<endl;
	cout << "scale  "<< scale <<endl;



	//imageMatchIn = BitmapToMat(rawImg).clone();
	tempImg = BitmapToMat(rawImg).clone();

	imageMatchIn = tempImg(imageROI).clone();
	if (scale > 1)
		resize(imageMatchIn, imageMatchIn, Size(), scale, scale);
	//cvtColor(imageMatchIn, imageMatchIn, COLOR_BGR2GRAY);
	//imageMatchIn = getSobel(imageMatchIn);

	//threshold(imageMatchIn, imageMatchIn, 120, 255, THRESH_BINARY);
	imgMatchInCpy = imageMatchIn.clone();

	int markerSizeb2 = markerSizeProp / 2;
	if (markerSample_enProp == 1  && sampleMarkerLostProp==0)
	{
		if (templateLost[0] == 0)
		{
			imageMatch(imageMatchIn(searchR_v[0]), M1P1Template, &locRetM1P1, &updateTempl[0], &templateLost[0],mmLocM1P1);
			if (templateLost[0] == 0)
			{
				mmcenterM1P1curr = (mmPerPixProp* Point2f(searchR_v[0].x , searchR_v[0].y ))+ mmLocM1P1;
				centerM1P1curr = Point2f(searchR_v[0].x + locRetM1P1.x, searchR_v[0].y + locRetM1P1.y);
				templR_v[0] = Rect(centerM1P1curr.x - (scale * (W_templS / 2)), centerM1P1curr.y - (scale * (H_templS / 2)), scale * W_templS, scale * H_templS);
				searchR_v[0] = Rect(centerM1P1curr.x - (scale * (W_tmpl_searchArF / 2.0) * W_templS), centerM1P1curr.y - (scale * (H_tmpl_searchArF / 2.0) * H_templS), scale * W_tmpl_searchArF * W_templS, scale * H_tmpl_searchArF * H_templS);
				
			}
		}
		else
		{
			cout << "template lost M1 P1----------------------------" << endl;
		}
		if (templateLost[1] == 0)
		{
			imageMatch(imageMatchIn(searchR_v[1]), M1P2Template, &locRetM1P2, &updateTempl[1], &templateLost[1],mmlocM1P2);
			if (templateLost[1] == 0)
			{
				mmcenterM1P2curr =(mmPerPixProp* Point2f(searchR_v[1].x , searchR_v[1].y ))+ mmlocM1P2;
				centerM1P2curr = Point2f(searchR_v[1].x + locRetM1P2.x, searchR_v[1].y + locRetM1P2.y);
				templR_v[1] = Rect(centerM1P2curr.x - (scale * (W_templS / 2)), centerM1P2curr.y - (scale * (H_templS / 2)), scale * W_templS, scale * H_templS);
				searchR_v[1] = Rect(centerM1P2curr.x - (scale * (W_tmpl_searchArF / 2.0) * W_templS), centerM1P2curr.y - (scale*(H_tmpl_searchArF / 2.0) * H_templS), scale * W_tmpl_searchArF * W_templS, scale * H_tmpl_searchArF * H_templS);
			}
		}
		else
		{
			cout << "template lost M1 P22222----------------------------" << endl;
		}

		sampleMarkerLostProp = templateLost[0] + templateLost[1];
		std::cout << std::fixed;

		/*rectangle(imageMatchIn, r_M11, Scalar(0, 255, 0), 2);
		rectangle(imageMatchIn, r_M12, Scalar(0, 255, 0), 2);*/

		cout << "center 1:"<<mmcenterM1P1curr<<"  center2: "<<mmcenterM1P2curr << endl;
		displacementProp = distancePoint(mmcenterM1P1curr, mmcenterM1P2curr) /(int) scale;
		if (updateTempl[0] == 1)
		{
			cout << "template update +++++++++++++++++++++++++++M1+++++P1" << endl;
			M1P1Template = imgMatchInCpy(templR_v[0]).clone();
			updateTempl[0] = 0;
		}

		if (updateTempl[1] == 1)
		{
			cout << "template update +++++++++++++++++++++++++++M1+++++P2" << endl;
			M1P2Template = imgMatchInCpy(templR_v[1]).clone();
			updateTempl[1] = 0;
		}
		//imshow("M1_u", M1P1Template);
		//imshow("M2_u", M1P2Template);
	//	waitKey(5);
		M1P1pos_prop = System::Drawing::Point((templR_v[0].x / scale)+ W_templS/2 +imageROI.x, (templR_v[0].y / scale)+ H_templS/2+  imageROI.y);
		M1P2pos_prop = System::Drawing::Point((templR_v[1].x / scale)+ W_templS/2 + imageROI.x, (templR_v[1].y / scale)+ H_templS/2 +  imageROI.y);
		//distanceProp = distancePoint(Point(r_M11.x, r_M11.y), Point(r_M12.x, r_M12.y)) * mmPerPixProp;
	}
	if (markerGrip_enProp == 1  && sampleMarkerLost==0)
	{
		if (templateLost[2] == 0)
		{
			imageMatch(imageMatchIn(searchR_v[2]), M2P1Template, &locRetM2P1, &updateTempl[2], &templateLost[2],mmLocM2P1);
			if (templateLost[2] == 0)
			{
				mmcenterM2P1curr = (mmPerPixProp*Point2f(searchR_v[2].x , searchR_v[2].y))+ mmLocM2P1;
				centerM2P1curr = Point2f(searchR_v[2].x + locRetM2P1.x, searchR_v[2].y + locRetM2P1.y);
				templR_v[2] = Rect(centerM2P1curr.x - (scale * (W_templG / 2)), centerM2P1curr.y - (scale * (H_templG / 2)), scale * W_templG, scale * H_templG);
				searchR_v[2] = Rect(centerM2P1curr.x - (scale * (W_tmpl_searchArF / 2.0) * W_templG), centerM2P1curr.y - (scale * (H_tmpl_searchArF/2.0) * H_templG), scale * W_tmpl_searchArF * W_templG, scale * H_tmpl_searchArF * H_templG);
			}
		
		}
		else
		{
			cout << "template lost M2 P1----------------------------" << endl;
		}
		if (templateLost[3] == 0)
		{
			imageMatch(imageMatchIn(searchR_v[3]), M2P2Template, &locRetM2P2, &updateTempl[3], &templateLost[3],mmlocM2P2);
			if (templateLost[3] == 0)
			{
				mmcenterM2P2curr =(mmPerPixProp* Point2f(searchR_v[3].x, searchR_v[3].y ))+ mmlocM2P2;
				centerM2P2curr = Point2f(searchR_v[3].x + locRetM2P2.x, searchR_v[3].y + locRetM2P2.y);
				templR_v[3] = Rect(centerM2P2curr.x - (scale * (W_templG / 2)), centerM2P2curr.y - (scale * (H_templG / 2)), scale * W_templG, scale * H_templG);
				searchR_v[3] = Rect(centerM2P2curr.x - (scale * (W_tmpl_searchArF / 2.0) * W_templG), centerM2P2curr.y - (scale * (H_tmpl_searchArF / 2.0) * H_templG), scale * W_tmpl_searchArF * W_templG, scale * H_tmpl_searchArF * H_templG);
			}
		}
		else
		{
			cout << "template lost M2 P22222----------------------------" << endl;
		}
		std::cout << std::fixed;

		/*rectangle(imageMatchIn, r_M11, Scalar(0, 255, 0), 2);
		rectangle(imageMatchIn, r_M12, Scalar(0, 255, 0), 2);*/

		cout << "center 3:" << mmcenterM2P1curr << "  center4 : " << mmcenterM2P2curr << endl;
		displacementGripProp = distancePoint(mmcenterM2P1curr, mmcenterM2P2curr) /(float) scale;
		if (updateTempl[2] == 1)
		{
			cout << "template update ++++++++++++++++++++++++++++++M2++P1" << endl;
			M2P1Template = imgMatchInCpy(templR_v[2]).clone();
			updateTempl[2] = 0;
		}

		if (updateTempl[3] == 1)
		{
			cout << "template update ++++++++++++++++++++++++++++++M2++P2" << endl;
			M2P2Template = imgMatchInCpy(templR_v[3]).clone();
			updateTempl[3] = 0;
		}
		//imshow("M2_p1u", M2P1Template);
		//imshow("M2_p2u", M2P2Template);
		//waitKey(5);
		M2P1pos_prop = System::Drawing::Point((templR_v[2].x / scale)+ W_templG/2  + imageROI.x, (templR_v[2].y / scale)+ H_templG/2 + imageROI.y);
		M2P2pos_prop = System::Drawing::Point((templR_v[3].x / scale)+ W_templG/2 + imageROI.x, (templR_v[3].y / scale)+ H_templG/2 + imageROI.y);
		//distanceProp = distancePoint(Point(r_M11.x, r_M11.y), Point(r_M12.x, r_M12.y)) * mmPerPixProp;
	}


	return 1;
}
//end new markers
