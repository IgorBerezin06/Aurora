import QtQuick 2.0
import Sailfish.Silica 1.0

Page
{
    id: page

    property int selectedTimeMode: 1
    property string gameType: "chess"

    Rectangle
    {
        anchors.fill: parent
        color: "#2B2B2B"
    }

    Rectangle
    {
        width: 100
        height: 40
        color: "#4A90D9"
        radius: 5
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.margins: 20
        z: 10

        Text
        {
            anchors.centerIn: parent
            text: "← Меню"
            color: "white"
            font.pixelSize: 18
        }

        MouseArea
        {
            anchors.fill: parent
            onClicked: pageStack.pop()
        }
    }

    Column
    {
        anchors.centerIn: parent
        spacing: 30
        width: parent.width - 80

        Label
        {
            text: "Шахматы"
            anchors.horizontalCenter: parent.horizontalCenter
            font.pixelSize: 32
            color: "white"
        }

        Label
        {
            text: "Режим времени"
            anchors.horizontalCenter: parent.horizontalCenter
            font.pixelSize: 22
            color: "white"
            font.bold: true
        }

        ComboBox
        {
            id: timeCombo
            width: parent.width
            label: "Режим времени"
            menu: ContextMenu
            {
                MenuItem
                {
                    text: "Без ограничений"
                    onClicked: selectedTimeMode = 0
                }
                MenuItem
                {
                    text: "Блиц (3 мин + 2 сек)"
                    onClicked: selectedTimeMode = 1
                }
                MenuItem
                {
                    text: "Рапид (10 мин + 5 сек)"
                    onClicked: selectedTimeMode = 2
                }
                MenuItem
                {
                    text: "Классика (30 мин + 30 сек)"
                    onClicked: selectedTimeMode = 3
                }
            }
        }

        Label
        {
            text: ""
            height: 20
        }

        Button
        {
            text: "Два игрока"
            width: parent.width
            onClicked:
            {
                pageStack.push(Qt.resolvedUrl("ChessGame.qml"),
                {
                    aiMode: false,
                    timeMode: selectedTimeMode
                })
            }
        }

        Button
        {
            text: "Игра с роботом"
            width: parent.width
            onClicked:
            {
                pageStack.push(Qt.resolvedUrl("DifficultySelection.qml"),
                {
                    timeMode: selectedTimeMode
                })
            }
        }
    }
}
