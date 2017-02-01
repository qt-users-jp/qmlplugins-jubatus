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

    Jubatus.Recommender {
        id: recommender
        name: 'npb-similar-player'
        timeout: 10
        property bool hasError: false
        onError: {
            console.debug(message)
            hasError = true
        }
    }

    ListView {
        id: playerView
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        width: parent.width / 2
        model: ListModel {
            id: playerModel
        }

        delegate: Text {
            text: model.id
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    similarPlayerModel.clear()
                    if (!recommender.hasError) {
                        var results = recommender.similarRowFromId(model.id, 10)
                        results.shift()
                        for (var i in results) {
                            similarPlayerModel.append(results[i])
                        }
                    }
                }
            }
        }
    }

    ListView {
        id: similar_players
        anchors.left: playerView.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        model: ListModel {
            id: similarPlayerModel
        }
        delegate: Text {
            text: '%1(%2)'.arg(model.id).arg(model.score)
        }
    }

    Component.onCompleted: {
        var request = new XMLHttpRequest
        request.onreadystatechange = function() {
            switch (request.readyState) {
            case XMLHttpRequest.DONE:
                var learn = recommender.getAllRows().length === 0
                var lines = request.responseText.split('\n')
                var fields = ["打率", "試合数", "打席", "打数", "安打", "本塁打", "打点", "盗塁", "四球", "死球", "三振", "犠打", "併殺打", "長打率", "出塁率", "OPS", "RC27", "XR27"]
                for (var i in lines) {
                    if (lines[i] === '') continue
                    var columns = lines[i].split(',')
                    var id = columns.shift()
                    playerModel.append({'id': id})
                    if (learn && !recommender.hasError) {
                        var data = new Object
                        data["チーム"] = columns.shift()
                        for (var j = 0; j < fields.length; j++) {
                            data[fields[j]] = parseFloat(columns[j])
                        }
                        recommender.updateRow(id, data)
                    }
                }

                break
            }
        }

        request.open("GET", "qrc:/baseball.csv");
        request.send();
    }
}
