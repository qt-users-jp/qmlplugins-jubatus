TEMPLATE = app
TARGET = jubatus-AAAI
QT = core network jubatus
CONFIG += c++11
SOURCES = main.cpp

OTHER_FILES += AAAI.json

target.path = $$[QT_INSTALL_EXAMPLES]/jubatus/cpp/nearestneighbor/AAAI
INSTALLS += target

