#ifndef __EE698K_TOOLS_CRIMINISI_H__
#define __EE698K_TOOLS_CRIMINISI_H__

#include "../../algos/include/criminisi.h"

#include <opencv2/core/core.hpp>

namespace EE698K {
namespace tools {

cv::Mat criminisi (const cv::Mat& img, const cv::Mat& mask);
    
}} // namespaces EE698K, tools

#endif