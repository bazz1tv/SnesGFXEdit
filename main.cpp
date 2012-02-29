#include <QApplication>

#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
	QCoreApplication::setOrganizationName ("GBCFProject");
	QCoreApplication::setApplicationName ("GameBoyCartFlasher");
    MainWindow mainWin;
	mainWin.setWindowIcon(QPixmap(":/images/icon.png"));
    mainWin.show();
    return app.exec();
}
