#include "util.h"

double rgbToGrayscaleS(int r, int g, int b)
{
	double result = static_cast<double>(r) * 0.213
			+ static_cast<double>(g) * 0.715
			+ static_cast<double>(b) * 0.072;
	return result;
}

double rgbToGrayscale(int r, int g, int b)
{
	double result = static_cast<double>(r) * 0.299
			+ static_cast<double>(g) * 0.587
			+ static_cast<double>(b) * 0.114;
	return result;
}

double distance(const Point &point1, const Point &point2)
{
	double result;
	result = sqrt(
				static_cast<int>(
					(point2.x - point1.x) * (point2.x - point1.x)
					+ (point2.y - point1.y) * (point2.y - point1.y)
					)
				);
	return result;
}
