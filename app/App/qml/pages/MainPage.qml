import QtQuick 2.0
import Sailfish.Silica 1.0

Page
{
    id: page

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
            text: "Strategy Masters"
            anchors.horizontalCenter: parent.horizontalCenter
            font.pixelSize: 36
            color: "white"
            font.bold: true
        }

        Label
        {
            text: "Выбери игру"
            anchors.horizontalCenter: parent.horizontalCenter
            font.pixelSize: 28
            color: "white"
        }

        Button
        {
            text: "Шахматы"
            width: parent.width
            anchors.horizontalCenter: parent.horizontalCenter
            onClicked:
            {
                pageStack.push(Qt.resolvedUrl("ModeSelection.qml"),
                {
                    gameType: "chess"
                })
            }
        }

        Button
        {
            text: "Шашки"
            width: parent.width
            anchors.horizontalCenter: parent.horizontalCenter
            onClicked:
            {
                pageStack.push(Qt.resolvedUrl("CheckersModeSelection.qml"),
                {
                    gameType: "checkers"
                })
            }
        }
    }
}
