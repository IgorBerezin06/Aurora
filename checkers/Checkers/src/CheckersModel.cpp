#include "CheckersModel.h"
#include <QDebug>
#include <QVariantMap>
#include <cstdlib>

CheckersModel::CheckersModel(QObject *parent) : QObject(parent)
{
    initBoard();
    m_whiteTurn = true;
    m_gameOver = false;
    m_gameResult = "";
    m_aiMode = false;
    m_aiLevel = 1;
}

void CheckersModel::initBoard()
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

    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 8; ++j)
        {
            if ((i + j) % 2 == 1)
            {
                m_board[i][j] = "b";
            }
        }
    }

    for (int i = 5; i < 8; ++i)
    {
        for (int j = 0; j < 8; ++j)
        {
            if ((i + j) % 2 == 1)
            {
                m_board[i][j] = "w";
            }
        }
    }
}

QString CheckersModel::boardState() const
{
    return "";
}

QString CheckersModel::pieceAt(int row, int col) const
{
    if (row < 0 || row >= 8 || col < 0 || col >= 8)
    {
        return "";
    }
    return m_board[row][col];
}

bool CheckersModel::isCaptureMove(int fromRow, int fromCol, int toRow, int toCol) const
{
    int dr = toRow - fromRow;
    int dc = toCol - fromCol;
    return abs(dr) == 2 && abs(dc) == 2;
}

QVector<QVector<int>> CheckersModel::getAllCaptures(int row, int col, bool white, QVector<QVector<int>> currentChain) const
{
    QVector<QVector<int>> allChains;
    QString piece = m_board[row][col];
    bool isKing = (piece == "W" || piece == "B");
    int directions[4][2] = {{-1, -1}, {-1, 1}, {1, -1}, {1, 1}};

    for (int d = 0; d < 4; ++d)
    {
        int dr = directions[d][0];
        int dc = directions[d][1];
        int captureRow = row + dr;
        int captureCol = col + dc;
        int landRow = row + 2 * dr;
        int landCol = col + 2 * dc;

        if (landRow >= 0 && landRow < 8 && landCol >= 0 && landCol < 8)
        {
            QString target = pieceAt(captureRow, captureCol);
            QString land = pieceAt(landRow, landCol);

            if (!target.isEmpty() && land.isEmpty())
            {
                bool targetIsWhite = (target == "w" || target == "W");
                if (targetIsWhite != white)
                {
                    QVector<QVector<int>> newChain = currentChain;
                    newChain.append({row, col, landRow, landCol, captureRow, captureCol});
                    QVector<QVector<int>> subChains = getAllCaptures(landRow, landCol, white, newChain);
                    if (subChains.isEmpty())
                    {
                        allChains.append(newChain);
                    }
                    else
                    {
                        for (const auto& chain : subChains)
                        {
                            allChains.append(chain);
                        }
                    }
                }
            }
        }
    }

    if (isKing)
    {
        for (int d = 0; d < 4; ++d)
        {
            for (int dist = 1; dist <= 7; ++dist)
            {
                int captureRow = row + directions[d][0] * dist;
                int captureCol = col + directions[d][1] * dist;
                int landRow = row + directions[d][0] * (dist + 1);
                int landCol = col + directions[d][1] * (dist + 1);

                if (captureRow < 0 || captureRow >= 8 || captureCol < 0 || captureCol >= 8)
                {
                    break;
                }
                if (landRow < 0 || landRow >= 8 || landCol < 0 || landCol >= 8)
                {
                    break;
                }

                QString target = pieceAt(captureRow, captureCol);
                QString land = pieceAt(landRow, landCol);

                if (!target.isEmpty() && land.isEmpty())
                {
                    bool targetIsWhite = (target == "w" || target == "W");
                    if (targetIsWhite != white)
                    {
                        QVector<QVector<int>> newChain = currentChain;
                        newChain.append({row, col, landRow, landCol, captureRow, captureCol});
                        QVector<QVector<int>> subChains = getAllCaptures(landRow, landCol, white, newChain);
                        if (subChains.isEmpty())
                        {
                            allChains.append(newChain);
                        }
                        else
                        {
                            for (const auto& chain : subChains)
                            {
                                allChains.append(chain);
                            }
                        }
                        break;
                    }
                }
                else if (!land.isEmpty())
                {
                    break;
                }
            }
        }
    }

    return allChains;
}

