TEMPLATE = app
CONFIG += console c++14
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp


unix|win32: LIBS += -lglut

unix|win32: LIBS += -lGL

unix|win32: LIBS += -lGLU

unix|win32: LIBS += -lGLEW
