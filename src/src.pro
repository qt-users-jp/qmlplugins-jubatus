TEMPLATE = subdirs
SUBDIRS = jubatus

!isEmpty(QT.qml.name) {
    src_imports.subdir = imports
    src_imports.depends = jubatus
    SUBDIRS += src_imports
}
