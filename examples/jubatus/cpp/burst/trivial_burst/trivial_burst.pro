TEMPLATE = app
TARGET = jubatus-trivial-burst
QT = core jubatus
CONFIG += c++11
SOURCES = main.cpp

OTHER_FILES += trivial_burst.json

target.path = $$[QT_INSTALL_EXAMPLES]/jubatus/cpp/burst/trivial_burst
INSTALLS += target

