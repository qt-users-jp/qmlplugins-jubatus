TEMPLATE = app
TARGET = jubatus-npb-similar-player
QT = core network gui widgets jubatus
CONFIG += c++11
SOURCES = main.cpp \
    dialog.cpp

OTHER_FILES += npb_similar_player

RESOURCES += \
    npb_similar_player.qrc

FORMS += \
    dialog.ui

HEADERS += \
    dialog.h
