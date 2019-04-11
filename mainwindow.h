#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QActionGroup>
#include <QFileDialog>
#include <QInputDialog>
#include "defs.h"
#include "dialogpointsearch.h"
#include "imageconverter.h"
#include "imagematrixfunc.h"
#include "imatrixedgeresolver.h"
#include "matrixedgeresolverfactory.h"
#include "scalecontainer.h"
#include "gscene.h"
#include "util.h"

namespace Ui {
	class MainWindow;
}

class MainWindow : public QMainWindow
{
		Q_OBJECT

	public:
		explicit MainWindow(QWidget *parent = nullptr);
		~MainWindow();

	private slots:
		void on_actionLoad_triggered();
		void on_actionEdgeResolveNull_triggered();
		void on_actionEdgeResolveNearest_triggered();
		void on_actionEdgeResolveMirror_triggered();
		void on_actionEdgeResolveRepeat_triggered();
		void on_actionBlurGaussian_triggered();
		void on_actionSobel_triggered();

		void on_actionBlurUniform_triggered();

		void on_actionReset_triggered();

		void on_actionBlurGaussianSeparate_triggered();

		void on_actionMakePyramid_triggered();

		void on_actionPointSearch_triggered();

	private:
		Ui::MainWindow *ui;
		GScene *scOrig, *scResult;
		QImage *imgOrig, *imgResult;
		QActionGroup *actGrpEdgeResolve;
		IMatrixEdgeResolver *currentResolver;
		ScaleContainer *scaleContainer;
		void initResultImg();
		void showImgOrig();
		void showImgResult();
};

#endif // MAINWINDOW_H
