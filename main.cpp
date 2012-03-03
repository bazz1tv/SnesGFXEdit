#include <QApplication>

#include "mainwindow.h"
#include "globals.h"

int main(int argc, char *argv[])
{
	//m_Cursor = QCursor();
	debugfile.open(QIODevice::WriteOnly);
	debug.setDevice(&debugfile);
	debug.setIntegerBase(16);
	debug<<"Debug Information\n";
    QApplication app(argc, argv);
	QCoreApplication::setOrganizationName ("GBCFProject");
	QCoreApplication::setApplicationName ("GameBoyCartFlasher");
    MainWindow mainWin;
	mainWin.setWindowIcon(QPixmap(":/images/icon.png"));
    mainWin.show();
    return app.exec();
}
