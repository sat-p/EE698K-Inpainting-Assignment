#ifndef __EE698K_TOOLS_SPARSE_INPAINT_H__
#define __EE698K_TOOLS_SPARSE_INPAINT_H__

#include "../../algos/include/criminisi.h"

#include <opencv2/core/core.hpp>

namespace EE698K {
namespace tools {

cv::Mat sparse_inpaint (const cv::Mat& img, const cv::Mat& mask);
    
}} // namespaces EE698K, tools

#endif