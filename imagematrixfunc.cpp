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
		currentResolver = MatrixEdgeResolverFactory::getInstance()->getLastResolver();
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
					double valueKernel = kernel.get(
								v + kernelOffsetX,
								u + kernelOffsetY
								);
					double valueResolved = currentResolver->resolve(
								matrix,
								j - v,
								i - u
								);
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
	double mainCoeff = (1.0 / (M_PI * mean));
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

ImageMatrix kernelGenerateGaussian(int radius)
{
	// для соблюдения правила трёх сигм
	// сигма будет зависеть от размера ядра
	double sigma = static_cast<double>(radius) / 3.0;
	int diameter = radius * 2 + 1;
	int arraySize = diameter * diameter;
	double value = 0.0;
	double *kernelData = new double[arraySize];
	// среднеквадратическое отклонение
	double mean = 2.0 * sigma * sigma;
	double mainCoeff = (1.0 / (M_PI * mean));
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
							  IMatrixEdgeResolver *resolver,
							  ImageMatrix *angleMap)
{
	int width = matrix.getWidth();
	int height = matrix.getHeight();
	ImageMatrix kernelX = kernelGeneratePartialDeriv(false, kernelType);
	ImageMatrix kernelY = kernelGeneratePartialDeriv(true, kernelType);
	ImageMatrix matrixX = matrixConvolute(matrix, kernelX, resolver);
	ImageMatrix matrixY = matrixConvolute(matrix, kernelY, resolver);
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
	if (angleMap != nullptr)
	{
		for (int i = 0; i < height; i++)
			for (int j = 0; j < width; j++)
			{
				angleMap->set(j, i, atan2(matrixY.get(j, i), matrixX.get(j, i)));
			}
	}
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
	//int widthResult = (((width % 2) == 0) ? (width / 2) : ((width - 1) / 2 + 1));
	int widthResult = width / 2;
	//int heightResult = (((height % 2) == 0) ? (height / 2) : ((height - 1) / 2 + 1));
	int heightResult = height / 2;
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


QVector<Point> pointsMoravecPointOp(const ImageMatrix &matrix,
									const double &threshold,
									const int &winRadius,
									IMatrixEdgeResolver *resolver,
									ImageMatrix *responseMap)
{
	int width = matrix.getWidth();
	int height = matrix.getHeight();
	double *minima = new double[width * height];
	IMatrixEdgeResolver *currentResolver = resolver;
	if (resolver == nullptr)
		currentResolver = MatrixEdgeResolverFactory::getInstance()->getLastResolver();
	QVector<Point> result;
	// окрестность
	QVector<Point> neighborhood;
	// расчёт миинимумов
	// цикл по Y
	for (int i = 0; i < height; i++)
	{
		// цикл по X
		for (int j = 0; j < width; j++)
		{
			double minValue = DBL_MAX;
			neighborhood.clear();
			// берём соседние 8 точек
			for (int py = -1; py <= 1; py++)
				for (int px = -1; px <= 1; px++)
					if (!(py == 0 && px == 0))
						neighborhood.push_back(Point(j + px, i + py));
			// цикл по точкам окрестности
			for (Point neighbor : neighborhood)
			{
				// значение точки C(x, y, d)
				double value = 0.0;
				// цикл по окну (Y)
				for (int v = -winRadius; v <= winRadius; v++)
				{
					// цикл по окну (X)
					for (int u = -winRadius; u <= winRadius; u++)
					{
						Point curPoint(neighbor.x + u, neighbor.y + v);
						double valCenterWindow = currentResolver->resolve(matrix, j + u, i + v);
						double valNeighborWindow = currentResolver->resolve(matrix, curPoint.x, curPoint.y);
						double valWindow = (valNeighborWindow - valCenterWindow);
						value += valWindow * valWindow;
					}
				}
				minValue = qMin(minValue, value);
			}
			minima[i * width + j] = minValue;
			if (responseMap != nullptr)
				responseMap->set(j, i, minValue);
		}
	}
	// теперь выбираем нужные точки
	// они должны соответствовать требованиям локального максимума
	// и иметь значение выше порогового
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			double thisValue = minima[i * width + j];
			bool ok = true;
			neighborhood.clear();
			// берём соседние 8 точек
			for (int py = -winRadius; py <= winRadius; py++)
				for (int px = -winRadius; px <= winRadius; px++)
					if (!(py == 0 && px == 0))
						neighborhood.push_back(Point(j + px, i + py));
			for (Point neighbor : neighborhood)
			{
				if (neighbor.x < 0
						|| neighbor.y < 0
						|| neighbor.x >= width
						|| neighbor.y >= height)
					continue;
				// если не выполняется условие локального максимума
				if (thisValue < minima[neighbor.y * width + neighbor.x])
				{
					ok = false;
					break;
				}
			}
			if (ok && (thisValue > threshold))
				result.push_back(Point(j, i, minima[i * width + j]));
		}
	}
	delete[] minima;
	return result;
}

