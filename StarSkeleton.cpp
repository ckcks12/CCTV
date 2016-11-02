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
    vector<pair<Point, double>> features_bag;
    vector<Point> features;

    bool increase = true;
    for( int i=1; i<dist2.size(); i++ )
    {
        if( increase && dist2[i - 1] > dist2[i] )
        {
            features_bag.push_back(pair<Point, double>(contour[i - 1], dist2[i-1]));
            increase = false;
        }
        else if( ! increase && dist2[i - 1] < dist2[i] )
        {
            increase = true;
        }
    }

    std::sort(features_bag.begin(), features_bag.end(), [](pair<Point, double> p1, pair<Point, double> p2) {
        return p1.second < p2.second;
    });

    for( int i=0; i<features_bag.size(); i++ )
    {
        features.push_back(features_bag[i].first);
    }
    
    return features;
}
