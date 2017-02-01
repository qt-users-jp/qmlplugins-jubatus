TEMPLATE = app
TARGET = jubatus-network-intrusion-detection
QT = core network gui widgets jubatus
CONFIG += c++11
SOURCES = main.cpp

OTHER_FILES += network_intrusion_detection.json

target.path = $$[QT_INSTALL_EXAMPLES]/jubatus/cpp/anomaly/network_intrusion_detection
INSTALLS += target

