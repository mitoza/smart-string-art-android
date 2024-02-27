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

    struct Line {
        int startPin, endPin;
        int distance;
        std::vector<Point> points;
    };

    class StringArtGenerator {

    public:
        // NumberOfNails
        // TotalLinesToDraw
        // Seed
        // LineWeight
        // SkipNeighbors
        const int MAX_LINES = 4000;
        const int LINE_WEIGHT = 8;


    public:
        StringArtGenerator();

        Mat generateCircle(const Mat src, int sizeOfPins, int minDistance, int maxLines);

        void sobelFilter(const Mat &src_gray, Mat &dst);
        void circleCrop(const Mat &src, Mat &dst);

        void fillCircleOfPins(const Mat &src, int sizeOfPins, Point *pins);

        // Bresenham Path
        void precalculateLines(const Mat &src, int sizeOfPins, int minDistance, Point *pins, Line *lines);
        void calculateLines(const Mat &src, int sizeOfPins, int minDistance, int maxLines,
                                   Point *pins, Line *lines);


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
