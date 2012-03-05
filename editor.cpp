#include <QtGui>

#include "editor.h"
#include "globals.h"
#include "tileview.h"
#include "tile.h"


Editor::Editor(QWidget *parent) : QMainWindow(parent)
{
	debug<<"Editor created\n";
	//layout()->setSizeConstraint(QLayout::SetFixedSize);
	gridimg = NULL;
	view = new TileView(this);
	//setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	
	//cursorBuf = new QPixmap;
	//m_LPixmap = new QPixmap(view->zoom*(view->tileWHLSize),view->zoom*(view->tileHHLSize));
	/*m_LPixmap->fill(Qt::transparent); // Otherwise you get a black background :(
	QPainter painter(m_LPixmap);
	QColor red(255,0,0,128);
	
	painter.setPen(Qt::NoPen);        // Otherwise you get an thin black border
	painter.setBrush(red);
	
	painter.drawRect(0,0,view->zoom*(view->tileWHLSize),view->zoom*(view->tileHHLSize));
	m_Cursor = QCursor(*m_LPixmap);*/
	//setCursor(m_Cursor);
	
	VRAM = NULL;
	rows = 32;
	cols = 16;
	
	gridPI = NULL;
	//cursor = new Tile;
	
	
	scene = new QGraphicsScene(this);
	//scene->setSceneRect(QRectF(0,0,(TWIDTH*cols+1)+1, (THEIGHT*rows+1)+1));
	QRadialGradient gradient(50,0,90);
	gradient.setSpread(QGradient::ReflectSpread);
	gradient.setColorAt(0, QColor(143,31,43));
	gradient.setColorAt(0.5, QColor(0,40,80));
	gradient.setColorAt(1, QColor(100,31,143));
	scene->setBackgroundBrush(gradient);
	//scene->setSceneRect(0,0,(9*16)+1);
	//scene->setSceneRect(QRectF(0,0,(9*16)+1,(9*32)+1));
	
	
	
	view->setAlignment(Qt::AlignLeft | Qt::AlignTop);
	//view->centerOn(0,0);
	view->setResizeAnchor(QGraphicsView::NoAnchor);
	view->setTransformationAnchor(QGraphicsView::NoAnchor);
	view->setScene(scene);
	view->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
	//view->setMinimumSize(QSize(400,400));
	view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	//view->resize(parent->geometry().width(), parent->geometry().height());
	//view->scale(view->zoom,view->zoom);
	setCentralWidget(view);
	setZoom(2);
	
    action = new QAction(this);
    action->setCheckable(true);
	QObject::connect(action, SIGNAL(triggered()), this, SLOT(show()));
	QObject::connect(action, SIGNAL(triggered()), this, SLOT(setFocus()));

	//connect(view, SIGNAL(paintEvent(QPaintEvent*)()), this, SLOT(initCursorPos()));
    isUntitled = true;

    /*connect(document(), SIGNAL(contentsChanged()),
            this, SLOT(documentWasModified()));*/

    //setWindowIcon(QPixmap(":/images/document.png"));
    setWindowTitle("[*]");
    setAttribute(Qt::WA_DeleteOnClose);
	
	//connect(scene, SIGNAL(mouseMoveEvent(QGraphicsSceneMouseEvent*)), this, SLOT(mouseMove(QGraphicsSceneMouseEvent*)));
}

void Editor::mouseMove(QGraphicsSceneMouseEvent *event)
{
	debug<<"hello??";
	//QPointF pos(event->pos());
	//int x,y;
	
	// tile
	//x =  pos.x()/(9*view->zoom);
	//y = pos.y()/(9*view->zoom);
	
	//cursor->setPos(VRAMgrid[x][y]->gridx,VRAMgrid[x][y]->gridy);
	
}

Editor::~Editor()
{
	//delete debugstream;
	//delete debugfile;
	if (gridPI !=NULL)
		delete gridPI;
	if (VRAM != NULL)
		delete VRAM;
	/*for (int row = 0; row<rows;row++)
	{
		for (int col=0; col<cols; col++)
		{
			// vars??
			delete VRAMgrid[row][col];
		}
	}*/
	//delete m_LPixmap;
	//delete cursorBuf;
	
	if (gridimg != NULL)
		delete gridimg;
}

