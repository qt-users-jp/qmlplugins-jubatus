TEMPLATE = subdirs
!isEmpty(QT.jubatus.name) {
	SUBDIRS += cpp
	!isEmpty(QT.qml.name): SUBDIRS += qml
}

