#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QAction;
class QActionGroup;
class QLabel;
class QMdiArea;
class QMenu;
class QToolBar;
class Editor;
class QRadioButton;
class QButtonGroup;
//class TileView;


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent=0);
	//void showEvent ( QShowEvent * event );

public slots:
    void newFile();
    void openFile(const QString &fileName);
	void setZoom(int id);
	void updateCursor(int id);

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void open();
    void save();
    void saveAs();
    void cut();
    void copy();
    void paste();
    void about();
    void updateActions();
    void loadFiles();

private:
    void createActions();
    void createMenus();
    void createToolBars();
    void createStatusBar();
    void addEditor(Editor *editor);
    Editor *activeEditor();

    QMdiArea *mdiArea;
    QLabel *readyLabel;
    QWidgetList windows;

    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *windowMenu;
    QMenu *helpMenu;
	
	QRadioButton *zoomButton1, *zoomButton2, *zoomButton3;
	QButtonGroup *zoomButtonGroup;
	QRadioButton *cursorButton1, *cursorButton2, *cursorButton3, *cursorButton4;
	QButtonGroup *cursorButtonGroup;
	
    QToolBar *fileToolBar;
    QToolBar *editToolBar;
	QToolBar *zoomToolBar;
	QToolBar *cursorToolBar;
    QActionGroup *windowActionGroup;
	QAction *zoomAction1;
	QAction *zoomAction2;
	QAction *zoomAction3;
	//QAction *zoomAction4;
    QAction *newAction;
    QAction *openAction;
    QAction *saveAction;
    QAction *saveAsAction;
    QAction *exitAction;
    QAction *cutAction;
    QAction *copyAction;
    QAction *pasteAction;
    QAction *closeAction;
    QAction *closeAllAction;
    QAction *tileAction;
    QAction *cascadeAction;
    QAction *nextAction;
    QAction *previousAction;
    QAction *separatorAction;
    QAction *aboutAction;
    QAction *aboutQtAction;
};

#endif
