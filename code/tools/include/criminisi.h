#ifndef __EE604A_TOOLS_CRIMINISI_H__
#define __EE604A_TOOLS_CRIMINISI_H__

#include "../../algos/include/criminisi.h"

#include <opencv2/core/core.hpp>

namespace EE604A {
namespace tools {

cv::Mat criminisi (const cv::Mat& img, const cv::Mat& mask);
    
}} // namespaces EE604A, tools

#endif