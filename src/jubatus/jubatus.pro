TARGET = QtJubatus
MODULE = jubatus
QT = core

load(qt_module)

CONFIG += link_pkgconfig
PKGCONFIG += jubatus-client

include(./classifier/classifier.pri)

DEFINES += JUBATUS_LIBRARY

HEADERS += \
    jubatus_global.h
