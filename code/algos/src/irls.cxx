#include "../include/irls.h"

#include <iostream>

/*****************************************************************************/

cv::Mat_<double> irls
(const cv::Mat_<double>& D, const cv::Mat_<double>& X, const double epsilon = 1e-4)
{
    cv::Mat a = cv::Mat_<double>::ones (D.cols, 1);
    
    while (cv::norm (X - D * a) > epsilon) {
    
        cv::Mat W_2 = cv::Mat::diag (a.mul (a));
        a = W_2 * D.t() * (D * W_2 * D.t()).inv() * X;
    }
    
    std::cout << "norm : " << cv::norm (X - D * a) << '\n';
    
    return a;
}

/*****************************************************************************/
/*****************************************************************************/