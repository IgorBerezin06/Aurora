import QtQuick 2.0
import Sailfish.Silica 1.0
import Checkers 1.0

Page
{
    id: page

    property bool aiMode: false
    property int aiLevel: 1
    property int timeMode: 1
    property int selectedRow: -1
    property int selectedCol: -1
    property int updateTrigger: 0
    property var validMoves: []
    property bool offerDrawPending: false
    property bool gameEnded: false
    property string gameResultText: ""
    property int whiteTime: 0
    property int blackTime: 0
    property bool lastDrawOfferRejected: false
    property bool currentWhiteTurn: true
    property var moveHistory: []

    function formatTime(seconds)
    {
        if (seconds <= 0)
        {
            return "00:00"
        }
        var mins = Math.floor(seconds / 60)
        var secs = seconds % 60
        return (mins < 10 ? "0" + mins : mins) + ":" + (secs < 10 ? "0" + secs : secs)
    }

    function addTimeIncrement(forWhite)
    {
        if (timeMode === 1)
        {
            if (forWhite)
            {
                whiteTime += 2
            }
            else
            {
                blackTime += 2
            }
        }
        else if (timeMode === 2)
        {
            if (forWhite)
            {
                whiteTime += 5
            }
            else
            {
                blackTime += 5
            }
        }
        else if (timeMode === 3)
        {
            if (forWhite)
            {
                whiteTime += 30
            }
            else
            {
                blackTime += 30
            }
        }
        updateTrigger++
    }

    function resetGame()
    {
        gameEnded = false
        gameResultText = ""
        offerDrawPending = false
        lastDrawOfferRejected = false
        selectedRow = -1
        selectedCol = -1
        validMoves = []
        moveHistory = []

        if (timeMode === 1)
        {
            whiteTime = 180
            blackTime = 180
        }
        else if (timeMode === 2)
        {
            whiteTime = 600
            blackTime = 600
        }
        else if (timeMode === 3)
        {
            whiteTime = 1800
            blackTime = 1800
        }
        else
        {
            whiteTime = 0
            blackTime = 0
        }

        checkersModel.resetBoard()
        updateTrigger++
    }

    function undoMove()
    {
        if (moveHistory.length === 0)
        {
            return
        }
        if (aiMode && !checkersModel.isWhiteTurn())
        {
            return
        }
        var lastMove = moveHistory.pop()
        checkersModel.undoMove(lastMove.fromRow, lastMove.fromCol, lastMove.toRow, lastMove.toCol, lastMove.piece, lastMove.captured)
        if (moveHistory.length > 0 && aiMode && !checkersModel.isWhiteTurn())
        {
            var aiMove = moveHistory.pop()
            checkersModel.undoMove(aiMove.fromRow, aiMove.fromCol, aiMove.toRow, aiMove.toCol, aiMove.piece, aiMove.captured)
        }
        updateTrigger++
    }

    Component.onCompleted:
    {
        if (timeMode === 1)
        {
            whiteTime = 180
            blackTime = 180
        }
        else if (timeMode === 2)
        {
            whiteTime = 600
            blackTime = 600
        }
        else if (timeMode === 3)
        {
            whiteTime = 1800
            blackTime = 1800
        }
        else
        {
            whiteTime = 0
            blackTime = 0
        }
        updateTrigger++
    }

    Dialog
    {
        id: drawOfferDialog
        property bool fromPlayer: true
        canAccept: true

        SilicaFlickable
        {
            anchors.fill: parent
            contentHeight: drawColumn.height + Theme.paddingLarge

            Column
            {
                id: drawColumn
                width: parent.width
                spacing: Theme.paddingMedium

                DialogHeader
                {
                    title: drawOfferDialog.fromPlayer ? "Предложить ничью" : "Предложение ничьей"
                }

                Label
                {
                    text: drawOfferDialog.fromPlayer ? "Отправить предложение о ничьей?" : "Противник предлагает ничью. Согласиться?"
                    font.pixelSize: 18
                    color: "white"
                    anchors.horizontalCenter: parent.horizontalCenter
                    wrapMode: Text.WordWrap
                    width: parent.width - 2 * Theme.horizontalPageMargin
                }
            }
        }

        onAccepted:
        {
            if (fromPlayer)
            {
                offerDrawPending = true
            }
            else
            {
                gameEnded = true
                gameResultText = "Ничья"
                checkersModel.gameResult = "Ничья"
                updateTrigger++
            }
        }

        onRejected:
        {
            if (!fromPlayer)
            {
                offerDrawPending = false
            }
            else
            {
                lastDrawOfferRejected = true
            }
        }
    }

    Dialog
    {
        id: resignDialog
        canAccept: true

        SilicaFlickable
        {
            anchors.fill: parent
            contentHeight: resignColumn.height + Theme.paddingLarge

            Column
            {
                id: resignColumn
                width: parent.width
                spacing: Theme.paddingMedium

                DialogHeader
                {
                    title: "Сдаться"
                }

                Label
                {
                    text: "Вы уверены, что хотите сдаться?"
                    font.pixelSize: 18
                    color: "white"
                    anchors.horizontalCenter: parent.horizontalCenter
                    wrapMode: Text.WordWrap
                    width: parent.width - 2 * Theme.horizontalPageMargin
                }
            }
        }

        onAccepted:
        {
            gameEnded = true
            if (checkersModel.isWhiteTurn())
            {
                gameResultText = "Белые сдались. Чёрные победили"
                checkersModel.gameResult = "Белые сдались. Чёрные победили"
            }
            else
            {
                gameResultText = "Чёрные сдались. Белые победили"
                checkersModel.gameResult = "Чёрные сдались. Белые победили"
            }
            updateTrigger++
        }
    }

    Dialog
    {
        id: newGameDialog
        canAccept: true

        SilicaFlickable
        {
            anchors.fill: parent
            contentHeight: newGameColumn.height + Theme.paddingLarge

            Column
            {
                id: newGameColumn
                width: parent.width
                spacing: Theme.paddingMedium

                DialogHeader
                {
                    title: "Новая игра"
                }

                Label
                {
                    text: "Вы уверены, что хотите начать новую игру? Текущая партия будет потеряна."
                    font.pixelSize: 18
                    color: "white"
                    anchors.horizontalCenter: parent.horizontalCenter
                    wrapMode: Text.WordWrap
                    width: parent.width - 2 * Theme.horizontalPageMargin
                }
            }
        }

        onAccepted:
        {
            resetGame()
        }
    }

    Timer
    {
        id: clockTimer
        interval: 1000
        running: !checkersModel.isGameOver() && !gameEnded && (timeMode !== 0) && !(aiMode && !checkersModel.isWhiteTurn())
        repeat: true
        onTriggered:
        {
            if (checkersModel.isWhiteTurn())
            {
                if (whiteTime > 0)
                {
                    whiteTime--
                    updateTrigger++
                }
                if (whiteTime <= 0 && !gameEnded)
                {
                    clockTimer.running = false
                    gameEnded = true
                    gameResultText = "Белые проиграли по времени"
                    checkersModel.gameResult = "Белые проиграли по времени"
                    updateTrigger++
                }
            }
            else
            {
                if (blackTime > 0)
                {
                    blackTime--
                    updateTrigger++
                }
                if (blackTime <= 0 && !gameEnded)
                {
                    clockTimer.running = false
                    gameEnded = true
                    gameResultText = "Чёрные проиграли по времени"
                    checkersModel.gameResult = "Чёрные проиграли по времени"
                    updateTrigger++
                }
            }
        }
    }

    Rectangle
    {
        anchors.fill: parent
        color: "#2B2B2B"
        z: -1
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

    Rectangle
    {
        width: 100
        height: 40
        color: "#E74C3C"
        radius: 5
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.margins: 20
        z: 10

        Text
        {
            anchors.centerIn: parent
            text: "Сдаться"
            color: "white"
            font.pixelSize: 18
        }

        MouseArea
        {
            anchors.fill: parent
            onClicked: resignDialog.open()
        }
    }

    Rectangle
    {
        width: 100
        height: 40
        color: "#F39C12"
        radius: 5
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.margins: 20
        z: 10

        Text
        {
            anchors.centerIn: parent
            text: "Ничья"
            color: "white"
            font.pixelSize: 18
        }

        MouseArea
        {
            anchors.fill: parent
            onClicked:
            {
                if (aiMode && !checkersModel.isWhiteTurn())
                {
                    return
                }
                if (lastDrawOfferRejected)
                {
                    return
                }
                drawOfferDialog.fromPlayer = true
                drawOfferDialog.open()
            }
        }
    }

    Row
    {
        anchors.top: parent.top
        anchors.topMargin: 80
        anchors.horizontalCenter: parent.horizontalCenter
        spacing: 100

        Rectangle
        {
            width: 120
            height: 60
            color: currentWhiteTurn ? "#4CAF50" : "#333333"
            radius: 10
            border.color: "white"
            border.width: 1

            Column
            {
                anchors.centerIn: parent
                spacing: 5

                Text
                {
                    text: "Белые"
                    color: "white"
                    font.pixelSize: 14
                    anchors.horizontalCenter: parent.horizontalCenter
                }

                Text
                {
                    text: formatTime(whiteTime)
                    color: whiteTime < 10 ? "red" : "white"
                    font.pixelSize: 24
                    font.bold: true
                    anchors.horizontalCenter: parent.horizontalCenter
                }
            }
        }

        Rectangle
        {
            width: 120
            height: 60
            color: !currentWhiteTurn ? "#4CAF50" : "#333333"
            radius: 10
            border.color: "white"
            border.width: 1

            Column
            {
                anchors.centerIn: parent
                spacing: 5

                Text
                {
                    text: "Чёрные"
                    color: "white"
                    font.pixelSize: 14
                    anchors.horizontalCenter: parent.horizontalCenter
                }

                Text
                {
                    text: formatTime(blackTime)
                    color: blackTime < 10 ? "red" : "white"
                    font.pixelSize: 24
                    font.bold: true
                    anchors.horizontalCenter: parent.horizontalCenter
                }
            }
        }
    }

    Label
    {
        text: currentWhiteTurn ? "Ход белых" : "Ход чёрных"
        anchors.horizontalCenter: parent.horizontalCenter
        y: 160
        font.pixelSize: 20
        color: "white"
    }

    Label
    {
        text: gameResultText
        anchors.horizontalCenter: parent.horizontalCenter
        y: 190
        font.pixelSize: 24
        color: "red"
        visible: gameResultText !== ""
    }

    Loader
    {
        id: boardLoader
        anchors.centerIn: parent
        width: 400
        height: 400
        sourceComponent: boardComponent
    }

    Rectangle
    {
        width: 200
        height: 50
        color: "#4A90D9"
        radius: 5
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: boardLoader.bottom
        anchors.topMargin: 20
        z: 10

        Text
        {
            anchors.centerIn: parent
            text: "Новая игра"
            color: "white"
            font.pixelSize: 20
            font.bold: true
        }

        MouseArea
        {
            anchors.fill: parent
            onClicked: newGameDialog.open()
        }
    }

    Rectangle
    {
        width: 200
        height: 50
        color: "#3498DB"
        radius: 5
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: boardLoader.bottom
        anchors.topMargin: 80
        z: 10

        Text
        {
            anchors.centerIn: parent
            text: "Отмена хода"
            color: "white"
            font.pixelSize: 20
            font.bold: true
        }

        MouseArea
        {
            anchors.fill: parent
            onClicked: undoMove()
        }
    }

    onUpdateTriggerChanged:
    {
        var oldComponent = boardLoader.sourceComponent
        boardLoader.sourceComponent = undefined
        boardLoader.sourceComponent = oldComponent
    }

    CheckersModel
    {
        id: checkersModel
        aiMode: page.aiMode
        aiLevel: page.aiLevel

        onBoardStateChanged:
        {
            updateTrigger++
        }

        onWhiteTurnChanged:
        {
            currentWhiteTurn = checkersModel.isWhiteTurn()

            if (!gameEnded && timeMode !== 0 && !checkersModel.isGameOver())
            {
                if (!checkersModel.isWhiteTurn())
                {
                    addTimeIncrement(true)
                }
                else
                {
                    addTimeIncrement(false)
                }
            }

            lastDrawOfferRejected = false
            updateTrigger++

            if (!checkersModel.isWhiteTurn() && !checkersModel.isGameOver() && page.aiMode)
            {
                aiMoveTimer.start()
            }
            else if (checkersModel.isWhiteTurn() && !checkersModel.isGameOver() && page.aiMode)
            {
                if (checkersModel.shouldOfferDraw() && !offerDrawPending && !gameEnded)
                {
                    offerDrawPending = true
                }
            }

            if (offerDrawPending && !checkersModel.isGameOver())
            {
                offerDrawPending = false
                if (aiMode && !checkersModel.isWhiteTurn())
                {
                    if (checkersModel.shouldAcceptDraw())
                    {
                        gameEnded = true
                        gameResultText = "Ничья"
                        checkersModel.gameResult = "Ничья"
                        updateTrigger++
                    }
                }
                else
                {
                    drawOfferDialog.fromPlayer = false
                    drawOfferDialog.open()
                }
            }
        }

        onGameOverChanged:
        {
            if (checkersModel.gameResult !== "")
            {
                gameEnded = true
                gameResultText = checkersModel.gameResult
                updateTrigger++
            }
        }
    }

    Timer
    {
        id: aiMoveTimer
        interval: 500
        onTriggered:
        {
            checkersModel.aiMove()
        }
    }

    Component
    {
        id: boardComponent
        Grid
        {
            id: checkersGrid
            rows: 8
            columns: 8
            spacing: 0
            width: 400
            height: 400

            Repeater
            {
                model: 64

                Rectangle
                {
                    width: checkersGrid.width / 8
                    height: checkersGrid.height / 8
                    color: (Math.floor(index / 8) + index) % 2 === 0 ? "#F0D9B5" : "#B58863"
                    border.color: "#333"
                    border.width: 0.5

                    property int row: Math.floor(index / 8)
                    property int col: index % 8

                    Rectangle
                    {
                        anchors.fill: parent
                        color:
                        {
                            for (var i = 0; i < page.validMoves.length; i++)
                            {
                                if (page.validMoves[i].row === row && page.validMoves[i].col === col)
                                {
                                    return "#88FF88"
                                }
                            }
                            return "transparent"
                        }
                        opacity: 0.6
                    }

                    Rectangle
                    {
                        anchors.fill: parent
                        color: (row === page.selectedRow && col === page.selectedCol) ? "#FFFF00" : "transparent"
                        opacity: 0.5
                    }

                    MouseArea
                    {
                        anchors.fill: parent
                        onClicked:
                        {
                            if (checkersModel.isGameOver() || gameEnded)
                            {
                                return
                            }
                            if (selectedRow === -1)
                            {
                                if (checkersModel.pieceAt(row, col) !== "")
                                {
                                    selectedRow = row
                                    selectedCol = col
                                    validMoves = checkersModel.getValidMoves(selectedRow, selectedCol)
                                }
                            }
                            else
                            {
                                var piece = checkersModel.pieceAt(selectedRow, selectedCol)
                                var fromRow = selectedRow
                                var fromCol = selectedCol
                                var toRow = row
                                var toCol = col
                                var capturedPiece = checkersModel.pieceAt(toRow, toCol)

                                checkersModel.movePiece(selectedRow, selectedCol, row, col)
                                moveHistory.push({
                                    fromRow: fromRow,
                                    fromCol: fromCol,
                                    toRow: toRow,
                                    toCol: toCol,
                                    piece: piece,
                                    captured: capturedPiece
                                })

                                selectedRow = -1
                                selectedCol = -1
                                validMoves = []
                            }
                        }
                    }

                    Rectangle
                    {
                        anchors.centerIn: parent
                        width: parent.width - 8
                        height: parent.height - 8
                        radius: width / 2
                        color:
                        {
                            var p = checkersModel.pieceAt(row, col)
                            if (p === "") return "transparent"
                            if (p === "w" || p === "W") return "white"
                            if (p === "b" || p === "B") return "black"
                            return "transparent"
                        }
                        border.color: "gray"
                        border.width: 1

                        Text
                        {
                            anchors.centerIn: parent
                            text:
                            {
                                var p = checkersModel.pieceAt(row, col)
                                if (p === "W") return "♔"
                                if (p === "B") return "♚"
                                return ""
                            }
                            font.pixelSize: parent.width * 0.5
                            color:
                            {
                                var p = checkersModel.pieceAt(row, col)
                                if (p === "W") return "gold"
                                if (p === "B") return "gold"
                                return "transparent"
                            }
                            visible: (p === "W" || p === "B")
                        }
                    }
                }
            }
        }
    }
}
