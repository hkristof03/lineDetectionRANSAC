#include "points.h"

std::vector<cv::Point> GetPoints
(
	const std::string &str,
	const std::size_t & kernel_size,
	const std::size_t & min_val,
	const std::size_t & max_val
)
{
	std::cout << "Trying to read image from source: " << str << std::endl;
	cv::Mat img, gray, edge;
	std::vector<cv::Point> points;

	img = cv::imread(str, cv::IMREAD_COLOR);
	cv::cvtColor(img, gray, CV_RGB2GRAY);

	cv::blur(gray, gray, cv::Size(kernel_size, kernel_size));
	cv::Canny(gray, edge, min_val, max_val);
	cv::findNonZero(edge, points);

	return points;
}