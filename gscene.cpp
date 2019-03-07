#include "gscene.h"

GScene::GScene()
{
	imgClip = NULL;
}

void GScene::initImg(QImage *img)
{
	imgBackup = img;
	this->clear();
	this->setSceneRect(QRectF(img->rect()));
	this->clear();
	currImg = QPixmap(img->rect().width(), img->rect().height());
	QPainter pntr(&currImg);
	pntr.drawImage(0, 0, *img);
	pntr.end();
	this->addPixmap(currImg);
	deselect();
}

void GScene::deselect()
{
	region.x1 = region.y1 = 0;
	region.x2 = currImg.width();
	region.y2 = currImg.height();
	showRegion(region);
}

QImage GScene::getSelectedImage()
{
	return currImg.copy(region.x1, region.y1, region.x2 - region.x1, region.y2 - region.y1).toImage();
}

QImage GScene::getFullImage()
{
	return currImg.copy(currImg.rect()).toImage();
}

void GScene::setSelectedImage(QImage src)
{
	QPainter pntr(&currImg);
	pntr.drawImage(region.x1, region.y1, src);
	pntr.end();
	this->clear();
	this->addPixmap(currImg);
}

void GScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
	QPointF p;
	int startX, startY, endX, endY;
	if (mode == 1)
	{
		p = mouseEvent->scenePos();
		if (oldX < currX)
		{
			startX = oldX;
			endX = currX;
		}
		else
		{
			startX = currX;
			endX = oldX;
		}
		if (oldY < currY)
		{
			startY = oldY;
			endY = currY;
		}
		else
		{
			startY = currY;
			endY = oldY;
		}
		QImage im;
		im = currImg.toImage();
		QPainter pnt(&currImg);
		for (int i = startX; i <= endX; i++)
		{
			pnt.setPen(QPen(QColor(imgBackup->pixel(i, startY))));
			pnt.drawPoint(i, startY);
			pnt.setPen(QPen(QColor(imgBackup->pixel(i, endY))));
			pnt.drawPoint(i, endY);
		}
		for (int i = startY; i <= endY; i++)
		{
			pnt.setPen(QPen(QColor(imgBackup->pixel(startX, i))));
			pnt.drawPoint(startX, i);
			pnt.setPen(QPen(QColor(imgBackup->pixel(endX, i))));
			pnt.drawPoint(endX, i);
		}
		//if (p.x() < 0 || p.x() >= this->width() || p.y() < 0 || p.y() >= this->height()) return;
		if (p.x() > 0 && p.x() < width())
		currX = p.x();
		if (p.y() > 0 && p.y() < height())
		currY = p.y();
		if (oldX < currX)
		{
			startX = oldX;
			endX = currX;
		}
		else
		{
			startX = currX;
			endX = oldX;
		}
		if (oldY < currY)
		{
			startY = oldY;
			endY = currY;
		}
		else
		{
			startY = currY;
			endY = oldY;
		}
		pnt.setPen(QPen(Qt::black));
		pnt.drawLine(startX, startY, endX, startY);
		pnt.drawLine(startX, startY, startX, endY);
		pnt.drawLine(startX, endY, endX, endY);
		pnt.drawLine(endX, startY, endX, endY);
		addPixmap(currImg);
	}
}

void GScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
	QPointF p;
	if (mouseEvent->button() != Qt::LeftButton) return;
	p = mouseEvent->scenePos();
	oldX = p.x();
	oldY = p.y();
	currX = oldX;
	currY = oldY;
	imgBackup = new QImage(currImg.toImage());
	//imgBackup = currImg.toImage();
	mode = 1;
}

void GScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
	QPointF p;
	int startX, startY, endX, endY;
	if (mode == 1)
	{
		p = mouseEvent->scenePos();
		if (oldX < currX)
		{
			startX = oldX;
			endX = currX;
		}
		else
		{
			startX = currX;
			endX = oldX;
		}
		if (oldY < currY)
		{
			startY = oldY;
			endY = currY;
		}
		else
		{
			startY = currY;
			endY = oldY;
		}
		region.x1 = startX;
		region.y1 = startY;
		region.x2 = endX;
		region.y2 = endY;
		QImage im;
		im = currImg.toImage();
		QPainter pnt(&currImg);
		for (int i = startX; i <= endX; i++)
		{
			pnt.setPen(QPen(QColor(imgBackup->pixel(i, startY))));
			pnt.drawPoint(i, startY);
			pnt.setPen(QPen(QColor(imgBackup->pixel(i, endY))));
			pnt.drawPoint(i, endY);
		}
		for (int i = startY; i <= endY; i++)
		{
			pnt.setPen(QPen(QColor(imgBackup->pixel(startX, i))));
			pnt.drawPoint(startX, i);
			pnt.setPen(QPen(QColor(imgBackup->pixel(endX, i))));
			pnt.drawPoint(endX, i);
		}
		if (p.x() > 0 && p.x() < width())
		currX = p.x();
		if (p.y() > 0 && p.y() < height())
		currY = p.y();
		addPixmap(currImg);
		mode = 0;
		delete imgBackup;
		imgBackup = NULL;
		if (currX != oldX || currY != oldY)
			showRegion(region);
		else
		{
			deselect();
		}
	}
}
