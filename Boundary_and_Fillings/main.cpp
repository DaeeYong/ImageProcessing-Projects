#include <opencv2/highgui.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

#include<iostream>
#include<Windows.h>

using namespace cv;
using namespace std;

void onMouse(int event, int x, int y, int flags, void*);
bool isEqual(const Mat& before, const Mat& after);

Mat orign;
Mat img_erosion;
Mat img_bound;
Mat img_c;
Mat boxKernel;
Mat crossKernel;
Mat processing;

int main()
{
	orign = imread("../img/Profile.png", 0);
	processing = Mat::zeros(orign.size(), CV_8UC1); //for dilate processing
	
	boxKernel = getStructuringElement(MORPH_RECT, Size(3, 3)); //SE Box
	crossKernel = getStructuringElement(MORPH_CROSS, Size(3, 3), Point(1, 1)); //SE Cross
	
	//For finding boundary
	erode(orign, img_erosion, boxKernel, Point(-1, -1), 1);
	subtract(orign, img_erosion, img_bound);

	//complement image for flood fill
	img_c = ~img_bound;

	namedWindow("img");
	setMouseCallback("img", onMouse);
	
	imshow("img", img_bound);

	waitKey();

	return 0;
}

void onMouse(int event, int x, int y, int flag, void*) {
	Mat tmp = Mat::zeros(orign.size(), CV_8UC1);
	if (event == EVENT_LBUTTONDOWN) {
		processing.at<uchar>(y, x) = 255;
		while (true) {
			dilate(processing, processing, crossKernel);
			processing = processing & img_c; // processing for pixel of out of boundary
			if (isEqual(tmp, processing)) {
				break;
			}
			tmp = processing.clone(); //deep copy

			img_bound += processing;

			imshow("img", img_bound);
			waitKey(5);
		}
	}
}

bool isEqual(const Mat& before, const Mat& after) {
	Mat result = after ^ before;
	if (countNonZero(result) > 0) return false;
	else return true;
}
