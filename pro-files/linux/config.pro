TARGET  = prg.exe

TEMPLATE = app
CONFIG += qt thread warn_off release
QT = core gui

greaterThan(QT_MAJOR_VERSION, 4) {
    QT += widgets
}

DEFINES += 
# -Wall
QMAKE_CXXFLAGS  += -O2 -fPIC -static -std=c++14 -g -fpermissive -fms-extensions
QMAKE_LFLAGS    +=

INCLUDEPATH += ../../src
LIBS += -lpthread

HEADERS = ../../src/*.h ../../src/gui/*.h ../../src/xmath/*.h
SOURCES = ../../src/*.cpp ../../src/gui/*.cpp ../../src/xmath/*.cpp


unix:OBJECTS_DIR          = obj
unix:MOC_DIR              = moc

# unix:QMAKE_POST_LINK=strip $(TARGET)
