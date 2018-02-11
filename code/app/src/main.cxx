#include <iostream>
#include <cstdlib>
#include <string>
#include <sstream>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "../../tools/include/sparse_inpaint.h"

int main (int argc, char* argv[])
{
    if (!(argc == 4 || argc == 3)) {
    
        std::cout << "Please input epsilon followed by dictionary size."
                  << " Also, append some extra data for irls"
                  << std::endl;
        return 0;
    }
    
    const auto spects_image = cv::imread ("./photos/image1.jpg",
                                          cv::IMREAD_GRAYSCALE);    
    const auto mask = cv::imread ("./photos/patch.bmp", cv::IMREAD_GRAYSCALE);
    
    auto with_mask = spects_image.clone();
    
    cv::namedWindow("Original", cv::WINDOW_NORMAL);
    cv::imshow ("Original", with_mask);
    
    mask.copyTo (with_mask, mask);
    
    auto result = EE698K::tools::sparse_inpaint (spects_image, mask,
                                                 std::atoi (argv[2]),
                                                 std::stod (argv[1]),
                                                 argc == 4);    
    
    std::stringstream output;
    output << "./data/" << argv[1] << "_"  << argv[2];
    
    if (argc == 3)
        cv::imwrite (output.str() + "_omp.jpg", result);
    else
        cv::imwrite (output.str() + "_irls.jpg", result);

    return 0;
}