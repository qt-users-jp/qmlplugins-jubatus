TEMPLATE = app
TARGET = jubatus-gender-widget
QT += widgets jubatus
CONFIG += c++11
SOURCES = main.cpp

HEADERS += \
    widget.h

SOURCES += \
    widget.cpp

FORMS += \
    widget.ui

OTHER_FILES += gender.json

target.path = $$[QT_INSTALL_EXAMPLES]/jubatus/cpp/classifier/gender
INSTALLS += target
