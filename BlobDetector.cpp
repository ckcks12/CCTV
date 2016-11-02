//
// Created by lec on 2016. 11. 2..
//

#include "BlobDetector.h"

Mat &BlobDetector::subtract(Mat &mat) {
    this->bgsub.operator()(mat, mat, this->LEARNING_RATE);
    return mat;
}

Mat &BlobDetector::deleteNoise(Mat &mat, int iteration) {
    erode(mat, mat, this->KERNEL, Point(-1, -1), iteration);
    morphologyEx(mat, mat, MORPH_CLOSE, this->KERNEL);
    threshold(mat, mat, 254, 255, 0);
}
