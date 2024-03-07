//
//  StringGenerator.cpp
//  smArt Strings
//
// Created by Dmitry on 23.02.2024.
// Copyright © 2024 FlyCatcher. All rights reserved.
//

#include "StringArtGenerator.h"

using namespace cv;

namespace fc {

    StringArtGenerator::StringArtGenerator() {

    }

    StringArtGenerator::~StringArtGenerator() {
        release();
    }

    Mat StringArtGenerator::generateCircle(const Mat src) {
        progress(0);
        /* Image preparation */
        Mat bsrc = prepareImage(src);
        progress(3);

        /* Find the lines */

        // Set pins
        if (_pins.empty()) {
            _pins = fillPinsAsCircle(bsrc);
        }
        progress(4);

        // Precalculate all potential Lines
        long linesTime = currentTimeInNanos();
        if (_preLines.empty()) {
            _preLines = precalculateLines(_pins);
        }
        log(format("Pre-lines time: %ldns", (currentTimeInNanos() - linesTime)));
        progress(5);

        // In Loop search the best line with most darkest color from one pin to other
        _lineSequence.clear();
        _lineSequence = calculateLines(bsrc, _preLines);
        if (!progress(99)) {
            log(format("Progress dead"));
        }

        // Draw lines
        Mat dst = draw(bsrc.size(), _pins, _lineSequence, _lineWeight);

        // Draw pins
        drawPins(dst, _pins);
        progress(100);

        /* Result */
        Mat result;
        cvtColor(dst, result, COLOR_GRAY2RGBA);
//        cvtColor(bsrc, result, COLOR_GRAY2RGBA);


        // Release resources
        dst.release();
        bsrc.release();

        return result;
    } // End of generateCircle()

    Mat StringArtGenerator::prepareImage(const Mat &src) {

        // Make image gray
        GaussianBlur(src, src, Size(3, 3), 0, 0, BORDER_DEFAULT);

        // Resize
        Mat resize;
        resizeAndCrop(src, resize);

        // Make Grey IMAGE
        Mat dst(resize.size(), CV_8UC1);
        cvtColor(resize, dst, COLOR_RGB2GRAY); // Average method
        //extractChannel(src, dst, 0); // Extract RED channel
        resize.release();

        // Invert image
        int kernelSize = 9;
        GaussianBlur(dst, dst, cv::Size(kernelSize, kernelSize), 0.0);
        //adaptiveThreshold(bsrc, bsrc, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, kernelSize, 10);
        //sobelFilter(dst, dst); // Sobel invert
        bitwise_not(dst, dst); // Bitwise invert

        // Remove noise
        //int kernelSize2 = 9;
        //GaussianBlur(bsrc, bsrc, cv::Size(kernelSize2, kernelSize2), 0.0);

        // Circle crop
        cropCircle(dst, dst);

        return dst;
    } // End of prepareImage()

