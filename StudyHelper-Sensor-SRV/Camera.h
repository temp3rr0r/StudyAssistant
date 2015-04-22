//#include <iostream>
#include<opencv2/opencv.hpp>
//using namespace std;
using namespace cv;

class Camera
{
	private:
		string name;
		string stamp;
		string filePath;
		string fileName;
		int doTakeScreenshot(void)
		{
			try
			{		
				// Open the Web camera videoCaptureDevice device
				VideoCapture videoCaptureDevice(0);
	
				// Set resolution
				videoCaptureDevice.set(CV_CAP_PROP_FRAME_WIDTH,800);
				videoCaptureDevice.set(CV_CAP_PROP_FRAME_HEIGHT,600);
	
				if(!videoCaptureDevice.isOpened()){
					cout << "Failed to connect to the camera." << endl;
					return -1;
				}

				// Allocate the two output files
				Mat cleanFrame, edgesFrame;
	
				// Do videoCaptureDevice an image
				videoCaptureDevice >> cleanFrame;
	
				if(cleanFrame.empty()){
					cout << "Failed to videoCaptureDevice an image" << endl;
					return -1;
				}

				// Save clean image
				imwrite(filePath + fileName + ".png", cleanFrame);

				// Overlay text on the clean cleanFrame
				putText(cleanFrame, stamp, cvPoint(10, 10), 
					FONT_HERSHEY_COMPLEX_SMALL, 0.5, cvScalar(0,0,0), 1, CV_AA);
	
				// Save the image with the overlay
				imwrite(filePath + "overlay_" + fileName + ".png", cleanFrame);
				
				// Release the capture device object
				videoCaptureDevice.release();
			}
			catch(...)
			{

			}
			
			return 0;
		}
	public:
		Camera();
		
		int getScreenshot(void)
		{
			return doTakeScreenshot();
		}
		string getName(void)
		{
			return name;
		}
		void setStamp(string _inputStr)
		{
			stamp = _inputStr;
		}
		void setFileName(string _inputStr)
		{
			fileName = _inputStr;
		}
		void setFilePath(string _inputStr)
		{
			filePath = _inputStr;
		}
};		

Camera::Camera()
{
	name = "Raspi Camera";
	stamp = "default";
	fileName = "clean";
	filePath = "/home/madks/Beez/BeeHive-WS-Rest/images/";
}