QVector<Point> pointsHarrisPointOp(const ImageMatrix &matrix,
								   const double &threshold,
								   const int &winRadius,
								   const int &kernelType,
								   IMatrixEdgeResolver *resolver,
								   ImageMatrix *responseMap)
{
	int width = matrix.getWidth();
	int height = matrix.getHeight();
	IMatrixEdgeResolver *currentResolver = resolver;
	if (resolver == nullptr)
		currentResolver = MatrixEdgeResolverFactory::getInstance()->getLastResolver();
	QVector<Point> result;
	QVector<Point> neighborhood;
	double *minima = new double[width * height];
	// ядра частных производных
	ImageMatrix kernelPartialDivX = kernelGeneratePartialDeriv(false, kernelType);
	ImageMatrix kernelPartialDivY = kernelGeneratePartialDeriv(true, kernelType);
	ImageMatrix kernelWeight = kernelGenerateGaussian(winRadius);
	// частные производные
	ImageMatrix matrixDivX = matrixConvolute(matrix, kernelPartialDivX, currentResolver);
	ImageMatrix matrixDivY = matrixConvolute(matrix, kernelPartialDivY, currentResolver);
	// цикл по Y
	for (int i = 0; i < height; i++)
	{
		// цикл по X
		for (int j = 0; j < width; j++)
		{
			double A = 0.0, B = 0.0, C = 0.0;
			for (int v = -winRadius; v <= winRadius; v++)
			{
				for (int u = -winRadius; u <= winRadius; u++)
				{
					double multiplier = kernelWeight.get(
								u + winRadius,
								v + winRadius
								);
					double valDivX = currentResolver->resolve(matrixDivX, j + u, i + v);
					double valDivY = currentResolver->resolve(matrixDivY, j + u, i + v);
					A += multiplier * valDivX * valDivX;
					B += multiplier * valDivX * valDivY;
					C += multiplier * valDivY * valDivY;
				}
			}
			double det = A * C - B * B;
			double trace = A + C;
			double discr = trace * trace - 4.0 * det;
			double eig1 = 0.0, eig2 = 0.0;
			if (discr > DBL_EPSILON)
			{
				eig1 = 0.5 * (trace + sqrt(discr));
				eig2 = 0.5 * (trace - sqrt(discr));
			}
			else if (abs(discr) < DBL_EPSILON)
			{
				eig1 = eig2 = 0.5 * trace;
			}
			double eigMin = qMin(eig1, eig2);
			//double eigMin = det - 0.05 * trace;
			minima[i * width + j] = eigMin;
			if (responseMap != nullptr)
				responseMap->set(j, i, eigMin);
		}
	}
	// выбираем точки
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			double thisValue = minima[i * width + j];
			bool ok = true;
			neighborhood.clear();
			// берём соседние 8 точек
			for (int py = -winRadius; py <= winRadius; py++)
				for (int px = -winRadius; px <= winRadius; px++)
					if (!(py == 0 && px == 0))
						neighborhood.push_back(Point(j + px, i + py));
			for (Point neighbor : neighborhood)
			{
				if (neighbor.x < 0
						|| neighbor.y < 0
						|| neighbor.x >= width
						|| neighbor.y >= height)
					continue;
				// если не выполняется условие локального максимума
				if (thisValue <= minima[neighbor.y * width + neighbor.x])
				{
					ok = false;
					break;
				}
			}
			if (ok && (thisValue > threshold))
				result.push_back(Point(j, i, minima[i * width + j]));
		}
	}
	delete[] minima;
	return result;
}

