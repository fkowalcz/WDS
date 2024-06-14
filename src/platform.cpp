#include "platform.h"
#include <QDebug>

/**
 * @brief Constructs a Platform object.
 * @param parent The parent QGraphicsItem.
 */
Platform::Platform(QGraphicsItem *parent)
    : QGraphicsRectItem(parent), m_angle(0)
{
    setRect(-200, -20, 400, 40); // Zwiększone wymiary platformy
}

/**
 * @brief Sets the angle of the platform.
 * @param angle The angle to set.
 */
void Platform::setAngle(double angle)
{
    m_angle = angle;
    setRotation(m_angle);
    qDebug() << "Platform angle set to:" << m_angle; // Debugging angle
}

/**
 * @brief Paints the platform.
 * @param painter The painter used for drawing.
 * @param option The style options for the item.
 * @param widget The widget being painted on.
 */
void Platform::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QGraphicsRectItem::paint(painter, option, widget);
}
