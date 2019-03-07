#ifndef IMAGEMATRIX_H
#define IMAGEMATRIX_H

#include "defs.h"

/*!
 * \brief Матрица изображения
 */
class ImageMatrix
{
	protected:
		// размер
		int width, height;
		// изображение (один канал)
		double *data;
	public:
		ImageMatrix(int w, int h);
		ImageMatrix(int w, int h, double source[]);
		int getWidth();
		int getHeight();
		double get(int x, int y);
		void set(int x, int y, double source);
		~ImageMatrix();
};

#endif // IMAGEMATRIX_H
