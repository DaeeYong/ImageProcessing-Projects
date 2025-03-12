#include <opencv2/highgui.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include<vector>
#include<algorithm>

using namespace cv;
using namespace std;

void myMedianFilter(InputArray input, OutputArray output, int windowSize);

int main() {

	Mat image = imread("../img/MedianFilterInput.png", 0);
	Mat out1;

	myMedianFilter(image, out1, 5);

	imshow("before", image);
	imshow("after", out1);

	waitKey();
	
	return 0;
}

void myMedianFilter(InputArray input, OutputArray output, int windowSize) {
	const Mat& img = input.getMat();
	output.create(img.size(), img.type());
	Mat dst = output.getMat();
	unsigned char pixelVal;
	unsigned char medianOfPixel;

	int range = (windowSize - 1) / 2;

	for (int y = 0; y < img.rows; y++) {
		for (int x = 0; x < img.cols; x++) {
			//vector For saving pixelValue
			vector<unsigned char> vec;

			for (int t = -range; t <= range; t++) {
				for (int s = -range; s <= range; s++) {
					pixelVal = img.at<unsigned char>(min(img.rows - 1, max(0, y + t)), min(img.cols - 1, max(0, x + s))); //for padding
					vec.push_back(pixelVal);
				}
			}
			sort(vec.begin(), vec.end());
			int midPos = vec.size() / 2; // idx of median
			medianOfPixel = vec[midPos];
			dst.at<unsigned char>(y, x) = medianOfPixel;
		}
	}
}
