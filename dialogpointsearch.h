#ifndef DIALOGPOINTSEARCH_H
#define DIALOGPOINTSEARCH_H

#include <QDialog>

namespace Ui {
	class DialogPointSearch;
}

class DialogPointSearch : public QDialog
{
		Q_OBJECT

	public:

		explicit DialogPointSearch(QWidget *parent = nullptr);
		int getWinSize();
		int getMethod();
		double getThreshold();
		int getPointCount();
		~DialogPointSearch();

	private slots:
		void on_buttonBox_accepted();

	private:
		int winSize, method, pointCount;
		double threshold;
		Ui::DialogPointSearch *ui;
};

#endif // DIALOGPOINTSEARCH_H
