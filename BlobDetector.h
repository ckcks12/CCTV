//
// Created by lec on 2016. 11. 2..
//

#ifndef SAFEDESK_BACKGROUNDSUBTRACTOR_H
#define SAFEDESK_BACKGROUNDSUBTRACTOR_H

#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

class BlobDetector {
public:
    const int LEARNING_RATE = 0;
    const Mat KERNEL = Mat();

    BackgroundSubtractorMOG2 bgsub;
    Mat& subtract(Mat&);
    Mat& deleteNoise(Mat& mat, int iteration = 1);
};


#endif //SAFEDESK_BACKGROUNDSUBTRACTOR_H
