#ifndef PLATFORM_H
#define PLATFORM_H

#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include <QPainter>
#include <QGraphicsView>

/**
 * @class Platform
 * @brief The Platform class represents a rectangular platform in the graphics scene.
 *
 * This class is a QGraphicsRectItem that provides functionality to set the angle
 * of the platform and customize its painting behavior.
 */
class Platform : public QGraphicsRectItem
{
public:
    /**
     * @brief Constructs a Platform object.
     * @param parent The parent graphics item.
     */
    Platform(QGraphicsItem *parent = nullptr);

    /**
     * @brief Sets the angle of the platform.
     * @param angle The new angle of the platform.
     */
    void setAngle(double angle);

protected:
    /**
     * @brief Custom paint function for the platform.
     * @param painter The painter used for drawing.
     * @param option The style options for the item.
     * @param widget The widget on which the item is drawn.
     */
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

private:
    double m_angle; ///< The angle of the platform.
};

#endif // PLATFORM_H
