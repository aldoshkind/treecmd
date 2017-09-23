TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH += ./treeipc

SOURCES += main.cpp \
    tree/filepath_utils.cpp \
    tree/resource.cpp \
    socket_client.cpp \
    io_service.cpp \
    treeipc/client.cpp \
    treeipc/package.cpp \
    treeipc/client_node.cpp \
    tree/property_listener.cpp

LIBS += -lreadline -lboost_system -pthread

HEADERS += \
    tree/filepath_utils.h \
    tree/node.h \
    tree/resource.h \
    reliable_serial.h \
    socket_client.h \
    io_service.h \
    treeipc/observable.h
