#include "charts.h"

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
    setupChart(rollChart, rollSeries, "Roll Angle", "Roll Angle");
    setupChart(pitchChart, pitchSeries, "Pitch Angle", "Pitch Angle");

    rollChartView->setRenderHint(QPainter::Antialiasing);
    pitchChartView->setRenderHint(QPainter::Antialiasing);
}

ChartManager::~ChartManager()
{
    delete rollChartView;
    delete pitchChartView;
    delete rollChart;
    delete pitchChart;
    delete rollSeries;
    delete pitchSeries;
}

QChartView* ChartManager::getRollChartView()
{
    return rollChartView;
}

QChartView* ChartManager::getPitchChartView()
{
    return pitchChartView;
}

void ChartManager::updateCharts(qint64 currentTime, double rollValue, double pitchValue)
{
    rollSeries->append(currentTime, rollValue);
    pitchSeries->append(currentTime, pitchValue);

    // Update time axis
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

    // Refresh chart views
    rollChartView->repaint();
    pitchChartView->repaint();
}

void ChartManager::setChartDuration(qint64 duration)
{
    chartDuration = duration;
}

void ChartManager::setupChart(QChart *chart, QLineSeries *series, const QString &title, const QString &yTitle)
{
    QDateTimeAxis *axisX = new QDateTimeAxis();
    axisX->setFormat("hh:mm:ss");
    axisX->setTitleText("Time");

    QValueAxis *axisY = new QValueAxis();
    axisY->setRange(-90, 90);
    axisY->setTitleText(yTitle);

    chart->addSeries(series);
    chart->addAxis(axisX, Qt::AlignBottom);
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisX);
    series->attachAxis(axisY);
    chart->setTitle(title);
    chart->legend()->hide();
}
