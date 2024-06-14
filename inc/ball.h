#ifndef BALL_H
#define BALL_H

#include <QGraphicsEllipseItem>

/**
 * @class Ball
 * @brief The Ball class represents a ball in a graphical scene.
 *
 * This class manages the position and movement of a ball in a QGraphicsScene.
 */
class Ball : public QGraphicsEllipseItem
{
public:
    /**
     * @brief Constructs a Ball object.
     * @param parent The parent QGraphicsItem.
     */
    Ball(QGraphicsItem *parent = nullptr);

    /**
     * @brief Sets the position of the ball.
     * @param x The x-coordinate of the position.
     * @param y The y-coordinate of the position.
     */
    void setPosition(double x, double y);

    /**
     * @brief Starts the movement of the ball.
     */
    void startMovement();

    /**
     * @brief Stops the movement of the ball.
     */
    void stopMovement();

    /**
     * @brief Makes the ball fall.
     */
    void fall();

    /**
     * @brief Checks if the ball is falling.
     * @return True if the ball is falling, false otherwise.
     */
    bool isFalling() const;

    /**
     * @brief Checks if the ball is moving.
     * @return True if the ball is moving, false otherwise.
     */
    bool isMoving() const;

private:
    double m_x;          ///< The x-coordinate of the ball's position.
    double m_y;          ///< The y-coordinate of the ball's position.
    bool m_isFalling;    ///< Flag indicating if the ball is falling.
    bool m_isMoving;     ///< Flag indicating if the ball is moving.
};

#endif // BALL_H
