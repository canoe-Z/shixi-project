#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

class BinocularCamera
{
private:
    VideoCapture cap;
    int w;
    int h;

public:
    BinocularCamera();
    ~BinocularCamera();

    /**
     * @brief 初始化摄像机
     * 
     * @return true 
     * @return false 
     */
    bool init_camara();

    /**
     * @brief 返回分割后的图像
     * 
     * @return tuple<Mat, Mat> [L,R]
     */
    tuple<Mat, Mat> get_images();
};
