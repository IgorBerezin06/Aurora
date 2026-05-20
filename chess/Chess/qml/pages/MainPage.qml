import QtQuick 2.0
import Sailfish.Silica 1.0
import Chess 1.0

Page
{
    id: page

    property int selectedRow: -1
    property int selectedCol: -1
    property int updateTrigger: 0
    property var validMoves: []

    BoardModel
    {
        id: boardModel
        onBoardStateChanged:
        {
            updateTrigger++
        }
        onWhiteTurnChanged:
        {
            updateTrigger++
        }
    }

    Rectangle {
        anchors.fill: parent
        color: "#2B2B2B"
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
                            if (boardModel.isGameOver()) return
                            if (selectedRow === -1)
                            {
                                if (boardModel.pieceAt(row, col) !== "")
                                {
                                    selectedRow = row
                                    selectedCol = col
                                    validMoves = boardModel.getValidMoves(selectedRow, selectedCol)
                                    console.log("Selected:", row, col, "Moves:", validMoves.length)
                                }
                            }
                            else
                            {
                                console.log("Move from", selectedRow, selectedCol, "to", row, col)
                                boardModel.movePiece(selectedRow, selectedCol, row, col)
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
                            var piece = boardModel.pieceAt(row, col)
                            if (piece === "") return ""
                            if (piece === "♜") return "Chess_rdt45.svg"
                            if (piece === "♞") return "Chess_ndt45.svg"
                            if (piece === "♝") return "Chess_bdt45.svg"
                            if (piece === "♛") return "Chess_qdt45.svg"
                            if (piece === "♚") return "Chess_kdt45.svg"
                            if (piece === "♟") return "Chess_pdt45.svg"
                            if (piece === "♖") return "Chess_rlt45.svg"
                            if (piece === "♘") return "Chess_nlt45.svg"
                            if (piece === "♗") return "Chess_blt45.svg"
                            if (piece === "♕") return "Chess_qlt45.svg"
                            if (piece === "♔") return "Chess_klt45.svg"
                            if (piece === "♙") return "Chess_plt45.svg"
                            return ""
                        }
                    }
                }
            }
        }
    }
}
