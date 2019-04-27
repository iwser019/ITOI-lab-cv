#ifndef IMAGEDESCRIPTOR_H
#define IMAGEDESCRIPTOR_H

#include "defs.h"
#include "util.h"

class ImageDescriptor
{
	private:
		Point coord;
		QVector<double> data;
	public:
		ImageDescriptor();
		ImageDescriptor(const Point &coord, const QVector<double> &data);
		ImageDescriptor(const ImageDescriptor &source);
		Point getPoint() const;
		QVector<double> getData() const;
		ImageDescriptor& operator=(const ImageDescriptor &other);
};

double descriptorCompareEuclidean(const ImageDescriptor &descFirst,
								  const ImageDescriptor &descSecond);
double descriptorCompareManhattan(const ImageDescriptor &descFirst,
								  const ImageDescriptor &descSecond);
double descriptorCompareSSD(const ImageDescriptor &descFirst,
							const ImageDescriptor &descSecond);
ImageDescriptor descriptorFindClosest(const ImageDescriptor &descriptor,
									  const QVector<ImageDescriptor> &otherDescriptors);

#endif // IMAGEDESCRIPTOR_H
