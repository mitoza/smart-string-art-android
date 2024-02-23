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

    Mat StringArtGenerator::generateCircle(const Mat src, int numberOfPoints) {
        Mat grey, result;
        cvtColor(src, grey, COLOR_RGB2GRAY);
        cvtColor(grey, result, COLOR_GRAY2RGBA);
        line(result, Point(0,0), Point(src.cols, src.rows/2), (255,0,0), 1);
        grey.release();
        return result;
    }

}
