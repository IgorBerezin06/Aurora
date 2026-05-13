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

                    property int row: Math.floor(index / 8)
                    property int col: index % 8

                    Image {
                        anchors.centerIn: parent
                        width: parent.width - 4
                        height: parent.height - 4
                        fillMode: Image.PreserveAspectFit
                        source: {
                            if (row === 0) {
                                if (col === 0 || col === 7) return "Chess_rdt45.svg"
                                if (col === 1 || col === 6) return "Chess_ndt45.svg"
                                if (col === 2 || col === 5) return "Chess_bdt45.svg"
                                if (col === 3) return "Chess_qdt45.svg"
                                if (col === 4) return "Chess_kdt45.svg"
                            }
                            if (row === 1) return "Chess_pdt45.svg"
                            if (row === 6) return "Chess_plt45.svg"
                            if (row === 7) {
                                if (col === 0 || col === 7) return "Chess_rlt45.svg"
                                if (col === 1 || col === 6) return "Chess_nlt45.svg"
                                if (col === 2 || col === 5) return "Chess_blt45.svg"
                                if (col === 3) return "Chess_qlt45.svg"
                                if (col === 4) return "Chess_klt45.svg"
                            }
                            return ""
                        }
                    }
                }
            }
        }
    }
}
