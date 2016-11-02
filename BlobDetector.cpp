//
// Created by lec on 2016. 11. 2..
//

#include "BackgroundSubtractor.h"

Mat &BackgroundSubtractor::subtract(Mat &mat) {
    Mat mat2;
    this->bgsub.operator()(mat, mat2, this->LEARNING_RATE);
    return mat2;
}

Mat &BackgroundSubtractor::deleteNoise(Mat &mat, int iteration = 3) {
    erode(mat, mat, this->KERNEL, Point(-1, -1), iteration);
    morphologyEx(mat, mat, MORPH_CLOSE, this->KERNEL);
}
