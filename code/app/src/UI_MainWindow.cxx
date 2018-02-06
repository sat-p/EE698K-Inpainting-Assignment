#include "../include/UI_MainWindow.h"
#include "../../tools/include/slic.h"
#include "../../tools/include/sparse_inpaint.h"

#include <opencv2/highgui/highgui.hpp>

#include <FL/Fl_File_Chooser.H>

#include <vector>

/*****************************************************************************/

typedef EE698K::GUI::UI_MainWindow UI_MainWindow;

/*****************************************************************************/

void UI_MainWindow::WOpen_cb (void)
{
    const char* filename = fl_file_chooser ("Choose an Image file",
                                            "JPEG Files (*.jpg)\t"
                                            "BMP Files (*.bmp)\t"
                                            "PNG Files (*.png)",
                                            "", // default
                                            0); // absolute path
    
    wImageBox1->image (cv::imread (filename, cv::IMREAD_COLOR));
}

void UI_MainWindow::WQuit_cb (void)
{ exit(0); }

/*****************************************************************************/

void UI_MainWindow::WInpaint_cb (void)
{
    cv::Mat mask = cv::Mat::zeros (wImageBox1->image().size(), CV_8UC1);
    
    for (const auto& p : wImageBox1->_coordinates)
        cv::circle (mask,
                    cv::Point (p.first - wImageBox1->offset_x,
                               p.second - wImageBox1->offset_y),
                    wImageBox1->circle_radius,
                    255,
                    -1);
        
    auto result = EE698K::tools::sparse_inpaint (wImageBox1->image(), mask);
    wImageBox2->image (result);
}

/*****************************************************************************/

void UI_MainWindow::WSmall_cb (void)
{
    wImageBox1->circle_radius = wImageBox1->SMALL_RADIUS;
}

/*****************************************************************************/

void UI_MainWindow::WMedium_cb (void)
{
    wImageBox1->circle_radius = wImageBox1->MEDIUM_RADIUS;
}

/*****************************************************************************/

void UI_MainWindow::WSavePoints_cb (void)
{
    const char* filename = fl_file_chooser ("Choose data file",
                                            "", // default
                                            0); // absolute path
    
    wImageBox1->save_points (filename);
}

/*****************************************************************************/

void UI_MainWindow::WLoadPoints_cb (void)
{
    const char* filename = fl_file_chooser ("Choose data file",
                                            "", // default
                                            0); // absolute path
    
    wImageBox1->load_points (filename);
}

/*****************************************************************************/
/*****************************************************************************/