QT += core

TEMPLATE = app
CONFIG += console c++11

INCLUDEPATH += ./treeipc

SOURCES += main.cpp \
    tree/filepath_utils.cpp \
    tree/resource.cpp \
    tree/property_listener.cpp \
    cmd.cpp \
    numeric_property_gsg.cpp \
    treeipc/client.cpp \
    treeipc/client_node.cpp \
    treeipc/io_service.cpp \
    treeipc/package.cpp \
    treeipc/server.cpp \
    treeipc/socket_client.cpp

LIBS += -lreadline -lboost_system -pthread

HEADERS += \
    tree/filepath_utils.h \
    tree/node.h \
    tree/resource.h \
    treeipc/reliable_serial.h \
    treeipc/io_service.h \
	treeipc/client.h \
    treeipc/observable.h \
    treeipc/device.h \
    treeipc/socket_device.h \
    treeipc/socket_client.h \
    cmd.h \
    typeidable.h \
    property_setter.h \
    property_getter.h \
    property_generator.h \
    numeric_property_gsg.h \
    treeipc/acceptor.h \
    treeipc/client.h \
    treeipc/client_node.h \
    treeipc/conn_server.h \
    treeipc/device.h \
    treeipc/io_service.h \
    treeipc/object_status.h \
    treeipc/observable.h \
    treeipc/package.h \
    treeipc/property_serializer.h \
    treeipc/pseudodevice.h \
    treeipc/server.h \
    treeipc/socket_client.h \
    treeipc/socket_device.h
