TARGET  = prg.exe

TEMPLATE = app
CONFIG += qt thread warn_on release 
QT = core gui

greaterThan(QT_MAJOR_VERSION, 4) {
    QT += widgets
}

DEFINES += 
QMAKE_CXXFLAGS  += -O2 -fPIC -static -std=c++11 -Wall
QMAKE_LFLAGS    +=

INCLUDEPATH += ../../src
LIBS +=

HEADERS = ../../src/*.h
SOURCES = ../../src/*.cpp


unix:OBJECTS_DIR          = obj
unix:MOC_DIR              = moc

# unix:QMAKE_POST_LINK=strip $(TARGET)
