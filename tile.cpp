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
	selected = true;
	//originalpix = pixmap();
}

QVariant Tile::itemChange(GraphicsItemChange change, const QVariant &value)
{	
	shit = false;
	/*if (change == ItemSelectedHasChanged)
	{
		TileView *view = (TileView*)scene()->views().at(0);
		QPointF pf(pos());
		QPoint p;
		
		p = scene()->views().at(0)->mapFromScene(pf);
		p = scene()->views().at(0)->mapToGlobal(p);
		p.setX(p.x()+(view->zoom*(view->tileWHLSize/2)));
		p.setY(p.y()+(view->zoom*(view->tileHHLSize/2)));
		//m_Cursor.setPos(p);
		//something_happened
	}
	if (change == ItemScenePositionHasChanged && scene()) {
		debug<<"Item Position Change\n";
		// checking if more than half of the bounding rect is colliding with ancolliding tile
		// if so, then switch
		// value is the new position.
		newPos = value.toPointF();
		collide_center_old = collide_center;
		
		QList<QGraphicsItem *> list;
		
		list = collidingItems(Qt::IntersectsItemShape);
		// lets try just for 1 collision for now
		//Tile *tile;
		int chosen=-1;
		for (int i=0; i < list.count(); i++)
		{
			collidingTile = (Tile*)qgraphicsitem_cast<QGraphicsPixmapItem*>(list.at(i));
			debug<<"testing";
			if (collidingTile->pixmap().width() == twidth)
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
			debug<<"X: "<<collidingTile->x()<<" Y: "<<collidingTile->y()<<endl;
			collide_center.setX(collidingTile->x()+(collidingTile->pixmap().width()/2));
			collide_center.setY(collidingTile->y()+(collidingTile->pixmap().height()/2));
			
			/*if (collide_center != collide_center_old)
				colliding_different_tile = 0;
			else {
				colliding_different_tile = 1151;
			}*/

			/*
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
								approvedTile = collidingTile;
								//collidingTile = tile;
								// do the rest when the mouse click comes up
								
								// Snap code
								/*if (colliding_different_tile < 1150)
								{
									TileView *view = (TileView*)scene()->views().at(0);
									// snap cursor to the colliding tile
									QPointF pf(collidingTile->pos());
									QPoint p;
									
									p = view->mapFromScene(pf);
									p = view->mapToGlobal(p);
									p.setX(p.x()+(view->zoom*(view->tileWHLSize/2)));
									p.setY(p.y()+(view->zoom*(view->tileHHLSize/2)));
									//QCursor::setPos(p);
									//m_Cursor.setPos(p);
									//setPos(collidingTile->gridx,collidingTile->gridy);
								}
								else if (colliding_different_tile > 50)
									colliding_different_tile = 1151;*//*
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
	}*/
	return QGraphicsItem::itemChange(change, value);
}

void Tile::mouseReleaseEvent ( QGraphicsSceneMouseEvent * event )
{
	//QGraphicsPixmapItem::mouseReleaseEvent(event);
	
	selected = false;
	ungrabMouse();
	
	if (prev_collided_Tile)
	{
		/*QPixmap buffer;
		buffer = this->pixmap();
		this->setPixmap(collidingTile->pixmap());
		collidingTile->setPixmap(buffer);
		newPos.setX(gridx);
		newPos.setY(gridy);
		setPos(newPos);
		approvedTile = NULL;
		setSelected(false);*/
		originalpix = pixmap();
		prev_collided_Tile->originalpix = prev_collided_Tile->pixmap();
	}
	else {
		//setPixmap(originalpix);
		//setPos(gridx,gridy);
		//setTransform(QTransform(1,0,0,0,1,0,0,0,1));
		//update();
	}


	//return newPos;
}

