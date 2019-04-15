// SimpleUvcViewer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

// opencv 
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

// list
#include <list>

// thread
#include <thread>

std::list<cv::Mat> frames;

using namespace cv;
using namespace std;

string strUvcWindowName = "SimpleUvcViewer";

void printUsage(char** argv)
{
	cout <<
		"\n"
		"Usage :\n" << argv[0] << " your UVC device ID\n"
		"\n"
		"Example usage :\n" << argv[0] << " 0-9\n";
}

int main(int argc, char *argv[])
{
	int uvc_id = 0;

	if (argc > 1) {
		// safety get string to integer
		try {
			uvc_id = std::stoi(argv[1]);
		}
		catch (std::invalid_argument& e) {
			// if no conversion could be performed
			e.what();
			printUsage(argv);
			return EXIT_SUCCESS;
		}
		catch (std::out_of_range& e) {
			// if the converted value would fall out of the range of the result type 
			// or if the underlying function (std::strtol or std::strtoull) sets errno 
			// to ERANGE.
			e.what();
			printUsage(argv);
			return EXIT_SUCCESS;
		}
	}
	else {
		printUsage(argv);
		return EXIT_SUCCESS;
	}

	if (uvc_id < 0 || uvc_id>9)
	{
		printUsage(argv);
		return EXIT_SUCCESS;
	}

	VideoCapture cap(cv::CAP_DSHOW + uvc_id);
	
	// set for maximum resolution
	cap.set(cv::CAP_PROP_FRAME_WIDTH, 9999);
	cap.set(cv::CAP_PROP_FRAME_HEIGHT, 9999);

	if (!cap.isOpened()) {
		std::cerr << "UVC open failed : " << uvc_id << std::endl;
		return EXIT_FAILURE;
	}

	cv::namedWindow(strUvcWindowName, cv::WindowFlags::WINDOW_AUTOSIZE);

	cv::Mat inFrame;
	while (cap.read(inFrame)) {
		cv::imshow(strUvcWindowName, inFrame);
		char key = cv::waitKey(2);
		if (key == 's') {
			cv::imwrite("cap.jpg", frames.front());
		}else if (key == 'q') {
			break;
		}
	}
	
	return EXIT_SUCCESS;
}