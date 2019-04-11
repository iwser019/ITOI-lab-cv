#ifndef IMATRIXEDGERESOLVER_H
#define IMATRIXEDGERESOLVER_H

#include "defs.h"
#include "imagematrix.h"
#include "util.h"

/*!
 * \brief Интерфейс обработчика края изображения
 */
class IMatrixEdgeResolver
{
	public:
		IMatrixEdgeResolver();
		virtual double resolve(const ImageMatrix &matrix, int x, int y) = 0;
		virtual ~IMatrixEdgeResolver(){}
};

/*!
 * \brief Обработчик края изображения с обнулением
 */
class MatrixEdgeResolverNull : public IMatrixEdgeResolver
{
	public:
		MatrixEdgeResolverNull(){}
		double resolve(const ImageMatrix &matrix, int x, int y);

};

/*!
 * \brief Обработчик края изображения с копированием края
 */
class MatrixEdgeResolverNearest : public IMatrixEdgeResolver
{
	public:
		MatrixEdgeResolverNearest(){}
		double resolve(const ImageMatrix &matrix, int x, int y);
};

/*!
 * \brief Обработчик края изображения с отражением
 */
class MatrixEdgeResolverMirror : public IMatrixEdgeResolver
{
	public:
		MatrixEdgeResolverMirror(){}
		double resolve(const ImageMatrix &matrix, int x, int y);
};

class MatrixEdgeResolverRepeat : public IMatrixEdgeResolver
{
	public:
		MatrixEdgeResolverRepeat(){}
		double resolve(const ImageMatrix &matrix, int x, int y);
};

#endif // IMATRIXEDGERESOLVER_H
