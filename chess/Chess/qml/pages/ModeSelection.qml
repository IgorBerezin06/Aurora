import QtQuick 2.0
import Sailfish.Silica 1.0

Page
{
    id: page

    Rectangle {
        anchors.fill: parent
        color: "#2B2B2B"
    }

    Rectangle {
        width: 100
        height: 40
        color: "#4A90D9"
        radius: 5
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.margins: 20
        z: 10

        Text {
            anchors.centerIn: parent
            text: "← Меню"
            color: "white"
            font.pixelSize: 18
        }

        MouseArea {
            anchors.fill: parent
            onClicked: {
                pageStack.pop()
            }
        }
    }

    Column
    {
        anchors.centerIn: parent
        spacing: 30

        Label
        {
            text: "Шахматы"
            anchors.horizontalCenter: parent.horizontalCenter
            font.pixelSize: 32
            color: "white"
        }

        Button
        {
            text: "Два игрока"
            onClicked:
            {
                pageStack.push(Qt.resolvedUrl("MainPage.qml"), {aiMode: false})
            }
        }

        Button
        {
            text: "Игра с роботом"
            onClicked:
            {
                pageStack.push(Qt.resolvedUrl("DifficultySelection.qml"))
            }
        }
    }
}
