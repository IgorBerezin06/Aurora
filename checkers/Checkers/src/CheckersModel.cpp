#include "CheckersModel.h"
#include <QDebug>
#include <QVariantMap>
#include <cstdlib>
#include <QThread>
#include <QSet>

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
    m_board.clear();
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

bool CheckersModel::hasAnyCapture(bool white) const
{
    for (int i = 0; i < 8; ++i)
    {
        for (int j = 0; j < 8; ++j)
        {
            QString piece = pieceAt(i, j);
            if (piece.isEmpty()) continue;
            bool isWhite = (piece == "w" || piece == "W");
            if (isWhite != white) continue;
            bool isKing = (piece == "W" || piece == "B");

            int directions[4][2] = {{-1, -1}, {-1, 1}, {1, -1}, {1, 1}};
            for (int d = 0; d < 4; ++d)
            {
                int dr = directions[d][0];
                int dc = directions[d][1];

                if (!isKing)
                {
                    int captureRow = i + dr;
                    int captureCol = j + dc;
                    int landRow = i + 2 * dr;
                    int landCol = j + 2 * dc;
                    if (landRow >= 0 && landRow < 8 && landCol >= 0 && landCol < 8)
                    {
                        QString target = pieceAt(captureRow, captureCol);
                        QString land = pieceAt(landRow, landCol);
                        if (!target.isEmpty() && land.isEmpty())
                        {
                            bool targetWhite = (target == "w" || target == "W");
                            if (targetWhite != white)
                            {
                                return true;
                            }
                        }
                    }
                }
                else
                {
                    for (int dist = 1; dist <= 7; ++dist)
                    {
                        int captureRow = i + dr * dist;
                        int captureCol = j + dc * dist;
                        int landRow = i + dr * (dist + 1);
                        int landCol = j + dc * (dist + 1);
                        if (landRow < 0 || landRow >= 8 || landCol < 0 || landCol >= 8) break;
                        QString target = pieceAt(captureRow, captureCol);
                        QString land = pieceAt(landRow, landCol);
                        if (!target.isEmpty() && land.isEmpty())
                        {
                            bool targetWhite = (target == "w" || target == "W");
                            if (targetWhite != white)
                            {
                                return true;
                            }
                            break;
                        }
                        else if (!target.isEmpty())
                        {
                            break;
                        }
                    }
                }
            }
        }
    }
    return false;
}

void CheckersModel::findAllCaptures(int row, int col, bool white, QVector<QVector<int>>& result, QVector<int>& current, int depth)
{
    if (depth > 10) return;

    QString piece = pieceAt(row, col);
    bool isKing = (piece == "W" || piece == "B");
    int directions[4][2] = {{-1, -1}, {-1, 1}, {1, -1}, {1, 1}};
    bool foundCapture = false;

    for (int d = 0; d < 4; ++d)
    {
        int dr = directions[d][0];
        int dc = directions[d][1];

        if (!isKing)
        {
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
                    bool targetWhite = (target == "w" || target == "W");
                    if (targetWhite != white)
                    {
                        foundCapture = true;
                        current.append(row);
                        current.append(col);
                        current.append(landRow);
                        current.append(landCol);
                        current.append(captureRow);
                        current.append(captureCol);

                        QString savedTarget = target;
                        QString savedLand = land;

                        const_cast<CheckersModel*>(this)->m_board[captureRow][captureCol] = "";
                        const_cast<CheckersModel*>(this)->m_board[landRow][landCol] = piece;
                        const_cast<CheckersModel*>(this)->m_board[row][col] = "";

                        findAllCaptures(landRow, landCol, white, result, current, depth + 1);

                        const_cast<CheckersModel*>(this)->m_board[captureRow][captureCol] = savedTarget;
                        const_cast<CheckersModel*>(this)->m_board[landRow][landCol] = "";
                        const_cast<CheckersModel*>(this)->m_board[row][col] = piece;

                        current.removeLast();
                        current.removeLast();
                        current.removeLast();
                        current.removeLast();
                        current.removeLast();
                        current.removeLast();
                    }
                }
            }
        }
        else
        {
            for (int dist = 1; dist <= 7; ++dist)
            {
                int captureRow = row + dr * dist;
                int captureCol = col + dc * dist;
                int landRow = row + dr * (dist + 1);
                int landCol = col + dc * (dist + 1);
                if (landRow < 0 || landRow >= 8 || landCol < 0 || landCol >= 8) break;
                QString target = pieceAt(captureRow, captureCol);
                QString land = pieceAt(landRow, landCol);
                if (!target.isEmpty() && land.isEmpty())
                {
                    bool targetWhite = (target == "w" || target == "W");
                    if (targetWhite != white)
                    {
                        foundCapture = true;
                        current.append(row);
                        current.append(col);
                        current.append(landRow);
                        current.append(landCol);
                        current.append(captureRow);
                        current.append(captureCol);

                        QString savedTarget = target;
                        QString savedLand = land;
                        QString savedPiece = piece;

                        const_cast<CheckersModel*>(this)->m_board[captureRow][captureCol] = "";
                        const_cast<CheckersModel*>(this)->m_board[landRow][landCol] = piece;
                        const_cast<CheckersModel*>(this)->m_board[row][col] = "";

                        findAllCaptures(landRow, landCol, white, result, current, depth + 1);

                        const_cast<CheckersModel*>(this)->m_board[captureRow][captureCol] = savedTarget;
                        const_cast<CheckersModel*>(this)->m_board[landRow][landCol] = "";
                        const_cast<CheckersModel*>(this)->m_board[row][col] = savedPiece;

                        current.removeLast();
                        current.removeLast();
                        current.removeLast();
                        current.removeLast();
                        current.removeLast();
                        current.removeLast();
                    }
                    break;
                }
                else if (!target.isEmpty())
                {
                    break;
                }
            }
        }
    }

    if (!foundCapture && current.size() > 0)
    {
        result.append(current);
    }
}

