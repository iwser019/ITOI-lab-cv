#ifndef UTIL_H
#define UTIL_H

#include "defs.h"

template <typename T> T clamp(T x, T a, T b)
{
	return ((x >= a) ? ((x <= b) ? x : b) : a);
}

template <typename T> T clampPeriodic(T x, T a, T b)
{
	T result = x;
	T distance = (b - a);
	while (result < a)
		result += distance;
	while (result > b)
		result -= distance;
	return result;
}

struct Point
{
		int x, y;
		double value;
		Point()
		{
			x = 0;
			y = 0;
			value = 0.0;
		}
		Point(int x, int y)
		{
			this->x = x;
			this->y = y;
			value = 0.0;
		}
		Point(int x, int y, double value)
		{
			this->x = x;
			this->y = y;
			this->value = value;
		}
};

/*!
 * \brief Преобразование к оттенкам серого (стандарт PAL/NTSC)
 * \param r Значение красного канала
 * \param g Значение зелёного канала
 * \param b Значение синего канала
 * \return Значение светимости
 */
double rgbToGrayscale(int r, int g, int b);
/*!
 * \brief Преобразование к оттенкам серого (стандарт sRGB)
 * \param r Значение красного канала
 * \param g Значение зелёного канала
 * \param b Значение синего канала
 * \return Значение светимости
 */
double rgbToGrayscaleS(int r, int g, int b);

double distance(const Point &point1, const Point &point2);

void vectorDblNormalize(QVector<double> *vec);

#endif // UTIL_H

