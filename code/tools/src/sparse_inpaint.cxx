#include "../include/sparse_inpaint.h"

#include <opencv2/imgproc/imgproc.hpp>

/*****************************************************************************/

/*
 * Written by using
 * https://github.com/sat-p/Exempler-Inpainting/blob/master/test.cxx as a
 * reference
 */

cv::Mat EE698K::tools::sparse_inpaint (const cv::Mat& img, const cv::Mat& mask,
                                       const int sparsity)
{
    constexpr int RADIUS = 4;
    
    SparseInpaint sparse_inpaint (img, "/home/satya/workspace/Acads/EE698K/patch/",
                                  256,
                                  sparsity,
                                  RADIUS);
    
    cv::Mat mask_clone = mask.clone();
    sparse_inpaint.mask (mask_clone);
    
    const auto& res = sparse_inpaint.generate();    
    return res;
}

/*****************************************************************************/
/*****************************************************************************/