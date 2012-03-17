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
	ref = this;
	selected = false;
	
	topleft16x16 = NULL;
	
	sig = 0xfafa;
	zone16x16 = true;
	zone32x32 = true;
	
	hovering = false;
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

/*void Tile::doMouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	mouseReleaseEvent(event);
}*/

void Tile::mouseReleaseEvent ( QGraphicsSceneMouseEvent * event )
{
	
	//TileView *view = (TileView*)scene()->views().at(0);
	selected = !selected;
	
	if (selected)
	{
		ref = this;
		// save original pixmap
		if (view->tilemode == MODE_SWAP)
		{
			if (view->tilesize == SIZE_16x16)
			{
				ref = ref->topleft16x16;
			}
			else if (view->tilesize == SIZE_32x32)
			{
				ref = ref->topleft32x32;
			}
			else if (view->tilesize == SIZE_64x64)
			{
				ref = ref->topleft64x64;
			}
		}
		
		ref->originalpix = ref->pixmap();
		movebywidth = (view->tileWHLSize/8)-1;
		movebyheight = (view->tileHHLSize/8)-1;
		
		// selected tile
		QPixmap selection(view->tileWHLSize+((view->tileWHLSize/8)-1), view->tileHHLSize+((view->tileWHLSize/8)-1));
		selection.fill(Qt::transparent);
		QPainter painter(&selection);
		QRect rect(0,0,view->tileWHLSize+((view->tileWHLSize/8)-1), view->tileHHLSize+((view->tileWHLSize/8)-1));
		QRect viewport(ref->gridx,ref->gridy,view->tileWHLSize+((view->tileWHLSize/8)-1), view->tileHHLSize+((view->tileWHLSize/8)-1));
		view->scene()->render(&painter, rect, viewport);
		
		view->selected_tile->setPixmap(selection);
		view->selected_tile->setPos(ref->gridx,ref->gridy);
		view->selected_tile->setVisible(true);
		//QGraphicsPixmapItem::mousePressEvent(event);
		debug<<"mouse press\n";
		debug<<"VRAM["<<row<<"]["<<col<<"] Pressed"<<endl;
		//selected = true;
		grabMouse();
		prev_collided_Tile = NULL;
		//row_offset = col_offset = 0;
		view->placeritem->setVisible(true);
		view->cursoritem->setVisible(false);
		view->placeritem->setPos(ref->gridx,ref->gridy);
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
		
		view->selected_tile->setVisible(false);
	
		if (prev_collided_Tile)
		{
			for (int y=0; y <= movebyheight; y++)
			{
				for (int x=0; x <= movebywidth; x++)
				{
					QImage buf;
					
					Tile *original_tile = view->VRAMgrid[ref->row+y][ref->col+x];
					Tile *newtile = view->VRAMgrid[prev_collided_Tile->row+y][prev_collided_Tile->col+x];
					
					// swap QImage copy' tiles
					buf = view->VRAMgrid_img[ref->row+y][ref->col+x];
					view->VRAMgrid_img[ref->row+y][ref->col+x] = view->VRAMgrid_img[prev_collided_Tile->row+y][prev_collided_Tile->col+x];
					view->VRAMgrid_img[prev_collided_Tile->row+y][prev_collided_Tile->col+x] = buf;
					
					original_tile->setPixmap(newtile->pixmap());
					
					newtile->setPixmap(original_tile->originalpix);
					newtile->originalpix = original_tile->originalpix;
					
					//newtile->setPixmap(original_tile->originalpix);
					//newtile->originalpix = original_tile->originalpix;
					
					original_tile->originalpix = original_tile->pixmap();
				}
			}
			//setPixmap(prev_collided_Tile->pixmap());
			//prev_collided_Tile->setPixmap(originalpix);
			//prev_collided_Tile->originalpix = originalpix;
			
			//originalpix = pixmap();
			//prev_collided_Tile->originalpix = prev_collided_Tile->pixmap();
		}
	}


	//return newPos;
}

