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

        bd.LEARNING_RATE = 0;

        cam1.open(2);
        cam1.set(CV_CAP_PROP_FRAME_WIDTH, WIDTH);
        cam1.set(CV_CAP_PROP_FRAME_HEIGHT, HEIGHT);
//        cam1.set(CV_CAP_PROP_FOCUS, 1);
//        cam1.set(CV_CAP_PROP_AUTO_EXPOSURE, false);
//        cam1.set(CV_CAP_PROP_AUTOGRAB, false);
        cam1.set(CV_CAP_PROP_FPS, 25);

        while( waitKey(30) != 32 )
        {
            cam1 >> mat;
            imshow("background select", mat);
        }

        while( waitKey(1) != 32 )
        {
            cam1 >> mat;
            que.push_back(mat.clone());
            imshow("background select", mat);
        }

        VideoWriter writer;
//        int ex = (int)cam1.get(CV_CAP_PROP_FOURCC);
//        ex = CV_FOURCC('M', 'P', '4', '2');
//        ex = CV_FOURCC('M', 'J', 'P', 'G');
//        ex = CV_FOURCC('X', 'V', 'I', 'D');
//        ex = CV_FOURCC('D', 'I', 'V', 'X');
        int ex = CV_FOURCC('m', 'p', '4', 'v'); // for OSX
        writer.open("/Users/lec/Desktop/SafeDesk/1.avi", ex, 15.0, Size(WIDTH, HEIGHT));
        while( ! que.empty() )
        {
            mat = que.front();
            writer << mat;
            que.pop_front();
        }
        writer.release();

        return 0;

        while( waitKey(33) != 32 )
        {
            cam1 >> mat;
            bd.subtract(mat);
            bd.deleteNoise(mat);
            imshow("deleted noise", mat);

            vector<vector<Point>> contours;
            contours = bd.getContoursBiggerFirst(mat);

            int minArea = ((WIDTH * HEIGHT) / 8);

            for( int i=0; i<contours.size(); i++ )
            {
                if( contourArea(contours[i]) >= minArea )
                {
                    Mat mat2;
                    cvtColor(mat, mat2, CV_GRAY2BGR);
                    drawContours(mat2, contours, 0, Scalar(0, 0, 255), 3);
                    imshow("mat2", mat2);
                }
            }
        }
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