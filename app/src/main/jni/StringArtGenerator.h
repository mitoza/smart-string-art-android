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

    using ProgressCallback = bool (*)(int progress);

    class StringArtGenerator {
        
    private:
        int _width = 500;
        int _height = 500;
        int _sizeOfPins = 288;
        int _minDistance = 20;
        int _maxLines = 2000;
        int _lineWeight = 20;

        std::vector<Point> _pins;
        std::vector<std::vector<Point>> _preLines;
        std::vector<int> _lineSequence;

        ProgressCallback _progressCallback = nullptr;

    public:
        StringArtGenerator();
        ~StringArtGenerator();

        void setSize(int width, int height) { _width = width; _height = height; };
        void setSizeOfPins(int sizeOfPins) { _sizeOfPins = sizeOfPins; };
        void setMinDistance(int minDistance) { _minDistance = minDistance; };
        void setMaxLines(int maxLines) { _maxLines = maxLines; };
        void setLineWeight(int lineWeight) { _lineWeight = lineWeight; };

        Mat generateCircle(const Mat src);

        void addCallback(ProgressCallback fptr) { _progressCallback = fptr; };
        void removeCallback() { _progressCallback = nullptr; };

        void release();

    private: // Internal methods

        Mat prepareImage(const Mat &src);

        void sobelFilter(const Mat &src_gray, Mat &dst);

        void resizeAndCrop(const Mat &src, Mat &dst);

        void cropCircle(const Mat &src, Mat &dst);

        std::vector<Point> fillPinsAsCircle(const Mat &src);

        std::vector<std::vector<Point>> precalculateLines(std::vector<Point> &pins);

        std::vector<int> calculateLines(Mat &src, std::vector<std::vector<Point>> &lines);


        Mat draw(Size matSize, const std::vector<Point> &pins, std::vector<int> &lineSequence,
                 int lineWeight = 20);

        void drawPins(const Mat &src, const std::vector<Point> &pins, const int radius = 1, const Scalar color = 128);


        bool progress(int progress) {
            if (nullptr == _progressCallback) return true;
            return _progressCallback(progress);
        }

    private: // Android methods

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
