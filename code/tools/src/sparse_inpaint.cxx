#include "../include/sparse_inpaint.h"

#include <opencv2/imgproc/imgproc.hpp>

/*****************************************************************************/

/*
 * Written by using
 * https://github.com/sat-p/Exempler-Inpainting/blob/master/test.cxx as a
 * reference
 */

cv::Mat EE698K::tools::sparse_inpaint (const cv::Mat& img, const cv::Mat& mask)
{
    constexpr int RADIUS = 9;
    
    cv::Mat lab_img;
    cv::cvtColor (img, lab_img, CV_BGR2Lab);
    
    SparseInpaint sparse_inpaint (lab_img, RADIUS);
    
    cv::Mat mask_clone = mask.clone();
    sparse_inpaint.mask (mask_clone);
    const auto& res_lab = sparse_inpaint.generate();
    cv::Mat res;
    
    cv::cvtColor (res_lab, res, CV_Lab2BGR);
    return res;
}

/*****************************************************************************/
/*****************************************************************************/