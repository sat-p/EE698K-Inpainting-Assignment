#include "../include/convert.h"

#include <opencv2/imgproc/imgproc.hpp>

/*****************************************************************************/

Fl_RGB_Image* EE698K::tools::Mat_to_Fl_Image (const cv::Mat& img)
{   
    cv::Mat reversed;
    if (reversed.channels() > 1) {
        cv::cvtColor (img, reversed, cv::COLOR_BGR2RGB);
    
        return new Fl_RGB_Image (reversed.data,
                                 reversed.cols,
                                 reversed.rows,
                                 reversed.channels());
    }
    else {
    
        return new Fl_RGB_Image (img.data,
                                 img.cols,
                                 img.rows,
                                 img.channels());
    }
}

/*****************************************************************************/
/*****************************************************************************/