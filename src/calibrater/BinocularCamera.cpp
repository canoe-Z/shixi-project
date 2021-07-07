#include "BinocularCamera.h"

bool BinocularCamera::init_camara()
{
    cap = VideoCapture(0);
    w = cap.get(CAP_PROP_FRAME_WIDTH);
    h = cap.get(CAP_PROP_FRAME_HEIGHT);
    cap.set(CAP_PROP_FRAME_WIDTH, 2 * w); //设置捕获视频的宽度
    cap.set(CAP_PROP_FRAME_HEIGHT, h);    //设置捕获视频的高度
    if (!cap.isOpened())                  //判断是否成功打开相机
    {
        cout << "摄像头打开失败!" << endl;
        return -1;
    }
}

tuple<Mat, Mat> BinocularCamera::get_images()
{
    namedWindow("test");
    namedWindow("testR");
    while (1)
    {
        Mat frame, frame_L, frame_R;
        cap >> frame;
        frame_L = frame(Rect(0, 0, w, h));
        frame_R = frame(Rect(w, 0, w, h));
        imshow("test", frame_L);
        imshow("testR", frame_R);
        if (waitKey(30) == 32)
        {
            cout << "截取图片" << endl;
            return make_tuple(frame_L, frame_R);
        }
    }
}

BinocularCamera::BinocularCamera() {}

BinocularCamera::~BinocularCamera() {}