void Editor::render ( QPaintDevice * target, const QPoint & targetOffset, const QRegion & sourceRegion, RenderFlags renderFlags)
{
	//resize(view->zoom*(cols*(TWIDTH+1)), view->zoom*(rows*(THEIGHT+1)));
	QMainWindow::render(target, targetOffset, sourceRegion, renderFlags);
	
	//scene->update(scene->sceneRect());
}

void Editor::setZoom(int factor)
{
	QTransform scale;
	//QWidget *viewport;
	view->oldzoom = view->zoom; //old
	view->zoom = (double)factor;
	
	scale.scale(factor,factor);
	view->setTransform(scale);
	
}

/*void Editor::paintEvent ( QPaintEvent * event )
{
	//view->scale(width(),height());
	//scene->setSceneRect(geometry());//QRectF(0, 0, width(), height()));
	//scene->update();
	QDialog::paintEvent(event);
	
	
}*/

void Editor::resizeEvent ( QResizeEvent * event )
{
	//updateCursor();
	QMainWindow::resizeEvent(event);
	//resize(view->zoom*cols*(twidth+1)+1,view->zoom*rows*(theight+1)+1);
	//QResizeEvent *test;
	//view->centerOn(0,0);
	
	//scene->setSceneRect(0,0,event->size().width(),event->size().height() );
	//view->setSceneRect(0,0,event->size().width(), event->size().height());
	//view->fitInView(0,0,cols*(TWIDTH+1)+1, rows*(THEIGHT+1)+1, Qt::KeepAspectRatioByExpanding);
}


void Editor::newFile()
{
    static int documentNumber = 1;

    curFile = tr("document%1.txt").arg(documentNumber);
    setWindowTitle(curFile + "[*]");
    action->setText(curFile);
    isUntitled = true;
    ++documentNumber;
}

bool Editor::save()
{
    if (isUntitled) {
        return saveAs();
    } else {
        return saveFile(curFile);
    }
}

bool Editor::saveAs()
{
    QString fileName =
            QFileDialog::getSaveFileName(this, tr("Save As"), curFile);
    if (fileName.isEmpty())
        return false;

    return saveFile(fileName);
}

QSize Editor::sizeHint() const
{
    if (VRAM)
	{
		//QSize size(view->zoom*cols*(twidth+1)+1,view->zoom*rows*(theight+1)+1);
    //if (view->zoom >= 3)
        //size += QSize(1, 1);
		return QSize(view->zoom*cols*(twidth+1)+1,view->zoom*rows*(theight+1)+1);
	}
	else return QMainWindow::sizeHint();
}

Editor *Editor::open(QWidget *parent)
{
    QString fileName =
            QFileDialog::getOpenFileName(parent, tr("Open SNES GFX"), ".");
    if (fileName.isEmpty())
        return 0;

    return openFile(fileName, parent);
}

Editor *Editor::openFile(const QString &fileName, QWidget *parent)
{
    Editor *editor = new Editor(parent);
    if (editor->readFile(fileName)) {
        editor->setCurrentFile(fileName);
		//parent->setWindowTitle("BLAH");
		//editor->setWindowTitle("LBA");
        return editor;
    } else {
        delete editor;
        return 0;
    }
}

void Editor::closeEvent(QCloseEvent *event)
{
    if (okToContinue()) {
        event->accept();
    } else {
        event->ignore();
    }
}

void Editor::documentWasModified()
{
    setWindowModified(true);
}

bool Editor::okToContinue()
{
    if (view->isWindowModified()) {
        int r = QMessageBox::warning(this, tr("MDI Editor"),
                        tr("File %1 has been modified.\n"
                           "Do you want to save your changes?")
                        .arg(strippedName(curFile)),
                        QMessageBox::Yes | QMessageBox::No
                        | QMessageBox::Cancel);
        if (r == QMessageBox::Yes) {
            return save();
        } else if (r == QMessageBox::Cancel) {
            return false;
        }
    }
    return true;
}

bool Editor::saveFile(const QString &fileName)
{
    if (writeFile(fileName)) {
        setCurrentFile(fileName);
        return true;
    } else {
        return false;
    }
}

