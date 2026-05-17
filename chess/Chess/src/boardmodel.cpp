#include "boardmodel.h"
#include <QDebug>

BoardModel::BoardModel(QObject *parent) : QObject(parent)
{
    initBoard();
    m_whiteTurn = true;
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
    for (int i = 0; i < 8; ++i)
    {
        m_board[1][i] = "♟";
    }

    m_board[7][0] = "♖"; m_board[7][7] = "♖";
    m_board[7][1] = "♘"; m_board[7][6] = "♘";
    m_board[7][2] = "♗"; m_board[7][5] = "♗";
    m_board[7][3] = "♕"; m_board[7][4] = "♔";
    for (int i = 0; i < 8; ++i)
    {
        m_board[6][i] = "♙";
    }
}

QString BoardModel::boardState() const
{
    return "";
}

QString BoardModel::pieceAt(int row, int col) const
{
    if (row < 0 || row >= 8 || col < 0 || col >= 8)
    {
        return "";
    }
    return m_board[row][col];
}

bool BoardModel::isValidMove(int fromRow, int fromCol, int toRow, int toCol) const
{
    QString piece = pieceAt(fromRow, fromCol);
    if (piece.isEmpty())
    {
        return false;
    }

    QString target = pieceAt(toRow, toCol);
    bool sameColor = false;
    if (!target.isEmpty())
    {
        sameColor = (piece == "♙" || piece == "♖" || piece == "♘" || piece == "♗" || piece == "♕" || piece == "♔") ?
                        (target == "♙" || target == "♖" || target == "♘" || target == "♗" || target == "♕" || target == "♔") :
                        (target == "♟" || target == "♜" || target == "♞" || target == "♝" || target == "♛" || target == "♚");
        if (sameColor)
        {
            return false;
        }
    }

    int dr = toRow - fromRow;
    int dc = toCol - fromCol;

    if (piece == "♙")
    {
        if (dr == -1 && dc == 0 && target.isEmpty()) return true;
        if (fromRow == 6 && dr == -2 && dc == 0 && target.isEmpty() && pieceAt(5, fromCol).isEmpty()) return true;
        if (dr == -1 && abs(dc) == 1 && !target.isEmpty()) return true;
        return false;
    }

    if (piece == "♟")
    {
        if (dr == 1 && dc == 0 && target.isEmpty()) return true;
        if (fromRow == 1 && dr == 2 && dc == 0 && target.isEmpty() && pieceAt(2, fromCol).isEmpty()) return true;
        if (dr == 1 && abs(dc) == 1 && !target.isEmpty()) return true;
        return false;
    }

    if (piece == "♖" || piece == "♜")
    {
        if (fromRow != toRow && fromCol != toCol)
        {
            return false;
        }
        int stepRow = (toRow - fromRow) == 0 ? 0 : (toRow - fromRow) / abs(toRow - fromRow);
        int stepCol = (toCol - fromCol) == 0 ? 0 : (toCol - fromCol) / abs(toCol - fromCol);
        int r = fromRow + stepRow, c = fromCol + stepCol;
        while (r != toRow || c != toCol)
        {
            if (!pieceAt(r, c).isEmpty())
            {
                return false;
            }
            r += stepRow;
            c += stepCol;
        }
        return true;
    }

    if (piece == "♗" || piece == "♝")
    {
        if (abs(dr) != abs(dc))
        {
            return false;
        }
        int stepRow = dr > 0 ? 1 : -1;
        int stepCol = dc > 0 ? 1 : -1;
        int r = fromRow + stepRow, c = fromCol + stepCol;
        while (r != toRow || c != toCol)
        {
            if (!pieceAt(r, c).isEmpty())
            {
                return false;
            }
            r += stepRow;
            c += stepCol;
        }
        return true;
    }

    if (piece == "♕" || piece == "♛")
    {
        if (fromRow == toRow || fromCol == toCol)
        {
            int stepRow = (toRow - fromRow) == 0 ? 0 : (toRow - fromRow) / abs(toRow - fromRow);
            int stepCol = (toCol - fromCol) == 0 ? 0 : (toCol - fromCol) / abs(toCol - fromCol);
            int r = fromRow + stepRow, c = fromCol + stepCol;
            while (r != toRow || c != toCol)
            {
                if (!pieceAt(r, c).isEmpty())
                {
                    return false;
                }
                r += stepRow;
                c += stepCol;
            }
            return true;
        }
        if (abs(dr) == abs(dc))
        {
            int stepRow = dr > 0 ? 1 : -1;
            int stepCol = dc > 0 ? 1 : -1;
            int r = fromRow + stepRow, c = fromCol + stepCol;
            while (r != toRow || c != toCol)
            {
                if (!pieceAt(r, c).isEmpty())
                {
                    return false;
                }
                r += stepRow;
                c += stepCol;
            }
            return true;
        }
        return false;
    }

    if (piece == "♘" || piece == "♞")
    {
        return (abs(dr) == 2 && abs(dc) == 1) || (abs(dr) == 1 && abs(dc) == 2);
    }

    if (piece == "♔" || piece == "♚")
    {
        if (abs(dr) <= 1 && abs(dc) <= 1)
        {
            if (target.isEmpty() || !sameColor)
            {
                return true;
            }
        }
        return false;
    }

    return false;
}

bool BoardModel::isKingInCheck(bool whiteKing) const
{
    int kingRow = -1, kingCol = -1;
    for (int i = 0; i < 8; ++i)
    {
        for (int j = 0; j < 8; ++j)
        {
            QString piece = m_board[i][j];
            if (piece == (whiteKing ? "♔" : "♚"))
            {
                kingRow = i;
                kingCol = j;
                break;
            }
        }
        if (kingRow != -1)
        {
            break;
        }
    }

    if (kingRow == -1)
    {
        return false;
    }

    for (int i = 0; i < 8; ++i)
    {
        for (int j = 0; j < 8; ++j)
        {
            QString piece = m_board[i][j];
            if (piece.isEmpty())
            {
                continue;
            }

            bool isWhite = (piece == "♙" || piece == "♖" || piece == "♘" || piece == "♗" || piece == "♕" || piece == "♔");
            if (isWhite == whiteKing)
            {
                continue;
            }

            if (isValidMove(i, j, kingRow, kingCol))
            {
                return true;
            }
        }
    }
    return false;
}

void BoardModel::movePiece(int fromRow, int fromCol, int toRow, int toCol)
{
    QString piece = pieceAt(fromRow, fromCol);
    if (piece.isEmpty())
    {
        return;
    }

    bool isWhitePiece = (piece == "♙" || piece == "♖" || piece == "♘" || piece == "♗" || piece == "♕" || piece == "♔");
    if ((m_whiteTurn && !isWhitePiece) || (!m_whiteTurn && isWhitePiece))
    {
        qDebug() << "Not your turn!";
        return;
    }

    if (!isValidMove(fromRow, fromCol, toRow, toCol))
    {
        qDebug() << "Invalid move";
        return;
    }

    m_board[toRow][toCol] = piece;
    m_board[fromRow][fromCol] = "";
    m_whiteTurn = !m_whiteTurn;
    emit whiteTurnChanged();
    emit boardStateChanged();
}

bool BoardModel::isWhiteTurn() const
{
    return m_whiteTurn;
}
