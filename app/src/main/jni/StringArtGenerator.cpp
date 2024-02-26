//
//  StringGenerator.cpp
//  smArt Strings
//
// Created by Dmitry on 23.02.2024.
// Copyright © 2024 FlyCatcher. All rights reserved.
//

#include "StringArtGenerator.h"

using namespace cv;

namespace FlyCatcher {

    StringArtGenerator::StringArtGenerator() {

    }

    Mat StringArtGenerator::generateCircle(const Mat src, int sizeOfPins, int minDistance) {
        /* Image preparation */
        Mat bsrc = Mat(src.size(), CV_8UC1);

        // Make image gray
        GaussianBlur(src, src, Size(3, 3), 0, 0, BORDER_DEFAULT);
        cvtColor(src, bsrc, COLOR_RGB2GRAY);

        // Sobel Filter
        //sobelFilter(bsrc, bsrc);

        // Inverted image
        int kernelSize = 9;
        GaussianBlur(bsrc, bsrc, cv::Size(kernelSize, kernelSize), 0.0);
        //adaptiveThreshold(bsrc, bsrc, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, kernelSize, 10);
        sobelFilter(bsrc, bsrc);
        //bitwise_not(bsrc, bsrc);
        int kernelSize2 = 9;
        GaussianBlur(bsrc, bsrc, cv::Size(kernelSize2, kernelSize2), 0.0);


        // Circle crop
        circleCrop(bsrc, bsrc);

        /* Find the lines */

        // Set pins
        Point pins[sizeOfPins];
        long pinsTime = currentTimeInNanos();
        fillCircleOfPins(bsrc, sizeOfPins, pins);
        //log(format("Pins time: %ldns", (currentTimeInNanos() - pinsTime)));

        // Precalculate all potential Lines
        Line preLines[sizeOfPins * sizeOfPins];
        long linesTime = currentTimeInNanos();
        precalculateLines(bsrc, sizeOfPins, minDistance, pins, preLines);
        log(format("Lines time: %ldns", (currentTimeInNanos() - linesTime)));

        // In Loop search the best line with most darkest color from one nail to other

        /* Result */
        Mat result;
        cvtColor(bsrc, result, COLOR_GRAY2RGBA);

        // Release resources
        bsrc.release();

        //line(result, Point(0,0), Point(src.cols, src.rows/2), (255,0,0), 1);
        return result;
    }

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
    }

    void StringArtGenerator::circleCrop(const Mat &src, Mat &dst) {
        Mat circleMask = Mat(src.size(), CV_8UC1, 255);
        circle(circleMask, Point(src.cols / 2, src.rows / 2), src.cols / 2, 0, -1);
        Mat circleImage;
        subtract(src, circleMask, dst);
        circleImage.release();
    }

    void StringArtGenerator::fillCircleOfPins(const Mat &src, int sizeOfPins, Point *pins) {
        int nailRadius = 1;
        int center = min(src.rows, src.cols) / 2;
        int radius = center - 1;
        Vec3b nailColor = 128;
        double angle;

        for (int i = 0; i < sizeOfPins; i++) {
            angle = 2 * CV_PI * i / sizeOfPins;
            pins[i] = Point((int) (center + radius * cos(angle)),
                            (int) (center + radius * sin(angle)));
            circle(src, pins[i], nailRadius, nailColor, -1);
            log(format("Pin[%d]: %d, %d", i, pins[i].x, pins[i].y));
        }
    }

    void StringArtGenerator::precalculateLines(
            const Mat &src, int sizeOfPins, int minDistance, Point *pins, Line *lines) {
        int first, second, distance;
        double deltaX, deltaY;
        Point p0, p1;

        for (int i = 0; i < sizeOfPins; i++) {
            for (int j = i + minDistance; j < sizeOfPins; j++) {
                first = j * sizeOfPins + i;
                second = i * sizeOfPins + j;
                p0 = pins[i];
                p1 = pins[j];
                distance = (int) sqrt(
                        (p1.x - p0.x) * (p1.x - p0.x) + (p1.y - p0.y) * (p1.y - p0.y));
                // Fill line with pixels coordinates
                if (distance != 1) {
                    deltaX = (p1.x - p0.x) / (distance - 1.0);
                    deltaY = (p1.y - p0.y) / (distance - 1.0);
                    for (int k = 0; k < distance - 1; ++k) {
                        Point linePoint = Point((int) (p0.x + deltaX * k),
                                                (int) (p0.y + deltaY * k));
                        lines[first].points.push_back(linePoint);
                        if (first != second) {
                            lines[second].points.push_back(linePoint);
                        }
                        if (i == 0) {
                            circle(src, linePoint, 1, 255, -1);
                        }
                    }
                    // Ensure that the last point of line is p1
                    lines[first].points.push_back(p1);
                    if (first != second) {
                        lines[second].points.push_back(p1);
                    }
                }

                lines[first].distance = distance;
                lines[first].startPin = i;
                lines[first].endPin = j;
                //log(format("Line[%d](%d, %d): %d", first, lines[first].startPin, lines[first].endPin, lines[first].distance));
                if (first != second) {
                    lines[second].distance = distance;
                    lines[second].startPin = j;
                    lines[second].endPin = i;
                    //    log(format("Line[%d](%d, %d): %d", second, lines[second].startPin, lines[second].endPin, lines[second].distance));
                }
            }

        }


    }

//    void parallelWork(int numberOfNails) {
//        parallel_for_(Range(0, numberOfNails), [&](const Range& range) {
//            for (int y = range.start; y < range.end; ++y) {
//                dst.at<uchar>(y, x) = dst.at<uchar>(y - 1, x -1);
//            }
//        });
//    }

}
