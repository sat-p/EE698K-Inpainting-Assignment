#ifndef __EE698K_GUI_UI_DRAW_IMAGE_BOX_H__
#define __EE698K_GUI_UI_DRAW_IMAGE_BOX_H__

#include "UI_ImageBox.h"

#include <set>
#include <utility>
#include <string>

/*****************************************************************************/

namespace EE698K {
namespace GUI {

class UI_DrawImageBox : public UI_ImageBox
{
public:
    static constexpr int SMALL_RADIUS = 4;
    static constexpr int MEDIUM_RADIUS = 10;    
    
public:
    int circle_radius;
    
public:
    UI_DrawImageBox(int X, int Y, int W, int H, const char *L = 0) :
        UI_ImageBox     (X, Y, W, H, L),
        circle_radius   (SMALL_RADIUS)
    {}

public:
    using UI_ImageBox::image;
    
    void image (const cv::Mat& mat) override
    {
        _coordinates.clear();
        UI_ImageBox::image (mat);
    }
    void image (cv::Mat&& mat) override
    {
        _coordinates.clear();
        UI_ImageBox::image (std::move (mat));
    }
    
    void save_points (const std::string& s);
    void load_points (const std::string& s);
    
private:
    int handle (int event) override;
    
public:
    std::set<std::pair<int, int>> _coordinates;
};

}} // namespaces EE698K, GUI

/*****************************************************************************/
/*****************************************************************************/

#endif