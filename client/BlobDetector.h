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
    double LEARNING_RATE = 0;
     Mat KERNEL = Mat();

    BackgroundSubtractorMOG2 bgsub;
    Mat& subtract(Mat&);
    Mat& deleteNoise(Mat& mat, int iteration = 1);
    vector<vector<Point>> getContours(Mat& mat);
    vector<vector<Point>> getContoursBiggerFirst(Mat& mat);
};


#endif //SAFEDESK_BACKGROUNDSUBTRACTOR_H
