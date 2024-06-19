/**
 * @file ball.h
 * @brief Declaration of the Ball class which represents a ball in a graphical scene.
 */

#ifndef BALL_H
#define BALL_H

#include <QGraphicsEllipseItem>

/**
 * @class Ball
 * @brief The Ball class represents a ball in a graphical scene.
 *
 * This class inherits from QGraphicsEllipseItem and provides methods to control
 * the ball's position, movement, and state.
 */
class Ball : public QGraphicsEllipseItem
{
public:
    /**
     * @brief Constructs a Ball object.
     * @param parent The parent QGraphicsItem, default is nullptr.
     */
    Ball(QGraphicsItem *parent = nullptr);

    /**
     * @brief Sets the position of the ball.
     * @param x The x-coordinate of the ball.
     * @param y The y-coordinate of the ball.
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
    double m_x; ///< The x-coordinate of the ball.
    double m_y; ///< The y-coordinate of the ball.
    bool m_isFalling; ///< Flag indicating if the ball is falling.
    bool m_isMoving; ///< Flag indicating if the ball is moving.
};

#endif // BALL_H
