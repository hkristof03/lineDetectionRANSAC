#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>


cv::Mat GetEdges(
	std::string& str,
	const int& kernel_size,
	const double& min_val,
	const double& max_val
);

std::vector<cv::Point> GetPoints(cv::Mat& edges);