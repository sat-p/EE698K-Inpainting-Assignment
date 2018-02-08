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
    constexpr int RADIUS = 4;
    
    SparseInpaint sparse_inpaint (img, "/home/satya/patch/", 256, RADIUS);
    
    cv::Mat mask_clone = mask.clone();
    sparse_inpaint.mask (mask_clone);
    
    const auto& res = sparse_inpaint.generate();
    
    return res;
}

/*****************************************************************************/
/*****************************************************************************/