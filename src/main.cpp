#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <math.h>

#include "calibrater/Calibrater.h"

using namespace std;
using namespace cv;

int main()
{
    auto c = Calibrater(Size(11, 8), 5);
    c.calibrate();
    return 0;
}