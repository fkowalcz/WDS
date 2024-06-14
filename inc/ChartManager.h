#ifndef CHARTMANAGER_H
#define CHARTMANAGER_H

#include <QObject>
#include <QtCharts>

using namespace QtCharts;

/**
 * @class ChartManager
 * @brief The ChartManager class manages roll and pitch charts.
 *
 * This class handles the creation, updating, and retrieval of charts
 * for displaying roll and pitch data.
 */
class ChartManager : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief Constructs a ChartManager object.
     * @param parent The parent QObject.
     */
    explicit ChartManager(QObject *parent = nullptr);

    /**
     * @brief Gets the roll chart view.
     * @return A pointer to the QChartView displaying the roll chart.
     */
    QChartView* getRollChartView() const;

    /**
     * @brief Gets the pitch chart view.
     * @return A pointer to the QChartView displaying the pitch chart.
     */
    QChartView* getPitchChartView() const;

    /**
     * @brief Gets the roll chart.
     * @return A pointer to the QChart displaying the roll data.
     */
    QChart* getRollChart() const;

    /**
     * @brief Gets the pitch chart.
     * @return A pointer to the QChart displaying the pitch data.
     */
    QChart* getPitchChart() const;

    /**
     * @brief Updates the charts with new roll and pitch data.
     * @param currentTime The current time in milliseconds.
     * @param rollValue The roll value.
     * @param pitchValue The pitch value.
     */
    void updateCharts(qint64 currentTime, double rollValue, double pitchValue);

private:
    QLineSeries *rollSeries;       ///< Series for roll data.
    QLineSeries *pitchSeries;      ///< Series for pitch data.
    QChart *rollChart;             ///< Chart for displaying roll data.
    QChart *pitchChart;            ///< Chart for displaying pitch data.
    QChartView *rollChartView;     ///< View for displaying the roll chart.
    QChartView *pitchChartView;    ///< View for displaying the pitch chart.
    qint64 chartDuration;          ///< Duration for displaying chart data.
};

#endif // CHARTMANAGER_H
