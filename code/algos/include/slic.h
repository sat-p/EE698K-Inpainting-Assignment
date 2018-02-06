#ifndef SLIC_H
#define SLIC_H

/* slic.h.
 *
 * Written by: Pascal Mettes.
 * Revisioned by: Satya Panuganti
 *
 * This file contains the class elements of the class Slic. This class is an
 * implementation of the SLIC Superpixel algorithm by Achanta et al. [PAMI'12,
 * vol. 34, num. 11, pp. 2274-2282].
 *
 * This implementation is created for the specific purpose of creating
 * over-segmentations in an OpenCV-based environment.
 */

#include <opencv2/core/core.hpp>
#include <cmath>
#include <vector>

/* 2d matrices are handled by 2d vectors. */
typedef std::vector<std::vector<double>> vec2dd;
typedef std::vector<std::vector<int>> vec2di;
typedef std::vector<std::vector<bool>> vec2db;
/* The number of iterations run by the clustering algorithm. */
constexpr int NR_ITERATIONS = 10;

/*
 * class Slic.
 *
 * In this class, an over-segmentation is created of an image, provided by the
 * step-size (distance between initial cluster locations) and the colour
 * distance parameter.
 */
class Slic {
    protected:
        /* The cluster assignments and distance values for each pixel. */
        vec2di clusters;
        vec2dd distances;
        
        /* The LAB and xy values of the centers. */
        vec2dd centers;
        /* The number of occurences of each center. */
        std::vector<int> center_counts;
        
        /* The points which lie along the contours */
        std::vector<cv::Point> contours;
        
        /* The mean colours of the clusters */
        std::vector<cv::Vec3d> colours;
        
        /* The step size per cluster, and the colour (nc) and distance (ns)
         * parameters. */
        int step, nc, ns;
        int rows, cols;
        
    private:
            
        /* Compute the distance between a center and an individual pixel. */
        double compute_dist(int ci, const cv::Point& pixel, const cv::Vec3b& colour);
        /* Find the pixel with the lowest gradient in a 3x3 surrounding. */
        cv::Point find_local_minimum(const cv::Mat& image, const cv::Point& center);
        
        /* Remove and initialize the 2d vectors. */
        void clear_data();
        void init_data(const cv::Mat& image);

    public:
        /* Class constructors and deconstructors. */
        Slic();
        ~Slic();
        
        /* Generate an over-segmentation for an image. */
        void generate_superpixels(const cv::Mat& image, int step, int nc);
        /* Enforce connectivity for an image. */
//         void create_connectivity(const cv::Mat& image);
        
        /* Other generation functions */
        void generate_contours (void);
        void generate_cluster_means (const cv::Mat& image);
        
        /* Draw functions. Resp. displayal of the centers and the contours. */
        void display_center_grid(cv::Mat& image, const cv::Vec3b& colour);
        void display_contours(cv::Mat& image, const cv::Vec3b& colour);
        void colour_with_cluster_means(cv::Mat& image);
};

#endif
