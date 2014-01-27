import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.1
import QtJubatus 0.1 as Jubatus

ApplicationWindow {
    title: qsTr("Iris")
    width: 300
    height: 200

    Jubatus.Regression {
        id: regression
        name: 'iris'
        timeout: 10
        property bool hasError: false
        onError: {
            console.debug(message)
            hasError = true
        }
    }

    Component.onCompleted: {
        var request = new XMLHttpRequest
        request.onreadystatechange = function() {
            switch (request.readyState) {
            case XMLHttpRequest.DONE:
                var lines = request.responseText.split('\n')
                for (var i in lines) {
                    if (lines[i] === '') continue
                    var columns = lines[i].split(',')
                    var data = new Object
                    data.petal_width = parseFloat(columns[2])
                    data.class = columns[4]
                    regression.train(data, parseFloat(columns[3]))
                    patalLength.update()
                }
                break
            }
        }

        request.open("GET", "./iris.data")
        request.send()
    }

    menuBar: MenuBar {
        Menu {
            title: qsTr("File")
            MenuItem {
                text: qsTr("Exit")
                onTriggered: Qt.quit();
            }
        }
    }

    GridLayout {
        anchors.fill: parent
        columns: 2
        Label {
            text: 'Petal Width:'
        }
        SpinBox {
            id: petalWidth
            Layout.fillWidth: true
            decimals: 1
            minimumValue: 0.0
            maximumValue: 6.0
            value: 2.0
            onValueChanged: patalLength.update()
        }
        Label {
            text: 'Class:'
        }
        ComboBox {
            id: irisClass
            Layout.fillWidth: true
            model: ['Iris-setosa', 'Iris-versicolor', 'Iris-virginica']
            onCurrentIndexChanged: patalLength.update()
        }
        Label {
            text: 'Petal Length:'
        }
        Label {
            id: patalLength
            Layout.fillWidth: true
            function update() {
                var data = new Object
                data.petal_width = petalWidth.value
                data.class = irisClass.currentText
                var score = regression.estimate(data)
                patalLength.text = score
            }
        }
    }
}
