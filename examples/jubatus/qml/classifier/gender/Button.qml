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

MouseArea {
    id: root
    width: 100
    height: 100

    property bool checkable: false
    property bool checked: false
    property alias backgroundColor: background.color
    property alias color: text.color
    property alias font: text.font
    property alias text: text.text

    Rectangle {
        id: background
        anchors.fill: parent
        anchors.margins: 2
        color: 'tomato'
        z: -1
    }

    Text {
        id: text
        anchors.fill: parent
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        elide: Text.ElideRight
        font.pixelSize: 26
        color: 'white'
    }

    StateGroup {
        states: [
            State {
                name: "checked"
                when: root.checkable && root.checked
                PropertyChanges {
                    target: root
                    opacity: 1
                }
            },
            State {
                name: "unchecked"
                when: root.checkable && !root.checked
                PropertyChanges {
                    target: root
                    opacity: 0.5
                }
            },
            State {
                name: "pressed"
                when: !root.checkable && root.pressed
                PropertyChanges {
                    target: root
                    scale: 0.8
                }
            }
        ]
    }
}
