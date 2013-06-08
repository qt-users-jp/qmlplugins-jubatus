IMPORT_VERSION = 0.1

QT = qml jubatus
LIBS += -L$$QT.jubatus.libs

CONFIG += exceptions

SOURCES += main.cpp

load(qml_plugin)

OTHER_FILES = plugins.qmltypes qmldir
