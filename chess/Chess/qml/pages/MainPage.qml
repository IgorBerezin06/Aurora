import QtQuick 2.0
import Sailfish.Silica 1.0

Page {
    id: page

    Rectangle {
        anchors.fill: parent
        color: "#2B2B2B"
    }

    Label {
        text: "Шахматы"
        anchors.horizontalCenter: parent.horizontalCenter
        y: 20
        font.pixelSize: 36
        color: "white"
    }

    Rectangle {
        id: board
        width: 400
        height: 400
        anchors.centerIn: parent
        color: "transparent"

        Grid {
            id: chessGrid
            rows: 8
            columns: 8
            spacing: 0
            width: parent.width
            height: parent.height

            Repeater {
                model: 64
                Rectangle {
                    width: chessGrid.width / 8
                    height: chessGrid.height / 8
                    color: (Math.floor(index / 8) + index) % 2 === 0 ? "#F0D9B5" : "#B58863"
                    border.color: "#333"
                    border.width: 0.5
                }
            }
        }
    }
}
