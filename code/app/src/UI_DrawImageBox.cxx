#include "../include/UI_DrawImageBox.h"
#include "../../tools/include/convert.h"

#include <opencv2/imgproc/imgproc.hpp>

#include <fstream>
#include <iostream>

/*****************************************************************************/

typedef EE698K::GUI::UI_DrawImageBox UI_DrawImageBox;

/*****************************************************************************/

/*
 * Developed with help from http://seriss.com/people/erco/fltk/
 */
int UI_DrawImageBox::handle (int event)
{
    int ret = UI_ImageBox::handle (event);
    switch (event) {
        case FL_PUSH: 
        case FL_DRAG: if (Fl::event_button() ==  1) {
            
            int x = Fl::event_x() - wBox->x();
            int y = Fl::event_y() - wBox->y();
            
            if (x >= 0 && x <= wBox->w() &&
                y >= 0 && y <= wBox->h()) {
                
                _coordinates.emplace (x, y);
                
                cv::circle (_shown,
                            cv::Point (x, y),
                            circle_radius,
                            cv::Vec3b (0, 0, 0),
                            -1);

                _image.reset (EE698K::tools::Mat_to_Fl_Image (_shown));
    
                wBox->image (_image.get());
                wBox->redraw();
            }
            
            return 1;
        }
    }
    
    return ret;
}

/*****************************************************************************/

void UI_DrawImageBox::save_points (const std::string& s)
{
    std::ofstream output;
    output.open (s);
    for (const auto& p : _coordinates)
        output << p.first << '\t' << p.second << '\n';
    output.close();
}

/*****************************************************************************/

void UI_DrawImageBox::load_points (const std::string& s)
{
    int x, y;
    
    std::ifstream input (s);
    if (input.is_open()) {
    
        while (input >> x >> y) {
            if (x >= 0 && x <= wBox->w() &&
                y >= 0 && y <= wBox->h()) {
                _coordinates.emplace (x, y);
                
                cv::circle (_shown,
                            cv::Point (x, y),
                            circle_radius,
                            cv::Vec3b (0, 0, 0),
                            -1);
            }
        }
        
        _image.reset (EE698K::tools::Mat_to_Fl_Image (_shown));
        
        wBox->image (_image.get());
        wBox->redraw();
    }
    else 
        std::cerr << "Unable to open file" << std::endl;
}

/*****************************************************************************/
/*****************************************************************************/