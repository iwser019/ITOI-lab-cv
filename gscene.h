#ifndef GSCENE_H
#define GSCENE_H

#include <QObject>
#include <QGraphicsScene>
#include "defs.h"

struct selRegion
{
	int x1, y1, x2, y2;
	selRegion()
	{
		x1 = y1 = x2 = y2 = 0;
	}
};

class GScene : public QGraphicsScene
{
	Q_OBJECT
	public:
		int oldX, oldY;
		int currX, currY;
		int mode;
		QPixmap currImg;
		QImage *imgClip;
		QImage *imgBackup;
		selRegion region;
		GScene();
		void initImg(QImage *img);
		void deselect();
		QImage getSelectedImage();
		QImage getFullImage();
		void setSelectedImage(QImage src);
	protected:
		void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);
		void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);
		void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent);
	signals:
		void showRegion(selRegion reg);
};

#endif // GSCENE_H
