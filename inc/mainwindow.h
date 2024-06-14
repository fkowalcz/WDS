#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include "ChartManager.h"
#include "SerialManager.h"
#include "TerminalLogger.h"
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
    void updateCharts(double rollValue, double pitchValue);
    void increasePlatformWidth();
    void decreasePlatformWidth();

private:
    Ui::MainWindow *ui;
    QTimer *animationTimer;
    QTimer *clockTimer;
    ChartManager *chartManager;
    SerialManager *serialManager;
    TerminalLogger *terminalLogger;
    QGraphicsScene *scene;
    Platform *platform;
    Ball *ball;
    QTime startTime;
    bool isCounting;
    qint64 chartDuration;

    void updateBallPosition(double pitch);
    double calculateBallPosition(double pitch);
};

#endif // MAINWINDOW_H
