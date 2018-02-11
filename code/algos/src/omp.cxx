#include "../include/omp.h"

#include <cmath>
#include <vector>

/*****************************************************************************/

cv::Mat_<double> omp (const cv::Mat_<double>& D, const cv::Mat_<double>& X,
                      const double epsilon = 1e-4)
{
    cv::Mat_<double> phi;
    const int num_atoms = D.cols;
    
    cv::Mat_<double> r = X.clone();
    cv::Mat_<double> a;
    std::vector<int> col_order;
    
    while (cv::norm (r) > epsilon) {
        
        double best_inner = 0;
        int    best_idx = 0;
        
        for (int i = 0; i < num_atoms; ++i) {
            
            const double inner = std::abs (r.dot (D.col (i)));
            
            if (inner > best_inner) {
                
                best_idx = i;
                best_inner = inner;
            }
        }
        
        col_order.push_back (best_idx);
        if (phi.empty())
            phi = D.col (best_idx);
        else
            cv::hconcat (phi, D.col (best_idx), phi);

        const auto phi_inv = phi.inv (cv::DECOMP_SVD);
        
        a = phi_inv * X;
        
        r = X - phi * a;
    }
    
    cv::Mat_<double> a_ = cv::Mat_<double>::zeros (D.cols, 1);
    
    if (!a.empty()) {
        
        int count = 0;
        
        cv::MatConstIterator_<double> it = a.begin();
        const cv::MatConstIterator_<double> it_end = a.end();
        
        while (it != it_end) {
            
            a_.at<double> (col_order[count++]) = *it;
            ++it;
        }
    }
    
    return a_;
}

/*****************************************************************************/
/*****************************************************************************/