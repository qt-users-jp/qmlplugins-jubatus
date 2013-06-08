/* Copyright (c) 2012 Silk Project.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the Silk nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL SILK BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

import QtQuick 2.0
import QtJubatus 0.1 as Jubatus

Rectangle {
    id: root
    width: 600
    height: 400

    Column {
        width: parent.width
        Selector {
            id: hair
            width: parent.width
            model: ['short', 'long']
            color: 'black'
        }

        Selector {
            id: top
            width: parent.width
            model: ['sweeter', 'shirt', 'jacket', 'T shirt']
            color: 'red'
        }

        Selector {
            id: bottom
            width: parent.width
            model: ['jeans', 'skirt', 'chino']
            color: 'blue'
        }

        TextInput {
            id: height
            width: 100
            height: 40
            anchors.horizontalCenter: parent.horizontalCenter
            font.pixelSize: 26
            horizontalAlignment: TextInput.AlignHCenter
            verticalAlignment: TextInput.AlignVCenter
            text: '1.60'
            validator: DoubleValidator {
                top: 2.00
                bottom: 1.00
                decimals: 2
            }

            Rectangle {
                anchors.fill: parent
                anchors.margins: 2
                z: -1
                color: 'yellow'
            }
        }

        Row {
            anchors.horizontalCenter: parent.horizontalCenter
            Button {
                text: 'male'
                backgroundColor: 'blue'
                onClicked: root.train('male')
            }
            Button {
                text: 'female'
                backgroundColor: 'red'
                onClicked: root.train('female')
            }
            Button {
                text: 'other'
                backgroundColor: 'purple'
                onClicked: root.train('other')
            }
        }

        Row {
            anchors.horizontalCenter: parent.horizontalCenter
            Item {
                width: 300
                height: 100
                Rectangle {
                    anchors.fill: parent
                    anchors.margins: 2
                    color: 'black'
                }
                Text {
                    id: result
                    anchors.fill: parent
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    elide: Text.ElideRight
                    font.pixelSize: 26
                    color: 'white'
                }
            }
            Button {
                text: 'classify'
                backgroundColor: 'green'
                onClicked: {
                    var data = {'hair': hair.curerntText, 'top': top.curerntText, 'bottom': bottom.curerntText, 'height': parseInt(height.text)}
                    var results = classifier.classify(data)
                    var text = []
                    for (var i in results) {
                        text.push('%1(%2)'.arg(results[i].label).arg(results[i].score))
                    }
                    result.text = text.join('\n')
                }
            }
        }
    }

    function train(label) {
        var data = {'hair': hair.curerntText, 'top': top.curerntText, 'bottom': bottom.curerntText, 'height': parseInt(height.text)}
        classifier.train(label, data)
    }

    Jubatus.Classifier {
        id: classifier
        name: 'test'
    }
}
