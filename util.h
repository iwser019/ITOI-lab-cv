#ifndef UTIL_H
#define UTIL_H

#include "defs.h"

template <typename T> T clamp(T x, T a, T b)
{
	return ((x >= a) ? ((x <= b) ? x : b) : a);
}

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
#endif // UTIL_H
