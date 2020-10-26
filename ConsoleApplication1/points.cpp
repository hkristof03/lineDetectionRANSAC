#include "points.h"


cv::Mat GetEdges(
	std::string& str,
	const int& kernel_size,
	const double& min_val,
	const double& max_val
)
{
	std::cout << "Reading image from source: " << str << std::endl;
	cv::Mat img, gray, edges;
	std::vector<cv::Point> points;

	img = cv::imread(str, cv::IMREAD_COLOR);

	if (img.empty())
	{
		std::cout << "Could not read image from source!" << std::endl;
		return cv::Mat();
	}
	cv::cvtColor(img, gray, CV_RGB2GRAY);

	cv::blur(gray, gray, cv::Size(kernel_size, kernel_size));
	cv::Canny(gray, edges, min_val, max_val);
	std::cout << "Finished reading!" << std::endl;

	return edges;
}

std::vector<cv::Point> GetPoints(cv::Mat &edges)
{
	std::vector<cv::Point> points;

	cv::findNonZero(edges, points);

	return points;
}