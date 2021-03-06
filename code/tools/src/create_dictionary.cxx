#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <vector>
#include <cstdlib>
#include <iostream>
#include <string>

/*****************************************************************************/

std::vector<cv::Mat> create_dictionary
(const std::vector<cv::Mat>& images, const int patch_size, const int N)
{
    const int num_images = images.size();
    const int patches_per_image = N / num_images;
    
    std::vector<cv::Mat> patches;
    
    for (const auto& img: images) {
        for (int it = 0; it < patches_per_image; ++it) {
        
            patches.emplace_back (patch_size, patch_size, img.type());
            
            const int r_idx = rand() % (img.rows - patch_size);
            const int c_idx = rand() % (img.cols - patch_size);
            
            const auto& chosen_patch = img (cv::Range (r_idx, r_idx + patch_size),
                                            cv::Range (c_idx, c_idx + patch_size));
            
            chosen_patch.copyTo (patches.back());
        }
    }
    
    int images_left = N - (patches_per_image * num_images);
    if (images_left) for (const auto& img : images) {
        
        patches.emplace_back (patch_size, patch_size, img.type());
            
        const int r_idx = rand() % (img.rows - patch_size);
        const int c_idx = rand() % (img.cols - patch_size);
            
        const auto& chosen_patch = img (cv::Range (r_idx, r_idx + patch_size),
                                        cv::Range (c_idx, c_idx + patch_size));
            
        chosen_patch.copyTo (patches.back());
        
        if (!(--images_left)) break;
    }
    
    return patches;
}

/*****************************************************************************/

int main (int argc, char** argv)
{
    if (argc != 3) {
    
        std::cout << "Please input number of images and path of images"
                  << std::endl;
        return 0;
    }
    
    constexpr int dict_len = 8;
    
    const int num_images = std::stoi (argv[1]);
    constexpr int dictionary_size = 256;
    const std::string path (argv[2]);
    
    std::vector<cv::Mat> images;
    
    for (int it = 1; it <= num_images; ++it) {
        
        const std::string image_path = path + "/image"
                                        + std::to_string (it) + ".jpg";
        
        images.emplace_back (cv::imread (image_path, cv::IMREAD_GRAYSCALE));
        
        const int pixels = images.back().rows * images.back().cols;
    
        if (!pixels) {
        
            std::cerr << "Unable to load one of the images. "
                      << "Please enter correct path"
                      << std::endl;
                  
            return -1;
        }
    }
    
    const auto& patches = create_dictionary (images, dict_len, dictionary_size);
    const std::string patch_path = path + "/../patch/";
    
    int count = 1;
    for (const auto& patch : patches) {
        
        const std::string write_name = patch_path + "image" +
                                       std::to_string (count) + ".png";
        
        std::cout << write_name << std::endl;
                                       
        cv::imwrite (write_name, patch);
        
        ++count;
    }
    
    cv::Mat dict128 (cv::Size (8 * 16, 8 * 8), patches.front().type());

    auto atom = patches.begin();
    for (int count = 0; count < 128; ++count) {
    
        const int i = count % 16;
        const int j = count / 16;
        
        atom->copyTo (dict128 (cv::Range (j * 8, (j + 1) * 8),
                               cv::Range (i * 8, (i + 1) * 8)));
        
        ++atom;
    }
    
    cv::imwrite (patch_path + "dict128.png", dict128);
    
    cv::Mat dict256 (cv::Size (8 * 16, 8 * 16), patches.front().type());

    atom = patches.begin();
    for (int count = 0; count < 256; ++count) {
    
        const int i = count % 16;
        const int j = count / 16;
        
        atom->copyTo (dict256 (cv::Range (j * 8, (j + 1) * 8),
                               cv::Range (i * 8, (i + 1) * 8)));
        
        ++atom;
    }
    
    cv::imwrite (patch_path + "dict256.png", dict256);
    
    return 0;
}

/*****************************************************************************/
/*****************************************************************************/