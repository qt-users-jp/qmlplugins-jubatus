TEMPLATE = app
TARGET = jubatus-nikke-stock-avarage
QT = core network gui widgets jubatus
CONFIG += c++11
SOURCES = main.cpp

OTHER_FILES += nikkei_stock_avarage.json

target.path = $$[QT_INSTALL_EXAMPLES]/jubatus/jubatus-nikke-stock-avarage
INSTALLS += target

