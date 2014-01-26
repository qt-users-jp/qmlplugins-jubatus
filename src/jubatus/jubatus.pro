TARGET = QtJubatus
MODULE = jubatus
QT = core

load(qt_module)

CONFIG += link_pkgconfig
PKGCONFIG += jubatus-client

CONFIG += exception

DEFINES += JUBATUS_LIBRARY
HEADERS += \
    jubatus_global.h

include(./client/client.pri)
include(./anomaly/anomaly.pri)
include(./classifier/classifier.pri)
include(./recommender/recommender.pri)
