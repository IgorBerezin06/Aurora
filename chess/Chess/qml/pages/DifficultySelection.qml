import QtQuick 2.0
import Sailfish.Silica 1.0

Page
{
    id: page

    Rectangle {
        anchors.fill: parent
        color: "#2B2B2B"
    }

    Column
    {
        anchors.centerIn: parent
        spacing: 30

        Label
        {
            text: "Выбери сложность"
            anchors.horizontalCenter: parent.horizontalCenter
            font.pixelSize: 32
            color: "white"
        }

        Button
        {
            text: "Лёгкий"
            onClicked:
            {
                pageStack.push(Qt.resolvedUrl("MainPage.qml"), {aiMode: true, aiLevel: 1})
            }
        }

        Button
        {
            text: "Средний"
            onClicked:
            {
                pageStack.push(Qt.resolvedUrl("MainPage.qml"), {aiMode: true, aiLevel: 2})
            }
        }

        Button
        {
            text: "Сложный"
            onClicked:
            {
                pageStack.push(Qt.resolvedUrl("MainPage.qml"), {aiMode: true, aiLevel: 3})
            }
        }
    }
}
