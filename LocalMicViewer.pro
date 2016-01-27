TEMPLATE = app
QT = gui core opengl
CONFIG += qt release warn_on console
DESTDIR = bin
OBJECTS_DIR = build
MOC_DIR = build
UI_DIR = build
FORMS = ui/mainwindow.ui
HEADERS = src/mainwindowimpl.h src/locmicview.h src/datapoint.h src/imagewidget.h
SOURCES = src/mainwindowimpl.cpp \
 src/main.cpp \
 src/locmicview.cpp \
 src/datapoint.cpp \
 src/imagewidget.cpp
LIBS +=
