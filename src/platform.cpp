#include "platform.h"
#include <QDebug>

Platform::Platform(QGraphicsItem *parent)
    : QGraphicsRectItem(parent), m_angle(0)
{
    setRect(-200, -20, 400, 40); // Zwiększone wymiary platformy
}

void Platform::setAngle(double angle)
{
    m_angle = angle;
    setRotation(m_angle);
    qDebug() << "Platform angle set to:" << m_angle; // Debugging angle
}

void Platform::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QGraphicsRectItem::paint(painter, option, widget);
}
