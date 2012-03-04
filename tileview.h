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
	QPixmap cursorbuf, cursorbuf2,cursorpix;
	QPixmap placerbuf, placerbuf2,placerpix;
	bool firstPaint, swap;
	bool preview_original;
	Tile *VRAMgrid[32][16];
	QPixmap blanktile;
	

protected:
	QSize sizeHint() const;
	void paintEvent ( QPaintEvent * event );
    void wheelEvent(QWheelEvent *event);
	void showEvent ( QShowEvent * event );
	void resizeEvent ( QResizeEvent * event );
	
};

#endif
