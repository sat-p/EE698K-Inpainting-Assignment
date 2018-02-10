#include <iostream>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "../../tools/include/sparse_inpaint.h"

int main (int argc, char* argv[])
{
    if (argc != 3) {
    
        std::cout << "Please input paths to image with spectacles and"
                  << " to the mask" << std::endl;
        return 0;
    }
    
    const auto spects_image = cv::imread (argv[1], cv::IMREAD_GRAYSCALE);    
    const auto mask = cv::imread (argv[2], cv::IMREAD_GRAYSCALE);
    
    auto with_mask = spects_image.clone();
    
    const auto patch = cv::imread ("/home/satya/workspace/Acads/EE698K/patch/image1.jpg");
    cv::imshow ("patch", patch);
    
    cv::namedWindow("Original", cv::WINDOW_NORMAL);
    cv::imshow ("Original", with_mask);
    
    mask.copyTo (with_mask, mask);
    
    auto result = EE698K::tools::sparse_inpaint (spects_image, mask, 1e-5);    
    
    cv::namedWindow("Inpainted", cv::WINDOW_NORMAL);
    cv::imshow ("Inpainted", result);
    
    cv::waitKey (0);
    return 0;
}