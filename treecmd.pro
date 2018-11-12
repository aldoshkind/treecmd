QT += core

TEMPLATE = app
CONFIG += console c++11

INCLUDEPATH += ./treeipc ../tree ../

SOURCES += main.cpp \
    cmd.cpp \
    ../tree/filepath_utils.cpp \
    ../tree/property_listener.cpp \
    ../tree/resource.cpp \
    ../tree/tree_node.cpp \
    ../treeipc/client.cpp \
    ../treeipc/client_node.cpp \
    ../treeipc/io_service.cpp \
    ../treeipc/package.cpp \
    ../treeipc/socket_client.cpp

LIBS += -lreadline -lboost_system -pthread

HEADERS += \
    cmd.h \
    typeidable.h \
    ../tree/cnode.h \
    ../tree/filepath_utils.h \
    ../tree/property_listener.h \
    ../tree/resource.h \
    ../tree/tree_node.h \
    type.h \
    ../treeipc/client.h \
    ../treeipc/client_node.h \
    ../treeipc/device.h \
    ../treeipc/io_service.h \
    ../treeipc/object_status.h \
    ../treeipc/observable.h \
    ../treeipc/package.h \
    ../treeipc/property_serializer.h \
    ../treeipc/pseudodevice.h \
    ../treeipc/reliable_serial.h \
    ../treeipc/socket_client.h \
    ../treeipc/socket_device.h
