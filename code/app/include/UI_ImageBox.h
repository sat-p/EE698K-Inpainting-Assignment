#ifndef __EE698K_GUI_UI_IMAGE_BOX_H__
#define __EE698K_GUI_UI_IMAGE_BOX_H__

#include "WC_ImageBox.h"

#include <memory>

#include <opencv2/core/core.hpp>

#include <Fl/Fl_Image.H>

/*****************************************************************************/

namespace EE698K {
namespace GUI {

class UI_ImageBox : public WC_ImageBox
{
public:
    UI_ImageBox(int X, int Y, int W, int H, const char *L = 0) :
        WC_ImageBox (X, Y, W, H, L)
    {}

public:
    virtual void image (const cv::Mat& mat);
    virtual void image (cv::Mat&& mat);    
    
    const cv::Mat& image (void)
    { return _resize; }
    
private:
    void scale (void);

public:
    int offset_x;
    int offset_y;
    
protected:
    std::unique_ptr<Fl_RGB_Image> _image;
    cv::Mat _shown;
    
private:
    cv::Mat _orig;
    cv::Mat _resize;
};

}} // namespaces EE698K, GUI

/*****************************************************************************/
/*****************************************************************************/

#endif
