//
//  StringGenerator.h
//  smArt Strings
//
//  Created by Dmitry on 23.02.2024.
//  Copyright © 2024 Flycatcher Corporation Ltd. All rights reserved.
//

#include "OpenCV-4.9.0-android-sdk/sdk/native/jni/include/opencv2/core/core.hpp"
#include "OpenCV-4.9.0-android-sdk/sdk/native/jni/include/opencv2/imgproc/imgproc.hpp"

#ifndef STRINGARTGENERATOR_H
#define STRINGARTGENERATOR_H


namespace FlyCatcher {

    class StringArtGenerator {

    public:
        // NumberOfNails
        // TotalLinesToDraw
        // Seed
        // LineWeight
        // SkipNeighbors

    public:
        StringArtGenerator();

        static cv::Mat generateCircle(const cv::Mat src, int numberOfNails);

        // Bresenham Path
        // Choose Darkest Path
        //
    private:

    };

}

#endif // STRINGARTGENERATOR_H
