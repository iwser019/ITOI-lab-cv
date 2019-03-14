#include "imatrixedgeresolver.h"

IMatrixEdgeResolver::IMatrixEdgeResolver()
{

}

double MatrixEdgeResolverNull::resolve(const ImageMatrix &matrix, int x, int y)
{
	double result = 0.0;
	int width = matrix.getWidth();
	int height = matrix.getHeight();
	if ((x >= 0 && y >= 0) && (x < width && y < height))
		result = matrix.get(x, y);
	return result;
}

double MatrixEdgeResolverNearest::resolve(const ImageMatrix &matrix, int x, int y)
{
	double result = 0.0;
	int width = matrix.getWidth();
	int height = matrix.getHeight();
	int xDest = clamp(x, 0, width - 1);
	int yDest = clamp(y, 0, height - 1);
	result = matrix.get(xDest, yDest);
	return result;
}

double MatrixEdgeResolverMirror::resolve(const ImageMatrix &matrix, int x, int y)
{
	double result = 0.0;
	int width = matrix.getWidth();
	int height = matrix.getHeight();
	int xDest = (x < 0 || x >= width) ? clamp(0 - x, 0, width - 1) : x;
	int yDest = (y < 0 || y >= height) ? clamp(0 - y, 0, height - 1) : y;
	result = matrix.get(xDest, yDest);
	return result;
}

double MatrixEdgeResolverRepeat::resolve(const ImageMatrix &matrix, int x, int y)
{
	double result = 0.0;
	int width = matrix.getWidth();
	int height = matrix.getHeight();

	return result;
}
