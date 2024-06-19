#ifndef CHARTMANAGER_H
#define CHARTMANAGER_H

#include <QObject>
#include <QtCharts>

using namespace QtCharts;

/**
 * @class ChartManager
 * @brief The ChartManager class manages the roll and pitch charts.
 *
 * This class is responsible for creating, updating, and providing access
 * to the roll and pitch charts used in the application.
 */
class ChartManager : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief Constructs a ChartManager object.
     * @param parent The parent QObject, default is nullptr.
     */
    explicit ChartManager(QObject *parent = nullptr);

    /**
     * @brief Gets the roll chart view.
     * @return A pointer to the roll chart view.
     */
    QChartView* getRollChartView() const;

    /**
     * @brief Gets the pitch chart view.
     * @return A pointer to the pitch chart view.
     */
    QChartView* getPitchChartView() const;

    /**
     * @brief Gets the roll chart.
     * @return A pointer to the roll chart.
     */
    QChart* getRollChart() const;

    /**
     * @brief Gets the pitch chart.
     * @return A pointer to the pitch chart.
     */
    QChart* getPitchChart() const;

    /**
     * @brief Updates the charts with new roll and pitch values.
     * @param currentTime The current timestamp in milliseconds.
     * @param rollValue The roll value.
     * @param pitchValue The pitch value.
     */
    void updateCharts(qint64 currentTime, double rollValue, double pitchValue);

private:
    QLineSeries *rollSeries; ///< Series for roll data points.
    QLineSeries *pitchSeries; ///< Series for pitch data points.
    QChart *rollChart; ///< Chart for displaying roll data.
    QChart *pitchChart; ///< Chart for displaying pitch data.
    QChartView *rollChartView; ///< View for the roll
    QChartView *pitchChartView; ///< View for the pitch chart.
    qint64 chartDuration; ///< Duration for displaying chart data.
};



#endif // CHARTMANAGER_H
