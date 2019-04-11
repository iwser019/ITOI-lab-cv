#include "imagematrix.h"

ImageMatrix::ImageMatrix()
{
	this->width = 0;
	this->height = 0;
	this->data = nullptr;
}

ImageMatrix::ImageMatrix(int w, int h) : ImageMatrix()
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

ImageMatrix::ImageMatrix(const ImageMatrix &source)
	: ImageMatrix(source.getWidth(), source.getHeight())
{
	memcpy(this->data,
		   source.getSource(),
		   source.getWidth() * source.getHeight() * sizeof(double));
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

double ImageMatrix::get(const Point &point) const
{
	return get(point.x, point.y);
}

void ImageMatrix::set(int x, int y, const double &source)
{
	if ((x >= 0 && x < width) && (y >= 0 && y < height))
		this->data[y * width + x] = source;
}

void ImageMatrix::set(const Point &point, const double &source)
{
	set(point.x, point.y, source);
}

double *ImageMatrix::getSource() const
{
	return data;
}

ImageMatrix &ImageMatrix::operator=(const ImageMatrix &right)
{
	if (this == &right)
		return *this;
	double *dataOldPtr = this->data;
	this->data = right.getSource();
	this->height = right.getHeight();
	this->width = right.getWidth();
	delete[] dataOldPtr;
	return *this;
}

ImageMatrix::~ImageMatrix()
{
	delete[] data;
}
