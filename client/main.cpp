#include <iostream>
#include <opencv2/opencv.hpp>
#include "BlobDetector.h"
#include "GraphTool.h"
#include "FilterTool.h"
#include "StarSkeleton.h"
#include "curl/curl.h"
#include <thread>


using namespace cv;
using namespace std;

int main() {
    const int INPUT = 1; // default 0;
    const int WIDTH = 640;
    const int HEIGHT = 360;

    int input;
    deque<Mat> que;

    curl_global_init(CURL_GLOBAL_ALL);

//    thread t([](){
//        CURL *curl = curl_easy_init();
//        if( curl )
//        {
//            curl_easy_setopt(curl, CURLOPT_URL, "http://localhost:5000/data");
//            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, [](char *data, size_t size, size_t nmemb, void* stream) {
//                cout << data << endl;
//            });
//            CURLcode res = curl_easy_perform(curl);
//            if( res != CURLE_OK )
//            {
//                cerr << curl_easy_strerror(res) << endl;
//            }
//        }
//    });
//
//    t.join();
//
//    return 0;

    if( ! INPUT )
    {
        cout << "===== SAFE TASK =====" << endl;
        cout << "1. 웹캠" << endl;
        cout << "2. 사진" << endl;
        cout << endl;

        cout << ">>> ";
        cin >> input;
    }
    else
    {
        input = INPUT;
    }

    // 웹캠
    if( input == 1 )
    {
        VideoCapture cam1, cam2;
        Mat mat;
        BlobDetector bd;
        StarSkeleton skeleton;
        GraphTool graph_tool;
        FilterTool filter_tool;
        bool FLAG_WRITING = false;

        bd.LEARNING_RATE = 0;

        cam1.open(2); FLAG_WRITING = true;
        cam1.open("/Users/lec/Desktop/SafeDesk/2.avi"); FLAG_WRITING = false;
        cam1.set(CV_CAP_PROP_FRAME_WIDTH, WIDTH);
        cam1.set(CV_CAP_PROP_FRAME_HEIGHT, HEIGHT);
//        cam1.set(CV_CAP_PROP_FOCUS, 1);
//        cam1.set(CV_CAP_PROP_AUTO_EXPOSURE, false);
//        cam1.set(CV_CAP_PROP_AUTOGRAB, false);
//        cam1.set(CV_CAP_PROP_FPS, 25);

//        namedWindow("background select");
//        while( waitKey(33) != 32 )
//        {
//            cam1 >> mat;
//            imshow("background select", mat);
//        }
//
//        destroyAllWindows();
//        namedWindow("original");

        while( waitKey(33) != 32 )
        {
            cam1 >> mat;
            Mat origin_mat = mat.clone();
            imshow("original", origin_mat);
            que.push_back(origin_mat.clone());

            bd.subtract(mat);
            imshow("subtract", mat);
            bd.deleteNoise(mat);
            imshow("deleted noise", mat);

            vector<vector<Point>> contours;
            contours = bd.getContoursBiggerFirst(mat);

            int minArea = ((WIDTH * HEIGHT) / 12);

            for( int i=0; i<contours.size(); i++ )
            {
                if( contourArea(contours[i]) >= minArea )
                {
                    vector<double> dist = skeleton.getDistanceFromCentroid(contours[i]);
//                    Mat graph1 = graph_tool.drawGraph(dist, WIDTH, HEIGHT, Scalar(0, 255, 0));
//                    vector<double> dist2 = filter_tool.filter(dist, vector<double>({0.5, 0.9, 1, 0.9, 0.5}));
//                    Mat graph2 = graph_tool.drawGraph(dist2, WIDTH, HEIGHT, Scalar(0, 255, 0));
//                    imshow("graph1", graph1);
//                    imshow("graph2", graph2);

                    vector<Point> features = skeleton.getFeatures(contours[i], dist);
                    if( features.size() == 0 )
                        break;

                    // heuristic
                    Point head, lhand, rhand, c;

                    // retrieve center point
                    Moments m = moments(contours[i]);
                    c = Point((int)(m.m10/m.m00), (int)(m.m01/m.m00));

                    // lhand && rhand
                    lhand = Point(WIDTH, 0);
                    rhand = Point(0, 0);
                    for_each(features.begin(), features.end(), [&](Point &f) {
                        if( abs(c.x - f.x) < 50 )
                            return;
                        if( lhand.x == WIDTH )
                        {
                            if( f.x < lhand.x )
                                lhand = f;
                        }
                        else
                        {
                            if( abs(lhand.x - f.x) > 20 )
                                if( f.x < lhand.x )
                                    lhand = f;
                        }
                        if( rhand.x == WIDTH )
                        {
                            if( f.x > rhand.x )
                                rhand = f;
                        }
                        else
                        {
                            if( abs(rhand.x - f.x) > 20 )
                                if( f.x > rhand.x )
                                    rhand = f;
                        }
                    });
                    //head
                    head = Point(0, HEIGHT);
                    for_each(features.begin(), features.end(), [&](Point &f) {
                        if( abs(c.x - f.x) > 50 )
                            return;

                        if( lhand.x < f.x && f.x < rhand.x )
                            if( f.y < head.y )
                                head = f;
                    });

                    circle(origin_mat, head, 3, Scalar(255, 0, 0), 3);
                    circle(origin_mat, lhand, 3, Scalar(0, 255, 0), 3);
                    circle(origin_mat, rhand, 3, Scalar(0, 0, 255), 3);
                    imshow("skeletonized", origin_mat);
                    break;
                }
            }
        }

        if( FLAG_WRITING )
        {
            VideoWriter writer;
            int ex = CV_FOURCC('m', 'p', '4', 'v'); // for OSX
            writer.open("/Users/lec/Desktop/SafeDesk/2.avi", ex, 15.0, Size(WIDTH, HEIGHT));
            while( ! que.empty() )
            {
                mat = que.front();
                writer << mat;
                que.pop_front();
            }
            writer.release();
        }

        return 0;
    }
    // 사진
    else if( input == 2 ) {
        BlobDetector bd;
        StarSkeleton skeleton;

        Mat mat = imread("/Users/lec/Desktop/SafeDesk/noisy.png", 0);
        resize(mat, mat, Size(WIDTH, HEIGHT));

        Mat original_mat = imread("/Users/lec/Desktop/SafeDesk/noisy.png");
        resize(original_mat, original_mat, Size(WIDTH, HEIGHT));

        // delete noise
        bd.deleteNoise(mat);
        imshow("delete noise", mat);

        // retrieve contours
        vector<vector<Point>> contours;
        contours = bd.getContoursBiggerFirst(mat);
        assert(contours.size() > 0);

        // contours[0] is the biggest blob
        vector<Point> contour = contours[0];
        vector<double> dist = skeleton.getDistanceFromCentroid(contour);
        assert(dist.size() > 0);

        // get star skeleton features
        vector<Point> features = skeleton.getFeatures(contour, dist);
        assert(features.size() > 0);

        for( int i=0; i<features.size(); i++ )
        {
            circle(original_mat, features[i], 3, Scalar(0, 255, 0), 3);
        }

        imshow("original_mat", original_mat);
        waitKey(0);
    }
    // 그외
    else
    {
        cout << "idiot..." << endl;
    }

    return 0;
}