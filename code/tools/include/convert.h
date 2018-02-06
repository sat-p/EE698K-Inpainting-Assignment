#ifndef __EE604A_TOOLS_CONVERT_H__
#define __EE604A_TOOLS_CONVERT_H__

#include <Fl/Fl_Image.H>
#include <opencv2/core/core.hpp>

namespace EE604A {
namespace tools  {

/*****************************************************************************/
    
Fl_RGB_Image* Mat_to_Fl_Image (const cv::Mat& img);

/*****************************************************************************/
    
}} // namespaces EE604A, tools

#endif