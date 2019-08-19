QT += core

TEMPLATE = app
CONFIG += console c++11

INCLUDEPATH += ./treeipc ../tree ../

SOURCES += main.cpp \
    cmd.cpp \
    ../tree/filepath_utils.cpp \
    ../tree/property_listener.cpp \
    ../tree/tree_node.cpp \
    ../treeipc/client.cpp \
    ../treeipc/client_node.cpp \
    ../treeipc/io_service.cpp \
    ../treeipc/package.cpp \
    ../treeipc/socket_client.cpp \
    ../tree/tree_node_inherited.cpp \
    ../treeipc/property_fake.cpp \
    ../treeipc/proxy_node_generator.cpp \
    ../treeipc/node_sync.cpp \
    ../treeipc/client_nonroot.cpp \
	../treeipc/package_stream.cpp

LIBS += -lreadline -lboost_system -pthread

HEADERS += \
    cmd.h \
    typeidable.h \
    ../tree/filepath_utils.h \
    ../tree/property_listener.h \
    ../tree/tree_node.h \
    type.h \
    ../treeipc/client.h \
    ../treeipc/client_node.h \
    ../treeipc/package_stream.h \
    ../treeipc/io_service.h \
	../treeipc/node_sync.h \
    ../treeipc/object_status.h \
    ../treeipc/observable.h \
    ../treeipc/package.h \
    ../treeipc/property_serializer.h \
    ../treeipc/reliable_bytestream_base.h \
    ../treeipc/socket_client.h \
    ../treeipc/package_stream.h \
    ../treeipc/property_fake.h \
    ../treeipc/locking_queue.h \
    ../treeipc/client_nonroot.h \
    ../tree/property.h
