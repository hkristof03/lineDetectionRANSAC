#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <filesystem>
#include <vector>

namespace fs = std::filesystem;


int main(int argc, const char** argv)
{
    std::cout << argv[1] << std::endl;
    std::string path = argv[1];
    std::vector<std::string> file_paths;
    file_paths.reserve(4);
    

    for (const auto& entry : fs::directory_iterator(path))
        file_paths.push_back(entry.path().string());

    cv::Mat img = cv::imread(file_paths.at(0), cv::IMREAD_COLOR);
    cv::imshow("Display window", img);
    cv::waitKey(0);

    cv::Mat gray, edge;
    cv::cvtColor(img, gray, CV_RGB2GRAY);
    cv::imshow("Display window", gray);
    cv::waitKey(0);

    cv::blur(gray, gray, cv::Size(3, 3));
    cv::imshow("Display window", gray);
    cv::waitKey(0);

    cv::Canny(gray, edge, 50, 150, 3);
    cv::imshow("Display window", edge);
    cv::waitKey(0);

    cv::Mat locations; // output, locations of non-zero pixels
    cv::findNonZero(edge, locations);

    int rows = locations.rows;
    int cols = locations.cols;
    
    std::cout << rows << "," << cols << std::endl;
   
    std::cout << edge.rows << "," << edge.cols << std::endl;

    std::vector<cv::Point> points;
    cv::findNonZero(edge, points);

    std::cout << points.size() << std::endl;
    std::cout << points.at(0) << std::endl;



}
