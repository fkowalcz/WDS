#ifndef CHARTMANAGER_H
#define CHARTMANAGER_H

#include <QObject>
#include <QtCharts>

using namespace QtCharts;

class ChartManager : public QObject
{
    Q_OBJECT

public:
    explicit ChartManager(QObject *parent = nullptr);

    QChartView* getRollChartView() const;
    QChartView* getPitchChartView() const;
    QChart* getRollChart() const;  // Add this method
    QChart* getPitchChart() const; // Add this method
    void updateCharts(qint64 currentTime, double rollValue, double pitchValue);

private:
    QLineSeries *rollSeries;
    QLineSeries *pitchSeries;
    QChart *rollChart;
    QChart *pitchChart;
    QChartView *rollChartView;
    QChartView *pitchChartView;
    qint64 chartDuration;
};

#endif // CHARTMANAGER_H
