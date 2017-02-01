TEMPLATE = app
TARGET = jubatus-gender-quick
QT += widgets quick jubatus

SOURCES = main.cpp

RESOURCES += \
    gender.qrc

OTHER_FILES += gender.qml gender.json \
    Selector.qml \
    Button.qml

target.path = $$[QT_INSTALL_EXAMPLES]/jubatus/qml/classifier/gender
INSTALLS += target
