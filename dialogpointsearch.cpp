#include "dialogpointsearch.h"
#include "ui_dialogpointsearch.h"

DialogPointSearch::DialogPointSearch(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::DialogPointSearch)
{
	ui->setupUi(this);
}

int DialogPointSearch::getWinSize()
{
	return winSize;
}

int DialogPointSearch::getMethod()
{
	return method;
}

double DialogPointSearch::getThreshold()
{
	return threshold;
}

int DialogPointSearch::getPointCount()
{
	return pointCount;
}

DialogPointSearch::~DialogPointSearch()
{
	delete ui;
}

void DialogPointSearch::on_buttonBox_accepted()
{
	winSize = ui->sbWinSize->value();
	threshold = ui->sbThreshold->value();
	method = ui->comboOperator->currentIndex();
	if (ui->cbFilter->isChecked())
	{
		pointCount = ui->sbPointCount->value();
	}
	else
	{
		pointCount = -1;
	}
}
