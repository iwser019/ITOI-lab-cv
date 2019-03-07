#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
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
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::on_actionLoad_triggered()
{

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
