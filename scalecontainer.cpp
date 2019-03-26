#include "scalecontainer.h"

ScaleContainer::ScaleContainer(ImageMatrix *source)
{
	this->currentResolver = nullptr;
	this->numOctaves = 0;
	this->numLevels = 0;
	this->imageSource = new ImageMatrix(*source);
	this->maxOctaves = matrixMaxAvailableOctaves(*source);
}

void ScaleContainer::setEdgeResolver(IMatrixEdgeResolver *resolver)
{
	currentResolver = resolver;
}

void ScaleContainer::build(int octaveCount,
						   int levelCount,
						   double baseSigma,
						   double initSigma)
{
	this->numOctaves = qMin(octaveCount, maxOctaves);
	this->numLevels = levelCount;
	this->sigmaBase = baseSigma;
	ImageMatrix initKernel = kernelGenerateGaussian(
				sqrt(baseSigma * baseSigma - initSigma * initSigma));
	/*
	ImageMatrix workingMatrix(
				matrixConvolute(
					*this->imageSource,
					initKernel,
					currentResolver)
				);
				*/
	ImageMatrix *workingMatrix = new ImageMatrix(
				matrixConvolute(
					*this->imageSource,
					initKernel,
					currentResolver
					)
				);
	double sigmaInterval = pow(2.0, 1.0 / static_cast<double>(levelCount));
	double sigmaCurrent = this->sigmaBase;
	for (int oct = 0; oct < numOctaves; oct++)
	{
		octaves.push_back(QVector<ImageMatrix *>());
		if (oct > 0)
		{
			workingMatrix = new ImageMatrix(
						matrixDownsample(
							*(octaves[oct - 1].back())
						)
					);
			octaves[oct].push_back(new ImageMatrix(*workingMatrix));
		}
		else
			octaves[0].push_back(new ImageMatrix(*workingMatrix));
		for (int level = 1; level <= numLevels; level++)
		{
			double sigmaInterscale = sqrt(
						(sigmaCurrent * sigmaInterval)
						* (sigmaCurrent * sigmaInterval)
						- sigmaCurrent * sigmaCurrent);
			ImageMatrix *oldPtr = workingMatrix;
			ImageMatrix kernel = kernelGenerateGaussian(sigmaInterscale);
			ImageMatrix *convResultMatrix = new ImageMatrix(
						matrixConvolute(
							*workingMatrix,
							kernel,
							currentResolver
							)
						);
			workingMatrix = convResultMatrix;
			delete oldPtr;
			octaves[oct].push_back(new ImageMatrix(*workingMatrix));
			if (level < (numLevels))
				sigmaCurrent *= sigmaInterval;
			else
				sigmaCurrent = sigmaBase;
		}
	}
}

ImageMatrix *ScaleContainer::getClosest(double sigma)
{
	// тупое решение
	double factor = sigma / sigmaBase;
	double scaleInterval = pow(2.0, 1.0 / static_cast<double>(numLevels));
	double octaveIndex = log2(factor);
	int octaveClosest = static_cast<int>(floor(octaveIndex));
	double octaveClosestValue = sigmaBase * pow(2.0, floor(octaveIndex));
	double levelIndex = log(scaleInterval) / log(sigma / octaveClosestValue);
	int levelClosest = static_cast<int>(round(levelIndex));
	return octaves[octaveClosest][levelClosest];
}

Point &ScaleContainer::translateCoord(const int &x,
									  const int &y,
									  const double &sigma) const
{
	// тупое решение
	double factor = sigma / sigmaBase;
	double octaveIndex = log2(factor);
	int scaleFactor = static_cast<int>(pow(2.0, floor(octaveIndex)));
	Point result(x / scaleFactor, y / scaleFactor);
	return result;
}

Point &ScaleContainer::translateCoord(const Point &point,
									  const double &sigma) const
{
	return translateCoord(point.x, point.y, sigma);
}

int ScaleContainer::getOctaveNum() const
{
	return numOctaves;
}

int ScaleContainer::getLevelNum() const
{
	return numLevels;
}

QVector<ScaleContainerRecord> ScaleContainer::getAllImages()
{
	QVector<ScaleContainerRecord> result;
	double sigmaLocal, sigmaGlobal;
	double sigmaInterval = pow(2.0, 1.0 / static_cast<double>(numLevels));
	sigmaGlobal = sigmaBase;
	for (int oct = 0; oct < numOctaves; oct++)
	{
		sigmaLocal = sigmaBase;
		for (int lvl = 0; lvl <= numLevels; lvl++)
		{
			ScaleContainerRecord record;
			record.octave = oct;
			record.level = lvl;
			record.sigmaLocal = sigmaLocal;
			record.sigmaGlobal = sigmaGlobal;
			record.image = octaves[oct][lvl];
			result.push_back(record);
			sigmaLocal *= sigmaInterval;
			if (lvl < numLevels)
				sigmaGlobal *= sigmaInterval;
		}
	}
	return result;
}
