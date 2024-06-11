#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QtCharts>
#include <QSerialPort>
#include <QGraphicsScene>
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
    void updateCharts();
    void readSerialData();

private:
    Ui::MainWindow *ui;
    QTimer *timer;
    QSerialPort *serial;
    QLineSeries *rollSeries;
    QLineSeries *pitchSeries;
    QChart *rollChart;
    QChart *pitchChart;
    QChartView *rollChartView;
    QChartView *pitchChartView;
    QString serialBuffer;
    qint64 chartDuration;
    Platform *platform;
    Ball *ball;
    QGraphicsScene *scene;
    void updatePlatform();
    void updateBallPosition(double pitch);
    double calculateBallPosition(double pitch);
};

#endif // MAINWINDOW_H
