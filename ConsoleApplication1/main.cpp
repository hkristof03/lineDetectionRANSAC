#include <iostream>
#include <filesystem>
#include "points.h"
#include "ransac.h"

namespace fs = std::filesystem;

cv::Scalar WHITE(255, 255, 255);
cv::Scalar GREEN(0, 255, 0);
cv::Scalar RED(0, 0, 255);
double RADIUS = 1.0, THICKNESS = 1.0;
int MIN_INLIERS = 20;

std::string WINDOW_NAME("RANSAC 2D");


int main(int argc, const char** argv)
{   
    std::string path = argv[1];
    const int n_iterations = atof(std::string(argv[2]).c_str());
    const double threshold = 10.0;
    std::string path_save = std::string(argv[3]).c_str();
    
    std::cout << path_save << std::endl;

    const int kernel_size = 3; 
    const double min_val = 50.0; 
    const double max_val = 150.0;
    
    for (const auto& entry : fs::directory_iterator(path))
    {
        std::string file_path = entry.path().string();

        cv::Mat edges = GetEdges(file_path, kernel_size,
            min_val, max_val);

        std::vector<cv::Point> points = GetPoints(edges);

        std::vector<int> inliers;
        std::vector<double> best_line;

        cv::Mat rgb_edges;
        cv::cvtColor(edges, rgb_edges, cv::COLOR_GRAY2RGB);

        FitLineRANSAC(points, inliers, best_line, threshold,
            n_iterations, &rgb_edges);

        std::string file_name = path_save + 
            "res_" + entry.path().filename().string();
        
        std::cout << file_name << std::endl;
        cv::imwrite(file_name, rgb_edges);
    }
}
