#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	imgOrig = nullptr;
	imgResult = nullptr;
	scOrig = new GScene();
	scResult = new GScene();
	actGrpEdgeResolve = new QActionGroup(this);
	ui->graphViewOrig->setScene(scOrig);
	ui->graphViewResult->setScene(scResult);
	actGrpEdgeResolve->addAction(ui->actionEdgeResolveNull);
	actGrpEdgeResolve->addAction(ui->actionEdgeResolveNearest);
	actGrpEdgeResolve->addAction(ui->actionEdgeResolveMirror);
	actGrpEdgeResolve->addAction(ui->actionEdgeResolveRepeat);
	ui->actionEdgeResolveNull->setChecked(true);
	currentResolver = MatrixEdgeResolverFactory::getInstance()->getResolver("null");
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::on_actionLoad_triggered()
{
	QString fname = QFileDialog::getOpenFileName(
				this,
				"Открыть...",
				QDir::currentPath(),
				tr("Изображения (*.jpg *.jpeg *.png *.bmp *.gif)")
				);
	if (!fname.isNull())
	{
		imgOrig = new QImage(fname);
		initResultImg();
		scOrig->initImg(imgOrig);
		scResult->initImg(imgResult);
	}
}

void MainWindow::on_actionEdgeResolveNull_triggered()
{
	currentResolver = MatrixEdgeResolverFactory
			::getInstance()
			->getResolver("null");
}

void MainWindow::on_actionEdgeResolveNearest_triggered()
{
	currentResolver = MatrixEdgeResolverFactory
			::getInstance()
			->getResolver("nearest");
}

void MainWindow::on_actionEdgeResolveMirror_triggered()
{
	currentResolver = MatrixEdgeResolverFactory
			::getInstance()
			->getResolver("mirror");
}

void MainWindow::on_actionEdgeResolveRepeat_triggered()
{
	currentResolver = MatrixEdgeResolverFactory
			::getInstance()
			->getResolver("repeat");
}

void MainWindow::initResultImg()
{
	if (imgOrig != nullptr)
	{
		if (imgResult != nullptr)
			delete imgResult;
		imgResult = new QImage(*imgOrig);
	}
}

void MainWindow::showImgOrig()
{
	if (imgOrig != nullptr)
		scOrig->initImg(imgOrig);
}

void MainWindow::showImgResult()
{
	if (imgResult != nullptr)
		scResult->initImg(imgResult);
}

void MainWindow::on_actionBlurGaussian_triggered()
{
	double sigma;
	bool okSigma = false;
	ImageMatrix *matrix = ImageConverter::qImageToMatrix(*imgResult);
	sigma = QInputDialog::getDouble(
				this,
				"Ввод параметра",
				"Сигма:",
				0.5,
				0.00000001,
				DBL_MAX,
				4,
				&okSigma
				);
	if (!okSigma)
		return;
	ImageMatrix kernelGaussian = kernelGenerateGaussian(sigma);
	ImageMatrix convolutedMatrix = matrixConvolute(
				*matrix,
				kernelGaussian,
				currentResolver);
	ImageMatrix resultMatrix = matrixNormalize(convolutedMatrix);
	QImage *resultImage = ImageConverter::matrixToQImage(resultMatrix);
	delete imgResult;
	imgResult = resultImage;
	delete matrix;
	//resultMatrix.~ImageMatrix();
	//kernelGaussian.~ImageMatrix();
	showImgResult();
}

void MainWindow::on_actionSobel_triggered()
{
	int kernelType = 0;
	kernelType =QInputDialog::getInt(
				this,
				"Ввод параметра",
				"Тип ядра\n0 - Собель\n1 - Щарр",
				0,
				0,
				1);
	ImageMatrix *matrix = ImageConverter::qImageToMatrix(*imgResult);
	ImageMatrix matrixResult = matrixFilterSobel(*matrix, 0, currentResolver);
	QImage *resultImage = ImageConverter::matrixToQImage(matrixResult);
	delete imgResult;
	imgResult = resultImage;
	delete matrix;
	showImgResult();
}

void MainWindow::on_actionBlurUniform_triggered()
{
	int radius;
	bool okRadius = false;
	ImageMatrix *matrix = ImageConverter::qImageToMatrix(*imgResult);
	radius = QInputDialog::getInt(
				this,
				"Ввод параметра",
				"Радиус:",
				1,
				1,
				1024,
				4,
				&okRadius
				);
	if (!okRadius)
		return;
	ImageMatrix kernelUniform = kernelGenerateUniformBlur(radius);
	ImageMatrix convolutedMatrix = matrixConvolute(
				*matrix,
				kernelUniform,
				currentResolver);
	ImageMatrix resultMatrix = matrixNormalize(convolutedMatrix);
	QImage *resultImage = ImageConverter::matrixToQImage(resultMatrix);
	delete imgResult;
	imgResult = resultImage;
	delete matrix;
	//resultMatrix.~ImageMatrix();
	//kernelGaussian.~ImageMatrix();
	showImgResult();
}

void MainWindow::on_actionReset_triggered()
{
	if (imgOrig != nullptr)
	{
		if (imgResult != nullptr)
			delete imgResult;
		imgResult = new QImage(*imgOrig);
		showImgResult();
	}
}

void MainWindow::on_actionBlurGaussianSeparate_triggered()
{
	double sigma;
	bool okSigma = false;
	ImageMatrix *matrix = ImageConverter::qImageToMatrix(*imgResult);
	sigma = QInputDialog::getDouble(
				this,
				"Ввод параметра",
				"Сигма:",
				0.5,
				0.00000001,
				DBL_MAX,
				4,
				&okSigma
				);
	if (!okSigma)
		return;
	ImageMatrix kernelGaussianX = kernelGenerateGaussianSeparate(sigma, false);
	ImageMatrix kernelGaussianY = kernelGenerateGaussianSeparate(sigma, true);
	ImageMatrix convolutedMatrix = matrixConvolute(
				*matrix,
				kernelGaussianX,
				currentResolver);
	ImageMatrix convolutedMatrixFull = matrixConvolute(
				convolutedMatrix,
				kernelGaussianY,
				currentResolver);
	ImageMatrix resultMatrix = matrixNormalize(convolutedMatrixFull);
	QImage *resultImage = ImageConverter::matrixToQImage(resultMatrix);
	delete imgResult;
	imgResult = resultImage;
	delete matrix;
	showImgResult();
}
