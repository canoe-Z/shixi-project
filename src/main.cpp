#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <math.h>

using namespace std;
using namespace cv;

int main(int argc, char const *argv[])
{
	Size board_size = Size(11, 8); //棋盘角点的数目

	Mat img;
	VideoCapture cap(0); //打开电脑摄像头
	int n_boards = 8;
	int image_count = 0;
	while (image_count < n_boards) //读取图片
	{
		cap >> img;
		cout << img.cols << "  " << img.rows << std::endl;
		cout << img.type() << std::endl;

		vector<Point2f> corners;										//保存一张棋盘角点
		bool is_find = findChessboardCorners(img, board_size, corners); //找棋盘角点
		if (is_find)													//找到全部角点时
		{
			image_count++;
			Mat gray;
			cvtColor(img, gray, COLOR_RGB2GRAY);

			//棋盘格角点精检
			cornerSubPix(gray, corners, Size(5, 5), Size(-1, -1), TermCriteria(TermCriteria::COUNT + TermCriteria::EPS, 30, 0.1));
			drawChessboardCorners(img, board_size, corners, is_find); //画出棋盘角点

			imshow("img", img);
			waitKey(-1);
		}
	}

	
	return 0;
}
