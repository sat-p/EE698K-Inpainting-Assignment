#ifndef __EE698K_TOOLS_SLIC_H__
#define __EE698K_TOOLS_SLIC_H__

#include "../../algos/include/slicGC.h"

#include <cmath>
#include <iostream>
#include <string>

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

namespace EE698K {
namespace tools  {
    
/*****************************************************************************/

/*
 * Written by using
 * https://github.com/sat-p/SLIC-Superpixels/blob/master/test_slic.cpp as a
 * reference
 */

template<typename container>
cv::Mat slic (const cv::Mat& img, const container& connected)
{
    constexpr int NR_SUPERPIXELS = 40;
    constexpr int NC = 100;
    
    if (!connected.size()) {
    
        std::cerr << "No point on input image has been selected" << std::endl;
        return img;
    }
    
    cv::Mat lab_img;
    cv::cvtColor (img, lab_img, CV_BGR2Lab);
    
    const int w = img.cols;
    const int h = img.rows; 
    
    const double step = std::sqrt (w * h) / NR_SUPERPIXELS;
    
    EE698K::algos::SlicGC slic;
    slic.generate_superpixels (lab_img, step, NC);
    slic.generate_contours();
    slic.generate_cluster_means (lab_img);
    
    slic.create_connections (connected);
    
    slic.create_graph();
        
    const cv::Mat& mask = slic.fetch_mask (connected);
    
    cv::Mat res;
//     cv::blur (img, res, cv::Size (100, 100));
    
    constexpr int SIZE = 21;
    
    cv::Mat kernal = cv::Mat::zeros (cv::Size (SIZE, SIZE), CV_32FC1);
    
    float val = 2.0 / (SIZE + 1);
    
    for (int i = SIZE / 2; i < SIZE; ++i)
        kernal.at<float> (i, SIZE / 2) = val;
    
//     cv::imshow ("img", img);
//     cv::imshow ("kernal", kernal);
    cv::Point anchor = cv::Point (-1, -1);
    cv::filter2D (img, res, -1, kernal, anchor);
    
    cv::Mat dilatedMask;
    cv::dilate (mask, dilatedMask, cv::Mat(), cv::Point (-1, -1), SIZE / 2);
    
//     cv::namedWindow ("mask", mask);
//     cv::namedWindow ("res", res);
//     cv::imshow ("mask", mask);
//     cv::imshow ("res", res);
    img.copyTo (res, ~mask);
//     slic.display_contours (res, cv::Vec3b (0,0,255));
    
    cv::waitKey (0);
    
    return res;
}

/*****************************************************************************/
    
}} // namespaces EE698K, tools

#endif