// algorithmLib.h

#pragma once
#include <opencv2/imgcodecs.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <string>
#include <vector>
#include <fstream>
#include <iterator>
#include "EdgesSubPix.h"
#include <cmath>

using namespace std;
using namespace System;
using namespace cv;
namespace algorithmLib {

	public ref class Class1
	{
	public:
		System::Drawing::Bitmap^ algorithmLib::Class1::detectPoints(System::Drawing::Bitmap^ bitmap,int mode);//boot
		System::Drawing::Bitmap^ algorithmLib::Class1::detectPointsHSV(System::Drawing::Bitmap^ bitmap,int mode);//boot
		int algorithmLib::Class1::initialPoint(float x1,float y1,float x2,float y2);//boot
		System::Drawing::Bitmap^ algorithmLib::Class1::clickResp(float x1,float y1,float x2,float y2,float mouseLocX,float mouseLocY);//boot
		System::Drawing::Bitmap^ algorithmLib::Class1::calibUsingTarget(System::Drawing::Bitmap^ bitmap);//boot
		int algorithmLib::Class1::calibUsingTargetCircle(System::Drawing::Bitmap^ bitmap);//boot
		System::Drawing::Bitmap^ algorithmLib::Class1::copy_BaseImage(System::Drawing::Bitmap^ bitmap,float x1,float y1,float x2,float y2);
		int algorithmLib::Class1::setBaseImage(System::Drawing::Bitmap^ rawImg);
		int pathReader();

	



		int algorithmLib::Class1::updateColorTop(float x1,float y1);
		int algorithmLib::Class1::updateColorBottom(float x1,float y1);
		int algorithmLib::Class1::updateColorDumbell(float x1,float y1);
		int algorithmLib::Class1::updateColorBackGround(float x1,float y1);
		int algorithmLib::Class1::clearColorVectors();
		System::Drawing::Bitmap^ algorithmLib::Class1::processColorVector(int code);//boot

		int algorithmLib::Class1::setMarker(int x, int y,int markerCode);
		int algorithmLib::Class1::setMarkerP1(int x, int y);
		int algorithmLib::Class1::setMarkerP2(int x, int y);
		int algorithmLib::Class1::cropTemplatesBoth(System::Drawing::Bitmap^ rawImg,int code);
		int algorithmLib::Class1::cropTemplates(System::Drawing::Bitmap^ rawImg);
		int algorithmLib::Class1::trackPositions(System::Drawing::Bitmap^ rawImg);

		int algorithmLib::Class1::getMeasurements2c(System::Drawing::Bitmap^ rawImg);
		int algorithmLib::Class1::getMeasurements3c(System::Drawing::Bitmap^ rawImg);
		int algorithmLib::Class1::getMeasurements4c(System::Drawing::Bitmap^ rawImg);
		int algorithmLib::Class1::getMeasurements5c(System::Drawing::Bitmap^ rawImg);
		int algorithmLib::Class1::getMeasurements6c(System::Drawing::Bitmap^ rawImg);
		int algorithmLib::Class1::getMeasurementsMultiCore(System::Drawing::Bitmap^ rawImg);
		int algorithmLib::Class1::getMeasurements7c(System::Drawing::Bitmap^ rawImg);
		int algorithmLib::Class1::getMeasurements8c(System::Drawing::Bitmap^ rawImg);
		int algorithmLib::Class1::flatCable(System::Drawing::Bitmap^ rawImg);
		float algorithmLib::Class1::getSectorThickness(int sectorNum);
		int algorithmLib::Class1::updateProcessROI();





		int algorithmLib::Class1::updateSettings();
		int algorithmLib::Class1::resetTest();
		// TODO: Add your methods for this class here.
		static int tagetFound = 0;



		static float minThickness = 0.0;  //Width
		property float minThicknessProp
		{
			float get()
			{
				return minThickness;
			}
			void set(float  value)
			{
				minThickness = value;
			};
		};


		static float avgThickness = 0.0;  //Width
		property float avgThicknessProp
		{
			float get()
			{
				return avgThickness;
			}
			void set(float  value)
			{
				avgThickness = value;
			};
		};
		static float outerDia = 0.0;  //Width
		property float outerDiaProp
		{
			float get()
			{
				return outerDia;
			}
			void set(float  value)
			{
				outerDia = value;
			};
		};
		static float concentricity = 0.0;  //Width
		property float concentricityProp
		{
			float get()
			{
				return concentricity;
			}
			void set(float  value)
			{
				concentricity = value;
			};
		};

		static float ovality = 0.0;

		property float ovalityProp {
			float get()
			{
				return ovality;
			}
			void set(float value)
			{
				ovality = value;
			};
		};

	    

		static System::String^ jsonFile;

		property System::String^ jsonFileProp {
			System::String^ get() {
				return jsonFile;
			}
			void set(System::String^ rhs) {
				jsonFile = rhs;
			};
		}
		System::Drawing::Point M1P1Pos = System::Drawing::Point(-1, -1);
		property System::Drawing::Point M1P1pos_prop
		{
			System::Drawing::Point get()
			{
				return M1P1Pos;
			}
			void set(System::Drawing::Point value)
			{
				M1P1Pos = value;
			};
		
		
		}


		System::Drawing::Point M1P2Pos = System::Drawing::Point(-1, -1);
		property System::Drawing::Point M1P2pos_prop
		{
			System::Drawing::Point get()
			{
				return M1P2Pos;
			}
			void set(System::Drawing::Point value)
			{
				M1P2Pos = value;
			};


		}

		System::Drawing::Point M2P1Pos = System::Drawing::Point(-1, -1);
		property System::Drawing::Point M2P1pos_prop
		{
			System::Drawing::Point get()
			{
				return M2P1Pos;
			}
			void set(System::Drawing::Point value)
			{
				M2P1Pos = value;
			};


		}


		System::Drawing::Point M2P2Pos = System::Drawing::Point(-1, -1);
		property System::Drawing::Point M2P2pos_prop
		{
			System::Drawing::Point get()
			{
				return M2P2Pos;
			}
			void set(System::Drawing::Point value)
			{
				M2P2Pos = value;
			};


		}
		static int  sectorCount = 0;  //marker template size
		property int sectorCountProp
		{
			int get()
			{
				return sectorCount;
			}
			void set(int value)
			{
				sectorCount = value;
			};
		};//


		static int  sampleMarkerLost = 0;  //marker template size
		property int sampleMarkerLostProp
		{
			int get()
			{
				return sampleMarkerLost;
			}
			void set(int value)
			{
				sampleMarkerLost = value;
			};
		};//
//---------------------marker size and tracking----------------------------------------
		static int  markerSize = 30;  //marker template size
		property int markerSizeProp
		{
			int get()
			{
				return markerSize;
			}
			void set(int value)
			{
				markerSize = value;
			};
		};//
		//sample
		static int  H_templS = 30;  //marker template size
		property int H_templSProp
		{
			int get()
			{
				return H_templS;
			}
			void set(int value)
			{
				H_templS = value;
			};
		};//
		static int  W_templS = 30;  //marker template size
		property int W_templSProp
		{
			int get()
			{
				return W_templS;
			}
			void set(int value)
			{
				W_templS = value;
			};
		};//
		//grip
		static int  H_templG = 30;  //marker template size
		property int H_templGProp
		{
			int get()
			{
				return H_templG;
			}
			void set(int value)
			{
				H_templG = value;
			};
		};//
		static int  W_templG = 30;  //marker template size
		property int W_templGProp
		{
			int get()
			{
				return W_templG;
			}
			void set(int value)
			{
				W_templG = value;
			};
		};//
		static int  imageScaleF = 1;  //marker template size
		property int imageScaleFProp
		{
			int get()
			{
				return imageScaleF;
			}
			void set(int value)
			{
				imageScaleF = value;
			};
		};//
		static int  templScaleF = 4;  //marker template size
		property int templScaleFProp
		{
			int get()
			{
				return templScaleF;
			}
			void set(int value)
			{
				templScaleF = value;
			};
		};//
		static float H_tmpl_searchArF = 3.0;  //Height
		property float H_tmpl_searchArFProp
		{
			float get()
			{
				return H_tmpl_searchArF;
			}
			void set(float  value)
			{
				H_tmpl_searchArF = value;
			};
		};

		static float W_tmpl_searchArF = 1.5;  //Width
		property float W_tmpl_searchArFProp
		{
			float get()
			{
				return W_tmpl_searchArF;
			}
			void set(float  value)
			{
				W_tmpl_searchArF = value;
			};
		};

		static float templLostTh = 0.92;  //Height
		property float templLostThProp
		{
			float get()
			{
				return templLostTh;
			}
			void set(float  value)
			{
				templLostTh = value;
			};
		};
		
		static float templUpdateTh = 0.95;  //Height
		property float templUpdateThProp
		{
			float get()
			{
				return templUpdateTh;
			}
			void set(float  value)
			{
				templUpdateTh = value;
			};
		};
		static float circleGridC2Cmm = 176.5;  //Height
		property float circleGridC2CmmProp
		{
			float get()
			{
				return circleGridC2Cmm;
			}
			void set(float  value)
			{
				circleGridC2Cmm = value;
			};
		};

//-----------------------end markerSize and tracking-------------------------------------
		static int  markerSample_en = 0;  //marker template size
		property int markerSample_enProp
		{
			int get()
			{
				return markerSample_en;
			}
			void set(int value)
			{
				markerSample_en = value;
			};
		};//

		static int  markerGrip_en = 0;  //marker template size
		property int markerGrip_enProp
		{
			int get()
			{
				return markerGrip_en;
			}
			void set(int value)
			{
				markerGrip_en = value;
			};
		};//



		static float displacement = 0.0;          //darkness   
		property float displacementProp
		{
			float get()
			{
				return displacement;
			}
			void set(float value)
			{
				displacement = value;
			};
		};//

		//displacementGripProp
		static float displacementGrip = 0.0;          //darkness   
		property float displacementGripProp
		{
			float get()
			{
				return displacementGrip;
			}
			void set(float value)
			{
				displacementGrip = value;
			};
		};//


		property int tagetFoundProp
		{
			int get()
			{
				return tagetFound;
			}
			void set(int  value)
			{
				tagetFound = value;
			};
		};
		static int outcode = 0;
		property int outCodeProp
		{
			int get()
			{
				return outcode;
			}
			void set(int  value)
			{
				outcode = value;
			};
		};
		static float distance = 0;
		property float distanceProp
		{
			float get()
			{
				return distance;
			}
			void set(float  value)
			{
				distance = value;
			};
		};
	//	static float mmperPix = 0.0188627;
		static float mmPerPix = 0.0195585;
		property float mmPerPixProp
		{
			float get()
			{
				return mmPerPix;
			}
			void set(float  value)
			{
				mmPerPix = value;
			};
		};

		static float mmPerPixTemp = 1;
		property float mmPerPixTempProp
		{
			float get()
			{
				return mmPerPixTemp;
			}
			void set(float  value)
			{
				mmPerPixTemp = value;
			};
		};
		static int sampleMarkContrast = 0;
		property int sampleMarkContrastProp
		{
			int get()
			{
				return sampleMarkContrast;
			}
			void set(int  value)
			{
				sampleMarkContrast = value;
			};
		};

		//machineType
		static int machineType = 0;
		property int machineTypeProp  //q
		{
			int get()
			{
				return machineType;
			}
			void set(int  value)
			{
				machineType = value;
			};
		};



	};
}
