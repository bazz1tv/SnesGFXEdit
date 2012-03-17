#ifndef EDITOR_H
#define EDITOR_H

#define TWIDTH 8
#define THEIGHT 8
//#define ROWS 32
//#define COLS 16

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QMainWindow>

class TileView;
class Tile;

class Editor : public QMainWindow
{
    Q_OBJECT // what does this mean?

public:
    Editor(QWidget *parent = 0);
	~Editor();
	void resizeEvent ( QResizeEvent * event );
	void	render ( QPaintDevice * target, const QPoint & targetOffset = QPoint(), const QRegion & sourceRegion = QRegion(), RenderFlags renderFlags = RenderFlags( DrawWindowBackground | DrawChildren ) );
	//void paintEvent ( QPaintEvent * event );
	void setZoom(int factor);
	//void updateCursor();
	
	/// should probs be private, but whatev
	QGraphicsScene *scene;
	TileView *view;
	int rows;
	int cols;
	double zoom,oldzoom;
	QPixmap pixmap, gridpixmap;
	QImage *VRAM, *gridimg;
	QGraphicsPixmapItem *gridPI;
	
	//QVector<QRgb> colortable;
	//Tile *VRAMgrid[32][16];

    void newFile();
    bool save();
    bool saveAs();
    QSize sizeHint() const;
    QAction *windowMenuAction() const { return action; }

    static Editor *open(QWidget *parent = 0);
    static Editor *openFile(const QString &fileName,
                            QWidget *parent = 0);

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void documentWasModified();
	void mouseMove(QGraphicsSceneMouseEvent * event);
	//void initCursorPos();

private:
    bool okToContinue();
    bool saveFile(const QString &fileName);
    void setCurrentFile(const QString &fileName);
    bool readFile(const QString &fileName);
	bool readColors(const QString &fileName);
	bool readTiles(const QString &fileName, int rows, int cols);
    bool writeFile(const QString &fileName);
    QString strippedName(const QString &fullFileName);

	//QPixmap *cursorBuf;
	//Tile *cursor;
	//QCursor m_Cursor;
	
    QString curFile;
    bool isUntitled;
    QAction *action;
};

#endif
