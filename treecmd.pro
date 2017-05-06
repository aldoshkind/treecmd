TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    tree/filepath_utils.cpp \
    tree/resource.cpp

LIBS += -lreadline

HEADERS += \
    tree/filepath_utils.h \
    tree/node.h \
    tree/resource.h