void Tile::mouseMoveEvent ( QGraphicsSceneMouseEvent * event )
{
	//QGraphicsPixmapItem::mouseMoveEvent(event);
	debug<<"mouseMove\n";
	if (selected)
	{
		int i;
		TileView *view = (TileView*)scene()->views().at(0);
		// algorithm
		// X = (col+1)+(col*TWIDTH), Y = (1+row)+(row*THEIGHT)
		QPointF p(event->scenePos());
		debug<<"oX: "<<p.x()<<" oY: "<<p.y()<<endl;
		Tile *compass[4];
		
		for (i=0; i < 4; i++)
			compass[i] = NULL;
		
		if (row+row_offset > 0)
			compass[0] = view->VRAMgrid[row-1+row_offset][col];
		if (row < (rows+row_offset-1))
			compass[1] = view->VRAMgrid[row+1+row_offset][col];
		if (col+col_offset > 0)
			compass[2] = view->VRAMgrid[row][col-1+col_offset];
		if (col < (cols+col_offset-1))
			compass[3] = view->VRAMgrid[row][col+1+col_offset];
		
		for (i=0; i<4; i++)
		{
			if (compass[i] != NULL)
			{
				//QPointF pf = mapToItem(compass[i], p);
				debug<<"iX: "<<compass[i]->pos().x()<<" iY: "<<compass[i]->pos().y()<<endl;
				
				// time to do the checking
				QPointF cp = compass[i]->pos();
				bool collided=false;
				for (int y=0; y < theight; y++)
				{
					for (int x=0; x < twidth; x++)
					{
						if (collided) break;
						
						debug<<"compare x("<<cp.x()+x<<") with ("<<floor(p.x())<<")\n";
						if ((cp.x()+x) == floor(p.x())) //|| (((cp.x()+x) < p.x()) && ((cp.x()+x+1) > p.x())))
						{
							debug<<"compare y("<<cp.y()+y<<") with ("<<floor(p.y())<<")\n";
							if ((cp.y()+y) == floor(p.y()))
							{
								debug<<"YES!!!\n";
								collided=true;
							}
						}
					}
					if (collided) break;
				}
				if (collided)
				{
					if (prev_collided_Tile && ((prev_collided_Tile->row == compass[i]->row) && (prev_collided_Tile->col == compass[i]->col)))
						break;
					debug<<"compass["<<i<<"] is a match\n";
					if (prev_collided_Tile)
						prev_collided_Tile->setPixmap(pixmap());
				
					row_offset = compass[i]->row-row;
					col_offset = compass[i]->col-col;
					setPixmap(compass[i]->pixmap());
					compass[i]->setPixmap(originalpix);
					view->cursoritem->setPos(compass[i]->gridx,compass[i]->gridy);
					prev_collided_Tile = compass[i];
				}
			}
			//setPos(gridx,gridy);
		}
		
		/*TileView *view = (TileView*)scene()->views().at(0);
		Tile *temp;
		if ((temp = (Tile*)qgraphicsitem_cast<QGraphicsPixmapItem*>(view->itemAt((p)))))
		{
			if (temp->pixmap().width() == twidth && temp->pixmap().height() == theight)
				debug<<"Moved into VRAM["<<temp->row<<"]["<<temp->col<<"]"<<endl;
		}*/
		//setPos(event->pos());
		//view->cursoritem->setPos(event->pos());
	}
	
}

void Tile::mousePressEvent ( QGraphicsSceneMouseEvent * event )
{
	originalpix = pixmap();
	//QGraphicsPixmapItem::mousePressEvent(event);
	debug<<"mouse press\n";
	debug<<"VRAM["<<row<<"]["<<col<<"] Pressed"<<endl;
	selected = true;
	grabMouse();
	prev_collided_Tile = NULL;
	row_offset = col_offset = 0;
	//QPointF coords(event->scenePos());
	//setOffset(coords);
}

void Tile::hoverEnterEvent ( QGraphicsSceneHoverEvent * event )
{
	//QGraphicsPixmapItem::hoverEnterEvent(event);
	TileView *view = (TileView*)scene()->views().at(0);
	view->cursoritem->setPos(gridx,gridy);
}


