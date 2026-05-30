import QtQuick 2.0
import Sailfish.Silica 1.0

Page
{
    id: page

    property int timeMode: 1

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
            text: "← Назад"
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
        spacing: 20
        width: parent.width - 80

        Label
        {
            text: "Выбери уровень сложности"
            anchors.horizontalCenter: parent.horizontalCenter
            font.pixelSize: 28
            color: "white"
        }

        Label
        {
            text: ""
            height: 20
        }

        Button
        {
            text: "Лёгкий"
            width: parent.width
            onClicked:
            {
                pageStack.push(Qt.resolvedUrl("CheckersGame.qml"),
                {
                    aiMode: true,
                    aiLevel: 1,
                    timeMode: page.timeMode
                })
            }
        }

        Button
        {
            text: "Сложный"
            width: parent.width
            onClicked:
            {
                pageStack.push(Qt.resolvedUrl("CheckersGame.qml"),
                {
                    aiMode: true,
                    aiLevel: 2,
                    timeMode: page.timeMode
                })
            }
        }
    }
}