bool CheckersModel::isValidMove(int fromRow, int fromCol, int toRow, int toCol) const
{
    QString piece = pieceAt(fromRow, fromCol);
    if (piece.isEmpty()) return false;
    if (!pieceAt(toRow, toCol).isEmpty()) return false;

    bool isWhite = (piece == "w" || piece == "W");
    if ((m_whiteTurn && !isWhite) || (!m_whiteTurn && isWhite)) return false;

    int dr = toRow - fromRow;
    int dc = toCol - fromCol;
    bool isKing = (piece == "W" || piece == "B");

    bool captureExists = hasAnyCapture(isWhite);

    if (abs(dr) == 2 && abs(dc) == 2)
    {
        int midRow = (fromRow + toRow) / 2;
        int midCol = (fromCol + toCol) / 2;
        QString captured = pieceAt(midRow, midCol);
        if (!captured.isEmpty())
        {
            bool capturedWhite = (captured == "w" || captured == "W");
            if (capturedWhite != isWhite)
            {
                if (captureExists)
                {
                    QVector<QVector<int>> allCaptures;
                    QVector<int> empty;
                    const_cast<CheckersModel*>(this)->findAllCaptures(fromRow, fromCol, isWhite, allCaptures, empty, 0);
                    for (const auto& cap : allCaptures)
                    {
                        if (cap.size() >= 3 && cap[2] == toRow && cap[3] == toCol)
                        {
                            return true;
                        }
                    }
                    return false;
                }
                return true;
            }
        }
        return false;
    }

    if (captureExists) return false;

    if (!isKing)
    {
        if (isWhite)
        {
            return (dr == -1 && abs(dc) == 1);
        }
        else
        {
            return (dr == 1 && abs(dc) == 1);
        }
    }
    else
    {
        if (abs(dr) != abs(dc) || dr == 0) return false;
        int stepRow = (dr > 0) ? 1 : -1;
        int stepCol = (dc > 0) ? 1 : -1;
        int r = fromRow + stepRow;
        int c = fromCol + stepCol;
        while (r != toRow || c != toCol)
        {
            if (!pieceAt(r, c).isEmpty()) return false;
            r += stepRow;
            c += stepCol;
        }
        return true;
    }
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
    if (m_gameOver) return;

    QString piece = pieceAt(fromRow, fromCol);
    if (piece.isEmpty()) return;

    bool isWhite = (piece == "w" || piece == "W");
    if ((m_whiteTurn && !isWhite) || (!m_whiteTurn && isWhite)) return;

    if (!isValidMove(fromRow, fromCol, toRow, toCol)) return;

    QStringList capturedList;
    bool isCapture = (abs(toRow - fromRow) == 2);
    int finalRow = toRow;
    int finalCol = toCol;
    QString finalPiece = piece;

    if (isCapture)
    {
        QVector<QVector<int>> allCaptures;
        QVector<int> empty;
        findAllCaptures(fromRow, fromCol, isWhite, allCaptures, empty, 0);

        for (const auto& cap : allCaptures)
        {
            if (cap.size() >= 3 && cap[2] == toRow && cap[3] == toCol)
            {
                finalRow = cap[cap.size() - 4];
                finalCol = cap[cap.size() - 3];
                for (int i = 0; i < cap.size(); i += 6)
                {
                    int capRow = cap[i + 4];
                    int capCol = cap[i + 5];
                    capturedList.append(pieceAt(capRow, capCol));
                    m_board[capRow][capCol] = "";
                }
                m_board[cap[0]][cap[1]] = "";
                finalPiece = piece;
                if ((finalPiece == "w" && finalRow == 0) || (finalPiece == "b" && finalRow == 7))
                {
                    finalPiece = (finalPiece == "w") ? "W" : "B";
                }
                m_board[finalRow][finalCol] = finalPiece;
                break;
            }
        }
    }

    if (!isCapture || capturedList.isEmpty())
    {
        m_board[toRow][toCol] = piece;
        m_board[fromRow][fromCol] = "";
        finalRow = toRow;
        finalCol = toCol;
        finalPiece = piece;
        if ((finalPiece == "w" && finalRow == 0) || (finalPiece == "b" && finalRow == 7))
        {
            finalPiece = (finalPiece == "w") ? "W" : "B";
            m_board[finalRow][finalCol] = finalPiece;
        }
    }

    MoveRecord record{fromRow, fromCol, finalRow, finalCol, piece, capturedList.join(",")};
    m_moveHistory.append(record);

    m_whiteTurn = !m_whiteTurn;

    int whiteCount = 0, blackCount = 0;
    for (int i = 0; i < 8; ++i)
    {
        for (int j = 0; j < 8; ++j)
        {
            QString p = m_board[i][j];
            if (p == "w" || p == "W") whiteCount++;
            else if (p == "b" || p == "B") blackCount++;
        }
    }

    if (whiteCount == 0 && blackCount > 0)
    {
        m_gameOver = true;
        m_gameResult = "Чёрные победили";
        emit gameOverChanged();
        emit boardStateChanged();
        emit whiteTurnChanged();
        return;
    }
    if (blackCount == 0 && whiteCount > 0)
    {
        m_gameOver = true;
        m_gameResult = "Белые победили";
        emit gameOverChanged();
        emit boardStateChanged();
        emit whiteTurnChanged();
        return;
    }

    if (!hasLegalMoves(m_whiteTurn))
    {
        m_gameOver = true;
        m_gameResult = m_whiteTurn ? "Чёрные победили" : "Белые победили";
        emit gameOverChanged();
    }

    emit boardStateChanged();
    emit whiteTurnChanged();
}

