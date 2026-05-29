import QtQuick 2.0
import Sailfish.Silica 1.0
import Chess 1.0

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
                boardModel.gameResult = "Ничья"
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
            if (boardModel.isWhiteTurn())
            {
                gameResultText = "Белые сдались. Чёрные победили"
                boardModel.gameResult = "Белые сдались. Чёрные победили"
            }
            else
            {
                gameResultText = "Чёрные сдались. Белые победили"
                boardModel.gameResult = "Чёрные сдались. Белые победили"
            }
            updateTrigger++
        }
    }

    Dialog
    {
        id: promotionDialog
        property bool isWhite: true
        property var onPieceSelected: null
        canAccept: false

        SilicaFlickable
        {
            anchors.fill: parent
            contentHeight: promoColumn.height + Theme.paddingLarge

            Column
            {
                id: promoColumn
                width: parent.width
                spacing: Theme.paddingMedium

                DialogHeader
                {
                    title: "Выбери фигуру"
                }

                Button
                {
                    text: promotionDialog.isWhite ? "♕  Ферзь" : "♛  Ферзь"
                    width: parent.width - 2 * Theme.horizontalPageMargin
                    anchors.horizontalCenter: parent.horizontalCenter
                    onClicked:
                    {
                        if (promotionDialog.onPieceSelected)
                        {
                            promotionDialog.onPieceSelected(promotionDialog.isWhite ? "♕" : "♛")
                        }
                        promotionDialog.accept()
                    }
                }

                Button
                {
                    text: promotionDialog.isWhite ? "♖  Ладья" : "♜  Ладья"
                    width: parent.width - 2 * Theme.horizontalPageMargin
                    anchors.horizontalCenter: parent.horizontalCenter
                    onClicked:
                    {
                        if (promotionDialog.onPieceSelected)
                        {
                            promotionDialog.onPieceSelected(promotionDialog.isWhite ? "♖" : "♜")
                        }
                        promotionDialog.accept()
                    }
                }

                Button
                {
                    text: promotionDialog.isWhite ? "♗  Слон" : "♝  Слон"
                    width: parent.width - 2 * Theme.horizontalPageMargin
                    anchors.horizontalCenter: parent.horizontalCenter
                    onClicked:
                    {
                        if (promotionDialog.onPieceSelected)
                        {
                            promotionDialog.onPieceSelected(promotionDialog.isWhite ? "♗" : "♝")
                        }
                        promotionDialog.accept()
                    }
                }

                Button
                {
                    text: promotionDialog.isWhite ? "♘  Конь" : "♞  Конь"
                    width: parent.width - 2 * Theme.horizontalPageMargin
                    anchors.horizontalCenter: parent.horizontalCenter
                    onClicked:
                    {
                        if (promotionDialog.onPieceSelected)
                        {
                            promotionDialog.onPieceSelected(promotionDialog.isWhite ? "♘" : "♞")
                        }
                        promotionDialog.accept()
                    }
                }
            }
        }
    }

    Timer
    {
        id: clockTimer
        interval: 1000
        running: !boardModel.isGameOver() && !gameEnded && (timeMode !== 0) && !(aiMode && !boardModel.isWhiteTurn())
        repeat: true
        onTriggered:
        {
            if (boardModel.isWhiteTurn())
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
                    boardModel.gameResult = "Белые проиграли по времени"
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
                    boardModel.gameResult = "Чёрные проиграли по времени"
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
                if (aiMode && !boardModel.isWhiteTurn())
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

    onUpdateTriggerChanged:
    {
        var oldComponent = boardLoader.sourceComponent
        boardLoader.sourceComponent = undefined
        boardLoader.sourceComponent = oldComponent
    }

    BoardModel
    {
        id: boardModel
        aiMode: page.aiMode
        aiLevel: page.aiLevel

        onBoardStateChanged:
        {
            updateTrigger++
        }

        onWhiteTurnChanged:
        {
            currentWhiteTurn = boardModel.isWhiteTurn()

            if (!gameEnded && timeMode !== 0 && !boardModel.isGameOver())
            {
                if (!boardModel.isWhiteTurn())
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

            if (!boardModel.isWhiteTurn() && !boardModel.isGameOver() && page.aiMode)
            {
                aiMoveTimer.start()
            }
            else if (boardModel.isWhiteTurn() && !boardModel.isGameOver() && page.aiMode)
            {
                if (boardModel.shouldOfferDraw() && !offerDrawPending && !gameEnded)
                {
                    offerDrawPending = true
                }
            }

            if (offerDrawPending && !boardModel.isGameOver())
            {
                offerDrawPending = false
                if (aiMode && !boardModel.isWhiteTurn())
                {
                    if (boardModel.shouldAcceptDraw())
                    {
                        gameEnded = true
                        gameResultText = "Ничья"
                        boardModel.gameResult = "Ничья"
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
            if (boardModel.gameResult !== "")
            {
                gameEnded = true
                gameResultText = boardModel.gameResult
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
            boardModel.aiMove()
        }
    }

    Component
    {
        id: boardComponent
        Grid
        {
            id: chessGrid
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
                    width: chessGrid.width / 8
                    height: chessGrid.height / 8
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
                            if (boardModel.isGameOver() || gameEnded)
                            {
                                return
                            }
                            if (selectedRow === -1)
                            {
                                if (boardModel.pieceAt(row, col) !== "")
                                {
                                    selectedRow = row
                                    selectedCol = col
                                    validMoves = boardModel.getValidMoves(selectedRow, selectedCol)
                                }
                            }
                            else
                            {
                                var piece = boardModel.pieceAt(selectedRow, selectedCol)
                                var isPawnPromotion = (piece === "♙" && row === 0) || (piece === "♟" && row === 7)
                                var fromRow = selectedRow
                                var fromCol = selectedCol
                                var toRow = row
                                var toCol = col

                                if (isPawnPromotion)
                                {
                                    promotionDialog.isWhite = (piece === "♙")
                                    promotionDialog.onPieceSelected = function(selectedPiece)
                                    {
                                        boardModel.movePiece(fromRow, fromCol, toRow, toCol)
                                        boardModel.promotePawn(toRow, toCol, selectedPiece)
                                    }
                                    promotionDialog.open()
                                }
                                else
                                {
                                    boardModel.movePiece(selectedRow, selectedCol, row, col)
                                }

                                selectedRow = -1
                                selectedCol = -1
                                validMoves = []
                            }
                        }
                    }

                    Image
                    {
                        anchors.centerIn: parent
                        width: parent.width - 4
                        height: parent.height - 4
                        fillMode: Image.PreserveAspectFit
                        source:
                        {
                            var p = boardModel.pieceAt(row, col)
                            if (p === "") return ""
                            if (p === "♜") return "Chess_rdt45.svg"
                            if (p === "♞") return "Chess_ndt45.svg"
                            if (p === "♝") return "Chess_bdt45.svg"
                            if (p === "♛") return "Chess_qdt45.svg"
                            if (p === "♚") return "Chess_kdt45.svg"
                            if (p === "♟") return "Chess_pdt45.svg"
                            if (p === "♖") return "Chess_rlt45.svg"
                            if (p === "♘") return "Chess_nlt45.svg"
                            if (p === "♗") return "Chess_blt45.svg"
                            if (p === "♕") return "Chess_qlt45.svg"
                            if (p === "♔") return "Chess_klt45.svg"
                            if (p === "♙") return "Chess_plt45.svg"
                            return ""
                        }
                    }
                }
            }
        }
    }
}
