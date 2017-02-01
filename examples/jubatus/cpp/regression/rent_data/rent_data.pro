TEMPLATE = app
TARGET = jubatus-rent-data
QT = core network gui widgets jubatus
CONFIG += c++11
SOURCES = main.cpp \
    dialog.cpp

OTHER_FILES += rent_data.json

RESOURCES += \
    rent_data.qrc

FORMS += \
    dialog.ui

HEADERS += \
    dialog.h

target.path = $$[QT_INSTALL_EXAMPLES]/jubatus/cpp/regression/rent_data
INSTALLS += target
