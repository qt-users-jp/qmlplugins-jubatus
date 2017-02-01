TEMPLATE = app
TARGET = jubatus-npb-similar-player
QT = core network gui widgets jubatus
CONFIG += c++11
SOURCES = main.cpp \
    dialog.cpp

OTHER_FILES += npb_similar_player.json

RESOURCES += \
    npb_similar_player.qrc

FORMS += \
    dialog.ui

HEADERS += \
    dialog.h

target.path = $$[QT_INSTALL_EXAMPLES]/jubatus/cpp/recommender/npb_similar_player
INSTALLS += target