QVector<Point> pointsFilterANMS(QVector<Point> points, int maxCount)
{
	QVector<Point> result;
	for (Point point : points)
		result.push_back(point);
	sort(result.begin(), result.end(), [](Point &a, Point &b) -> bool {
		return (a.value > b.value);
	});
	int pointCount = result.size();
	double radius = 0.9;
	while (pointCount > maxCount)
	{
		pointCount = result.size();
		for (int i = 0; i < pointCount; i++)
		{
			for (int j = i + 1; j < pointCount; j++)
			{
				if (distance(result[i], result[j]) <= radius)
				{
					result.removeAt(j);
					pointCount--;
					j--;
				}
			}
		}
		radius += 0.9;
	}
	return result;
}

QVector<ImageDescriptor> descrBuildByPatch(const ImageMatrix &matrix,
										   const QVector<Point> points,
										   int blockSize,
										   int gridSize,
										   int angleCount,
										   IMatrixEdgeResolver *resolver)
{
	QVector<ImageDescriptor> result;
	QVector<double> currentBinData;
	QVector<double> descriptorData;
	int descRadius = (blockSize * gridSize) / 2;
	const double dblPi = 2.0 * M_PI;
	double angleStep = dblPi / static_cast<double>(angleCount);
	double curGradVal = 0.0;
	double curGradAngle = 0.0;
	int width = matrix.getWidth();
	int height = matrix.getHeight();
	IMatrixEdgeResolver *currentResolver = resolver;
	if (resolver == nullptr)
		currentResolver = MatrixEdgeResolverFactory::getInstance()->getLastResolver();
	ImageMatrix angleMap(width, height);
	ImageMatrix gradientMap = matrixFilterSobel(matrix,
												KERNEL_SOBEL,
												currentResolver,
												&angleMap);
	for (Point point : points)
	{
		int anchorX = point.x - descRadius;
		int anchorY = point.y - descRadius;
		currentBinData.clear();
		descriptorData.clear();
		for (int blockY = 0; blockY < gridSize; blockY++)
			for (int blockX = 0; blockX < gridSize; blockX++)
			{
				currentBinData.fill(0.0, angleCount);
				int curX = anchorX + blockX * blockSize;
				int curY = anchorY + blockY * blockSize;
				int limitX = curX + (blockSize - 1);
				int limitY = curY + (blockSize - 1);
				for (; curY <= limitY; curY++)
					for (; curX <= limitX; curX++)
					{
						curGradVal = clampPeriodic(
									currentResolver->resolve(
										gradientMap,
										curX,
										curY
										),
									0.0,
									M_PI
									);
						curGradAngle = clampPeriodic(currentResolver->resolve(
														 angleMap,
														 curX,
														 curY
														 ),
													 0.0,
													 M_PI);
						double binNumCoeff = floor(curGradAngle / angleStep);
						double binCenter = binNumCoeff + 0.5;
						double binDistProp = fabs(binCenter - curGradAngle);
						int binFirst = static_cast<int>(binNumCoeff);
						int binSecond = (binFirst + 1) % angleCount;
						currentBinData[binFirst] += curGradVal * binDistProp;
						currentBinData[binSecond] += curGradVal * (1.0 - binDistProp);

					}
				vectorDblNormalize(&currentBinData);
				while(!currentBinData.empty())
					descriptorData.push_back(currentBinData.takeAt(0));
			}
		result.push_back(ImageDescriptor(point, descriptorData));
	}
	return result;
}
