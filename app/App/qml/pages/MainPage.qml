import QtQuick 2.0
import Sailfish.Silica 1.0

Page {
    id: page

    Rectangle {
        anchors.fill: parent
        color: "#2B2B2B"
    }

    Column {
        anchors.centerIn: parent
        spacing: 20
        width: parent.width - 80

        Item {
            anchors.horizontalCenter: parent.horizontalCenter
            width: 120
            height: 120

            Rectangle {
                anchors.centerIn: parent
                width: 110
                height: 110
                radius: 55
                color: "#4A4A4A"
                border.color: "#FFD700"
                border.width: 3
            }

            Canvas {
                anchors.centerIn: parent
                width: 80
                height: 80
                onPaint: {
                    var ctx = getContext("2d")
                    ctx.beginPath()
                    ctx.moveTo(40, 10)
                    ctx.lineTo(70, 25)
                    ctx.lineTo(70, 50)
                    ctx.lineTo(40, 70)
                    ctx.lineTo(10, 50)
                    ctx.lineTo(10, 25)
                    ctx.closePath()
                    ctx.fillStyle = "#FFD700"
                    ctx.fill()
                    ctx.strokeStyle = "#B8860B"
                    ctx.lineWidth = 2
                    ctx.stroke()
                }
            }

            Text {
                anchors.centerIn: parent
                text: "♞"
                font.pixelSize: 50
                color: "#2B2B2B"
                opacity: 0.9
            }

            Rectangle {
                anchors.bottom: parent.bottom
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.bottomMargin: 8
                width: 18
                height: 18
                radius: 9
                color: "#FFD700"
                border.color: "#B8860B"
                border.width: 1
            }
        }

        Label {
            text: "Strategy Masters"
            anchors.horizontalCenter: parent.horizontalCenter
            font.pixelSize: 32
            color: "#FFD700"
            font.bold: true
        }

        Label {
            text: "Выбери игру"
            anchors.horizontalCenter: parent.horizontalCenter
            font.pixelSize: 24
            color: "white"
        }

        Button {
            text: "Шахматы"
            width: parent.width
            anchors.horizontalCenter: parent.horizontalCenter
            onClicked: {
                pageStack.push(Qt.resolvedUrl("ModeSelection.qml"), {
                    gameType: "chess"
                })
            }
        }

        Button {
            text: "Шашки"
            width: parent.width
            anchors.horizontalCenter: parent.horizontalCenter
            onClicked: {
                pageStack.push(Qt.resolvedUrl("CheckersModeSelection.qml"), {
                    gameType: "checkers"
                })
            }
        }
    }
}
