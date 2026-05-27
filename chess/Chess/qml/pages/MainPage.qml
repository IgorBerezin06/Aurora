import QtQuick 2.0
import Sailfish.Silica 1.0
import Chess 1.0

Page
{
    id: page

    property bool aiMode: false
    property int aiLevel: 1
    property int selectedRow: -1
    property int selectedCol: -1
    property int updateTrigger: 0
    property var validMoves: []

    Dialog
    {
        id: promotionDialog
        property bool isWhite: true
        property var onPieceSelected: null

        SilicaFlickable
        {
            anchors.fill: parent
            contentHeight: column.height + Theme.paddingLarge

            Column
            {
                id: column
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
                        promotionDialog.close()
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
                        promotionDialog.close()
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
                        promotionDialog.close()
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
                        promotionDialog.close()
                    }
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

    BoardModel
    {
        id: boardModel
        aiMode: page.aiMode
        aiLevel: page.aiLevel

        onBoardStateChanged: updateTrigger++
        onWhiteTurnChanged:
        {
            updateTrigger++
            if (!boardModel.isWhiteTurn() && !boardModel.isGameOver() && page.aiMode)
            {
                aiMoveTimer.start()
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

    Label
    {
        text: "Шахматы"
        anchors.horizontalCenter: parent.horizontalCenter
        y: 20
        font.pixelSize: 36
        color: "white"
    }

    Label
    {
        text: (updateTrigger, boardModel.isWhiteTurn()) ? "Ход белых" : "Ход чёрных"
        anchors.horizontalCenter: parent.horizontalCenter
        y: 80
        font.pixelSize: 24
        color: "white"
    }

    Label
    {
        text: boardModel.gameResult
        anchors.horizontalCenter: parent.horizontalCenter
        y: 120
        font.pixelSize: 28
        color: "red"
        visible: boardModel.gameResult !== ""
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
                            if (boardModel.isGameOver())
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
