#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QtCharts>
#include <QSerialPort>
#include "platform.h"
#include "ball.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void startCountdown();
    void updateClock();
    void updateAnimation();
    void updateCharts();
    void readSerialData();
    void increasePlatformWidth();
    void decreasePlatformWidth();

private:
    Ui::MainWindow *ui;
    QTimer *animationTimer;
    QTimer *clockTimer;
    QSerialPort *serial;
    QLineSeries *rollSeries;
    QLineSeries *pitchSeries;
    QChart *rollChart;
    QChart *pitchChart;
    QChartView *rollChartView;
    QChartView *pitchChartView;
    QGraphicsScene *scene;
    Platform *platform;
    Ball *ball;
    QTime startTime;
    bool isCounting;
    QString serialBuffer;
    qint64 chartDuration;

    void updateBallPosition(double pitch);
    double calculateBallPosition(double pitch);
};

#endif // MAINWINDOW_H
