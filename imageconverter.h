#ifndef IMAGECONVERTER_H
#define IMAGECONVERTER_H

#include "defs.h"
#include "imagematrix.h"

/*!
 * \brief Преобразователь изображений
 */
class ImageConverter
{
	public:
		ImageConverter();
		/*!
		 * \brief Преобразование изображения в объект матрицы
		 * \param image Объект изображения
		 * \return Матрица изображения (оттенки серого)
		 */
		static ImageMatrix * qImageToMatrix(const QImage &image);
		static QImage * matrixToQImage(ImageMatrix &image);
};

#endif // IMAGECONVERTER_H
