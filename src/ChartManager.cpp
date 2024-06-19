#include "ChartManager.h"

/**
 * @brief Constructs a ChartManager object.
 * @param parent The parent QObject, default is nullptr.
 *
 * This constructor initializes the roll and pitch charts, sets up their views,
 * and configures the axes and series for both charts.
 */
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
    // Configure roll chart
    QDateTimeAxis *axisXRoll = new QDateTimeAxis();
    axisXRoll->setFormat("hh:mm:ss");
    axisXRoll->setTitleText("Time");

    QValueAxis *axisYRoll = new QValueAxis();
    axisYRoll->setRange(-90, 90);
    axisYRoll->setTitleText("Roll Angle");

    rollChart->addSeries(rollSeries);
    rollChart->addAxis(axisXRoll, Qt::AlignBottom);
    rollChart->addAxis(axisYRoll, Qt::AlignLeft);
    rollSeries->attachAxis(axisXRoll);
    rollSeries->attachAxis(axisYRoll);
    rollChart->setTitle("Roll Angle");
    rollChart->legend()->hide();

    // Configure pitch chart
    QDateTimeAxis *axisXPitch = new QDateTimeAxis();
    axisXPitch->setFormat("hh:mm:ss");
    axisXPitch->setTitleText("Time");

    QValueAxis *axisYPitch = new QValueAxis();
    axisYPitch->setRange(-90, 90);
    axisYPitch->setTitleText("Pitch Angle");

    pitchChart->addSeries(pitchSeries);
    pitchChart->addAxis(axisXPitch, Qt::AlignBottom);
    pitchChart->addAxis(axisYPitch, Qt::AlignLeft);
    pitchSeries->attachAxis(axisXPitch);
    pitchSeries->attachAxis(axisYPitch);
    pitchChart->setTitle("Pitch Angle");
    pitchChart->legend()->hide();

    // Set antialiasing for chart views
    rollChartView->setRenderHint(QPainter::Antialiasing);
    pitchChartView->setRenderHint(QPainter::Antialiasing);
}

/**
 * @brief Gets the roll chart view.
 * @return A pointer to the roll chart view.
 */
QChartView* ChartManager::getRollChartView() const {
    return rollChartView;
}

/**
 * @brief Gets the pitch chart view.
 * @return A pointer to the pitch chart view.
 */
QChartView* ChartManager::getPitchChartView() const {
    return pitchChartView;
}

/**
 * @brief Gets the roll chart.
 * @return A pointer to the roll chart.
 */
QChart* ChartManager::getRollChart() const {
    return rollChart;
}

/**
 * @brief Gets the pitch chart.
 * @return A pointer to the pitch chart.
 */
QChart* ChartManager::getPitchChart() const {
    return pitchChart;
}

/**
 * @brief Updates the charts with new roll and pitch values.
 * @param currentTime The current timestamp in milliseconds.
 * @param rollValue The roll value.
 * @param pitchValue The pitch value.
 *
 * This method appends new data points to the roll and pitch series, and removes
 * old data points that fall outside the specified chart duration. It also updates
 * the x-axis range of both charts to keep the data within the visible range.
 */
void ChartManager::updateCharts(qint64 currentTime, double rollValue, double pitchValue) {
    // Append new data points
    rollSeries->append(currentTime, rollValue);
    pitchSeries->append(currentTime, pitchValue);

    // Remove old data points outside the chart duration
    while (!rollSeries->points().isEmpty() && rollSeries->points().first().x() < currentTime - chartDuration) {
        rollSeries->remove(0);
    }

    while (!pitchSeries->points().isEmpty() && pitchSeries->points().first().x() < currentTime - chartDuration) {
        pitchSeries->remove(0);
    }

    // Update the x-axis range
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

    // Repaint the chart views
    rollChartView->repaint();
    pitchChartView->repaint();
}
