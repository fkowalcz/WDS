#include "ball.h"
#include <QDebug>

Ball::Ball(QGraphicsItem *parent)
    : QGraphicsEllipseItem(parent), m_x(0), m_y(0), m_isFalling(false)
{
    setRect(-10, -10, 20, 20); // Zwiększone wymiary piłki
    setCacheMode(QGraphicsItem::DeviceCoordinateCache);
}

void Ball::setPosition(double x, double y)
{
    m_x = x;
    m_y = y;
    setPos(m_x, m_y);
    m_isFalling = false; // Piłka nie spada
    qDebug() << "Ball position set to: (" << m_x << ", " << m_y << ")"; // Debugging position
}

void Ball::fall()
{
    m_isFalling = true; // Piłka spada
    // Move ball down by 5 units each call
    m_y += 5;

    // Check if the ball is within the boundaries of the scene
    if (m_y + rect().height() / 2 > scene()->sceneRect().bottom()) {
        m_y = scene()->sceneRect().bottom() - rect().height() / 2;
    }
    setPos(m_x, m_y);
    qDebug() << "Ball falling to: (" << m_x << ", " << m_y << ")";
}

bool Ball::isFalling() const
{
    return m_isFalling;
}

void Ball::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setRenderHint(QPainter::Antialiasing);
    QGraphicsEllipseItem::paint(painter, option, widget);
    painter->setBrush(Qt::blue);
    painter->drawEllipse(rect());
}