bool CheckersModel::isValidMove(int fromRow, int fromCol, int toRow, int toCol) const
{
    QString piece = pieceAt(fromRow, fromCol);
    if (piece.isEmpty())
    {
        return false;
    }

    QString target = pieceAt(toRow, toCol);
    if (!target.isEmpty())
    {
        return false;
    }

    bool isWhite = (piece == "w" || piece == "W");
    if ((m_whiteTurn && !isWhite) || (!m_whiteTurn && isWhite))
    {
        return false;
    }

    int dr = toRow - fromRow;
    int dc = toCol - fromCol;
    bool isKing = (piece == "W" || piece == "B");

    bool hasAnyCapture = false;
    for (int i = 0; i < 8; ++i)
    {
        for (int j = 0; j < 8; ++j)
        {
            QString p = pieceAt(i, j);
            if (!p.isEmpty())
            {
                bool pIsWhite = (p == "w" || p == "W");
                if (pIsWhite == isWhite)
                {
                    QVector<QVector<int>> captures = getAllCaptures(i, j, isWhite, {});
                    if (!captures.isEmpty())
                    {
                        hasAnyCapture = true;
                        break;
                    }
                }
            }
        }
        if (hasAnyCapture) break;
    }

    if (hasAnyCapture)
    {
        if (abs(dr) != 2 || abs(dc) != 2)
        {
            return false;
        }
        int captureRow = (fromRow + toRow) / 2;
        int captureCol = (fromCol + toCol) / 2;
        QString captured = pieceAt(captureRow, captureCol);
        if (captured.isEmpty())
        {
            return false;
        }
        bool capturedIsWhite = (captured == "w" || captured == "W");
        if (capturedIsWhite == isWhite)
        {
            return false;
        }
        return true;
    }

    if (!isKing)
    {
        if (isWhite)
        {
            if (dr != -1 || abs(dc) != 1)
            {
                return false;
            }
        }
        else
        {
            if (dr != 1 || abs(dc) != 1)
            {
                return false;
            }
        }
        return true;
    }
    else
    {
        if (abs(dr) != abs(dc))
        {
            return false;
        }
        int stepRow = (dr > 0) ? 1 : -1;
        int stepCol = (dc > 0) ? 1 : -1;
        int r = fromRow + stepRow;
        int c = fromCol + stepCol;
        bool piecePassed = false;
        while (r != toRow || c != toCol)
        {
            if (!pieceAt(r, c).isEmpty())
            {
                if (!piecePassed)
                {
                    piecePassed = true;
                }
                else
                {
                    return false;
                }
            }
            r += stepRow;
            c += stepCol;
        }
        return true;
    }
}

bool CheckersModel::wouldBeKingAfterMove(int row, int col) const
{
    QString piece = pieceAt(row, col);
    if (piece == "w" && row == 0)
    {
        return true;
    }
    if (piece == "b" && row == 7)
    {
        return true;
    }
    return false;
}

bool CheckersModel::hasLegalMoves(bool white) const
{
    for (int i = 0; i < 8; ++i)
    {
        for (int j = 0; j < 8; ++j)
        {
            QString piece = pieceAt(i, j);
            if (!piece.isEmpty())
            {
                bool isWhite = (piece == "w" || piece == "W");
                if (isWhite == white)
                {
                    for (int ti = 0; ti < 8; ++ti)
                    {
                        for (int tj = 0; tj < 8; ++tj)
                        {
                            if (isValidMove(i, j, ti, tj))
                            {
                                return true;
                            }
                        }
                    }
                }
            }
        }
    }
    return false;
}

void CheckersModel::movePiece(int fromRow, int fromCol, int toRow, int toCol)
{
    if (m_gameOver)
    {
        return;
    }

    QString piece = pieceAt(fromRow, fromCol);
    if (piece.isEmpty())
    {
        return;
    }

    bool isWhite = (piece == "w" || piece == "W");
    if ((m_whiteTurn && !isWhite) || (!m_whiteTurn && isWhite))
    {
        return;
    }

    if (!isValidMove(fromRow, fromCol, toRow, toCol))
    {
        return;
    }

    QString captured = "";
    bool isCapture = (abs(toRow - fromRow) == 2);

    if (isCapture)
    {
        int captureRow = (fromRow + toRow) / 2;
        int captureCol = (fromCol + toCol) / 2;
        captured = pieceAt(captureRow, captureCol);
        m_board[captureRow][captureCol] = "";
    }

    MoveRecord record;
    record.fromRow = fromRow;
    record.fromCol = fromCol;
    record.toRow = toRow;
    record.toCol = toCol;
    record.piece = piece;
    record.captured = captured;
    m_moveHistory.append(record);

    m_board[toRow][toCol] = piece;
    m_board[fromRow][fromCol] = "";

    if (wouldBeKingAfterMove(toRow, toCol))
    {
        m_board[toRow][toCol] = (piece == "w") ? "W" : "B";
    }

    m_whiteTurn = !m_whiteTurn;

    if (!hasLegalMoves(m_whiteTurn))
    {
        m_gameOver = true;
        if (m_whiteTurn)
        {
            m_gameResult = "Чёрные победили";
        }
        else
        {
            m_gameResult = "Белые победили";
        }
    }

    emit boardStateChanged();
    emit whiteTurnChanged();
}

QVariantList CheckersModel::getValidMoves(int row, int col)
{
    QVariantList moves;
    QString piece = pieceAt(row, col);
    if (piece.isEmpty())
    {
        return moves;
    }

    for (int i = 0; i < 8; ++i)
    {
        for (int j = 0; j < 8; ++j)
        {
            if (isValidMove(row, col, i, j))
            {
                QVariantMap move;
                move["row"] = i;
                move["col"] = j;
                moves.append(move);
            }
        }
    }
    return moves;
}

