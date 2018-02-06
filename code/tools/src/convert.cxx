#include "../include/convert.h"

#include <opencv2/imgproc/imgproc.hpp>

/*****************************************************************************/

Fl_RGB_Image* EE604A::tools::Mat_to_Fl_Image (const cv::Mat& img)
{   
    cv::Mat reversed;
    cv::cvtColor (img, reversed, cv::COLOR_BGR2RGB);
    
    return new Fl_RGB_Image (reversed.data,
                             reversed.cols,
                             reversed.rows,
                             reversed.channels());
}

/*****************************************************************************/
/*****************************************************************************/