import QtQuick 2.0
import Sailfish.Silica 1.0

Page
{
    id: page

    property int selectedTimeMode: 1

    Rectangle
    {
        anchors.fill: parent
        color: "#2B2B2B"
    }

    Column
    {
        anchors.centerIn: parent
        spacing: 30
        width: parent.width - 80

        Label
        {
            text: "Шашки"
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
                pageStack.push(Qt.resolvedUrl("CheckersGame.qml"),
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
                pageStack.push(Qt.resolvedUrl("CheckersDifficultySelection.qml"),
                {
                    timeMode: selectedTimeMode
                })
            }
        }
    }
}
