#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>


void DrawPoint(
	cv::Mat& img,
	const cv::Point& p1,
	const int& radius,
	const cv::Scalar& color
);

void DrawLine(
	cv::Mat& img,
	const int& a,
	const int& b,
	const int& c,
	const cv::Scalar& color,
	const int& thickness
);

void FitLineRANSAC
(
	const std::vector<cv::Point>& points,
	std::vector<int>& inliers,
	std::vector<double>& line,
	const double& threshold,
	const int& max_iteration_num,
	cv::Mat* img
);