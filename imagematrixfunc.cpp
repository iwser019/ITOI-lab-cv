#include "imagematrixfunc.h"

ImageMatrix matrixAdd(const ImageMatrix &matrix1, const ImageMatrix &matrix2)
{
	int width = qMin(matrix1.getWidth(), matrix2.getWidth());
	int height = qMin(matrix1.getHeight(), matrix2.getHeight());
	ImageMatrix result(width, height);
	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++)
			result.set(j, i, matrix1.get(j, i) + matrix2.get(j, i));
	return result;
}

ImageMatrix matrixSubtract(ImageMatrix &matrix1, ImageMatrix &matrix2)
{
	int width = qMin(matrix1.getWidth(), matrix2.getWidth());
	int height = qMin(matrix1.getHeight(), matrix2.getHeight());
	ImageMatrix result(width, height);
	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++)
			result.set(j, i, matrix1.get(j, i) - matrix2.get(j, i));
	return result;
}

ImageMatrix matrixConvolute(const ImageMatrix &matrix,
							const ImageMatrix &kernel,
							IMatrixEdgeResolver *resolver)
{
	int width = matrix.getWidth();
	int height = matrix.getHeight();
	int widthKernel = kernel.getWidth();
	int heightKernel = kernel.getHeight();
	IMatrixEdgeResolver *currentResolver = nullptr;
	if (resolver == nullptr)
		currentResolver = new MatrixEdgeResolverNull();
	else
		currentResolver = resolver;
	ImageMatrix result(width, height);
	int kernelOffsetX = (widthKernel - 1) / 2;
	int kernelOffsetY = (heightKernel - 1) / 2;
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			double value = 0.0;
			for (int u = -kernelOffsetY; u <= kernelOffsetY; u++)
				for (int v = -kernelOffsetX; v <= kernelOffsetX; v++)
				{
					double valueKernel = kernel.get(v + kernelOffsetX, u + kernelOffsetY);
					double valueResolved = currentResolver->resolve(matrix, j - v, i - u);
					value += (valueKernel * valueResolved);
				}
			result.set(j, i, value);
		}
	}
	return result;
}

ImageMatrix matrixNormalize(const ImageMatrix &matrix)
{
	int width = matrix.getWidth();
	int height = matrix.getHeight();
	double maxVal = DBL_MIN, minVal = DBL_MAX;
	double value = 0.0;
	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++)
		{
			value = matrix.get(j, i);
			maxVal = qMax(maxVal, value);
			minVal = qMin(minVal, value);
		}
	double distance = maxVal - minVal;
	ImageMatrix result(width, height);
	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++)
		{
			value = (matrix.get(j, i) - minVal) / distance;
			result.set(j, i, value);
		}
	return result;
}

ImageMatrix kernelGeneratePartialDeriv(bool orientation, int kernelType)
{
	const int diameter = 3;
	const int size = 9;
	double *kernelData;
	switch (kernelType)
	{
		// Собель
		case 0:
		{
			if (!orientation)
			{
				kernelData = new double[size]
				{
						1.0, 0.0, -1.0,
						2.0, 0.0, -2.0,
						1.0, 0.0, -1.0
				};
			}
			else
			{
				kernelData = new double[size]
				{
						1.0, 2.0, 1.0,
						0.0, 0.0, 0.0,
						-1.0, -2.0, -1.0
				};
			}
			break;
		}
		// Щарр
		case 1:
		{
			if (!orientation)
			{
				kernelData = new double[size]
				{
						3.0, 0.0, -3.0,
						10.0, 0.0, -10.0,
						3.0, 0.0, -3.0
				};
			}
			else
			{
				kernelData = new double[size]
				{
						3.0, 10.0, 3.0,
						0.0, 0.0, 0.0,
						-3.0, -10.0, -3.0
				};
			}
			break;
		}
		default:
		{
			kernelData = new double[size]
			{
					0.0, 0.0, 0.0,
					0.0, 1.0, 0.0,
					0.0, 0.0, 0.0
			};
		}
	}
	ImageMatrix result(diameter, diameter, kernelData);
	delete [] kernelData;
	return result;
}

ImageMatrix kernelGenerateShift1px(int hShift, int vShift)
{
	const int diameter = 3;
	const int size = 9;
	double *kernelData = new double[size]{
		0.0, 0.0, 0.0,
		0.0, 0.0, 0.0,
		0.0, 0.0, 0.0
	};
	// только по вертикали
	if (!hShift)
	{
		// вверх
		if (vShift == 1)
			kernelData[1] = 1.0;
		// вниз
		else if (vShift == 2)
			kernelData[7] = 1.0;
		// иначе - ничего
		else
			kernelData[4] = 1.0;
	}
	// только по горизонтали
	else if (!vShift)
	{
		// влево
		if (hShift == 1)
			kernelData[3] = 1.0;
		// вправо
		else if (hShift == 2)
			kernelData[5] = 1.0;
		// иначе - ничего
		else
			kernelData[4] = 1.0;
	}
	// по диагонали
	else if (hShift && vShift)
	{
		if (hShift == 1 && vShift == 1)
			kernelData[0] = 1.0;
		else if (hShift == 1 && vShift == 2)
			kernelData[6] = 1.0;
		else if (hShift == 2 && vShift == 1)
			kernelData[2] = 1.0;
		else if (hShift == 2 && vShift == 2)
			kernelData[8] = 1.0;
		// в случае некорректных параметров
		else
			kernelData[4] = 1.0;
	}
	else
		kernelData[4] = 1.0;
	// собираем ядро
	ImageMatrix result(diameter, diameter, kernelData);
	delete [] kernelData;
	return result;
}

