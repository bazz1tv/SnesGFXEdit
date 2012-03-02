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


Tile::Tile(QGraphicsItem *parent)
	: QGraphicsPixmapItem(parent)
{
    //debug<<"Item created\n";
	shit=false;
}

QVariant Tile::itemChange(GraphicsItemChange change, const QVariant &value)
{
	shit = false;
	if (change == ItemSelectedHasChanged)
	{
		QPointF pf(pos());
		QPoint p;
		TileView *view = (TileView*)scene()->views().at(0);
		p = scene()->views().at(0)->mapFromScene(pf);
		p = scene()->views().at(0)->mapToGlobal(p);
		p.setX(p.x()+(view->zoom*(view->tileWHLSize/2)));
		p.setY(p.y()+(view->zoom*(view->tileHHLSize/2)));
		m_Cursor.setPos(p);
	}
	if (change == ItemScenePositionHasChanged && scene()) {
		debug<<"Item Position Change\n";
		// checking if more than half of the bounding rect is colliding with another tile
		// if so, then switch
		// value is the new position.
		newPos = value.toPointF();
		QRect thisdimension;
		QRect collide_dimension;
		QPoint collide_center;
		thisdimension.setX(newPos.x());
		thisdimension.setY(newPos.y());
		thisdimension.setWidth(pixmap().width());
		thisdimension.setHeight(pixmap().height());
		
		QList<QGraphicsItem *> list;
		
		list = collidingItems(Qt::IntersectsItemShape);
		// lets try just for 1 collision for now
		Tile *tile;
		int chosen=-1;
		for (int i=0; i < list.count(); i++)
		{
			tile = (Tile*)qgraphicsitem_cast<QGraphicsPixmapItem*>(list.at(i));
			debug<<"testing";
			if (tile->pixmap().width() == twidth)
			{
				chosen = i;
				break;
			}
		}
		
		
		if (chosen != -1)
		{
			debug<<"FOUND ONE\n";
			int x_offset = x();
			int y_offset = y();
			//tile = (Tile*)qgraphicsitem_cast<QGraphicsPixmapItem*>(list.at(chosen));
			debug<<"X: "<<tile->x()<<" Y: "<<tile->y()<<endl;
			collide_center.setX(tile->x()+(tile->pixmap().width()/2));
			collide_center.setY(tile->y()+(tile->pixmap().height()/2));
			while(1)
			{
				for (int y=0; y < pixmap().height(); y++)
				{
					for (int x=0; x < pixmap().width(); x++)
					{
						if (x_offset+x == collide_center.x())
							if (y_offset+y == collide_center.y())
							{
								debug<<"HAPPENING";
								// set a boolean 
								shit=true;
								otherTile = tile;
								// do the rest when the mouse click comes up
								
							}
					}
				}
				break;
			}
		}
		
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
	QGraphicsPixmapItem::mouseReleaseEvent(event);
	if (shit)
	{
		QPixmap buffer;
		buffer = this->pixmap();
		this->setPixmap(otherTile->pixmap());
		otherTile->setPixmap(buffer);
		newPos.setX(gridx);
		newPos.setY(gridy);
		setPos(newPos);
	}
	else {
		setPos(gridx,gridy);
		//setTransform(QTransform(1,0,0,0,1,0,0,0,1));
		update();
	}


	//return newPos;
}

void Tile::mouseMoveEvent ( QGraphicsSceneMouseEvent * event )
{
	debug<<"mouseMove\n";
	QGraphicsPixmapItem::mouseMoveEvent(event);
	//QPointF coords(event->scenePos());
	//setOffset(coords);
	/*QList<Tile *> list;
	
	list = collidingItems(Qt::IntersectsItemShape);
	// lets try just for 1 collision for now
	Tile *tile = list.first();
	QPixmap buffer;
	buffer = this->pixmap();
	setPixmap(tile.pixmap());
	 */
	
}

void Tile::mousePressEvent ( QGraphicsSceneMouseEvent * event )
{
	debug<<"mouse press\n";
	QGraphicsPixmapItem::mousePressEvent(event);
	//QPointF coords(event->scenePos());
	//setOffset(coords);
}


