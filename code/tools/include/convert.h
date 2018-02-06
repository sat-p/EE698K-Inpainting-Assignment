#ifndef __EE698K_TOOLS_CONVERT_H__
#define __EE698K_TOOLS_CONVERT_H__

#include <Fl/Fl_Image.H>
#include <opencv2/core/core.hpp>

namespace EE698K {
namespace tools  {

/*****************************************************************************/
    
Fl_RGB_Image* Mat_to_Fl_Image (const cv::Mat& img);

/*****************************************************************************/
    
}} // namespaces EE698K, tools

#endif