ImageMatrix kernelGenerateGaussian(double sigma)
{
	// для соблюдения правила трёх сигм
	// размер ядра зависит от параметра "сигма"
	int radius = static_cast<int>(ceil(sigma * 3.0));
	int diameter = radius * 2 + 1;
	int arraySize = diameter * diameter;
	double value = 0.0;
	double *kernelData = new double[arraySize];
	// среднеквадратическое отклонение
	double mean = 2.0 * sigma * sigma;
	double mainCoeff = (1.0 / (2 * M_PI * sigma * sigma));
	for (int y = -radius; y <= radius; y++)
	{
		double yVal = static_cast<double>(y);
		for (int x = -radius; x <= radius; x++)
		{
			double xVal = static_cast<double>(x);
			value = mainCoeff * exp((0.0 - (xVal * xVal + yVal * yVal)) / mean);
			kernelData[(y + radius) * diameter + (x + radius)] = value;
		}
	}
	// собираем ядро
	ImageMatrix result(diameter, diameter, kernelData);
	delete [] kernelData;
	return result;
}

ImageMatrix kernelGenerateUniformBlur(int radius)
{
	int diameter = 2 * radius + 1;
	int arraySize = diameter * diameter;
	double value = 1.0 / static_cast<double>(arraySize);
	double *kernelData = new double[arraySize];
	for (int i = 0; i < arraySize; i++)
		kernelData[i] = value;
	ImageMatrix result(diameter, diameter, kernelData);
	delete [] kernelData;
	return result;
}

ImageMatrix matrixAddScalar(ImageMatrix &matrix, double value)
{
	int width = matrix.getWidth();
	int height = matrix.getHeight();
	ImageMatrix result(width, height);
	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++)
			result.set(j, i, matrix.get(j, i) + value);
	return result;
}

ImageMatrix matrixFilterSobel(const ImageMatrix &matrix,
							  int kernelType,
							  IMatrixEdgeResolver *resolver)
{
	int width = matrix.getWidth();
	int height = matrix.getHeight();
	ImageMatrix kernelX = kernelGeneratePartialDeriv(false, kernelType);
	ImageMatrix kernelY = kernelGeneratePartialDeriv(true, kernelType);
	ImageMatrix matrixX = matrixConvolute(matrix, kernelX, resolver);
	ImageMatrix matrixY = matrixConvolute(matrix, kernelY, resolver);
	//ImageMatrix matrixXNorm = matrixNormalize(matrixX);
	//ImageMatrix matrixYNorm = matrixNormalize(matrixY);
	ImageMatrix result(width, height);
	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++)
		{
			double valueX = matrixX.get(j, i)*matrixX.get(j, i);
			double valueY = matrixY.get(j, i)*matrixY.get(j, i);
			double square = sqrt(valueX + valueY);
			result.set(j, i, square);
		}
	ImageMatrix resultNorm = matrixNormalize(result);
	return resultNorm;
}

ImageMatrix kernelGenerateGaussianSeparate(double sigma, bool orientation)
{
	// для соблюдения правила трёх сигм
	// размер ядра зависит от параметра "сигма"
	int radius = static_cast<int>(ceil(sigma * 3.0));
	int diameter = radius * 2 + 1;
	int arraySize = diameter;
	double value = 0.0;
	double *kernelData = new double[arraySize];
	// среднеквадратическое отклонение
	double mean = 2.0 * sigma * sigma;
	double mainCoeff = (1.0 / (sqrt(2 * M_PI) * sigma));
	for (int x = -radius; x <= radius; x++)
	{
		double xVal = static_cast<double>(x);
		value = mainCoeff * exp((0.0 - (xVal * xVal)) / mean);
		kernelData[(x + radius)] = value;
	}
	// собираем ядро
	ImageMatrix result(
				(!orientation ? diameter : 1),
				(orientation ? diameter : 1),
				kernelData);
	delete [] kernelData;
	return result;
}

ImageMatrix matrixDownsample(const ImageMatrix &matrix)
{
	int width = matrix.getWidth();
	int height = matrix.getHeight();
	int widthResult = (((width % 2) == 0) ? (width / 2) : ((width - 1) / 2 + 1));
	int heightResult = (((height % 2) == 0) ? (height / 2) : ((height - 1) / 2 + 1));
	ImageMatrix result(widthResult, heightResult);
	for (int i = 0; i < heightResult; i++)
		for (int j = 0; j < widthResult; j++)
			result.set(j, i, matrix.get(j * 2, i * 2));
	return result;
}

int matrixMaxAvailableOctaves(const ImageMatrix &matrix)
{
	return static_cast<int>(
				log2(
					qMin(
						static_cast<double>(matrix.getHeight()),
						static_cast<double>(matrix.getWidth())
						)
					)
				);
}
