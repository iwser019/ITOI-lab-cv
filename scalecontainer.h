#ifndef SCALECONTAINER_H
#define SCALECONTAINER_H

#include "defs.h"
#include "imagematrix.h"
#include "imagematrixfunc.h"
#include "imatrixedgeresolver.h"
#include "util.h"

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
		/*!
		 * \brief Установка обработчика края
		 * \param resolver Объект обработчика края
		 */
		void setEdgeResolver(IMatrixEdgeResolver *resolver);
		/*!
		 * \brief Построение пирамиды масштаба
		 * \param octaveCount Число октав
		 * \param levelCount Число масштабов
		 * \param baseSigma Начальная сигма
		 * \param initSigma Предполагаемая исходная сигма
		 */
		void build(int octaveCount,
				   int levelCount,
				   double baseSigma,
				   double initSigma);
		/*!
		 * \brief Получение ближайшего изображения
		 * \param sigma Глобальная сигма
		 * \return Изображение
		 */
		ImageMatrix * getClosest(double sigma);
		Point & translateCoord(const int &x,
							   const int &y,
							   const double &sigma) const;
		Point & translateCoord(const Point &point,
							   const double &sigma) const;
		/*!
		 * \brief Получение числа октав
		 * \return Число октав
		 */
		int getOctaveNum() const;
		/*!
		 * \brief Получение числа масштабов
		 * \return Число масштабов
		 */
		int getLevelNum() const;
		QVector<ScaleContainerRecord> getAllImages();
		~ScaleContainer();
};

#endif // SCALECONTAINER_H
