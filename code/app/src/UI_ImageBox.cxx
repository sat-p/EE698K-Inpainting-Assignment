#include "../include/UI_ImageBox.h"

#include "../../tools/include/convert.h"

#include <opencv2/imgproc/imgproc.hpp>

/*****************************************************************************/

typedef EE698K::GUI::UI_ImageBox UI_ImageBox;

/*****************************************************************************/

void UI_ImageBox::image (const cv::Mat& mat)
{
    _orig = mat;
    scale();
}

void UI_ImageBox::image (cv::Mat&& mat)
{
    _orig = mat;
    scale();
}

/*****************************************************************************/
/*****************************************************************************/

void UI_ImageBox::scale (void)
{
    if (_orig.empty())
        return;
    
    const float box_x = wBox->w();
    const float box_y = wBox->h();
    
    const float x_scale = box_x / _orig.cols;
    const float y_scale = box_y / _orig.rows;
    
    int x_size, y_size;
    if (x_scale > y_scale) {
    
        y_size = box_y;
        x_size = _orig.cols * y_scale;
    }
    else {
    
        x_size = box_x;
        y_size = _orig.rows * x_scale;
    }
    
    cv::resize (_orig, _resize,
                cv::Size (x_size, y_size));
    
    offset_x = box_x / 2 - (x_size / 2);
    offset_y = box_y / 2 - (y_size / 2);
    
    _shown = cv::Mat (cv::Size (box_x, box_y), _resize.type());
    _shown = cv::Scalar::all (255);
    _resize.copyTo (_shown (cv::Rect (offset_x,
                                      offset_y,
                                      x_size, y_size)));
    
    _image.reset (EE698K::tools::Mat_to_Fl_Image (_shown));
    
    wBox->image (_image.get());
    wBox->redraw();
}

/*****************************************************************************/
/*****************************************************************************/