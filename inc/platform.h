#ifndef PLATFORM_H
#define PLATFORM_H

#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include <QPainter>
#include <QGraphicsView>

/**
 * @class Platform
 * @brief The Platform class represents a platform in a graphical scene.
 *
 * This class manages the appearance and behavior of a platform in a QGraphicsScene.
 */
class Platform : public QGraphicsRectItem
{
public:
    /**
     * @brief Constructs a Platform object.
     * @param parent The parent QGraphicsItem.
     */
    Platform(QGraphicsItem *parent = nullptr);

    /**
     * @brief Sets the angle of the platform.
     * @param angle The angle to set.
     */
    void setAngle(double angle);

protected:
    /**
     * @brief Paints the platform.
     * @param painter The painter used for drawing.
     * @param option The style options for the item.
     * @param widget The widget being painted on.
     */
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

private:
    double m_angle; ///< The angle of the platform.
};

#endif // PLATFORM_H
