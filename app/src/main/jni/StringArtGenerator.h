//
//  StringGenerator.h
//  smArt Strings
//
//  Created by Dmitry on 23.02.2024.
//  Copyright © 2024 Flycatcher Corporation Ltd. All rights reserved.
//

#include "OpenCV-4.9.0-android-sdk/sdk/native/jni/include/opencv2/core/core.hpp"
#include "OpenCV-4.9.0-android-sdk/sdk/native/jni/include/opencv2/imgproc/imgproc.hpp"
#include <android/log.h>
#include <iterator> // for iterators
#include <vector> // for vectors

#ifndef STRINGARTGENERATOR_H
#define STRINGARTGENERATOR_H

#define APP_NAME "StringArt"

using namespace cv;

namespace fc {

    class StringArtGenerator {

    public:
        // NumberOfNails
        // TotalLinesToDraw
        // Seed
        // LineWeight
        // SkipNeighbors
        static const int MAX_LINES = 4000;
        static const int PINS_NUMBER = 288;

    public:
        StringArtGenerator();

        Mat generateCircle(const Mat src, int sizeOfPins, int minDistance, int maxLines);

        void sobelFilter(const Mat &src_gray, Mat &dst);
        void circleCrop(const Mat &src, Mat &dst);

        void fillPinsAsCircle(const Mat &src, int sizeOfPins, std::vector<Point> &pins);
        // Bresenham Path
        void precalculateLines(const Mat &src, int sizeOfPins, int minDistance,
                               std::vector<Point> &pins, std::vector<std::vector<Point>> &lines);
        void calculateLines(cv::Mat &src, int sizeOfPins, int minDistance, int maxLines,
                            std::vector<Point> &pins, std::vector<std::vector<Point>> &lines);

        // Choose Darkest Path
        //
    private:

        void log(String msg) {
            log(msg.c_str());
        };

        void log(const char *fmt...) {
          __android_log_print(ANDROID_LOG_VERBOSE, APP_NAME, "%s", fmt);
        };

        long currentTimeInNanos() {
            struct timespec res;
            clock_gettime(CLOCK_MONOTONIC, &res);
            return (res.tv_sec * 1000000000) + res.tv_nsec;
        }

    };

}

#endif // STRINGARTGENERATOR_H
