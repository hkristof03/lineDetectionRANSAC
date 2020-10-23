#include <iostream>
#include <filesystem>
#include "points.h"
#include "ransac.h"

namespace fs = std::filesystem;


int main(int argc, const char** argv)
{   
    std::string path = argv[1];
    
    std::vector<std::string> file_paths;
    file_paths.reserve(4);
    
    const int kernel_size = 3; 
    const double min_val = 50.0; 
    const double max_val = 150.0;
    

    for (const auto& entry : fs::directory_iterator(path))
        file_paths.push_back(entry.path().string());

    
    cv::Mat edges = GetEdges(file_paths.at(0), kernel_size,
        min_val, max_val);

    std::vector<cv::Point> points = GetPoints(edges);
    
    //std::cout << points << std::endl;
    
    std::vector<int> inliers;
    std::vector<double> best_line;

    const double threshold = 20.0;
    const int n_iterations = 1000;
    
    cv::Mat rgb_edges;
    cv::cvtColor(edges, rgb_edges, cv::COLOR_GRAY2RGB);

    FitLineRANSAC(points, inliers, best_line, threshold,
        n_iterations, &rgb_edges);
}
