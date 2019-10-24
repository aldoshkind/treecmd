QT += core

TEMPLATE = app
CONFIG += console c++11

INCLUDEPATH += $$TREE_PATH/tree $$TREE_PATH/

SOURCES += main.cpp \
    cmd.cpp \
    $$TREE_PATH/tree/filepath_utils.cpp \
    $$TREE_PATH/tree/property_listener.cpp \
    $$TREE_PATH/tree/tree_node.cpp \
    $$TREE_PATH/treeipc/client.cpp \
    $$TREE_PATH/treeipc/client_node.cpp \
    $$TREE_PATH/treeipc/io_service.cpp \
    $$TREE_PATH/treeipc/package.cpp \
    $$TREE_PATH/treeipc/socket_client.cpp \
    $$TREE_PATH/tree/tree_node_inherited.cpp \
    $$TREE_PATH/treeipc/property_fake.cpp \
    $$TREE_PATH/treeipc/proxy_node_generator.cpp \
    $$TREE_PATH/treeipc/node_sync.cpp \
    $$TREE_PATH/treeipc/client_nonroot.cpp \
	$$TREE_PATH/treeipc/package_stream.cpp

LIBS += -lreadline -lboost_system -pthread

HEADERS += \
    cmd.h \
    typeidable.h \
    $$TREE_PATH/tree/filepath_utils.h \
    $$TREE_PATH/tree/property_listener.h \
    $$TREE_PATH/tree/tree_node.h \
    type.h \
    $$TREE_PATH/treeipc/client.h \
    $$TREE_PATH/treeipc/client_node.h \
    $$TREE_PATH/treeipc/package_stream.h \
    $$TREE_PATH/treeipc/io_service.h \
	$$TREE_PATH/treeipc/node_sync.h \
    $$TREE_PATH/treeipc/object_status.h \
    $$TREE_PATH/treeipc/observable.h \
    $$TREE_PATH/treeipc/package.h \
    $$TREE_PATH/treeipc/property_serializer.h \
    $$TREE_PATH/treeipc/reliable_bytestream_base.h \
    $$TREE_PATH/treeipc/socket_client.h \
    $$TREE_PATH/treeipc/package_stream.h \
    $$TREE_PATH/treeipc/property_fake.h \
    $$TREE_PATH/treeipc/locking_queue.h \
    $$TREE_PATH/treeipc/client_nonroot.h \
    $$TREE_PATH/tree/property.h
