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
	
	
	cursorbuf = cursorpix.scaled((tileWHLSize+factor8w),(tileHHLSize+factor8h));
	cursoritem->setPixmap(cursorbuf);
	
	placerbuf = placerpix.scaled((tileWHLSize+factor8w),(tileHHLSize+factor8h));
	placeritem->setPixmap(placerbuf);
	
	//mousepixbuf = mousepix.scaled(zoom*1,zoom*1);
	//cursor = QCursor(mousepixbuf);
	//setCursor(mousepixbuf);
	
	debug<<"cursor: w: "<<cursoritem->pixmap().width()<<" h: "<<cursoritem->pixmap().height()<<endl;
	//cursor->setOffset(1,1);
}


TileView::TileView(QWidget *parent)
    : QGraphicsView(parent)
{
	for (int row=0; row < rows; row++)
	{
		for (int col=0; col < cols; col++)
		{
			//VRAMgrid_img[row][col] = new QImage;
		}
	}
	
    setDragMode(NoDrag);
	tileWHLSize = tileHHLSize = 8;
	zoom = 1;
	firstPaint = true;
	tilemode = MODE_SWAP;
	tilesize = SIZE_8x8;
	preview_original = true;
	
	setupActions();
	
	start_marker.circle = new QGraphicsEllipseItem;
	stop_marker.circle = new QGraphicsEllipseItem;
	start_marker.row = 0;
	start_marker.col = 0;
	
	start_marker.pen = QPen(Qt::black);
	start_marker.brush = QBrush(Qt::green);
	stop_marker.pen = QPen(Qt::black);
	stop_marker.brush = QBrush(Qt::red);
	start_marker.circle->setPen(start_marker.pen);
	start_marker.circle->setBrush(start_marker.brush);
	stop_marker.circle->setPen(stop_marker.pen);
	stop_marker.circle->setBrush(stop_marker.brush);
	/*start_marker.circle.setWidth(2);
	start_marker.circle.setHeight(2);
	stop_marker.circle.setWidth(2);
	stop_marker.circle.setHeight(2);*/
	
	cursoritem = new QGraphicsPixmapItem;
	placeritem = new Tile;
	selected_tile = new Tile;
	mousepix = QPixmap(1,1);
	mousepix.fill(Qt::transparent);
	//mousepix.fill(QColor(0,0,0,255));
	//cursor = QCursor(mousepix);
	setCursor(mousepix);
	//cursor->setPos(1,1);
	cursorpix = QPixmap(zoom*(tileWHLSize),zoom*(tileHHLSize));
	placerpix = QPixmap(zoom*(tileWHLSize),zoom*(tileHHLSize));
	placerpix.fill(Qt::transparent);
	cursorpix.fill(Qt::transparent); // Otherwise you get a black background :(
	QPainter painter(&cursorpix), painter2(&placerpix);
	QColor red(120,30,45,125);
	QColor yellow(200,10,120,80); 
	
	painter.setPen(Qt::NoPen);        // Otherwise you get an thin black border
	painter2.setPen(Qt::NoPen);
	painter.setBrush(red);
	painter2.setBrush(yellow);
	
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
	// sets the cursor inside the view on load
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

void TileView::mouseReleaseEvent ( QMouseEvent * event )
{
	/*QGraphicsItem* item = (this->scene()->itemAt(event->pos()));
		
	debug<<"event pos: "<<event->pos().x()<<","<<event->pos().y()<<endl;
	if (item != NULL)
	{
		if (!item->acceptsHoverEvents())
		{
			Tile *tmp = (Tile*)qgraphicsitem_cast<QGraphicsPixmapItem*>(this->scene()->itemAt(event->pos().x()+4,event->pos().y()));
			if (tmp != NULL)
				tmp->doMouseReleaseEvent(NULL);
		}
	}*/
	QGraphicsView::mouseReleaseEvent(event);
}

void TileView::setupActions()
{
	setVRAMStartMarker = new QAction(tr("Set Start Marker"), this);
	setVRAMStopMarker = new QAction(tr("Set Stop Marker"), this);
	
	connect(setVRAMStopMarker, SIGNAL(triggered()), this, SLOT(setStopMarker()));
	connect(setVRAMStartMarker, SIGNAL(triggered()), this, SLOT(setStartMarker()));
	
	// questionable
	addAction(setVRAMStartMarker);
	addAction(setVRAMStopMarker);
	setContextMenuPolicy(Qt::ActionsContextMenu);
}

void TileView::setStartMarker()
{
	int x,y;
	//bool found=false;
	
	for (y=0; y<rows; y++)
	{
		for (x=0; x<cols; x++)
		{
			if (VRAMgrid[y][x]->hovering)
				break;
		}
		if (VRAMgrid[y][x]->hovering)
			break;
	}
	
	//start_marker.x = x;
	//start_marker.y = y;
	
	Tile *ptr = VRAMgrid[y][x];
	
	//start_marker.circle.setX(ptr->x()+(twidth-2));
	//start_marker.circle.setY(ptr->y()+2);
	start_marker.circle->setRect(ptr->x()+(twidth-2), ptr->y()+2, 2,2);
}

void TileView::setStopMarker()
{
	int x,y;
	debug<<"HAHAHAHAHA";
	for (y=0; y<rows; y++)
	{
		for (x=0; x<cols; x++)
		{
			if (VRAMgrid[y][x]->hovering)
				break;
		}
		if (VRAMgrid[y][x]->hovering)
			break;
	}
	
	//stop_marker.x = x;
	//stop_marker.y = y;
	
	Tile *ptr = VRAMgrid[0][1];
	
	//stop_marker.circle.setX(ptr->x()+(twidth-2));
	//stop_marker.circle.setY(ptr->y()+2);
	stop_marker.circle->setRect(ptr->x()+(twidth-2), ptr->y()+2, 2,2);
}
