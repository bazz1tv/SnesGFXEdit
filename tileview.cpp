#include <QtGui>
#include <cmath>

#include "tileview.h"

TileView::TileView(QWidget *parent)
    : QGraphicsView(parent)
{
    setDragMode(NoDrag);
}

void TileView::resizeEvent ( QResizeEvent * event )
{
	//parentWidget()->resize(event->size());
	QGraphicsView::resizeEvent(event);
	//parentWidget()->resize(event->size());
	//centerOn(0,0);
}

void TileView::wheelEvent(QWheelEvent *event)
{
    /*double numDegrees = -event->delta() / 8.0;
    double numSteps = numDegrees / 15.0;
	oldzoom = zoom;
    zoom = std::pow(1.125, numSteps);
    scale(zoom, zoom);*/
	QGraphicsView::wheelEvent(event);
}
