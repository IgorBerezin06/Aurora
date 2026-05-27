#ifndef BOARDMODEL_H
#define BOARDMODEL_H

#include <QObject>
#include <QVector>
#include <QPair>
#include <QVariant>

class BoardModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString boardState READ boardState NOTIFY boardStateChanged)
    Q_PROPERTY(bool whiteTurn READ isWhiteTurn NOTIFY whiteTurnChanged)
    Q_PROPERTY(bool gameOver READ isGameOver NOTIFY gameOverChanged)
    Q_PROPERTY(QString gameResult READ gameResult NOTIFY gameOverChanged)
    Q_PROPERTY(bool aiMode READ aiMode WRITE setAiMode NOTIFY aiModeChanged)
    Q_PROPERTY(int aiLevel READ aiLevel WRITE setAiLevel NOTIFY aiLevelChanged)

public:
    explicit BoardModel(QObject *parent = nullptr);

    QString boardState() const;
    Q_INVOKABLE QString pieceAt(int row, int col) const;
    Q_INVOKABLE void movePiece(int fromRow, int fromCol, int toRow, int toCol);
    Q_INVOKABLE bool isWhiteTurn() const;
    Q_INVOKABLE bool isGameOver() const;
    Q_INVOKABLE QString gameResult() const;
    Q_INVOKABLE QVariantList getValidMoves(int row, int col);
    Q_INVOKABLE void promotePawn(int row, int col, const QString& piece);
    Q_INVOKABLE void aiMove();
    Q_INVOKABLE bool shouldAcceptDraw() const;

    bool isValidMove(int fromRow, int fromCol, int toRow, int toCol) const;
    bool isKingInCheck(bool whiteKing) const;
    bool hasLegalMoves(bool white) const;
    bool wouldBeInCheckAfterMove(int fromRow, int fromCol, int toRow, int toCol, bool whiteKing) const;

    bool aiMode() const;
    void setAiMode(bool mode);
    int aiLevel() const;
    void setAiLevel(int level);

signals:
    void boardStateChanged();
    void whiteTurnChanged();
    void gameOverChanged();
    void aiModeChanged();
    void aiLevelChanged();

private:
    QVector<QVector<QString>> m_board;
    bool m_whiteTurn;
    bool m_gameOver;
    QString m_gameResult;

    bool m_whiteKingMoved;
    bool m_whiteRookMovedKingside;
    bool m_whiteRookMovedQueenside;
    bool m_blackKingMoved;
    bool m_blackRookMovedKingside;
    bool m_blackRookMovedQueenside;

    int m_enPassantTargetRow;
    int m_enPassantTargetCol;

    bool m_aiMode;
    int m_aiLevel;

    void initBoard();
};

#endif // BOARDMODEL_H
