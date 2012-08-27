TEMPLATE      = app
DEPENDPATH += .
INCLUDEPATH += .
HEADERS       += editor.h \
                mainwindow.h \
				tileview.h \
				globals.h \
				tile.h
SOURCES       += editor.cpp \
                main.cpp \
                mainwindow.cpp \
				tileview.cpp \
				globals.cpp \
				tile.cpp
RESOURCES     = mdieditor.qrc
#RC_FILE = res.rc
unix {
ICON = images/icon.icns
}
