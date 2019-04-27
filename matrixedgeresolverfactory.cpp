#include "matrixedgeresolverfactory.h"

MatrixEdgeResolverFactory * MatrixEdgeResolverFactory::instance = nullptr;

MatrixEdgeResolverFactory::MatrixEdgeResolverFactory()
{
	this->currentChoice = "null";
}

MatrixEdgeResolverFactory * MatrixEdgeResolverFactory::getInstance()
{
	if (instance == nullptr)
		instance = new MatrixEdgeResolverFactory();
	return instance;
}

void MatrixEdgeResolverFactory::chooseResolver(QString resolverType)
{
	this->currentChoice = resolverType;
}

IMatrixEdgeResolver *MatrixEdgeResolverFactory::getResolver(QString resolverType)
{
	if (!resolvers.contains(resolverType))
	{
		IMatrixEdgeResolver * resolver = nullptr;
		if (resolverType == "null" || resolverType == "black")
			resolver = new MatrixEdgeResolverNull();
		else if (resolverType == "mirror")
			resolver = new MatrixEdgeResolverMirror();
		else if (resolverType == "nearest")
			resolver = new MatrixEdgeResolverNearest();
		else if (resolverType == "repeat")
			resolver = new MatrixEdgeResolverRepeat();
		resolvers[resolverType] = resolver;
	}
	currentChoice = resolverType;
	return resolvers[resolverType];
}

IMatrixEdgeResolver *MatrixEdgeResolverFactory::getLastResolver()
{
	return getResolver(this->currentChoice);
}
