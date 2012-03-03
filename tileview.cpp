#include <QtGui>
#include <cmath>

#include "tileview.h"
#include "globals.h"

void TileView::updateCursor()
{
	//qreal zoomx, zoomy;
	//QTransform scale;
	//m_LPixmap = new QPixmap(zoom*(x),zoom*(y));
	//m_LPixmap->fill(Qt::transparent); // Otherwise you get a black background :(
	//QPainter painter(m_LPixmap);
	//QColor red(255,0,0,128);
	
	//painter.setPen(Qt::NoPen);        // Otherwise you get an thin black border
	//painter.setBrush(red);
	
	//painter.drawRect(0,0,zoom*(x),zoom*(y));
	int factor8w = (tileWHLSize/8)-1;
	int factor8h = (tileHHLSize/8)-1;
	
	// in 1:1 factor8w and h hold how many pixels must be increased by the cursor.
	// this number must be added into the + zoomfactor*8w 
	//scale=transform();
	//zoomx = scale.m11();
	//zoomy = scale.m22();
	
	//zoomx = round(zoomx);
	//zoomy = round(zoomy);
	
	if (swap)
	{
		cursorbuf2 = cursorpix.scaled((tileWHLSize+factor8w),(tileHHLSize+factor8h));
		cursoritem->setPixmap(cursorbuf2);
		cursorbuf = NULL;
	}
	else
	{
		cursorbuf = cursorpix.scaled((tileWHLSize+factor8w),(tileHHLSize+factor8h));
		cursoritem->setPixmap(cursorbuf);
		cursorbuf2=NULL;
	}
	
	debug<<"cursor: w: "<<cursoritem->pixmap().width()<<" h: "<<cursoritem->pixmap().height()<<endl;
	//cursor->setOffset(1,1);
}


TileView::TileView(QWidget *parent)
    : QGraphicsView(parent)
{
    setDragMode(NoDrag);
	tileWHLSize = tileHHLSize = 8;
	zoom = 1;
	firstPaint = true;
	swap = false;
	
	cursoritem = new QGraphicsPixmapItem;
	placeritem = new Tile;
	cursorpix = QPixmap(1,1);
	cursorpix.fill(QColor(0,0,0,0));
	setCursor(cursorpix);
	//cursor->setPos(1,1);
	cursorpix = QPixmap(zoom*(tileWHLSize),zoom*(tileHHLSize));
	placerpix = QPixmap(zoom*(tileWHLSize),zoom*(tileHHLSize));
	placerpix.fill(Qt::transparent);
	cursorpix.fill(Qt::transparent); // Otherwise you get a black background :(
	QPainter painter(&cursorpix), painter2(&placerpix);
	QColor red(255,0,0,128);
	QColor yellow (120,40,10,128);
	
	painter.setPen(Qt::NoPen);        // Otherwise you get an thin black border
	painter2.setPen(Qt::NoPen);
	painter.setBrush(yellow);
	painter2.setBrush(red);
	
	painter.drawRect(0,0,zoom*(tileWHLSize),zoom*(tileHHLSize));
	cursorbuf = QPixmap(cursorpix);
	
	painter2.drawRect(0,0,zoom*(tileWHLSize),zoom*(tileHHLSize));
	placerbuf = QPixmap(placerpix);
	
	cursoritem->setPixmap(cursorbuf);
	cursoritem->setPos(1,1);
	
	placeritem->setPixmap(placerbuf);
	placeritem->setVisible(false);
	
	debug<<"cursor: w: "<<cursoritem->pixmap().width()<<" h: "<<cursoritem->pixmap().height()<<endl;
	
	for (int row = 0; row<rows;row++)
	{
		for (int col=0; col<cols; col++)
		{
			// vars??
			VRAMgrid[row][col] = new Tile; 
		}
	}
}

void TileView::paintEvent ( QPaintEvent * event )
{
	if (firstPaint)
	{
		QCursor::setPos(mapToGlobal(QPoint(twidth/2,theight/2)));
		firstPaint = false;
	}
	QGraphicsView::paintEvent(event);
	//fitInView(0,0,(twidth*(cols+1)),(theight*(rows+1)), Qt::KeepAspectRatioByExpanding);
}

void TileView::showEvent ( QShowEvent * event )
{
	//parentWidget()->resize(event->size());
	QGraphicsView::showEvent(event);
	//parentWidget()->resize(event->size());
	//fitInView(0,0,(twidth*(cols+1)),(theight*(rows+1)), Qt::KeepAspectRatioByExpanding);
	//centerOn(0,0);
}

QSize TileView::sizeHint() const
{
	return QSize(zoom*cols*(twidth+1)+1,zoom*rows*(theight+1)+1);
}

void TileView::resizeEvent ( QResizeEvent * event )
{
	//parentWidget()->resize(event->size());
	QGraphicsView::resizeEvent(event);
	//parentWidget()->resize(event->size());
	//fitInView(0,0,(twidth*(cols+1)),(theight*(rows+1)), Qt::KeepAspectRatioByExpanding);
	centerOn(0,0);
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
