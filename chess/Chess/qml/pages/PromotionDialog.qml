import QtQuick 2.0
import Sailfish.Silica 1.0

Dialog {
    id: dialog
    property bool isWhite: true

    Column {
        anchors.centerIn: parent
        spacing: 20
        width: parent.width - 40

        Label {
            text: "Выбери фигуру"
            font.pixelSize: 28
            color: "white"
            anchors.horizontalCenter: parent.horizontalCenter
        }

        Column {
            spacing: 15
            width: parent.width

            Button {
                text: isWhite ? "♕  Ферзь" : "♛  Ферзь"
                width: parent.width
                onClicked: {
                    dialog.accept(isWhite ? "♕" : "♛")
                }
            }
            Button {
                text: isWhite ? "♖  Ладья" : "♜  Ладья"
                width: parent.width
                onClicked: {
                    dialog.accept(isWhite ? "♖" : "♜")
                }
            }
            Button {
                text: isWhite ? "♗  Слон" : "♝  Слон"
                width: parent.width
                onClicked: {
                    dialog.accept(isWhite ? "♗" : "♝")
                }
            }
            Button {
                text: isWhite ? "♘  Конь" : "♞  Конь"
                width: parent.width
                onClicked: {
                    dialog.accept(isWhite ? "♘" : "♞")
                }
            }
        }
    }
}
