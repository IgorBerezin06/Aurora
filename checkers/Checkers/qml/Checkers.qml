import QtQuick 2.0
import Sailfish.Silica 1.0

ApplicationWindow
{
    id: appWindow
    objectName: "applicationWindow"
    initialPage: Qt.resolvedUrl("pages/CheckersModeSelection.qml")
    cover: Qt.resolvedUrl("cover/DefaultCoverPage.qml")
    allowedOrientations: Orientation.All
}
