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
        spacing: 30

        Text {
            anchors.horizontalCenter: parent.horizontalCenter
            text: "Выбери игру"
            font.pixelSize: 32
            color: "white"
        }

        Button {
            text: "Шахматы"
            onClicked: {
                console.log("Запуск шахмат")
            }
        }

        Button {
            text: "Вторая игра"
            onClicked: {
                console.log("Запуск второй игры")
            }
        }
    }
}
