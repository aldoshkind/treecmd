QT += core

TEMPLATE = app
CONFIG += console c++11

INCLUDEPATH += ./treeipc ../tree

SOURCES += main.cpp \
    cmd.cpp \
    ../tree/filepath_utils.cpp \
    ../tree/property_listener.cpp \
    ../tree/resource.cpp \
    ../tree/tree_node.cpp

LIBS += -lreadline -lboost_system -pthread

HEADERS += \
    cmd.h \
    typeidable.h \
    ../tree/cnode.h \
    ../tree/filepath_utils.h \
    ../tree/property_listener.h \
    ../tree/resource.h \
    ../tree/tree_node.h \
    type.h
