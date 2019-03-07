#ifndef MATRIXEDGERESOLVERFACTORY_H
#define MATRIXEDGERESOLVERFACTORY_H

#include "defs.h"
#include "imatrixedgeresolver.h"

using namespace std;

class MatrixEdgeResolverFactory
{
	private:
		static MatrixEdgeResolverFactory *instance;
		QMap<QString, IMatrixEdgeResolver*> resolvers;
		MatrixEdgeResolverFactory();
	public:
		static MatrixEdgeResolverFactory * getInstance();
		IMatrixEdgeResolver * getResolver(QString resolverType);
};

#endif // MATRIXEDGERESOLVERFACTORY_H