void Tile::mouseMoveEvent ( QGraphicsSceneMouseEvent * event )
{
	//QGraphicsPixmapItem::mouseMoveEvent(event);
	debug<<"mouseMove\n";
	/*QPoint mousepos(event->scenePos().x(),event->scenePos().y());
	debug<<"old mouse pos: "<<event->scenePos().x()<<","<<event->scenePos().y()<<endl;
	if ((int)mousepos.x() % 10 == 9)
		mousepos.setX(mousepos.x()+1);
	if ((int)mousepos.y() % 10 == 9)
		mousepos.setY(mousepos.y()+1);
	QCursor::setPos(view->mapToGlobal(mousepos));
	mousepos = view->mapToGlobal(mousepos);
	debug<<"new mouse pos: "<<mousepos.x()<<","<<mousepos.y()<<endl;*/
	
	if (selected)
	{
		debug<<"Selected: Row="<<row<<", col="<<col<<endl;
		// algorithm
		// X = (col+1)+(col*TWIDTH), Y = (1+row)+(row*THEIGHT)
		QPointF p(event->scenePos());
		debug<<"oX: "<<p.x()<<" oY: "<<p.y()<<endl;
		Tile *tile_undercursor = (Tile*)qgraphicsitem_cast<QGraphicsPixmapItem*>(view->scene()->itemAt(floor(p.x()),floor(p.y())));
		
		/*if (tile_undercursor->gridx == -1) // if we detect the cursor image on top
		{
			// get tile underneath
			tile_undercursor = view->VRAMgrid[(int)(floor(p.y())-1)/(theight+1)][(int)(floor(p.x())-1)/(twidth+1)];
			
		}*/
		
		tile_undercursor = view->VRAMgrid[(int)(floor(p.y())-1)/(theight+1)][(int)(floor(p.x())-1)/(twidth+1)];

		
		//if (view->tilesize == SIZE_16x16 && (tile_undercursor->topleft16x16->row == row && tile_undercursor->topleft16x16->col == col))
			//return;
		
		if (view->tilesize == SIZE_16x16)
			tile_undercursor = tile_undercursor->topleft16x16;
		else if (view->tilesize == SIZE_32x32)
			tile_undercursor = tile_undercursor->topleft32x32;
		else if (view->tilesize == SIZE_64x64)
			tile_undercursor = tile_undercursor->topleft64x64;
		
		debug<<"undercursor: topleft16x16: "<<tile_undercursor->row<<","<<tile_undercursor->col<<endl;
		
		/*if (!((abs((tile_undercursor->col-col))%(2) == 0) && (abs((tile_undercursor->row-row))%(2) == 0)))
			return;*/
		// mode specific
		/*int col_diff16x16 = ((tile_undercursor->col-col));
		int row_diff16x16 = ((tile_undercursor->row-row));
		
		if (view->tileHHLSize == 16 && view->tileWHLSize == 16)
		{
			if (!(col_diff16x16%2 == 0 && row_diff16x16%2 == 0) &&
				!(col_diff16x16%2 == -1) && !(col_diff16x16%3 == -2))
				return;
		}*/
		
		// check tile zone
		
		if (view->tilemode == MODE_SWAP)
		{
			if ((tile_undercursor->pixmap().width() == twidth) && (tile_undercursor->pixmap().height() == theight))
			{
				if (view->tilesize == SIZE_8x8 && prev_collided_Tile && ((prev_collided_Tile->row == tile_undercursor->row) && (prev_collided_Tile->col == tile_undercursor->col)))
					return;
				
				// preview
				for (int y=0; y <= movebyheight; y++)
				{
					for (int x=0; x <= movebywidth; x++)
					{
						/*if (x == 0 && y == 0)
						{
							continue;
						}*/
						Tile *tile = view->VRAMgrid[tile_undercursor->row+y][tile_undercursor->col+x];
						view->VRAMgrid[ref->row+y][ref->col+x]->setPixmap(tile->pixmap());
					}
				}
				
				int x;  
				int y;
				
				
				x = tile_undercursor->gridx, 
				y=tile_undercursor->gridy;
				
				view->selected_tile->setPos(x,y);
				view->placeritem->setPos(x,y);
				
				prev_collided_Tile = tile_undercursor;
			}
		}
	}
	
}

/*void Tile::Swap(Tile *one, Tile *two)
{
	one->setPixmap(two->pixmap());
	
	int x=two->gridx, y=two->gridy;
	view->selected_tile->setPos(x,y);
	view->placeritem->setPos(x,y);
}*/

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
	//hovering = true;
}

void Tile::hoverEnterEvent ( QGraphicsSceneHoverEvent * event )
{
	//QGraphicsPixmapItem::hoverEnterEvent(event);
	//hovering = true;
	
	if (!view)
		view = (TileView*)scene()->views().at(0);
	if (view->tilesize == SIZE_16x16)
		view->cursoritem->setPos(topleft16x16->gridx,topleft16x16->gridy);
	else if(view->tilesize == SIZE_32x32)
		view->cursoritem->setPos(topleft32x32->gridx, topleft32x32->gridy);
	else if(view->tilesize == SIZE_64x64)
		view->cursoritem->setPos(topleft64x64->gridx, topleft64x64->gridy);
	else
		view->cursoritem->setPos(gridx,gridy);
}

void Tile::hoverLeaveEvent ( QGraphicsSceneHoverEvent * event )
{
	//hovering = false;
}
