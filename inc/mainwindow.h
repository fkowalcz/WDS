#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QTimer>
#include <QDateTimeAxis>
#include "ChartManager.h"
#include "SerialManager.h"
#include "TerminalLogger.h"
#include "platform.h"
#include "ball.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

#include <QTranslator>

/**
 * @class MainWindow
 * @brief The MainWindow class represents the main application window.
 *
 * This class manages the main GUI elements and interactions, including
 * charts, serial communication, and the main control logic.
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * @brief Constructs a MainWindow object.
     * @param parent The parent widget.
     */
    explicit MainWindow(QWidget *parent = nullptr);

    /**
     * @brief Destructor for MainWindow.
     */
    ~MainWindow();

private slots:
    /**
     * @brief Starts the countdown timer.
     *
     * This method initializes the countdown timer and starts the ball movement.
     */
    void startCountdown();

    /**
     * @brief Resets the ball position on the platform.
     */
    void resetBallPosition();

    /**
     * @brief Updates the clock display.
     */
    void updateClock();

    /**
     * @brief Updates the animation of the ball and platform.
     */
    void updateAnimation();

    /**
     * @brief Updates the charts with new roll and pitch values.
     * @param rollValue The roll value.
     * @param pitchValue The pitch value.
     */
    void updateCharts(double rollValue, double pitchValue);

    /**
     * @brief Updates the ball position based on the pitch value.
     * @param pitch The pitch value.
     */
    void updateBallPosition(double pitch);

    /**
     * @brief Calculates the new ball position based on the pitch value.
     * @param pitch The pitch value.
     * @return The new ball position.
     */
    double calculateBallPosition(double pitch);

    /**
     * @brief Decreases the platform width.
     */
    void decreasePlatformWidth();

    /**
     * @brief Increases the platform width.
     */
    void increasePlatformWidth();

    /**
     * @brief Updates the LED indicator based on the serial port status.
     * @param isOpen The status of the serial port.
     */
    void updateLedIndicator(bool isOpen);

    /**
     * @brief Decreases the time scale of the charts.
     */
    void decreaseTimeScale();

    /**
     * @brief Increases the time scale of the charts.
     */
    void increaseTimeScale();

    /**
     * @brief Changes the language of the application.
     * @param language The language code (e.g., "en" for English, "pl" for Polish).
     */
    void changeLanguage(const QString &language);

private:
    Ui::MainWindow *ui;                     ///< UI object for the main window.
    ChartManager *chartManager;             ///< Manages roll and pitch charts.
    SerialManager *serialManager;           ///< Manages serial communication.
    TerminalLogger *terminalLogger;         ///< Logs terminal output.
    QTimer *animationTimer;                 ///< Timer for updating animations.
    QTimer *clockTimer;                     ///< Timer for updating the clock.
    QLabel *ledIndicator;                   ///< LED indicator for serial status.
    qint64 chartDuration;                   ///< Duration for displaying chart data.
    bool isCounting;                        ///< Indicates if the countdown is active.
    QGraphicsScene *scene;                  ///< Scene for the graphical items.
    Platform *platform;                     ///< Platform object in the scene.
    Ball *ball;                             ///< Ball object in the scene.
    QTime startTime;                        ///< Start time for the countdown.
    QTranslator translator;                 ///< Translator for language changes.

    /**
     * @brief Applies the time scale to the charts.
     */
    void applyTimeScale();
};

#endif // MAINWINDOW_H
