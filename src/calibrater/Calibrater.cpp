#include "Calibrater.h"
#include "BinocularCamera.h"

void Calibrater::calibrate(const int board_num, const bool write_flag)
{
    auto camera = BinocularCamera();
    camera.init_camara();

    vector<vector<Point3f>> objRealPoint;
    vector<vector<Point2f>> imagePointL;
    vector<vector<Point2f>> imagePointR;

    Size image_size;

    for (int i = 0; i < board_num;)
    {
        auto [frame_L, frame_R] = camera.get_images();
        image_size = Size(frame_L.cols, frame_L.rows);
        auto cornersL = get_imagePoint(frame_L);
        auto cornersR = get_imagePoint(frame_R);

        if (cornersL.has_value() && cornersR.has_value())
        {
            imagePointL.push_back(cornersL.value());
            imagePointR.push_back(cornersR.value());
            drawChessboardCorners(frame_L, board_size, cornersL.value(), true); //画出棋盘角点
            drawChessboardCorners(frame_R, board_size, cornersR.value(), true); //画出棋盘角点
            auto obj = get_objPoint();
            objRealPoint.push_back(obj);
            i++;

            string path = R"(C:\Users\liwentao\Desktop\shixi-project\data\)";
            string filename1 = cv::format("1_%d", i) + "corner"; //图片文件名
            string filename2 = cv::format("2_%d", i) + "corner"; //图片文件名
            filename1 = path + filename1;
            filename2 = path + filename2;
            filename1 += ".png";
            filename2 += ".png";
            imwrite(filename1, frame_L);
            imwrite(filename2, frame_R);
        }
        else
        {
            std::cout << "未找到角点" << endl;
        }

        namedWindow("resultL");
        imshow("resultL", frame_L);
        namedWindow("resultR");
        imshow("resultR", frame_R);
        waitKey(-1);
        destroyWindow("result_L");
        destroyWindow("result_R");
    }

    cout << "标定中" << endl;
    stereoCalibrate(objRealPoint,
                    imagePointL,
                    imagePointR,
                    cameraMatrixL,
                    distCoeffL,
                    cameraMatrixR,
                    distCoeffR,
                    image_size,
                    R,
                    T,
                    E,
                    F,
                    CALIB_SAME_FOCAL_LENGTH,
                    TermCriteria(TermCriteria::COUNT + TermCriteria::EPS, 100, 1e-5));

    cout << "标定完毕" << endl;
    if (write_flag)
    {
        write_result();
    }
}

void Calibrater::write_result()
{
    string filename1 = "camera_matrix_L.xml";
    FileStorage fs1(filename1, FileStorage::WRITE);
    fs1 << "cameraMatrix" << cameraMatrixL; //写出内参矩阵
    fs1 << "distCoeffs" << distCoeffL;      //写出畸变系数
    fs1.release();                          //关闭文件

    string filename2 = "camera_matrix_R.xml";
    FileStorage fs2(filename2, FileStorage::WRITE);
    fs2 << "cameraMatrix" << cameraMatrixR; //写出内参矩阵
    fs2 << "distCoeffs" << distCoeffR;      //写出畸变系数
    fs2.release();

    string filenameR = "R.xml";
    FileStorage fsR(filenameR, FileStorage::WRITE);
    fsR << "R" << R; //写出旋转矩阵
    fsR.release();

    string filenameT = "T.xml";
    FileStorage fsT(filenameT, FileStorage::WRITE);
    fsT << "T" << T; //写出平移向量
    fsT.release();

    string filenameE = "E.xml";
    FileStorage fsE(filenameE, FileStorage::WRITE);
    fsE << "E" << E; //写出本质矩阵
    fsE.release();

    string filenameF = "F.xml";
    FileStorage fsF(filenameF, FileStorage::WRITE);
    fsF << "F" << F; //写出基本矩阵
    fsF.release();
}

Calibrater::Calibrater(const Size &board_size, const int &square_size)
{
    this->board_size = board_size;
    this->square_size = square_size;
}

Calibrater::~Calibrater() {}

optional<vector<Point2f>> Calibrater::get_imagePoint(const Mat &img)
{
    vector<Point2f> corners;

    //找到全部角点时
    if (bool find = findChessboardCorners(img, board_size, corners); find)
    {
        Mat gray;
        cvtColor(img, gray, COLOR_RGB2GRAY);
        cornerSubPix(gray, corners, Size(5, 5), Size(-1, -1), TermCriteria(TermCriteria::COUNT + TermCriteria::EPS, 30, 0.1));
        return corners;
    }
    else
    {
        return nullopt;
    }
}

vector<Point3f> Calibrater::get_objPoint()
{
    vector<Point3f> object_points; //角点的实际空间坐标

    Point3f temp;
    for (int i = 0; i < board_size.height; i++) //8
    {
        for (int j = 0; j < board_size.width; j++) //11
        {
            temp.x = i * square_size;
            temp.y = j * square_size;
            temp.z = 0;
            object_points.push_back(temp);
        }
    }
    return object_points;
}