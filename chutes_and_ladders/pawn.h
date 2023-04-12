#ifndef PAWN_H
#define PAWN_H

#include <QColor>
#include <QGraphicsItem>

class pawn : public QObject, public QGraphicsItem {

    Q_OBJECT

public:
    pawn(QColor color, const int x, const int y);

    void setXY(int x, int y);

    int getX() { return x_; };
    int getY() { return y_; };

    QRectF boundingRect() const override;
    QPainterPath shape() const override;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) override;

private:
    int x_;
    int y_;
    QColor color_;

    const int width_ = 30;
};

#endif // PAWN_H
