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
	currentResolver = MatrixEdgeResolverFactory
			::getInstance()->getResolver("null");
	scaleContainer = nullptr;
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
		QImage imgTmp(fname);
		imgOrig = new QImage(imgTmp.convertToFormat(QImage::Format_RGBA8888));
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

QVector<Point> MainWindow::searchPoints(const ImageMatrix &matrix)
{
	QVector<Point> pointsFound;
	int winSize = 1, method = 0, pointCount = -1;
	double threshold = 0.0;
	bool ok = false;
	DialogPointSearch *dialog = new DialogPointSearch(this);
	if (dialog->exec() == QDialog::Accepted)
	{
		ok = true;
		winSize = dialog->getWinSize();
		method = dialog->getMethod();
		threshold = dialog->getThreshold();
		pointCount = dialog->getPointCount();
	}
	delete dialog;
	if (!ok)
		return pointsFound;
	ImageMatrix matrixSmooth = matrixConvolute(matrix,
											   kernelGenerateGaussian(1.0),
											   currentResolver);

	ImageMatrix *responseMap = new ImageMatrix(matrix.getWidth(), matrix.getHeight());
	if (method == 0)
	{

		pointsFound = pointsMoravecPointOp(
					matrixSmooth,
					threshold,
					winSize,
					currentResolver, responseMap
					);
		ImageConverter::matrixToQImage(*responseMap)->save("responsemap.png");
		delete responseMap;
	}
	else if (method == 1)
	{
		pointsFound = pointsHarrisPointOp(
					matrixSmooth,
					threshold,
					winSize,
					0,
					currentResolver, responseMap
					);
		ImageConverter::matrixToQImage(*responseMap)->save("responsemap.png");
		delete responseMap;
	}
	else
		return pointsFound;
	if (pointCount > 0)
	{
		pointsFound = pointsFilterANMS(pointsFound, pointCount);
	}
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
				KERNEL_SOBEL,
				KERNEL_SOBEL,
				KERNEL_SCHARR);
	ImageMatrix *matrix = ImageConverter::qImageToMatrix(*imgResult);
	ImageMatrix matrixResult = matrixFilterSobel(*matrix,
												 kernelType,
												 currentResolver);
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

void MainWindow::on_actionMakePyramid_triggered()
{
	if (imgResult == nullptr)
		return;
	ImageMatrix *matrix = ImageConverter::qImageToMatrix(*imgResult);
	scaleContainer = new ScaleContainer(matrix);
	scaleContainer->setEdgeResolver(currentResolver);
	int maxOctaves = matrixMaxAvailableOctaves(*matrix);
	delete matrix;
	int numOctaves = 0, numLevels = 0;
	double sigmaBase = 0.0, sigmaInit = 0.0;
	bool ok = false;
	numOctaves = QInputDialog::getInt(
				this,
				"Ввод параметра",
				"Количество октав:",
				1,
				1,
				maxOctaves,
				1,
				&ok);
	if (!ok)
		return;
	ok = false;
	numLevels = QInputDialog::getInt(
				this,
				"Ввод параметра",
				"Количество масштабов:",
				1,
				1,
				1024,
				1,
				&ok);
	if (!ok)
		return;
	ok = false;
	sigmaBase = QInputDialog::getDouble(
				this,
				"Ввод параметра",
				"Сигма (базовая):",
				0.5,
				DBL_MIN,
				DBL_MAX,
				4,
				&ok);
	if (!ok)
		return;
	ok = false;
	sigmaInit = QInputDialog::getDouble(
				this,
				"Ввод параметра",
				"Сигма (предположительная):",
				0.5,
				DBL_MIN,
				DBL_MAX,
				4,
				&ok);
	if (!ok)
		return;
	scaleContainer->build(numOctaves, numLevels, sigmaBase, sigmaInit);
	QString dirName = QFileDialog
			::getExistingDirectory(
				this,
				"Выбор папки",
				"",
				QFileDialog::ShowDirsOnly);
	if (!dirName.isNull())
	{
		QVector<ScaleContainerRecord> records = scaleContainer->getAllImages();
		for (ScaleContainerRecord record : records)
		{
			QImage *image = ImageConverter::matrixToQImage(*record.image);
			image->save(
						dirName
						+ "\\"
						+ QString("img_oct%1_lvl%2_sigLoc%3_sigGlob%4.png")
						.arg(record.octave)
						.arg(record.level)
						.arg(record.sigmaGlobal)
						.arg(record.sigmaLocal).replace(',', '.')
						);
			delete image;
		}
	}
}

