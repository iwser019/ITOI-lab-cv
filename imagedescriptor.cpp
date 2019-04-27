#include "imagedescriptor.h"

ImageDescriptor::ImageDescriptor()
{
	this->coord = Point(0.0, 0.0, 0.0);
}

ImageDescriptor::ImageDescriptor(const Point &coord,
								 const QVector<double> &data)
{
	this->coord = coord;
	this->data = data;
}

ImageDescriptor::ImageDescriptor(const ImageDescriptor &source)
{
	this->coord = source.getPoint();
	this->data = source.getData();
}

Point ImageDescriptor::getPoint() const
{
	return this->coord;
}

QVector<double> ImageDescriptor::getData() const
{
	return this->data;
}

ImageDescriptor &ImageDescriptor::operator=(const ImageDescriptor &other)
{
	if (this == &other)
		return *this;
	this->coord = other.getPoint();
	this->data = other.getData();
	return *this;
}

double descriptorCompareEuclidean(const ImageDescriptor &descFirst,
								  const ImageDescriptor &descSecond)
{
	double result = 0.0;
	QVector<double> dataFirst = descFirst.getData(),
			dataSecond = descSecond.getData();
	int sizeInt = qMin(dataFirst.size(), dataSecond.size());
	for (int i = 0; i < sizeInt; i++)
	{
		double subtr = dataFirst[i] - dataSecond[i];
		result += (subtr * subtr);
	}
	result = sqrt(result);
	return result;
}

double descriptorCompareManhattan(const ImageDescriptor &descFirst,
								  const ImageDescriptor &descSecond)
{
	double result = 0.0;
	QVector<double> dataFirst = descFirst.getData(),
			dataSecond = descSecond.getData();
	int sizeInt = qMin(dataFirst.size(), dataSecond.size());
	for (int i = 0; i < sizeInt; i++)
	{
		double subtr = fabs(dataFirst[i] - dataSecond[i]);
		result += subtr;
	}
	result = sqrt(result);
	return result;
}

double descriptorCompareSSD(const ImageDescriptor &descFirst,
							const ImageDescriptor &descSecond)
{
	double result = 0.0;
	QVector<double> dataFirst = descFirst.getData(),
			dataSecond = descSecond.getData();
	int sizeInt = qMin(dataFirst.size(), dataSecond.size());
	for (int i = 0; i < sizeInt; i++)
	{
		double subtr = dataFirst[i] - dataSecond[i];
		result += subtr;
	}
	result = sqrt(result);
	return result;
}

ImageDescriptor descriptorFindClosest(const ImageDescriptor &descriptor,
									  const QVector<ImageDescriptor> &otherDescriptors)
{
	struct descrIndex
	{
		double dist;
		int index;
	};
	int idx = 0;
	int sizeInt = otherDescriptors.size();
	QVector<descrIndex> indexes;
	for (int i = 0; i < sizeInt; i++)
	{
		descrIndex dIdx;
		dIdx.index = i;
		dIdx.dist = descriptorCompareEuclidean(descriptor, otherDescriptors[i]);
		indexes.push_back(dIdx);
	}
	std::sort(indexes.begin(),
			  indexes.end(),
			  [](const descrIndex &a, const descrIndex &b) -> bool {
		return a.dist < b.dist;
	});
	idx = 0;

	int maxInt = 0;
	/*
	for (int i = 0; i < sizeInt - 1; i++)
	{
		if (fabs(indexes[i].dist - indexes[i + 1].dist) < DBL_EPSILON)
		{
			maxInt++;
		}
		else
			break;
	}
	*/
	return otherDescriptors[indexes[idx].index];
}
