#include "../include/sparse_inpaint.h"

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <vector>
#include <iostream>
#include <algorithm>
#include <numeric>
#include <cmath>

/*****************************************************************************/

SparseInpaint::SparseInpaint (const cv::Mat& image,
                              const std::string dictionary_path,
                              const int dictionary_size,
                              const double sparsity,
                              const int window_radius) :

    _original (image),
    _rows     (image.rows),
    _cols     (image.cols),
    _radius   (window_radius),
    _w        (DEFAULT_W),
    _delta    (DEFAULT_DELTA),
    _sparsity (sparsity)
{ construct_dictionary (dictionary_path, dictionary_size); }

/*****************************************************************************/

SparseInpaint::SparseInpaint (cv::Mat&& image,
                              const std::string dictionary_path,
                              const int dictionary_size,
                              const double sparsity,
                              const int window_radius) :
    _original (image),
    _rows     (image.rows),
    _cols     (image.cols),
    _radius   (window_radius),
    _w        (DEFAULT_W),
    _delta    (DEFAULT_DELTA),
    _sparsity (sparsity)
{ construct_dictionary (dictionary_path, dictionary_size); }

/*****************************************************************************/

cv::Mat remove_rows (const cv::Mat& mat, const cv::Mat_<bool>& remove_mask)
{
    cv::Mat res;
    
    const int n = mat.rows;
    for (int i = 0; i < n; ++i) {
        
        if (remove_mask.at<bool> (i)) continue;
        if (res.empty())
            res = mat.row (i);
        else
            cv::vconcat (res, mat.row (i), res);
    }
    
    return res;
}

cv::Mat SparseInpaint::generate (void)
{
    generate_contour();
    _modified = _original.clone().setTo (0,
                                         _mask);
    generate_priority();
    
    cv::Mat pMask;
    
    cv::namedWindow ("P");
    cv::namedWindow ("Q");
    cv::namedWindow ("M");
    
    cv::namedWindow ("modified", cv::WINDOW_NORMAL);
    
    while (_pq.size()) {
        
        const std::pair<int, int>& point = _pq.rbegin()->second;
        const cv::Point p (point.first, point.second);
        
        auto phi_p = patch(p, _modified);
        auto phi_p_ = phi_p.clone();
        
        phi_p_.convertTo (phi_p_, CV_64F);
        phi_p_ = phi_p_ / 255;
        
        phi_p_ = phi_p_.reshape (0, phi_p_.rows * phi_p_.cols);
        
        pMask = patch (p, _mask);
        
        auto pMask_ = pMask.clone();
        pMask_ = pMask_.reshape (0, pMask_.rows * pMask_.cols);
        
        const auto& D_ = remove_rows (_D, pMask_);
        phi_p_ = remove_rows (phi_p_, pMask_);
        
        const auto& a = _solver (D_, phi_p_, _sparsity);
        
        cv::Mat phi_q = _D * a;
        
        phi_q = phi_q * 255;
        phi_q.convertTo (phi_q, phi_p.type());
        
        phi_q = phi_q.reshape (0, phi_p.rows);
        
        cv::Mat P, Q, M;
        
        cv::resize (phi_p, P, cv::Size (90, 90));
        cv::resize (phi_q, Q, cv::Size (90, 90));
        cv::resize (pMask, M, cv::Size (90, 90));
        
        cv::imshow ("P", P);
        cv::imshow ("Q", Q);
        cv::imshow ("M", M);
        
        phi_q.copyTo (phi_p, pMask);
        
        cv::Mat confPatch = patch (p, _confidence);
        const double confidence = cv::sum (confPatch)[0] /
                                  confPatch.total();
        
        
        confPatch.setTo (confidence, pMask);
        
        pMask.setTo (0);
        
        update_contour (p);
        
        cv::imshow ("modified", _modified);
        
        cv::waitKey (1);
    }
    
    cv::destroyWindow ("P");
    cv::destroyWindow ("Q");
    cv::destroyWindow ("M");
    
    cv::destroyWindow ("modified");
    std::cout << "Completed" << std::endl;
    
    return _modified;
}