void MainWindow::on_actionPointSearch_triggered()
{
	if (imgOrig == nullptr)
		return;
	QString imgOtherPath = QFileDialog::getOpenFileName(
				this,
				"Открыть другое изображение...",
				QDir::currentPath(),
				tr("Изображения (*.jpg *.jpeg *.png *.bmp *.gif)")
				);
	if (imgOtherPath.isEmpty())
		return;
	QImage imgTmp(imgOtherPath);
	QImage *imgOther = new QImage(imgTmp.convertToFormat(QImage::Format_RGBA8888));
	ImageMatrix *matrix = ImageConverter::qImageToMatrix(*imgOrig);
	QVector<Point> pointsFound = searchPoints(*matrix);
	delete matrix;
	QImage *imageResult = new QImage(*imgOrig);
	QPainter painter(imageResult);
	painter.setBrush(QBrush(Qt::red));
	for(Point point : pointsFound)
	{
		painter.drawEllipse(QPoint(point.x, point.y), 2, 2);
	}
	imgResult = imageResult;
	delete imgOther;
	showImgResult();
}

void MainWindow::on_actionDescriptorsBuild_triggered()
{
	bool ok = false;
	if (imgOrig == nullptr)
		return;
	int blockSize = DESC_BLOCK_SIZE;
	int gridSize = DESC_GRID_SIZE;
	int angleCount = DESC_ANGLE_COUNT;
	blockSize = QInputDialog::getInt(
				this,
				"Ввод параметра",
				"Размер блока:",
				DESC_BLOCK_SIZE,
				1,
				1024,
				1,
				&ok);
	if (!ok)
		return;
	gridSize = QInputDialog::getInt(
				this,
				"Ввод параметра",
				"Размер сетки в блоках:",
				DESC_GRID_SIZE,
				1,
				1024,
				1,
				&ok);
	if (!ok)
		return;
	angleCount = QInputDialog::getInt(
				this,
				"Ввод параметра",
				"Количество корзин гистограммы:",
				DESC_ANGLE_COUNT,
				1,
				1024,
				1,
				&ok);
	if (!ok)
		return;
	QString imgOtherPath = QFileDialog::getOpenFileName(
				this,
				"Открыть другое изображение...",
				QDir::currentPath(),
				tr("Изображения (*.jpg *.jpeg *.png *.bmp *.gif)")
				);
	if (imgOtherPath.isEmpty())
		return;
	QImage imgTmp(imgOtherPath);
	QImage *imgOther = new QImage(imgTmp.convertToFormat(QImage::Format_RGBA8888));
	ImageMatrix *matrix = ImageConverter::qImageToMatrix(*imgOrig);
	ImageMatrix *matrixOther = ImageConverter::qImageToMatrix(*imgOther);
	QVector<Point> pointsFound = searchPoints(*matrix);
	QVector<Point> pointsFoundOther = searchPoints(*matrixOther);
	QVector<ImageDescriptor> descriptors = descrBuildByPatch(
				*matrix,
				pointsFound,
				blockSize,
				gridSize,
				angleCount,
				currentResolver);
	QVector<ImageDescriptor> descriptorsOther = descrBuildByPatch(
				*matrixOther,
				pointsFoundOther,
				blockSize,
				gridSize,
				angleCount,
				currentResolver);
	delete matrix;
	delete matrixOther;
	int widthOrig = imgOrig->width();
	QImage *imageResult = new QImage(widthOrig + imgOther->width(),
									 qMax(imgOrig->height(), imgOther->height()),
									 QImage::Format_RGBA8888
									 );
	QPainter painter(imageResult);
	painter.drawPixmap(0, 0,
					   widthOrig, imgOrig->height(),
					   QPixmap::fromImage(*imgOrig));
	painter.drawPixmap(widthOrig + 1, 0,
					   imgOther->width(), imgOther->height(),
					   QPixmap::fromImage(*imgOther));
	painter.setPen(QColor(Qt::cyan));
	painter.setBrush(QBrush(Qt::red));
	/*
	for(Point point : pointsFound)
	{
		painter.drawEllipse(QPoint(point.x, point.y), 2, 2);
	}
	*/
	for (ImageDescriptor descriptor : descriptors)
	{
		ImageDescriptor descriptorClosest = descriptorFindClosest(
					descriptor,
					descriptorsOther);
		Point point = descriptor.getPoint();
		Point pointClosest = descriptorClosest.getPoint();
		pointClosest.x += widthOrig;
		painter.drawEllipse(QPoint(point.x, point.y), 2, 2);
		painter.drawEllipse(QPoint(pointClosest.x, pointClosest.y), 2, 2);
		painter.drawLine(point.x,
						 point.y,
						 pointClosest.x,
						 pointClosest.y);
	}
	imgResult = imageResult;
	delete imgOther;
	showImgResult();
}
