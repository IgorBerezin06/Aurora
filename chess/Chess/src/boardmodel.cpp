#include "boardmodel.h"
#include <QDebug>

BoardModel::BoardModel(QObject *parent) : QObject(parent)
{
    initBoard();
    m_whiteTurn = true;
    m_gameOver = false;
    m_gameResult = "";

    m_whiteKingMoved = false;
    m_whiteRookMovedKingside = false;
    m_whiteRookMovedQueenside = false;
    m_blackKingMoved = false;
    m_blackRookMovedKingside = false;
    m_blackRookMovedQueenside = false;
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
        int r = fromRow + stepRow;
        int c = fromCol + stepCol;
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
        int r = fromRow + stepRow;
        int c = fromCol + stepCol;
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
            int r = fromRow + stepRow;
            int c = fromCol + stepCol;
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
            int r = fromRow + stepRow;
            int c = fromCol + stepCol;
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

    if (piece == "♔" && !m_whiteKingMoved && fromRow == 7 && fromCol == 4)
    {
        if (toRow == 7 && toCol == 6 && !m_whiteRookMovedKingside)
        {
            if (pieceAt(7, 5).isEmpty() && pieceAt(7, 6).isEmpty() &&
                !isKingInCheck(true) && !wouldBeInCheckAfterMove(7, 4, 7, 5, true) && !wouldBeInCheckAfterMove(7, 4, 7, 6, true))
            {
                return true;
            }
        }
        if (toRow == 7 && toCol == 2 && !m_whiteRookMovedQueenside)
        {
            if (pieceAt(7, 3).isEmpty() && pieceAt(7, 2).isEmpty() && pieceAt(7, 1).isEmpty() &&
                !isKingInCheck(true) && !wouldBeInCheckAfterMove(7, 4, 7, 3, true) && !wouldBeInCheckAfterMove(7, 4, 7, 2, true))
            {
                return true;
            }
        }
    }

    if (piece == "♚" && !m_blackKingMoved && fromRow == 0 && fromCol == 4)
    {
        if (toRow == 0 && toCol == 6 && !m_blackRookMovedKingside)
        {
            if (pieceAt(0, 5).isEmpty() && pieceAt(0, 6).isEmpty() &&
                !isKingInCheck(false) && !wouldBeInCheckAfterMove(0, 4, 0, 5, false) && !wouldBeInCheckAfterMove(0, 4, 0, 6, false))
            {
                return true;
            }
        }
        if (toRow == 0 && toCol == 2 && !m_blackRookMovedQueenside)
        {
            if (pieceAt(0, 3).isEmpty() && pieceAt(0, 2).isEmpty() && pieceAt(0, 1).isEmpty() &&
                !isKingInCheck(false) && !wouldBeInCheckAfterMove(0, 4, 0, 3, false) && !wouldBeInCheckAfterMove(0, 4, 0, 2, false))
            {
                return true;
            }
        }
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

bool BoardModel::wouldBeInCheckAfterMove(int fromRow, int fromCol, int toRow, int toCol, bool whiteKing) const
{
    QVector<QVector<QString>> tempBoard = m_board;
    QString piece = tempBoard[fromRow][fromCol];
    tempBoard[toRow][toCol] = piece;
    tempBoard[fromRow][fromCol] = "";

    int kingRow = -1, kingCol = -1;
    for (int i = 0; i < 8; ++i)
    {
        for (int j = 0; j < 8; ++j)
        {
            QString p = tempBoard[i][j];
            if (p == (whiteKing ? "♔" : "♚"))
            {
                kingRow = i;
                kingCol = j;
                break;
            }
        }
        if (kingRow != -1) break;
    }

    if (kingRow == -1) return false;

    for (int i = 0; i < 8; ++i)
    {
        for (int j = 0; j < 8; ++j)
        {
            QString p = tempBoard[i][j];
            if (p.isEmpty()) continue;

            bool isWhite = (p == "♙" || p == "♖" || p == "♘" || p == "♗" || p == "♕" || p == "♔");
            if (isWhite == whiteKing) continue;

            int dr = kingRow - i;
            int dc = kingCol - j;

            if (p == "♙")
            {
                if (dr == -1 && abs(dc) == 1) return true;
                continue;
            }
            if (p == "♟")
            {
                if (dr == 1 && abs(dc) == 1) return true;
                continue;
            }
            if (p == "♖" || p == "♜")
            {
                if (i != kingRow && j != kingCol) continue;
                int stepRow = (kingRow - i) == 0 ? 0 : (kingRow - i) / abs(kingRow - i);
                int stepCol = (kingCol - j) == 0 ? 0 : (kingCol - j) / abs(kingCol - j);
                int r = i + stepRow, c = j + stepCol;
                bool blocked = false;
                while (r != kingRow || c != kingCol)
                {
                    if (!tempBoard[r][c].isEmpty())
                    {
                        blocked = true;
                        break;
                    }
                    r += stepRow;
                    c += stepCol;
                }
                if (!blocked) return true;
                continue;
            }
            if (p == "♗" || p == "♝")
            {
                if (abs(kingRow - i) != abs(kingCol - j)) continue;
                int stepRow = (kingRow - i) > 0 ? 1 : -1;
                int stepCol = (kingCol - j) > 0 ? 1 : -1;
                int r = i + stepRow, c = j + stepCol;
                bool blocked = false;
                while (r != kingRow || c != kingCol)
                {
                    if (!tempBoard[r][c].isEmpty())
                    {
                        blocked = true;
                        break;
                    }
                    r += stepRow;
                    c += stepCol;
                }
                if (!blocked) return true;
                continue;
            }
            if (p == "♕" || p == "♛")
            {
                if (i == kingRow || j == kingCol)
                {
                    int stepRow = (kingRow - i) == 0 ? 0 : (kingRow - i) / abs(kingRow - i);
                    int stepCol = (kingCol - j) == 0 ? 0 : (kingCol - j) / abs(kingCol - j);
                    int r = i + stepRow, c = j + stepCol;
                    bool blocked = false;
                    while (r != kingRow || c != kingCol)
                    {
                        if (!tempBoard[r][c].isEmpty())
                        {
                            blocked = true;
                            break;
                        }
                        r += stepRow;
                        c += stepCol;
                    }
                    if (!blocked) return true;
                }
                if (abs(kingRow - i) == abs(kingCol - j))
                {
                    int stepRow = (kingRow - i) > 0 ? 1 : -1;
                    int stepCol = (kingCol - j) > 0 ? 1 : -1;
                    int r = i + stepRow, c = j + stepCol;
                    bool blocked = false;
                    while (r != kingRow || c != kingCol)
                    {
                        if (!tempBoard[r][c].isEmpty())
                        {
                            blocked = true;
                            break;
                        }
                        r += stepRow;
                        c += stepCol;
                    }
                    if (!blocked) return true;
                }
                continue;
            }
            if (p == "♘" || p == "♞")
            {
                if ((abs(kingRow - i) == 2 && abs(kingCol - j) == 1) ||
                    (abs(kingRow - i) == 1 && abs(kingCol - j) == 2))
                {
                    return true;
                }
                continue;
            }
            if (p == "♔" || p == "♚")
            {
                if (abs(kingRow - i) <= 1 && abs(kingCol - j) <= 1)
                {
                    return true;
                }
                continue;
            }
        }
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

bool BoardModel::hasLegalMoves(bool white) const
{
    for (int i = 0; i < 8; ++i)
    {
        for (int j = 0; j < 8; ++j)
        {
            QString piece = m_board[i][j];
            if (piece.isEmpty()) continue;

            bool isWhite = (piece == "♙" || piece == "♖" || piece == "♘" || piece == "♗" || piece == "♕" || piece == "♔");
            if (isWhite != white) continue;

            for (int ti = 0; ti < 8; ++ti)
            {
                for (int tj = 0; tj < 8; ++tj)
                {
                    if (!isValidMove(i, j, ti, tj)) continue;
                    if (!wouldBeInCheckAfterMove(i, j, ti, tj, white))
                    {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

void BoardModel::movePiece(int fromRow, int fromCol, int toRow, int toCol)
{
    if (m_gameOver)
    {
        qDebug() << "Game is over, no more moves";
        return;
    }

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

    if (wouldBeInCheckAfterMove(fromRow, fromCol, toRow, toCol, m_whiteTurn))
    {
        qDebug() << "Move would put your own king in check!";
        return;
    }

    if ((piece == "♔" && fromRow == 7 && fromCol == 4 && toRow == 7 && toCol == 6) ||
        (piece == "♚" && fromRow == 0 && fromCol == 4 && toRow == 0 && toCol == 6))
    {
        m_board[toRow][toCol] = piece;
        m_board[fromRow][fromCol] = "";
        m_board[toRow][5] = (piece == "♔") ? "♖" : "♜";
        m_board[toRow][7] = "";
        if (piece == "♔")
        {
            m_whiteKingMoved = true;
            m_whiteRookMovedKingside = true;
        }
        else
        {
            m_blackKingMoved = true;
            m_blackRookMovedKingside = true;
        }
        m_whiteTurn = !m_whiteTurn;
        emit boardStateChanged();
        emit whiteTurnChanged();
        return;
    }

    if ((piece == "♔" && fromRow == 7 && fromCol == 4 && toRow == 7 && toCol == 2) ||
        (piece == "♚" && fromRow == 0 && fromCol == 4 && toRow == 0 && toCol == 2))
    {
        m_board[toRow][toCol] = piece;
        m_board[fromRow][fromCol] = "";
        m_board[toRow][3] = (piece == "♔") ? "♖" : "♜";
        m_board[toRow][0] = "";
        if (piece == "♔")
        {
            m_whiteKingMoved = true;
            m_whiteRookMovedQueenside = true;
        }
        else
        {
            m_blackKingMoved = true;
            m_blackRookMovedQueenside = true;
        }
        m_whiteTurn = !m_whiteTurn;
        emit boardStateChanged();
        emit whiteTurnChanged();
        return;
    }

    m_board[toRow][toCol] = piece;
    m_board[fromRow][fromCol] = "";

    if (piece == "♙" && toRow == 0)
    {
        m_board[toRow][toCol] = "♕";
    }
    if (piece == "♟" && toRow == 7)
    {
        m_board[toRow][toCol] = "♛";
    }

    if (piece == "♔") m_whiteKingMoved = true;
    if (piece == "♚") m_blackKingMoved = true;
    if (piece == "♖" && fromRow == 7 && fromCol == 7) m_whiteRookMovedKingside = true;
    if (piece == "♖" && fromRow == 7 && fromCol == 0) m_whiteRookMovedQueenside = true;
    if (piece == "♜" && fromRow == 0 && fromCol == 7) m_blackRookMovedKingside = true;
    if (piece == "♜" && fromRow == 0 && fromCol == 0) m_blackRookMovedQueenside = true;

    m_whiteTurn = !m_whiteTurn;

    bool isCheck = isKingInCheck(m_whiteTurn);
    bool hasMoves = hasLegalMoves(m_whiteTurn);

    if (!hasMoves)
    {
        m_gameOver = true;
        if (isCheck)
        {
            m_gameResult = m_whiteTurn ? "Чёрные победили (мат)" : "Белые победили (мат)";
        }
        else
        {
            m_gameResult = "Пат (ничья)";
        }
        qDebug() << "Game over:" << m_gameResult;
        emit gameOverChanged();
    }

    emit whiteTurnChanged();
    emit boardStateChanged();
}

bool BoardModel::isWhiteTurn() const
{
    return m_whiteTurn;
}

bool BoardModel::isGameOver() const
{
    return m_gameOver;
}

QString BoardModel::gameResult() const
{
    return m_gameResult;
}
