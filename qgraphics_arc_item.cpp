#include "qgraphics_arc_item.hpp"

#include <cmath>

#include <QDebug>
#include <QPainter>

QGraphicsArcItem::QGraphicsArcItem(qreal x, qreal y, qreal width, qreal height, QGraphicsItem* parent)
    : QGraphicsEllipseItem(x, y, width, height, parent) {
}

void QGraphicsArcItem::paint(QPainter *painter,
                             const QStyleOptionGraphicsItem *option,
                             QWidget *widget) {
    painter->setPen(pen());
    // Retrieve start angle and span angle
    int startAngle = this->startAngle();
    int spanAngle = this->spanAngle();

    QRectF boundingRect = this->rect(); // Retrieve the bounding rectangle of the ellipse

    // Draw the ellipse arc with the start and span angles
    painter->drawArc(boundingRect, startAngle, spanAngle); 
}









