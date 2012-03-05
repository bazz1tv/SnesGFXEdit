#ifndef TILEVIEW_H
#define TILEVIEW_H

#include <QGraphicsView>
#include "tile.h"

class TileView : public QGraphicsView
{
    Q_OBJECT

public:
    TileView(QWidget *parent = 0);
	void updateCursor();
	double zoom,oldzoom;
	int tileWHLSize, tileHHLSize;
	QGraphicsPixmapItem *cursoritem;
	Tile *placeritem;
	Tile *selected_tile;
	QPixmap cursorbuf, cursorpix;
	QPixmap placerbuf, placerpix;
	bool firstPaint;
	unsigned int tilemode, tilesize;
	bool preview_original;
	Tile *VRAMgrid[32][16];
	QPixmap blanktile, VRAMpixmap;
	bool tilesize_lock;
	QCursor cursor;
	QPixmap mousepix,mousepixbuf;
	

protected:
	QSize sizeHint() const;
	void paintEvent ( QPaintEvent * event );
    void wheelEvent(QWheelEvent *event);
	void showEvent ( QShowEvent * event );
	void resizeEvent ( QResizeEvent * event );
	void mouseReleaseEvent ( QMouseEvent * event );
	
	
	
};

#endif
