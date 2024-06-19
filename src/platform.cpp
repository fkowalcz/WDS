#include "platform.h"
#include <QDebug>

/**
 * @brief Constructs a Platform object.
 * @param parent The parent graphics item.
 *
 * Initializes the platform with default dimensions and angle.
 */
Platform::Platform(QGraphicsItem *parent)
    : QGraphicsRectItem(parent), m_angle(0)
{
    setRect(-200, -20, 400, 40); // Set the dimensions of the platform
}

/**
 * @brief Sets the angle of the platform.
 * @param angle The new angle of the platform.
 *
 * Updates the angle and applies the rotation to the platform.
 */
void Platform::setAngle(double angle)
{
    m_angle = angle;
    setRotation(m_angle);
    qDebug() << "Platform angle set to:" << m_angle; // Debugging angle
}

/**
 * @brief Custom paint function for the platform.
 * @param painter The painter used for drawing.
 * @param option The style options for the item.
 * @param widget The widget on which the item is drawn.
 *
 * This function calls the base class paint function to handle the default painting.
 */
void Platform::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QGraphicsRectItem::paint(painter, option, widget);
}
