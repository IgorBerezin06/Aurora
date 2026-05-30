#ifndef CHECKERSMODEL_H
#define CHECKERSMODEL_H

#include <QObject>
#include <QVector>
#include <QVariant>

class CheckersModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString boardState READ boardState NOTIFY boardStateChanged)
    Q_PROPERTY(bool whiteTurn READ isWhiteTurn NOTIFY whiteTurnChanged)
    Q_PROPERTY(bool gameOver READ isGameOver NOTIFY gameOverChanged)
    Q_PROPERTY(QString gameResult READ gameResult NOTIFY gameOverChanged)
    Q_PROPERTY(bool aiMode READ aiMode WRITE setAiMode NOTIFY aiModeChanged)
    Q_PROPERTY(int aiLevel READ aiLevel WRITE setAiLevel NOTIFY aiLevelChanged)

public:
    explicit CheckersModel(QObject *parent = nullptr);

    QString boardState() const;
    Q_INVOKABLE QString pieceAt(int row, int col) const;
    Q_INVOKABLE void movePiece(int fromRow, int fromCol, int toRow, int toCol);
    Q_INVOKABLE bool isWhiteTurn() const;
    Q_INVOKABLE bool isGameOver() const;
    Q_INVOKABLE QString gameResult() const;
    Q_INVOKABLE QVariantList getValidMoves(int row, int col);
    Q_INVOKABLE void aiMove();
    Q_INVOKABLE void resetBoard();
    Q_INVOKABLE void undoMove(int fromRow, int fromCol, int toRow, int toCol, const QString& piece, const QString& captured);
    Q_INVOKABLE bool shouldAcceptDraw() const;
    Q_INVOKABLE bool shouldOfferDraw() const;

    bool isValidMove(int fromRow, int fromCol, int toRow, int toCol) const;
    bool hasLegalMoves(bool white) const;

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
    bool m_aiMode;
    int m_aiLevel;

    struct MoveRecord
    {
        int fromRow;
        int fromCol;
        int toRow;
        int toCol;
        QString piece;
        QString captured;
    };
    QVector<MoveRecord> m_moveHistory;

    void initBoard();
    bool hasAnyCapture(bool white) const;
    void findAllCaptures(int row, int col, bool white, QVector<QVector<int>>& result, QVector<int>& current, int depth);
};

#endif // CHECKERSMODEL_H