bool CheckersModel::isWhiteTurn() const
{
    return m_whiteTurn;
}

bool CheckersModel::isGameOver() const
{
    return m_gameOver;
}

QString CheckersModel::gameResult() const
{
    return m_gameResult;
}

bool CheckersModel::aiMode() const
{
    return m_aiMode;
}

void CheckersModel::setAiMode(bool mode)
{
    if (m_aiMode != mode)
    {
        m_aiMode = mode;
        emit aiModeChanged();
    }
}

int CheckersModel::aiLevel() const
{
    return m_aiLevel;
}

void CheckersModel::setAiLevel(int level)
{
    if (m_aiLevel != level)
    {
        m_aiLevel = level;
        emit aiLevelChanged();
    }
}

int CheckersModel::evaluatePosition() const
{
    int whiteScore = 0;
    int blackScore = 0;

    for (int i = 0; i < 8; ++i)
    {
        for (int j = 0; j < 8; ++j)
        {
            QString piece = m_board[i][j];
            if (piece == "w") whiteScore += 10;
            else if (piece == "W") whiteScore += 30;
            else if (piece == "b") blackScore += 10;
            else if (piece == "B") blackScore += 30;
        }
    }

    return whiteScore - blackScore;
}

void CheckersModel::aiMove()
{
    if (m_gameOver)
    {
        return;
    }
    if (m_whiteTurn)
    {
        return;
    }

    QVector<QVector<int>> moves;
    for (int i = 0; i < 8; ++i)
    {
        for (int j = 0; j < 8; ++j)
        {
            QString piece = pieceAt(i, j);
            if (piece.isEmpty())
            {
                continue;
            }
            bool isWhite = (piece == "w" || piece == "W");
            if (isWhite)
            {
                continue;
            }

            for (int ti = 0; ti < 8; ++ti)
            {
                for (int tj = 0; tj < 8; ++tj)
                {
                    if (isValidMove(i, j, ti, tj))
                    {
                        moves.append({i, j, ti, tj});
                    }
                }
            }
        }
    }

    if (moves.isEmpty())
    {
        return;
    }

    if (m_aiLevel == 1)
    {
        int idx = rand() % moves.size();
        movePiece(moves[idx][0], moves[idx][1], moves[idx][2], moves[idx][3]);
    }
    else
    {
        int bestScore = -1000000;
        QVector<int> bestMove;
        for (const auto& move : moves)
        {
            int score = 0;
            bool isCapture = (abs(move[2] - move[0]) == 2);
            if (isCapture)
            {
                int captureRow = (move[0] + move[2]) / 2;
                int captureCol = (move[1] + move[3]) / 2;
                QString captured = pieceAt(captureRow, captureCol);
                if (captured == "w") score += 10;
                else if (captured == "W") score += 30;
                else if (captured == "b") score += 10;
                else if (captured == "B") score += 30;
            }

            if ((move[2] == 3 || move[2] == 4) && (move[3] == 3 || move[3] == 4))
            {
                score += 5;
            }

            if (score > bestScore)
            {
                bestScore = score;
                bestMove = move;
            }
        }
        if (!bestMove.isEmpty())
        {
            movePiece(bestMove[0], bestMove[1], bestMove[2], bestMove[3]);
        }
    }
}

void CheckersModel::resetBoard()
{
    initBoard();
    m_whiteTurn = true;
    m_gameOver = false;
    m_gameResult = "";
    m_moveHistory.clear();

    emit boardStateChanged();
    emit whiteTurnChanged();
    emit gameOverChanged();
}

void CheckersModel::undoMove(int fromRow, int fromCol, int toRow, int toCol, const QString& piece, const QString& captured)
{
    m_board[fromRow][fromCol] = piece;
    m_board[toRow][toCol] = "";

    if (!captured.isEmpty())
    {
        int captureRow = (fromRow + toRow) / 2;
        int captureCol = (fromCol + toCol) / 2;
        m_board[captureRow][captureCol] = captured;
    }

    m_whiteTurn = !m_whiteTurn;
    m_gameOver = false;
    m_gameResult = "";

    if (m_moveHistory.size() > 0)
    {
        m_moveHistory.removeLast();
    }

    emit boardStateChanged();
    emit whiteTurnChanged();
    emit gameOverChanged();
}

bool CheckersModel::shouldAcceptDraw() const
{
    if (m_aiLevel == 1)
    {
        return (rand() % 100) < 20;
    }
    else
    {
        return (rand() % 100) < 40;
    }
}

bool CheckersModel::shouldOfferDraw() const
{
    if (m_gameOver)
    {
        return false;
    }

    if (m_aiLevel == 1)
    {
        return (rand() % 100) < 5;
    }
    else
    {
        return (rand() % 100) < 15;
    }
}
