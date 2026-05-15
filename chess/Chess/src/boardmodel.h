#ifndef BOARDMODEL_H
#define BOARDMODEL_H

#include <QObject>
#include <QVector>

class BoardModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString boardState READ boardState NOTIFY boardStateChanged)

public:
    explicit BoardModel(QObject *parent = nullptr);

    QString boardState() const;

    Q_INVOKABLE void movePiece(int fromRow, int fromCol, int toRow, int toCol);
    Q_INVOKABLE QString pieceAt(int row, int col) const;

signals:
    void boardStateChanged();

private:
    QVector<QVector<QString>> m_board;
    void initBoard();
};

#endif // BOARDMODEL_H
