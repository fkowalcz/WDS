#include "ball.h"
#include <QGraphicsScene>
#include <QDebug>

/**
 * @brief Constructs a Ball object.
 * @param parent The parent QGraphicsItem.
 */
Ball::Ball(QGraphicsItem *parent)
    : QGraphicsEllipseItem(parent), m_x(0), m_y(0), m_isFalling(false), m_isMoving(false)
{
    setRect(-10, -10, 20, 20); // Set the size of the ball
    setCacheMode(QGraphicsItem::DeviceCoordinateCache);
}

/**
 * @brief Sets the position of the ball.
 * @param x The x-coordinate of the position.
 * @param y The y-coordinate of the position.
 */
void Ball::setPosition(double x, double y)
{
    m_x = x;
    m_y = y;
    setPos(m_x, m_y);
    m_isFalling = false; // Ensure the ball is not falling
    m_isMoving = false;  // Ensure the ball is not moving
    qDebug() << "Ball position set to: (" << m_x << ", " << m_y << ")"; // Debugging position
}

/**
 * @brief Starts the movement of the ball.
 */
void Ball::startMovement()
{
    m_isMoving = true;
    qDebug() << "Ball movement started";
}

/**
 * @brief Stops the movement of the ball.
 */
void Ball::stopMovement()
{
    m_isMoving = false;
    qDebug() << "Ball movement stopped";
}

/**
 * @brief Makes the ball fall.
 *
 * This method updates the y-coordinate of the ball to simulate falling.
 * The ball moves down by 5 units each time this method is called.
 * If the ball reaches the bottom of the scene, it stops moving.
 */
void Ball::fall()
{
    m_isFalling = true; // Mark the ball as falling
    // Move ball down by 5 units each call
    m_y += 5;

    // Check if the ball is within the boundaries of the scene
    if (m_y + rect().height() / 2 > scene()->sceneRect().bottom()) {
        m_y = scene()->sceneRect().bottom() - rect().height() / 2;
    }
    setPos(m_x, m_y);
    qDebug() << "Ball falling to: (" << m_x << ", " << m_y << ")";
}

/**
 * @brief Checks if the ball is falling.
 * @return True if the ball is falling, false otherwise.
 */
bool Ball::isFalling() const
{
    return m_isFalling;
}

/**
 * @brief Checks if the ball is moving.
 * @return True if the ball is moving, false otherwise.
 */
bool Ball::isMoving() const
{
    return m_isMoving;
}
