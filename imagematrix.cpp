#include "imagematrix.h"

ImageMatrix::ImageMatrix(int w, int h)
{
	this->width = w;
	this->height = h;
	int sizeInt = w * h;
	this->data = new double[sizeInt];
	for (int i = 0; i < sizeInt; i++)
		data[i] = 0.0;
}

ImageMatrix::ImageMatrix(int w, int h, double source[]) : ImageMatrix(w, h)
{
	memcpy(this->data, source, w * h * sizeof (double));
}

int ImageMatrix::getWidth() const
{
	return width;
}

int ImageMatrix::getHeight() const
{
	return height;
}

double ImageMatrix::get(int x, int y) const
{
	double result = 0.0;
	if ((x >= 0 && x < width) && (y >= 0 && y < height))
		result = this->data[y * width + x];
	return result;
}

void ImageMatrix::set(int x, int y, double source)
{
	if ((x >= 0 && x < width) && (y >= 0 && y < height))
		this->data[y * width + x] = source;
}

ImageMatrix::~ImageMatrix()
{
	delete[] data;
}
