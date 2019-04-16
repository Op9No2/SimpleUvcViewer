// SimpleUvcViewer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

// opencv 
#include <opencv2/highgui.hpp>

// list
#include <list>

// time 
#include <time.h>

// namespace
using namespace cv;
using namespace std;

// global variable
int g_nUvcDevId = 0;
int g_nFrameWidth = 0;
int g_nFrameHeight = 0;
string g_strUvcWindowName = "SimpleUvcViewer";

void printUsage(char** argv);
std::string GenFileNameByTime(void);
int parseCmdArgs(int argc, char** argv);

std::string GenFileNameByTime(void)
{
	char file_name[64] = {0};
	
	// system time
	time_t T = time(NULL);
	struct  tm tm = *localtime(&T);

	// time string + ".jpg"
	sprintf(file_name, "%04d%02d%02d%02d%02d%02d.jpg", \
		tm.tm_year + 1900, tm.tm_mday, tm.tm_mon + 1, tm.tm_hour, tm.tm_min, tm.tm_sec);
	
	return file_name;
}

void printUsage(char** argv)
{
	cout <<
		"\n"
		"Usage :\n" << argv[0] << " [Flags]\n\n"
		"\nFlags:\n"
		"  --d\n"
		"      UVC device ID \n"
		"  --w\n"
		"      UVC frame width \n"
		"  --h\n"
		"      UVC frame height \n"
		"\nExample usage :\n" << argv[0] << " -d 0 -w 1280 -h 720\n\n"
		"Keyboard function:\n"
		"'s' Save current frame to jpg format\n"
		"'q' Quit"
		"\n";
}

int main(int argc, char *argv[])
{
	if (EXIT_FAILURE == parseCmdArgs(argc, argv)) {
		return EXIT_FAILURE;
	}

	if (g_nUvcDevId == -1 || \
		g_nFrameWidth == -1 || \
		g_nFrameHeight == -1) {
		return EXIT_FAILURE;
	}

	VideoCapture cap(cv::CAP_DSHOW + g_nUvcDevId);
	
	// setting resolution
	cap.set(cv::CAP_PROP_FRAME_WIDTH, g_nFrameWidth);
	cap.set(cv::CAP_PROP_FRAME_HEIGHT, g_nFrameHeight);

	if (!cap.isOpened()) {
		std::cerr << "UVC device open failed : " << g_nUvcDevId << std::endl;
		return EXIT_FAILURE;
	}

	cv::namedWindow(g_strUvcWindowName, cv::WindowFlags::WINDOW_AUTOSIZE);

	cv::Mat inFrame;
	while (cap.read(inFrame)) 
	{
		cv::imshow(g_strUvcWindowName, inFrame);
		char key = cv::waitKey(2);
		if (key == 's') {
			string strFileName = GenFileNameByTime();
			cv::imwrite(strFileName, inFrame);
		}else if (key == 'q') {
			break;
		}
	}
	
	return EXIT_SUCCESS;
}

int stoi_safety(char *strInteger)
{
	// safety get string to integer
	
	int nResult = 0;

	try {
		nResult = std::stoi(strInteger);
	}
	catch (std::invalid_argument& e) {
		// if no conversion could be performed
		e.what();
		nResult = -1;
	}
	catch (std::out_of_range& e) {
		// if the converted value would fall out of the range of the result type 
		// or if the underlying function (std::strtol or std::strtoull) sets errno 
		// to ERANGE.
		e.what();
		nResult = -1;
	}

	return nResult;
}

int parseCmdArgs(int argc, char** argv)
{
	if (argc == 1)
	{
		printUsage(argv);
		return EXIT_FAILURE;
	}

	for (int i = 1; i < argc; ++i)
	{
		if (string(argv[i]) == "--help" || string(argv[i]) == "/?")
		{
			printUsage(argv);
			return EXIT_FAILURE;
		}
		else if (string(argv[i]) == "--d")
		{
			g_nUvcDevId = stoi_safety(argv[i + 1]);
			i++;
		}
		else if (string(argv[i]) == "--w")
		{
			g_nFrameWidth = stoi_safety(argv[i + 1]);
			i++;
		}
		else if (string(argv[i]) == "--h")
		{
			g_nFrameHeight = stoi_safety(argv[i + 1]);
			i++;
		}else {
			printUsage(argv);
			return EXIT_FAILURE;
		}
	}
	return EXIT_SUCCESS;
}