QT += core

TEMPLATE = app
CONFIG += console c++11

INCLUDEPATH += $$TREE_PATH $$TREE_PATH/treeipc

SOURCES += main.cpp \
    $$TREE_PATH/tree/filepath_utils.cpp \
    $$TREE_PATH/tree/resource.cpp \
    $$TREE_PATH/tree/property_listener.cpp \
    cmd.cpp \
    numeric_property_gsg.cpp \
    $$TREE_PATH/treeipc/client.cpp \
    $$TREE_PATH/treeipc/client_node.cpp \
    $$TREE_PATH/treeipc/io_service.cpp \
    $$TREE_PATH/treeipc/package.cpp \
    $$TREE_PATH/treeipc/server.cpp \
    $$TREE_PATH/treeipc/socket_client.cpp

LIBS += -lreadline -lboost_system -pthread

HEADERS += \
    $$TREE_PATH/tree/filepath_utils.h \
    $$TREE_PATH/tree/node.h \
    $$TREE_PATH/tree/resource.h \
    $$TREE_PATH/treeipc/reliable_serial.h \
    $$TREE_PATH/treeipc/io_service.h \
	$$TREE_PATH/treeipc/client.h \
    $$TREE_PATH/treeipc/observable.h \
    $$TREE_PATH/treeipc/device.h \
    $$TREE_PATH/treeipc/socket_device.h \
    $$TREE_PATH/treeipc/socket_client.h \
    cmd.h \
    typeidable.h \
    property_setter.h \
    property_getter.h \
    property_generator.h \
    numeric_property_gsg.h \
    $$TREE_PATH/treeipc/acceptor.h \
    $$TREE_PATH/treeipc/client.h \
    $$TREE_PATH/treeipc/client_node.h \
    $$TREE_PATH/treeipc/conn_server.h \
    $$TREE_PATH/treeipc/device.h \
    $$TREE_PATH/treeipc/io_service.h \
    $$TREE_PATH/treeipc/object_status.h \
    $$TREE_PATH/treeipc/observable.h \
    $$TREE_PATH/treeipc/package.h \
    $$TREE_PATH/treeipc/property_serializer.h \
    $$TREE_PATH/treeipc/pseudodevice.h \
    $$TREE_PATH/treeipc/server.h \
    $$TREE_PATH/treeipc/socket_client.h \
    $$TREE_PATH/treeipc/socket_device.h
