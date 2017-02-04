TARGET = QtJubatus
MODULE = jubatus
QT = core

load(qt_module)

CONFIG += link_pkgconfig
PKGCONFIG += jubatus-client

CONFIG += exception
CONFIG -= create_cmake

DEFINES += JUBATUS_LIBRARY
HEADERS += \
    jubatus_global.h

include(./client/client.pri)
include(./anomaly/anomaly.pri)
include(./burst/burst.pri)
include(./classifier/classifier.pri)
include(./recommender/recommender.pri)
include(./regression/regression.pri)
include(./clustering/clustering.pri)
