#include "imageconverter.h"

ImageConverter::ImageConverter()
{

}

ImageMatrix *ImageConverter::qImageToMatrix(const QImage &image)
{
	int width = image.width();
	int height = image.height();
	int sizeInt = width * height;
	double *imageData = new double[sizeInt];
	QColor pixel;
	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++)
		{
			pixel = QColor(image.pixel(j, i));
			imageData[i * width + j] = rgbToGrayscaleS(
						pixel.red(),
						pixel.green(),
						pixel.blue()) / 255.0;
		}
	ImageMatrix *result = new ImageMatrix(width, height, imageData);
	delete[] imageData;
	return result;
}

QImage *ImageConverter::matrixToQImage(const ImageMatrix &image)
{
	int width = image.getWidth();
	int height = image.getHeight();
	QImage *result = new QImage(width, height, QImage::Format_RGBA8888);
	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++)
		{
			int value = clamp(
						static_cast<int>(image.get(j, i) * 255.0),
						0,
						255);
			result->setPixelColor(j, i, QColor(value, value, value));
		}
	return result;
}
