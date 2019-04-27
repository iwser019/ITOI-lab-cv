#ifndef MATRIXEDGERESOLVERFACTORY_H
#define MATRIXEDGERESOLVERFACTORY_H

#include "defs.h"
#include "imatrixedgeresolver.h"
#include "util.h"

using namespace std;

class MatrixEdgeResolverFactory
{
	private:
		static MatrixEdgeResolverFactory *instance;
		QString currentChoice;
		QMap<QString, IMatrixEdgeResolver*> resolvers;
		MatrixEdgeResolverFactory();
	public:
		static MatrixEdgeResolverFactory * getInstance();
		void chooseResolver(QString resolverType);
		IMatrixEdgeResolver * getResolver(QString resolverType);
		IMatrixEdgeResolver * getLastResolver();
};

#endif // MATRIXEDGERESOLVERFACTORY_H
