#ifndef TILEVIEW_H
#define TILEVIEW_H

#include <QGraphicsView>

class TileView : public QGraphicsView
{
    Q_OBJECT

public:
    TileView(QWidget *parent = 0);
	double zoom,oldzoom;

protected:
    void wheelEvent(QWheelEvent *event);
	void resizeEvent ( QResizeEvent * event );
};

#endif
