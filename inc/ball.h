#ifndef BALL_H
#define BALL_H

#include <QGraphicsEllipseItem>

class Ball : public QGraphicsEllipseItem
{
public:
    Ball(QGraphicsItem *parent = nullptr);
    void setPosition(double x, double y);
    void startMovement();
    void stopMovement();
    void fall();
    bool isFalling() const;
    bool isMoving() const;

private:
    double m_x;
    double m_y;
    bool m_isFalling;
    bool m_isMoving; // Add this flag
};

#endif // BALL_H
