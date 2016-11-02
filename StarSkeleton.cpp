//
// Created by lec on 2016. 11. 2..
//

#include "StarSkeleton.h"

vector<double> StarSkeleton::getDistanceFromCentroid(vector<Point> contour) {
    Moments m = moments(contour);
    Point c = Point((int)(m.m10/m.m00), (int)(m.m01/m.m00));
    vector<double> dist;
    for( int i=0; i<contour.size(); i++ )
    {
        dist.push_back(norm(c - contour[i]));
    }
    return dist;
}

vector<Point> StarSkeleton::getFeatures(vector<Point> &contour, vector<double> &dist) {
    return this->getFeatures(contour, dist, vector<double>({0.5, 0.9, 1, 0.9, 0.5}));
}

vector<Point> StarSkeleton::getFeatures(vector<Point> &contour, vector<double> &dist, vector<double> kernel) {
    FilterTool filter;
    vector<double> dist2 = filter.filter(dist, kernel);
    vector<Point> contour2;

    bool increase = true;
    for( int i=1; i<dist2.size(); i++ )
    {
        if( increase && dist2[i-1] > dist2[i] )
        {
            contour2.push_back(contour[i - 1]);
            increase = false;
        }
        else if( ! increase && dist2[i-1] < dist2[i] )
        {
            increase = true;
        }
    }

    return contour2;
}
