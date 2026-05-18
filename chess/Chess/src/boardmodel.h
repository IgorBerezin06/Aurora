#ifndef BOARDMODEL_H
#define BOARDMODEL_H

#include <QObject>
#include <QVector>

class BoardModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString boardState READ boardState NOTIFY boardStateChanged)
    Q_PROPERTY(bool whiteTurn READ isWhiteTurn NOTIFY whiteTurnChanged)
    Q_PROPERTY(bool gameOver READ isGameOver NOTIFY gameOverChanged)
    Q_PROPERTY(QString gameResult READ gameResult NOTIFY gameOverChanged)

public:
    explicit BoardModel(QObject *parent = nullptr);

    QString boardState() const;
    Q_INVOKABLE QString pieceAt(int row, int col) const;
    Q_INVOKABLE void movePiece(int fromRow, int fromCol, int toRow, int toCol);
    Q_INVOKABLE bool isWhiteTurn() const;
    Q_INVOKABLE bool isGameOver() const;
    Q_INVOKABLE QString gameResult() const;

    bool isValidMove(int fromRow, int fromCol, int toRow, int toCol) const;
    bool isKingInCheck(bool whiteKing) const;
    bool hasLegalMoves(bool white) const;
    bool wouldBeInCheckAfterMove(int fromRow, int fromCol, int toRow, int toCol, bool whiteKing) const;

signals:
    void boardStateChanged();
    void whiteTurnChanged();
    void gameOverChanged();

private:
    QVector<QVector<QString>> m_board;
    bool m_whiteTurn;
    bool m_gameOver;
    bool m_whiteKingMoved;
    bool m_whiteRookMovedKingside;
    bool m_whiteRookMovedQueenside;
    bool m_blackKingMoved;
    bool m_blackRookMovedKingside;
    bool m_blackRookMovedQueenside;
    QString m_gameResult;

    void initBoard();
};

#endif // BOARDMODEL_H
