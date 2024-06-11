#ifndef PLATFORM_H
#define PLATFORM_H

#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include <QPainter>
#include <QGraphicsView>

class Platform : public QGraphicsRectItem
{
public:
    Platform(QGraphicsItem *parent = nullptr);

    void setAngle(double angle);

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

private:
    double m_angle;
};

#endif // PLATFORM_H