/*****************************************************************************/

void SparseInpaint::draw_contour (cv::Mat& image, const cv::Vec3b& colour)
{
    for (const auto& c : _contour)
        image.at<cv::Vec3b> (c.second, c.first) = colour;
}

void SparseInpaint::draw_contour (cv::Mat& image, const uchar colour)
{
    for (const auto& c : _contour)
        image.at<uchar> (c.second, c.first) = colour;
}

/*****************************************************************************/

void SparseInpaint::construct_dictionary (const std::string dictionary_path,
                                          const int dictionary_size)
{
    for (int it = 1; it <= dictionary_size; ++it) {
    
        const std::string patch_path = dictionary_path + "image"
                                        + std::to_string (it) + ".jpg";
        
        cv::Mat patch = cv::imread (patch_path, cv::IMREAD_GRAYSCALE);
        
        auto patch_ = patch.clone();
        
        patch_ = patch_.reshape (0, patch_.rows * patch_.cols);
        
        patch_.convertTo (patch, CV_64FC1);
        
        const double norm = cv::norm (patch);
        patch = patch / norm;
        
        if (_D.empty())
            _D = patch;
        else
            cv::hconcat (_D, patch, _D);
    }
}

/*****************************************************************************/

void SparseInpaint::generate_contour (void)
{
    constexpr int NUM_N = 8;
    
    const int dx8[NUM_N] = {-1, -1,  0,  1, 1, 1, 0, -1};
	const int dy8[NUM_N] = { 0, -1, -1, -1, 0, 1, 1,  1};
    
    _contour.clear();
    
    for (int i = 0; i < _cols; ++i) {
        for (int j = 0; j < _rows; ++j) {
            for (int k = 0; k < NUM_N; ++k) {
                
                if (!_mask.at<uchar> (j, i))
                    continue;
                
                const int x = i + dx8[k];
                const int y = j + dy8[k];
                
                if (x >= 0 && x < _cols && y >= 0 && y < _rows) {
                
                    if (!_mask.at<uchar> (y, x)) {
                    
                        _contour.emplace (i, j);
                        break;
                    }
                }
            }
        }
    }
}

/*****************************************************************************/

void SparseInpaint::generate_priority (void)
{
    _confidence = cv::Mat::zeros (_rows, _cols, CV_64FC1);
    
    for (int i = 0; i < _cols; ++i)
        for (int j = 0; j < _rows; ++j)
            _confidence.at<double> (j, i) = !_mask.at<uchar> (j, i);
    
    _pq.clear();
    _map.clear();
    
    for (const auto& c : _contour) {
        const double pri = priority (c);
        _pq.emplace (pri, c);
        _map[c] = pri;
    }

}

/*****************************************************************************/

cv::Point2d SparseInpaint::generate_normal (const cv::Point& p)
{
    std::vector<double> X;
    std::vector<double> Y;
    
    for (int i = p.x - _radius; i < p.x + _radius; ++i) {
        for (int j = p.y - _radius; j < p.y + _radius; ++j) {
            if (_contour.count (std::make_pair (i, j))) {
            
                X.push_back (i);
                Y.push_back (j);
            }
        }
    }
    
    if (X.size() == 1)
        return cv::Point2d (1.0, 0);
    
    cv::Mat X_ (cv::Size (2, X.size()), CV_64FC1);
    cv::Mat Y_ (cv::Size (1, X.size()), CV_64FC1);
    
    for (int i = 0; i < (int) X.size(); ++i) {
    
        auto* x = X_.ptr<double> (i);
        auto* y = Y_.ptr<double> (i);
        
        x[0] = X[i];
        x[1] = 1.0;
        
        y[0] = Y[i];
    }
    
    cv::Mat sol;
    float slope;
    
    try {
        cv::solve (X_, Y_, sol, cv::DECOMP_SVD);
    }
    catch (...) {
        slope = 0;
    }
    
    slope = sol.at<float> (0);
    cv::Point2d normal (-slope, 1);
    
    if (std::isnan (slope))
        normal = cv::Point2d (1, 0);
    
    return normal / cv::norm (normal);
}