void Editor::setCurrentFile(const QString &fileName)
{
    curFile = fileName;
    isUntitled = false;
    action->setText(strippedName(curFile));
    //document()->setModified(false);
    this->setWindowTitle(strippedName(curFile) + "[*]");
    setWindowModified(false);
}

// Rewriting to just load linear 8x8 tiles with no grid
// we will later make a grid and displace the tiles
bool Editor::readTiles(const QString &fileName, int rows=32, int cols=16)
{
	quint8 tile[32];
	quint8 t1,t2,t3,t4;
	
	QFile file(fileName);
	file.open(QIODevice::ReadOnly);
    QDataStream input(&file);
	input.setByteOrder(QDataStream::LittleEndian);
	
	::rows = rows;
	::cols = cols;
	
	// Code to interpret the tile's index
	// for 8 rows. we need to take each bit from 4 bytes.
	// same position.. and shift them into their correct position..
	// then we set the pixel for every 4 bits.
	VRAM = new QImage((TWIDTH*cols),(THEIGHT*rows), QImage::Format_Indexed8);
	VRAM->setColorCount(129);
	
	// there are 32x16 = 512 tiles
	for (int row=0; row < rows; row++)
	{
		for (int col=0; col < cols; col++)
		{
			// for 1 tile
			
			for (int i=0; i<32; i++) // 32 bytes to a tile
			{
				if (input.status() == QDataStream::ReadPastEnd)
				{
					tile[i] = 0;
				}
				else input>>tile[i];
			}
			
			for (int i=0; i < 16; i+=2)
			{
				// if top || bottom fill line with grid color (16)
				/*if (i==0)
				{
					for (int p=0; p<THEIGHT; p++)
					{
						VRAM->setPixel(((col*TWIDTH)+p),((row)*THEIGHT),128);
						//VRAM.setPixel(((col*TWIDTH)+p),(row*THEIGHT)+(THEIGHT-1),16);
						
						VRAM->setPixel(col*TWIDTH,(row*THEIGHT)+p,128);
						//VRAM.setPixel((col*TWIDTH)+(TWIDTH-1),(row*THEIGHT)+p,16);
					}
					
				}*/
				// else fill first pixel and last pixel of row
				quint8 final;
				t1 = tile[i];
				t2 = tile[i+1];
				t3 = tile[i+16];
				t4 = tile[i+17];
				
				// first bit to last bit, 8 times
				for (int n=0; n < 8; n++)
				{
					t1 = (tile[i]>>n) & 0x01;
					t2 = (tile[i+1]>>n) & 0x01;
					t3 = (tile[i+16]>>n) & 0x01;
					t4 = (tile[i+17]>>n) & 0x01;
					
					final = (t4<<3) | (t3<<2) | (t2<<1) | t1;
					//debugstream<<"final pixel index for x="<<7-n<<": "<<final<<" ";
					VRAM->setPixel((((col*TWIDTH))+((TWIDTH-1)-n)),(((row*THEIGHT))+(i/2)),final);
					// prolly right, maybe not
				}
			}
		}
	}
	
	// do more grid work
	/*for (int i=0; i <= rows*THEIGHT; i++)
	{
		VRAM->setPixel(TWIDTH*16, i,128);
	}
	// ..
	for (int i=0; i <= cols*TWIDTH; i++)
	{
		VRAM->setPixel(i, THEIGHT*32, 128);
	}*/
	
	return true;
}

// Read Raw File 4bpp SNES
bool Editor::readFile(const QString &fileName)
{
	gridimg = new QImage(cols*(TWIDTH+1)+1, rows*(THEIGHT+1)+1, QImage::Format_RGB32);
	QRgb value = qRgb(0,0,0);
	for (int row=0; row <= rows; row++)
	{
		for (int x=0; x<cols*TWIDTH; x++)
			gridimg->setPixel(x, (row*THEIGHT)+(row),value);
	}
	for (int col=0; col <= cols; col++)
	{
		for (int y=0; y<rows*THEIGHT; y++)
			gridimg->setPixel((col*TWIDTH)+(col),y,value);
	}
		
	gridpixmap = QPixmap::fromImage(*gridimg, Qt::ColorOnly);	// there are options available
	gridPI = new QGraphicsPixmapItem;
	//QPainter painter(&gridpixmap);
	//scene->setBackgroundBrush(painter);
	gridPI = scene->addPixmap(gridpixmap);
	gridPI->setPos(0,0);
	
	
	if (!readTiles(fileName))
		return false;
	if (!readColors(fileName))
		return false;
	
	// Make Grid.. should probably make a function for this
	/*QPen pen(Qt::black);
	pen.setWidth(1);
	//pen.setCosmetic(false);
	pen.setStyle(Qt::SolidLine);*/
	
	/*for (int row=0; row <= rows*(THEIGHT+1); row+=(THEIGHT+1))
	{
		scene->addLine(0, row+1, cols*(TWIDTH+1), row, pen);
	}
	for (int col=0; col <= cols*(TWIDTH+1); col+=(TWIDTH+1))
	{
		scene->addLine(col, 0, col, rows*(THEIGHT+1), pen);
	}*/
	
	return true;
}

