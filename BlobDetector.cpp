//
// Created by lec on 2016. 11. 2..
//

#include "BlobDetector.h"

Mat &BlobDetector::subtract(Mat &mat) {
    this->bgsub.operator()(mat, mat, this->LEARNING_RATE);
    return mat;
}

Mat &BlobDetector::deleteNoise(Mat &mat, int iteration) {
    morphologyEx(mat, mat, MORPH_OPEN, this->KERNEL, Point(-1, -1), 3);
    morphologyEx(mat, mat, MORPH_CLOSE, this->KERNEL, Point(-1, -1), 1);
    threshold(mat, mat, 254, 255, 0);
    dilate(mat, mat, this->KERNEL, Point(-1, -1), iteration);
    erode(mat, mat, this->KERNEL, Point(-1, -1), iteration * 2);
//    dilate(mat, mat, this->KERNEL, Point(-1, -1), iteration);
}

vector<vector<Point>> BlobDetector::getContours(Mat &mat) {
    vector<vector<Point>> contours;
    vector<Vec4i> hierarchies;

    findContours(mat, contours, hierarchies, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
    return contours;
}

vector<vector<Point>> BlobDetector::getContoursBiggerFirst(Mat &mat) {
    vector<vector<Point>> contours = this->getContours(mat);

    std::sort(contours.begin(), contours.end(), [](vector<Point> c1, vector<Point> c2) { return contourArea(c1) > contourArea(c2); });
    return contours;
}
