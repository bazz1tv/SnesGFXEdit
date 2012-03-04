/*
 *  tile.cpp
 *  SNESGfxEdit
 *
 *  Created by bazz on 3/1/12.
 *  Copyright 2012 S.Crew. All rights reserved.
 *
 */
#include "globals.h"
#include "tile.h"
#include "tileview.h"
#include <QtGui>
#include <cmath>

#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3

Tile::Tile(QGraphicsItem *parent)
	: QGraphicsPixmapItem(parent)
{
    //debug<<"Item created\n";
	shit=false;
	alreadyentered_tilezone = false;
	collide_center = QPoint(0,0);
	collide_center_old = QPoint(3,3);
	colliding_different_tile = 0;
	approvedTile = NULL;
	prev_collided_Tile = NULL;
	view = NULL;
	selected = false;
	//preview_original = false;
	//originalpix = pixmap();
}

QVariant Tile::itemChange(GraphicsItemChange change, const QVariant &value)
{	
	QPointF newPos;
	if (change == ItemPositionChange)
	{
		newPos = value.toPointF();
		// how to match up bounding rects
		// do it only if the edge of this tile is in the center of colliding tile
		// take a QPoint for the center of colliding tile
		// see if any point in this tile touches center of colliding tile
		
		
		QRectF rect = scene()->sceneRect();
		if (!rect.contains(newPos)) {
			// Keep the item inside the scene rect.
			newPos.setX(qMin(rect.right(), qMax(newPos.x(), rect.left())));
			newPos.setY(qMin(rect.bottom(), qMax(newPos.y(), rect.top())));
			return newPos;
		}
	}
	return QGraphicsItem::itemChange(change, value);
}

void Tile::mouseReleaseEvent ( QGraphicsSceneMouseEvent * event )
{
	//TileView *view = (TileView*)scene()->views().at(0);
	selected = !selected;
	
	if (selected)
	{
		originalpix = pixmap();
		//QGraphicsPixmapItem::mousePressEvent(event);
		debug<<"mouse press\n";
		debug<<"VRAM["<<row<<"]["<<col<<"] Pressed"<<endl;
		//selected = true;
		grabMouse();
		prev_collided_Tile = NULL;
		//row_offset = col_offset = 0;
		view->placeritem->setVisible(true);
		view->cursoritem->setVisible(false);
		view->placeritem->setPos(gridx,gridy);
		//QPointF coords(event->scenePos());
		//setOffset(coords);
		//QGraphicsPixmapItem::mouseReleaseEvent(event);
	}
	else 
	{
		ungrabMouse();
		//setVisible(true);
		view->cursoritem->setVisible(true);
		view->placeritem->setVisible(false);
		view->cursoritem->setPos(view->placeritem->pos());
	
		if (prev_collided_Tile)
		{
			originalpix = pixmap();
			prev_collided_Tile->originalpix = prev_collided_Tile->pixmap();
		}
	}


	//return newPos;
}

void Tile::mouseMoveEvent ( QGraphicsSceneMouseEvent * event )
{
	//QGraphicsPixmapItem::mouseMoveEvent(event);
	debug<<"mouseMove\n";
	if (selected)
	{
		debug<<"Selected: Row="<<row<<", col="<<col<<endl;
		// algorithm
		// X = (col+1)+(col*TWIDTH), Y = (1+row)+(row*THEIGHT)
		QPointF p(event->scenePos());
		debug<<"oX: "<<p.x()<<" oY: "<<p.y()<<endl;

		//Tile *compass[4];
		Tile *adjacent;
		
		//for (i=0; i < 4; i++)
			//compass[i] = NULL;
		
		//new collision code
		
		//QGraphicsItem *tmp = 
		adjacent = (Tile*)qgraphicsitem_cast<QGraphicsPixmapItem*>(view->scene()->itemAt(floor(p.x()),floor(p.y())));
		
		
		if ((adjacent->pixmap().width() == twidth) && (adjacent->pixmap().height() == theight) && (adjacent->gridx != -1))
		{
			debug<<"Row : "<<adjacent->row<<" Col: "<<adjacent->col<<endl;
			if (prev_collided_Tile && ((prev_collided_Tile->row == adjacent->row) && (prev_collided_Tile->col == adjacent->col)))
				return;
			//debug<<"compass["<<i<<"] is a match\n";
			if (prev_collided_Tile)
			{
				prev_collided_Tile->setPixmap(prev_collided_Tile->originalpix);
			}
		
			if (view->preview_original == false)
			{
				setPixmap(view->blanktile);
			}
			else
			{
				setPixmap(adjacent->pixmap());
			}
			
			adjacent->setPixmap(originalpix);
			//compass[i]->setPixmap(originalpix);
			view->placeritem->setPos(adjacent->gridx,adjacent->gridy);
			prev_collided_Tile = adjacent;
		}
	}
	
}

void Tile::mousePressEvent ( QGraphicsSceneMouseEvent * event )
{
	/*TileView *view = (TileView*)scene()->views().at(0);
	originalpix = pixmap();
	//QGraphicsPixmapItem::mousePressEvent(event);
	debug<<"mouse press\n";
	debug<<"VRAM["<<row<<"]["<<col<<"] Pressed"<<endl;
	//selected = true;
	grabMouse();
	prev_collided_Tile = NULL;
	row_offset = col_offset = 0;
	view->placeritem->setVisible(true);
	view->cursoritem->setVisible(false);
	view->placeritem->setPos(gridx,gridy);
	//QPointF coords(event->scenePos());
	//setOffset(coords);*/
}

void Tile::hoverEnterEvent ( QGraphicsSceneHoverEvent * event )
{
	//QGraphicsPixmapItem::hoverEnterEvent(event);
	if (!view)
		view = (TileView*)scene()->views().at(0);
	view->cursoritem->setPos(gridx,gridy);
}


