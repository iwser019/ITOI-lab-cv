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
		ImageMatrix(const ImageMatrix &source);
		int getWidth() const;
		int getHeight() const;
		double get(int x, int y) const;
		double get(const Point &point) const;
		void set(int x, int y, const double &source);
		void set(const Point &point, const double &source);
		double * getSource() const;
		ImageMatrix& operator=(const ImageMatrix &right);
		~ImageMatrix();
};

#endif // IMAGEMATRIX_H