/*****************************************************************************/

void SparseInpaint::update_contour (const cv::Point& p)
{
    constexpr int NUM_N = 8;

    const int dx8[NUM_N] = {-1, -1,  0,  1, 1, 1, 0, -1};
    const int dy8[NUM_N] = { 0, -1, -1, -1, 0, 1, 1,  1};
    
    const int x_begin = std::max (p.x - 2 *_radius, 0);
    const int y_begin = std::max (p.y - 2 *_radius, 0);
    const int x_end = std::min (p.x + 2 *_radius, _cols - 1);
    const int y_end = std::min (p.y + 2 *_radius, _rows - 1);
    
    for (int i = x_begin; i <= x_end; ++i) {
        for (int j = y_begin; j <= y_end; ++j) {
            
            const std::pair<int, int> p = std::make_pair (i, j);
            
            if (_contour.count (p)) {
                const double pri = _map[p];
                _contour.erase (p);
                _pq.erase (std::make_pair (pri, p));
                _map.erase (p);
            }
        }
    }
    
    std::set<std::pair<int, int>> new_contour;
    
    for (int i = x_begin; i <= x_end; ++i) {
        for (int j = y_begin; j <= y_end; ++j) {
            for (int k = 0; k < NUM_N; ++k) {
                
                if (!_mask.at<uchar> (j, i))
                    continue;
                
                const int x = i + dx8[k];
                const int y = j + dy8[k];
                
                if (x >= 0 && x < _cols && y >= 0 && y < _rows) {
                
                    if (!_mask.at<uchar> (y, x)) {
                    
                        new_contour.emplace (i, j);
                        break;
                    }
                }
            }
        }
    }
    
    for (const auto& nc : new_contour)
        _contour.emplace (nc);
    
    for (const auto& nc : new_contour) {
        
        const double pri = priority (nc);
        _pq.emplace (std::make_pair (pri, nc));
        _map[nc] = pri;
    }
}

/*****************************************************************************/ 

double SparseInpaint::priority (const std::pair<int, int>& p)
{
    const cv::Point& point = cv::Point (p.first, p.second);
    
    const cv::Mat& confPatch = patch (point, _confidence);
    
    const cv::Mat& pMask = patch (point, _mask);
    
    cv::Mat maskedConfidence = cv::Mat::zeros (confPatch.size(),
                                               CV_64FC1);
    confPatch.copyTo (maskedConfidence, pMask == 0);
    
    const double confidence = (cv::sum (maskedConfidence)[0] /
                               confPatch.total()) * (1 - _w) +
                               _w;
    
//     const double confidence = (cv::sum (maskedConfidence)[0] /
//                                confPatch.total());
    
    const cv::Point2f& normal = generate_normal (point);
    
    const cv::Mat& phi_p = patch (point, _modified);
    
    cv::Mat dx, dy, magnitude;
    cv::Sobel (phi_p, dx, CV_64F, 1, 0);
    cv::Sobel (phi_p, dy, CV_64F, 0, 1);
    
    cv::magnitude (dx, dy, magnitude);
    magnitude.setTo (0, pMask);
    
    cv::Mat erodedMagnitude;
    cv::erode (magnitude, erodedMagnitude, cv::Mat());
    
    cv::Point maxPoint;

    cv::minMaxLoc (erodedMagnitude, NULL, NULL, NULL, &maxPoint);
        
    const double dx_ = dx.at<double> (maxPoint);
    const double dy_ = dy.at<double> (maxPoint);
    
    const double dot = -dy_ * normal.x + dx_ * normal.y;
    
    return confidence * std::abs (dot);
}

/*****************************************************************************/ 

cv::Mat SparseInpaint::patch (const cv::Point& p, const cv::Mat& img)
{
    return img  (cv::Range (p.y - _radius, p.y + _radius),
                 cv::Range (p.x - _radius, p.x + _radius));
}

/*****************************************************************************/
/*****************************************************************************/