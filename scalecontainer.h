#ifndef SCALECONTAINER_H
#define SCALECONTAINER_H

#include "defs.h"
#include "imagematrix.h"
#include "imagematrixfunc.h"
#include "imatrixedgeresolver.h"

struct ScaleContainerRecord
{
		int octave, level;
		double sigmaLocal, sigmaGlobal;
		ImageMatrix *image;
};

class ScaleContainer
{
	private:
		ImageMatrix *imageSource;
		QVector< QVector<ImageMatrix *> > octaves;
		int numOctaves, numLevels;
		int maxOctaves;
		double sigmaBase;
		IMatrixEdgeResolver *currentResolver;
	public:
		ScaleContainer(ImageMatrix *source);
		void setEdgeResolver(IMatrixEdgeResolver *resolver);
		void build(int octaveCount,
				   int levelCount,
				   double baseSigma,
				   double initSigma);
		ImageMatrix * getClosest(double sigma);
		Point & translateCoord(const int &x,
							   const int &y,
							   const double &sigma) const;
		Point & translateCoord(const Point &point,
							   const double &sigma) const;
		int getOctaveNum() const;
		int getLevelNum() const;
		QVector<ScaleContainerRecord> getAllImages();
};

#endif // SCALECONTAINER_H
