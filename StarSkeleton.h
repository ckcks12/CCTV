//
// Created by lec on 2016. 11. 2..
//

#ifndef SAFEDESK_STARSKELETON_H
#define SAFEDESK_STARSKELETON_H

#include <opencv2/opencv.hpp>
#include "FilterTool.h"


using namespace std;
using namespace cv;

class StarSkeleton {
public:
    vector<double> getDistanceFromCentroid(vector<Point> contour);
    vector<Point> getFeatures(vector<Point>& contour, vector<double>& dist);
    vector<Point> getFeatures(vector<Point>& contour, vector<double>& dist, vector<double> kernel, double minBetweenDist);
};


#endif //SAFEDESK_STARSKELETON_H
