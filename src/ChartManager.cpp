#include "ChartManager.h"

ChartManager::ChartManager(QObject *parent)
    : QObject(parent)
    , rollSeries(new QLineSeries())
    , pitchSeries(new QLineSeries())
    , rollChart(new QChart())
    , pitchChart(new QChart())
    , rollChartView(new QChartView(rollChart))
    , pitchChartView(new QChartView(pitchChart))
    , chartDuration(20 * 1000) // 20 seconds in milliseconds
{
    // Configure charts
    QDateTimeAxis *axisXRoll = new QDateTimeAxis();
    axisXRoll->setFormat("hh:mm:ss");
    axisXRoll->setTitleText("Time");

    QDateTimeAxis *axisXPitch = new QDateTimeAxis();
    axisXPitch->setFormat("hh:mm:ss");
    axisXPitch->setTitleText("Time");

    QValueAxis *axisYRoll = new QValueAxis();
    axisYRoll->setRange(-90, 90);
    axisYRoll->setTitleText("Roll Angle");

    QValueAxis *axisYPitch = new QValueAxis();
    axisYPitch->setRange(-90, 90);
    axisYPitch->setTitleText("Pitch Angle");

    rollChart->addSeries(rollSeries);
    rollChart->addAxis(axisXRoll, Qt::AlignBottom);
    rollChart->addAxis(axisYRoll, Qt::AlignLeft);
    rollSeries->attachAxis(axisXRoll);
    rollSeries->attachAxis(axisYRoll);
    rollChart->setTitle("Roll Angle");
    rollChart->legend()->hide();

    pitchChart->addSeries(pitchSeries);
    pitchChart->addAxis(axisXPitch, Qt::AlignBottom);
    pitchChart->addAxis(axisYPitch, Qt::AlignLeft);
    pitchSeries->attachAxis(axisXPitch);
    pitchSeries->attachAxis(axisYPitch);
    pitchChart->setTitle("Pitch Angle");
    pitchChart->legend()->hide();

    rollChartView->setRenderHint(QPainter::Antialiasing);
    pitchChartView->setRenderHint(QPainter::Antialiasing);
}

QChartView* ChartManager::getRollChartView() const {
    return rollChartView;
}

QChartView* ChartManager::getPitchChartView() const {
    return pitchChartView;
}

QChart* ChartManager::getRollChart() const { // Implement this method
    return rollChart;
}

QChart* ChartManager::getPitchChart() const { // Implement this method
    return pitchChart;
}

void ChartManager::updateCharts(qint64 currentTime, double rollValue, double pitchValue) {
    rollSeries->append(currentTime, rollValue);
    pitchSeries->append(currentTime, pitchValue);

    // Keep the data within the chart duration
    while (!rollSeries->points().isEmpty() && rollSeries->points().first().x() < currentTime - chartDuration) {
        rollSeries->remove(0);
    }

    while (!pitchSeries->points().isEmpty() && pitchSeries->points().first().x() < currentTime - chartDuration) {
        pitchSeries->remove(0);
    }

    QDateTime minTime = QDateTime::fromMSecsSinceEpoch(currentTime - chartDuration);
    QDateTime maxTime = QDateTime::fromMSecsSinceEpoch(currentTime);

    QDateTimeAxis *axisXRoll = qobject_cast<QDateTimeAxis*>(rollChart->axes(Qt::Horizontal).first());
    if (axisXRoll) {
        axisXRoll->setMin(minTime);
        axisXRoll->setMax(maxTime);
    }

    QDateTimeAxis *axisXPitch = qobject_cast<QDateTimeAxis*>(pitchChart->axes(Qt::Horizontal).first());
    if (axisXPitch) {
        axisXPitch->setMin(minTime);
        axisXPitch->setMax(maxTime);
    }

    rollChartView->repaint();
    pitchChartView->repaint();
}