QVariantList CheckersModel::getValidMoves(int row, int col)
{
    QVariantList moves;
    QString piece = pieceAt(row, col);
    if (piece.isEmpty()) return moves;

    bool isWhite = (piece == "w" || piece == "W");
    bool captureExists = hasAnyCapture(isWhite);

    if (captureExists)
    {
        QVector<QVector<int>> allCaptures;
        QVector<int> empty;
        findAllCaptures(row, col, isWhite, allCaptures, empty, 0);
        QSet<QString> added;
        for (const auto& cap : allCaptures)
        {
            if (cap.size() >= 3)
            {
                for (int i = 0; i < cap.size(); i += 6)
                {
                    int stepRow = cap[i + 2];
                    int stepCol = cap[i + 3];
                    QString key = QString::number(stepRow) + "," + QString::number(stepCol);
                    if (!added.contains(key))
                    {
                        added.insert(key);
                        QVariantMap move;
                        move["row"] = stepRow;
                        move["col"] = stepCol;
                        moves.append(move);
                    }
                }
            }
        }
    }
    else
    {
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

void CheckersModel::aiMove()
{
    if (m_gameOver) return;
    if (m_whiteTurn) return;

    QThread::msleep(10);

    QVector<QVector<int>> moves;
    for (int i = 0; i < 8; ++i)
    {
        for (int j = 0; j < 8; ++j)
        {
            QString piece = pieceAt(i, j);
            if (piece.isEmpty()) continue;
            bool pieceWhite = (piece == "w" || piece == "W");
            if (pieceWhite) continue;

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

    if (moves.isEmpty()) return;

    if (m_aiLevel == 1)
    {
        int idx = rand() % moves.size();
        movePiece(moves[idx][0], moves[idx][1], moves[idx][2], moves[idx][3]);
    }
    else
    {
        int bestScore = -1000000;
        QVector<int> bestMove;
        int limit = qMin(30, moves.size());
        for (int idx = 0; idx < limit; ++idx)
        {
            const auto& move = moves[idx];
            int score = 0;
            if (abs(move[2] - move[0]) == 2)
            {
                int midRow = (move[0] + move[2]) / 2;
                int midCol = (move[1] + move[3]) / 2;
                QString captured = pieceAt(midRow, midCol);
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
        QStringList capturedList = captured.split(",");
        if (abs(toRow - fromRow) == 2 && capturedList.size() == 1)
        {
            int midRow = (fromRow + toRow) / 2;
            int midCol = (fromCol + toCol) / 2;
            m_board[midRow][midCol] = capturedList[0];
        }
    }

    m_whiteTurn = !m_whiteTurn;
    m_gameOver = false;
    m_gameResult = "";
    if (!m_moveHistory.isEmpty())
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
    if (m_gameOver) return false;
    if (m_aiLevel == 1)
    {
        return (rand() % 100) < 5;
    }
    else
    {
        return (rand() % 100) < 15;
    }
}
