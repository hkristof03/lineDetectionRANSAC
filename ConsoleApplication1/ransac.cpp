#include "ransac.h"


extern cv::Scalar WHITE, GREEN, RED;
extern double RADIUS, THICKNESS;
extern int MIN_INLIERS;
extern std::string WINDOW_NAME;


void DrawPoint(
	cv::Mat& img,
	const cv::Point& p,
	const int& radius,
	const cv::Scalar& color
)
{
	cv::circle(img, p, radius, color, -1);
}

void DrawLine(
	cv::Mat& img,
	const double& a,
	const double& b,
	const double& c,
	const cv::Scalar& color,
	const int& thickness
)
{
	cv::line(
		img,
		cv::Point2d(0, -c / b),
		cv::Point2d(img.cols, (-a * img.cols - c) / b),
		color,
		thickness
	);
}

void FitLineRANSAC
(
	const std::vector<cv::Point>& points,
	std::vector<int>& inliers,
	std::vector<double> &line,
	const double &threshold,
	const int &max_iteration_num,
	cv::Mat* img
) 
{
	srand(time(NULL));
	// The current number of iterations
	int iteration_number = 0;
	// The numer of inliers of the current best model
	int best_inlier_number = 0;
	// The indices of the inliers of the current best model
	std::vector<int> best_inliers, current_inliers;
	best_inliers.reserve(points.size());
	current_inliers.reserve(points.size());
	// The parameters of the best line
	//cv::Mat best_line(3, 1, CV_64F);
	std::vector<double> best_line{ 0.0, 0.0, 0.0 };
	// The sample size i.e 2 for 2D lines
	constexpr int k_sample_size = 2;
	// The current sample
	std::vector<int> sample(k_sample_size);

	bool should_draw = img != nullptr;

	// RANSAC:
	// 1. Select a minimal sample in this case 2 random points
	// 2. Fit a line to the points
	// 3. Count the number of inliers: number of points closer than the threshold
	// 4. Store the inlier number and the line parameters if it is better than the previous best
	std::cout << "Number of iterations: " << max_iteration_num << std::endl;
	std::cout << "Threshold: " << threshold << std::endl;
	std::cout << "Number of inliers to draw the line: " << MIN_INLIERS << std::endl;

	while (iteration_number++ < max_iteration_num)
	{
		// 1. Select a minimal sample -> 2 random points
		for (std::size_t sample_idx = 0; sample_idx < k_sample_size; ++sample_idx)
		{
			do
			{
				// Generate a random index between [0, n_points]
				sample[sample_idx] =
					int((points.size() - 1.0) * static_cast<double>(rand()) / static_cast<double>(RAND_MAX));

				if (sample_idx == 0)
					break;

				if (sample_idx == 1 && sample[0] != sample[1])
					break;

			} while (true);
		}
		const cv::Point& p1 = points[sample[0]];
		const cv::Point& p2 = points[sample[1]];

		// 2. Fit a line to the points
		cv::Point2d v = static_cast<cv::Point2d>(p2) - static_cast<cv::Point2d>(p1); // Direction of the line
		v = v / cv::norm(v);

		cv::Point2d n;
		n.x = -v.y;
		n.y = v.x;
		// To get c use a point from the line
		double a = n.x;
		double b = n.y;
		double c = -(a * p1.x + b * p1.y);

		// Distance of a line and a point
		// Line's implicit equations: a * x + b * y + c = 0
		// a, b, c - parameters of the line. x,y coordinates of a point on the line
		// n = [a, b] - the normal of the line
		// Distance(line, point) = | a * x + b * y + c | / sqrt(a * a + b * b)
		// If ||n||_2 = 1 then sqrt(a * a + b * b) = 1 and I don't have to do the division

		// 3. Count the number of inliers -> the number of points closer than the threshold
		current_inliers.clear();
		for (std::size_t point_idx = 0; point_idx < points.size(); ++point_idx)
		{
			const cv::Point& point = points[point_idx];
			const double distance = abs(a * point.x + b * point.y + c);

			if (distance < threshold)
			{
				current_inliers.emplace_back(point_idx);
			}
		}
		// 4. Store the inlier number and the line parameters if it is better than the previous best
		if (current_inliers.size() > best_inliers.size())
		{
			best_inliers.swap(current_inliers);
			current_inliers.clear();
			current_inliers.resize(0);

			best_inlier_number = best_inliers.size();
			best_line.at(0) = a;
			best_line.at(1) = b;
			best_line.at(2) = c;
			
			std::cout << "RANSAC found a new line!" << std::endl
				<< "Current iteration: " << iteration_number << std::endl
				<< "Number of inliers: " << best_inlier_number << std::endl
				<< "a: " << a << std::endl << "b: " << b << std::endl 
				<< "c: " << c << std::endl;

			if (should_draw && best_inlier_number >= MIN_INLIERS)
			{
				for (const auto& idx : sample)
					DrawPoint(*img, points[idx], RADIUS + 2.0, RED);

				DrawLine(*img, a, b, c, GREEN, THICKNESS);
				cv::imshow(WINDOW_NAME, *img);
				cv::waitKey(100);
			}
		}
	}
	inliers = best_inliers;
	line = best_line;

	std::cout << "Max number of inliers found: " << inliers.size() << std::endl;
	std::cout << "Parameters of the best fitting line: " << std::endl << "a: " 
		<< line.at(0) << std::endl << "b: " << line.at(1) << std::endl << "c: " 
		<< line.at(2) << std::endl;
}

