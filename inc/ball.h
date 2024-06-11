#ifndef BALL_H
#define BALL_H

#include <QGraphicsEllipseItem>
#include <QGraphicsScene>
#include <QPainter>
#include <QGraphicsView>

class Ball : public QGraphicsEllipseItem
{
public:
    Ball(QGraphicsItem *parent = nullptr);

    void setPosition(double x, double y);
    void fall();
    bool isFalling() const;

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

private:
    double m_x;
    double m_y;
    bool m_isFalling;
};

#endif // BALL_H
