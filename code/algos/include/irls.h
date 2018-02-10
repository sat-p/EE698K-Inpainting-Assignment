#pragma once

#include <opencv2/core/core.hpp>

cv::Mat_<double> irls
(const cv::Mat_<double>& D, const cv::Mat_<double>& X, const double epsilon);