    void StringArtGenerator::sobelFilter(const Mat &src_gray, Mat &dst) {
        int kernelSize = 3; // 1, 3, 5, 7
        int scale = 2; // 1, 2, 3, ...
        int delta = 0; // 0
        int ddepth = CV_16S;
        Mat grad_x, grad_y;
        Mat abs_grad_x, abs_grad_y;
        Sobel(src_gray, grad_x, ddepth, 1, 0, kernelSize, scale, delta, BORDER_DEFAULT);
        Sobel(src_gray, grad_y, ddepth, 0, 1, kernelSize, scale, delta, BORDER_DEFAULT);
        // converting back to CV_8U
        convertScaleAbs(grad_x, abs_grad_x);
        convertScaleAbs(grad_y, abs_grad_y);
        addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, dst);
        grad_x.release();
        grad_y.release();
        abs_grad_x.release();
        abs_grad_y.release();
    } // End of sobelFilter()

    void StringArtGenerator::resizeAndCrop(const cv::Mat &src, cv::Mat &dst) {
        cv::Mat output;
        double h1 = _width * (src.rows/(double)src.cols);
        double w2 = _height * (src.cols/(double)src.rows);
        if( h1 <= _height) {
            resize(src, output, cv::Size(_width, h1));
        } else {
            resize(src, output, cv::Size(w2, _height));
        }
        int top = (_height-output.rows) / 2;
        int down = (_height-output.rows+1) / 2;
        int left = (_width - output.cols) / 2;
        int right = (_width - output.cols+1) / 2;
        cv::copyMakeBorder(output, dst, top, down, left, right, BORDER_CONSTANT, 0 );
        output.release();
    }

    void StringArtGenerator::cropCircle(const Mat &src, Mat &dst) {
        Mat circleMask = Mat(src.size(), CV_8UC1, 255);
        circle(circleMask, Point(src.cols / 2, src.rows / 2), src.cols / 2, 0, -1);
        subtract(src, circleMask, dst);
        circleMask.release();
    } // End of cropCircle()

    std::vector<Point> StringArtGenerator::fillPinsAsCircle(const Mat &src) {
        int center = min(src.rows, src.cols) / 2;
        int radius = center - 1;
        double angle;
        std::vector<Point> pins;
        for (int i = 0; i < _sizeOfPins; i++) {
            angle = 2 * CV_PI * i / _sizeOfPins;
            Point pin = Point((int) (src.cols / 2 + radius * cos(angle)),
                              (int) (src.rows / 2 + radius * sin(angle)));
            pins.push_back(pin);
        }
        return pins;
    } // End of fillPinsAsCircle()

    // TODO: Implement LineIterator from OpenCV
    std::vector<std::vector<Point>>
    StringArtGenerator::precalculateLines(std::vector<Point> &pins) {
        int first, second, distance;
        double deltaX, deltaY;
        Point p0, p1;
        Point linePoint;
        std::vector<std::vector<Point>> lines(pins.size() * pins.size());
        for (int i = 0; i < _sizeOfPins; i++) {
            for (int j = i + _minDistance; j < _sizeOfPins; j++) {
                first = j * _sizeOfPins + i;
                second = i * _sizeOfPins + j;
                p0 = pins[i];
                p1 = pins[j];
                // Calculate Bresenham Path or LineIterator(OpenCV)
                distance = (int) sqrt(
                        (p1.x - p0.x) * (p1.x - p0.x) + (p1.y - p0.y) * (p1.y - p0.y));
                // Fill line with pixels coordinates
                if (distance == 1) continue;
                deltaX = (p1.x - p0.x) / (distance - 1.0);
                deltaY = (p1.y - p0.y) / (distance - 1.0);
                for (int k = 0; k < distance - 1; k = k + 1) {
                    linePoint = Point((int) (p0.x + deltaX * k),
                                      (int) (p0.y + deltaY * k));
                    lines[first].push_back(linePoint);
                    if (first != second) {
                        lines[second].push_back(linePoint);
                    }
                }
                // Ensure that the last point of line is p1
                lines[first].push_back(p1);
                if (first != second) {
                    lines[second].push_back(p1);
                }

            }
        }
        return lines;
    } // End of precalculateLines()

    std::vector<int> StringArtGenerator::calculateLines(cv::Mat &src,
                                                        std::vector<std::vector<Point>> &lines) {

        std::vector<int> lineSequence, lastPins;
        int currentPin = 0;
        int bestPin;
        uint lineError;
        uint maxError;
        int index = 0;
        int innerIndex;

        lineSequence.push_back(currentPin);
        for (int i = 0; i < _maxLines; i++) {
            bestPin = -1;
            maxError = 0;

            for (int offset = _minDistance; offset < _sizeOfPins - _minDistance; offset++) {
                int testPin = (currentPin + offset) % _sizeOfPins;

                // Check if lastPins contains testPin
                if (std::find(lastPins.begin(), lastPins.end(), testPin) != lastPins.end()) {
                    continue;
                }

                innerIndex = testPin * _sizeOfPins + currentPin;

                // Calculate line error
                lineError = 0;
                uchar colorValue;
                for (auto ptr = lines[innerIndex].begin(); ptr < lines[innerIndex].end(); ptr++) {
                    colorValue = src.at<uchar>(*ptr);
                    if (colorValue > 0) {
                        lineError = lineError + (int) colorValue;
                    }
                }
                if (lineError > maxError) {
                    maxError = lineError;
                    bestPin = testPin;
                    index = innerIndex;
                }
            }

            lineSequence.push_back(bestPin);

            int lineSize = lines[index].size();
            for (int j = 0; j < lineSize; j++) {
                src.at<uchar>(lines[index][j]) =
                        max(0, src.at<uchar>(lines[index][j]) - _lineWeight);
            }

            // Make sure that we have only 20 previous pins that not repeated
            if (lastPins.size() > 20) {
                lastPins.erase(lastPins.begin());
            }
            lastPins.push_back(bestPin);
            currentPin = bestPin;
        }

        return lineSequence;
    } // End of calculateLines()

    cv::Mat StringArtGenerator::draw(Size matSize, const std::vector<Point> &pins,
                                     std::vector<int> &lineSequence, int lineWeight) {
        Mat lineCanvas(matSize, CV_8UC1);
        Mat tmp(matSize, CV_8UC1);
        lineCanvas.setTo(0);
        for (int i = 1; i < lineSequence.size(); i++) {
            if (lineSequence[i] == -1) break;
            tmp.setTo(0);
            line(tmp, pins[lineSequence[i - 1]], pins[lineSequence[i]], lineWeight, 1);
            add(lineCanvas, tmp, lineCanvas);
        }
        tmp.release();
        bitwise_not(lineCanvas, lineCanvas);
        return lineCanvas;
    } // End of draw()

    void StringArtGenerator::drawPins(const cv::Mat &src, const std::vector<Point> &pins,
                                      const int radius, const Scalar color) {
        for (auto ptr = pins.begin(); ptr < pins.end(); ptr++) {
            circle(src, *ptr, radius, color, -1);
        }
    } // End of drawPins()

    void StringArtGenerator::release() {
        // Destroy Pins
        _pins.clear();
        std::destroy(_pins.begin(), _pins.end());

        // Destroy PreLines
        for (auto ptr = _preLines.begin(); ptr < _preLines.end(); ptr++) {
            ptr->clear();
            std::destroy(ptr->begin(), ptr->end());
        }
        _preLines.clear();
        std::destroy(_preLines.begin(), _preLines.end());

        // Destroy Line Sequence
        _lineSequence.clear();
        std::destroy(_lineSequence.begin(), _lineSequence.end());
    } // End of release();

//    void parallelWork(int numberOfNails) {
//        parallel_for_(Range(0, numberOfNails), [&](const Range& range) {
//            for (int y = range.start; y < range.end; ++y) {
//                dst.at<uchar>(y, x) = dst.at<uchar>(y - 1, x -1);
//            }
//        });
//    }

}
