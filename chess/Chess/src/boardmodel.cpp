#include "boardmodel.h"

BoardModel::BoardModel(QObject *parent) : QObject(parent)
{
    initBoard();
}

void BoardModel::initBoard()
{
    m_board.resize(8);
    for (int i = 0; i < 8; ++i)
    {
        m_board[i].resize(8);
        for (int j = 0; j < 8; ++j)
        {
            m_board[i][j] = "";
        }
    }

    m_board[0][0] = "♜"; m_board[0][7] = "♜";
    m_board[0][1] = "♞"; m_board[0][6] = "♞";
    m_board[0][2] = "♝"; m_board[0][5] = "♝";
    m_board[0][3] = "♛"; m_board[0][4] = "♚";
    for (int i = 0; i < 8; ++i) m_board[1][i] = "♟";

    m_board[7][0] = "♖"; m_board[7][7] = "♖";
    m_board[7][1] = "♘"; m_board[7][6] = "♘";
    m_board[7][2] = "♗"; m_board[7][5] = "♗";
    m_board[7][3] = "♕"; m_board[7][4] = "♔";
    for (int i = 0; i < 8; ++i) m_board[6][i] = "♙";
}

QString BoardModel::boardState() const
{
    return "";
}

QString BoardModel::pieceAt(int row, int col) const
{
    if (row < 0 || row >= 8 || col < 0 || col >= 8) return "";
    return m_board[row][col];
}

void BoardModel::movePiece(int fromRow, int fromCol, int toRow, int toCol)
{
    QString piece = pieceAt(fromRow, fromCol);
    if (piece.isEmpty()) return;

    m_board[toRow][toCol] = piece;
    m_board[fromRow][fromCol] = "";
    emit boardStateChanged();
}