bool Editor::readColors(const QString &fileName)
{
	quint8 r,g,b,temp;
	quint8 highbits;
	QRgb result;
	bool nopalette;
	
	
	//VRAM->load(fileName, "pic");
	//*debugstream<<"Format: "<<VRAM->format()<<endl;
	//VRAM->setColorCount(16);
	
	QString clrfile(fileName);
	clrfile.chop(3);
	clrfile+="clr";
	/*fileName = QFileDialog::getOpenFileName(this,
	 tr("Open SNES gfx"), ".",
	 tr("gfx files (*.clr)"));*/
	QFile file(clrfile);
	
	if(QFile::exists(clrfile))
	{
		QMessageBox msgBox;
		msgBox.setText("Open .clr file?");
		//msgBox.setInformativeText("Do you want to save your changes?");
		msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
		msgBox.setDefaultButton(QMessageBox::Yes);
		int ret = msgBox.exec();
		
		switch (ret) 
		{
			case QMessageBox::Yes:
				// Save was clicked
				nopalette=false;
				break;
			case QMessageBox::No:
			{
				nopalette = true;
				break;
			}
		}
	}
	
	// Do a gray-scale palette if no color file is found
	if (!file.open(QIODevice::ReadOnly) || nopalette)
	{
		int c=255;
		for(int i=1; i<16; i++)
		{
			VRAM->setColor(i,qRgb(c,c,c));
			c-=25;
		}
	}
	else
	{
		QDataStream in(&file);
		in.setByteOrder(QDataStream::LittleEndian);
		for (int i=0; i < 16; i++)
		{
			in>>r; // need to make sure Intel reads low first then high
			temp = r;
			r = r<<3;
			highbits = (r & 0xe0) >> 5;
			r |= highbits;
			
			g = temp>>5;
			g = g & 0x1f;
			in>>b;
			temp = b;
			temp &= 3;	// %11
			temp = temp << 3;
			g |= temp;
			g = g << 3;
			
			highbits = (g & 0xe0) >> 5;
			g |= highbits;
			
			b &= 0x7c; // %01111100
			b = b<<1;
			highbits = (b & 0xe0) >> 5;
			b |= highbits;
			
			result = qRgb(r,g,b); //0xff000000 | (quint32)(r<<16) | (quint32)(g<<8) | (quint32)b;
			//*debugstream<<"Color "<<i<<": "<<result<<endl;
			VRAM->setColor(i, result);
		}
	}
	
	if (nopalette)
	{
		VRAM->setColor(128, qRgb(0x00,0x00,0x80));
		VRAM->setColor(0,qRgb(0xff,0xff,0xff));
	}
	else
		VRAM->setColor(128, qRgb(0,0,0)); // grid color
	
	// do BlankTile
	QImage blank(twidth,theight, QImage::Format_Indexed8);
	blank.setColorCount(1);
	blank.setColor(0, VRAM->color(0));
	for (int y=0; y<theight; y++)
	{
		for (int x=0; x<twidth; x++)
		{
			blank.setPixel(x,y, 0);
		}
	}
	
	view->blanktile = QPixmap::fromImage(blank, Qt::ColorOnly);
	
	
	
	pixmap = QPixmap::fromImage(*VRAM, Qt::ColorOnly);	// there are options available
	view->VRAMpixmap = pixmap;
	//gscene->addPixmap(pixmap);
	//setCentralWidget(gview); // change to graphics view
	//gview->show();
	//grid->addWidget((QWidget*)VRAM, 0,0);
	
	
	// need to fill up vram8x8
	// width and height stay 8x8
	// but must move 16 across and 32 down (tiles)
	// QRect(x,y,width,height)
	bool flip = true;
	quint8 twocount=0;
	Tile *ptr = view->VRAMgrid[0][0];
	
	for (int row = 0; row<rows;row++)
	{
		for (int col=0; col<cols; col++)
		{
			// vars?? 
			//VRAMgrid[row][col] = new QGraphicsPixmapItem; 
			// do the 16x16 top left algorithm
			if (row%2 == 0)
			{
				if (col%2 == 0)
				{
					ptr = view->VRAMgrid[row][col];
					debug<<"ptr = ["<<row<<"]["<<col<<"]\n";
					//twocount = 0;
				}
				view->VRAMgrid[row][col]->topleft16x16 = ptr;
				view->VRAMgrid[row+1][col]->topleft16x16 = ptr;
				debug<<"["<<row<<"], ["<<col<<"]\n";
			}
			//view->VRAMgrid[row][col]->zone16x16 = flip;
			
			view->VRAMgrid[row][col]->setPixmap(pixmap.copy(TWIDTH*col,THEIGHT*row,TWIDTH,THEIGHT));
			view->VRAMgrid[row][col]->originalpix = view->VRAMgrid[row][col]->pixmap();
			//VRAMgrid[row][col] = (Tile*)scene->addPixmap(pixmap.copy(TWIDTH*col,THEIGHT*row,TWIDTH,THEIGHT));
			//gscene->addPixmap(VRAM8x8[row][col]);
			view->VRAMgrid[row][col]->setPos((col+1)+(col*TWIDTH),(1+row)+(row*THEIGHT)); // +1 for the grid
			view->VRAMgrid[row][col]->gridx = (col+1)+(col*TWIDTH);
			view->VRAMgrid[row][col]->gridy = (1+row)+(row*THEIGHT);
			view->VRAMgrid[row][col]->row = row;
			view->VRAMgrid[row][col]->col = col;
			view->VRAMgrid[row][col]->setFlags(QGraphicsItem::ItemIsFocusable /*| QGraphicsItem::ItemIsSelectable*/
										 /*| QGraphicsItem::ItemIsMovable*/
										 |QGraphicsItem::ItemSendsScenePositionChanges);
			view->VRAMgrid[row][col]->setAcceptHoverEvents(true);
			scene->addItem(view->VRAMgrid[row][col]);
			
			//twocount++;
		}
	}
	
	
	debug<<"itemX: "<<view->VRAMgrid[0][0]->x()<<" itemY: "<<view->VRAMgrid[0][0]->y()<<endl;
	debug<<"itemX: "<<view->VRAMgrid[1][1]->x()<<" itemY: "<<view->VRAMgrid[1][1]->y()<<endl;
	
	view->selected_tile->gridx = -1;
	scene->addItem(view->selected_tile);
	scene->addItem(view->cursoritem);
	view->placeritem->gridx = -1;
	scene->addItem(view->placeritem);
	view->selected_tile->setVisible(false);
	
	
	/*VRAM8x8[0][0] = pixmap.copy(0,0,8,8);
	 gscene->addPixmap(VRAM8x8[0][0]);
	 VRAM8x8[0][1] = pixmap.copy(8,0,8,8);
	 gscene->addPixmap(VRAM8x8[0][1]);
	 */
	//setCentralWidget(view);
	//gview->centerOn(0,0);
	
	//resize(((TWIDTH*cols)+1)*2, ((THEIGHT*rows)+1)*2);
	//view->setWindowTitle("BLAH");
	//view->show();
	//adjustSize();
	return true;
}

bool Editor::writeFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, tr("MDI Editor"),
                             tr("Cannot write file %1:\n%2.")
                             .arg(file.fileName())
                             .arg(file.errorString()));
        return false;
    }

    QTextStream out(&file);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    //out << toPlainText();
    QApplication::restoreOverrideCursor();
    return true;
}

QString Editor::strippedName(const QString &fullFileName)
{
    return QFileInfo(fullFileName).fileName();
}
