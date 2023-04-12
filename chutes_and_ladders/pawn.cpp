#include "pawn.h"
#include <QtWidgets>

pawn::pawn(QColor color, const int x, const int y)
{
    this->color_ = color;
    x_ = x - width_/2;
    y_ = y - width_/2;
}

void pawn::setXY(int x, int y) {
//    qDebug() << "setting XY";
    x_ = x - width_/2;
//    qDebug() << "set x";
    y_ = y - width_/2;
//    qDebug() << "XY set";
}


QRectF pawn::boundingRect() const
{
    return QRectF(x_, y_, width_, width_);
}

QPainterPath pawn::shape() const
{
    QPainterPath path;
    path.addEllipse(x_, y_, width_, width_);
    return path;
}

void pawn::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);


    QBrush b = painter->brush();
    painter->setBrush(QBrush(color_));

    painter->drawEllipse(QRect(this->x_, this->y_, this->width_, this->width_));
    painter->setBrush(b);
}
