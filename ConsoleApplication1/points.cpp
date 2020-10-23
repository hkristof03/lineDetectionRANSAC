#include "points.h"


cv::Mat GetEdges(
	std::string& str,
	const int& kernel_size,
	const double& min_val,
	const double& max_val
)
{
	std::cout << "Trying to read image from source: " << str << std::endl;
	cv::Mat img, gray, edges;
	std::vector<cv::Point> points;

	img = cv::imread(str, cv::IMREAD_COLOR);
	cv::cvtColor(img, gray, CV_RGB2GRAY);

	cv::blur(gray, gray, cv::Size(kernel_size, kernel_size));
	cv::Canny(gray, edges, min_val, max_val);

	return edges;
}

std::vector<cv::Point> GetPoints(cv::Mat &edges)
{
	std::vector<cv::Point> points;

	cv::findNonZero(edges, points);

	return points;
}