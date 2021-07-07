#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <math.h>

using namespace std;
using namespace cv;

class Calibrater
{
private:
    VideoCapture cap;
    int board_num = 20;
    Size board_size = Size(11, 8);
    int square_size = 30;

    Mat cameraMatrixL = Mat(3, 3, CV_32FC1, Scalar::all(0));
    Mat distCoeffL = Mat(1, 5, CV_32FC1, Scalar::all(0));
    Mat cameraMatrixR = Mat(3, 3, CV_32FC1, Scalar::all(0));
    Mat distCoeffR = Mat(1, 5, CV_32FC1, Scalar::all(0));
    Mat R;
    Mat T;
    Mat E;
    Mat F;

    optional<vector<Point2f>> get_imagePoint(const Mat &img);
    vector<Point3f> get_objPoint();
    void write_result();

public:
    /**
     * @brief Construct a new Calibrater object
     * 
     * @param board_size 棋盘格数量(11,8)
     * @param square_size 每个棋盘格尺寸(30mm)
     */
    Calibrater(const Size &board_size, const int &square_size);
    ~Calibrater();

    /**
     * @brief 标定
     * 
     * @param board_num 标定次数
     * @param write_flag 是否保存标定结果
     */
    void calibrate(const int board_num = 20, const bool write_flag = true);

    pair<Mat, Mat> get_matrix();
    Mat get_camera_matrix();
    Mat get_dist_coeffs();
};
