#ifndef CHARTMANAGER_H
#define CHARTMANAGER_H

#include <QObject>
#include <QtCharts>

using namespace QtCharts;

class ChartManager : public QObject
{
    Q_OBJECT

public:
    ChartManager(QObject *parent = nullptr);
    ~ChartManager();

    QChartView* getRollChartView();
    QChartView* getPitchChartView();
    void updateCharts(qint64 currentTime, double rollValue, double pitchValue);
    void setChartDuration(qint64 duration);

private:
    QLineSeries *rollSeries;
    QLineSeries *pitchSeries;
    QChart *rollChart;
    QChart *pitchChart;
    QChartView *rollChartView;
    QChartView *pitchChartView;
    qint64 chartDuration;

    void setupChart(QChart *chart, QLineSeries *series, const QString &title, const QString &yTitle);
};

#endif // CHARTMANAGER_H
