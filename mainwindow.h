#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QActionGroup>
#include "defs.h"
#include "imatrixedgeresolver.h"
#include "matrixedgeresolverfactory.h"
#include "gscene.h"

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

	private:
		Ui::MainWindow *ui;
		GScene *scOrig, *scResult;
		QImage *imgOrig, *imgResult;
		QActionGroup *actGrpEdgeResolve;
		IMatrixEdgeResolver *currentResolver;
};

#endif // MAINWINDOW_H
