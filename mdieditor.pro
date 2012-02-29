TEMPLATE      = app
DEPENDPATH += .
INCLUDEPATH += .
HEADERS       += editor.h \
                mainwindow.h \
				tileview.h \
				globals.h
SOURCES       += editor.cpp \
                main.cpp \
                mainwindow.cpp \
				tileview.cpp \
				globals.cpp
RESOURCES     = mdieditor.qrc
#RC_FILE = res.rc
unix {
ICON = images/icon.icns
}
