TEMPLATE = app
TARGET = jubatus-npb-similar-player-quick
QT = quick jubatus

SOURCES = main.cpp

RESOURCES += \
    npb_similar_player.qrc

OTHER_FILES += npb_similar_player.qml npb_similar_player.json

target.path = $$[QT_INSTALL_EXAMPLES]/jubatus/qml/recommender/npb_similar_player
INSTALLS += target
