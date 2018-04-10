# -------------------------------------------------
# Project created by QtCreator 2009-01-10T14:03:23
# -------------------------------------------------
QT += network widgets
TARGET = MyQServer
TEMPLATE = app

SOURCES += ./src/main.cpp \
    ./src/MySocketServer.cpp \
    ./src/MySocketClient.cpp \
    ./src/dialog.cpp

HEADERS += ./src/MySocketServer.h \
    ./src/MySocketClient.h \
    ./src/dialog.h

OTHER_FILES += MyQServer.pro
CONFIG += c++11
