#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDateTime>
#include <QtMath>
#include <QTranslator>
#include <QLocale>
#include <QDebug>
#include <QApplication>

/**
 * @brief Constructs a MainWindow object.
 * @param parent The parent widget.
 */
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , chartManager(new ChartManager(this))
    , serialManager(new SerialManager(this))
    , terminalLogger(nullptr)
    , chartDuration(20 * 1000) // 20 seconds in milliseconds
    , isCounting(false)
{
    ui->setupUi(this);

    // Initialize TerminalLogger after UI setup
    terminalLogger = new TerminalLogger(ui->plainTextEdit_Terminal, this);

    // Initialize LED indicator
    ledIndicator = ui->labelLedIndicator;  // Assuming you named the QLabel 'labelLedIndicator' in Designer
    ledIndicator->setStyleSheet("background-color: red;"); // Initial state is "off"

    // Initialize timers
    animationTimer = new QTimer(this);
    connect(animationTimer, &QTimer::timeout, this, &MainWindow::updateAnimation);
    animationTimer->start(16); // 60 FPS (approx.)

    clockTimer = new QTimer(this);
    connect(clockTimer, &QTimer::timeout, this, &MainWindow::updateClock);

    connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::startCountdown);
    connect(ui->pushButton_5, &QPushButton::clicked, this, &MainWindow::decreasePlatformWidth);
    connect(ui->pushButton_6, &QPushButton::clicked, this, &MainWindow::increasePlatformWidth);

    // Add chart views to layout
    ui->horizontalLayout->addWidget(chartManager->getRollChartView());
    ui->horizontalLayout_2->addWidget(chartManager->getPitchChartView());

    // Start serial communication
    connect(serialManager, &SerialManager::newData, this, &MainWindow::updateCharts);
    connect(serialManager, &SerialManager::serialPortOpened, this, &MainWindow::updateLedIndicator);
    serialManager->startReading("/dev/ttyACM0", QSerialPort::Baud115200);

    // Initialize scene and items
    scene = new QGraphicsScene(0, 0, ui->graphicsView_3->width(), ui->graphicsView_3->height(), this);
    ui->graphicsView_3->setScene(scene);

    platform = new Platform();
    scene->addItem(platform);
    platform->setPos(ui->graphicsView_3->width() / 2, ui->graphicsView_3->height() / 2);

    ball = new Ball();
    scene->addItem(ball);
    resetBallPosition();

    // Configure QGraphicsView
    ui->graphicsView_3->setRenderHint(QPainter::Antialiasing);
    ui->graphicsView_3->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView_3->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // Connect time scale buttons
    connect(ui->pushButton_3, &QPushButton::clicked, this, &MainWindow::decreaseTimeScale);
    connect(ui->pushButton_4, &QPushButton::clicked, this, &MainWindow::increaseTimeScale);

    // Add language selection
    connect(ui->comboBoxLanguage, &QComboBox::currentTextChanged, this, &MainWindow::changeLanguage);

    ui->comboBoxLanguage->addItem("English", "en");
    ui->comboBoxLanguage->addItem("Polski", "pl");
}

/**
 * @brief Changes the language of the application.
 * @param language The language code (e.g., "en" for English, "pl" for Polish).
 */
void MainWindow::changeLanguage(const QString &language)
{
    QString qmPath = "translations/translations_" + language + ".qm";
    if (translator.load(qmPath)) {
        qApp->installTranslator(&translator);
        ui->retranslateUi(this);
    } else {
        qDebug() << "Failed to load translation file:" << qmPath;
    }
}

/**
 * @brief Destructor for MainWindow.
 */
MainWindow::~MainWindow()
{
    delete ui;
}

/**
 * @brief Starts the countdown timer.
 *
 * This method initializes the countdown timer and starts the ball movement.
 */
void MainWindow::startCountdown() {
    ui->lcdNumber->display("00:00:00");
    startTime = QTime::currentTime();
    isCounting = true;
    clockTimer->start(1000);

    ball->startMovement();
}

/**
 * @brief Resets the ball position on the platform.
 */
void MainWindow::resetBallPosition()
{
    ball->setPosition(platform->x(), platform->y() - 10);
}

/**
 * @brief Updates the clock display.
 */
void MainWindow::updateClock() {
    if (isCounting) {
        QTime currentTime = QTime::currentTime();
        int elapsedSeconds = startTime.secsTo(currentTime);
        QTime displayTime(0, 0);
        displayTime = displayTime.addSecs(elapsedSeconds);
        ui->lcdNumber->display(displayTime.toString("hh:mm:ss"));
    }
}

/**
 * @brief Updates the animation of the ball and platform.
 */
void MainWindow::updateAnimation() {
    if (ball->isMoving()) {
        updateBallPosition(0);
    }
    platform->update();
    ball->update();
}

/**
 * @brief Updates the charts with new roll and pitch values.
 * @param rollValue The roll value.
 * @param pitchValue The pitch value.
 */
void MainWindow::updateCharts(double rollValue, double pitchValue) {
    qint64 currentTime = QDateTime::currentMSecsSinceEpoch();

    chartManager->updateCharts(currentTime, rollValue, pitchValue);

    ui->labelCurrentValue->setText(QString("Roll: %1, Pitch: %2").arg(rollValue).arg(pitchValue));

    terminalLogger->logMeasurement(pitchValue, rollValue);

    platform->setAngle(pitchValue);
    updateBallPosition(pitchValue);
}

/**
 * @brief Updates the ball position based on the pitch value.
 * @param pitch The pitch value.
 */
void MainWindow::updateBallPosition(double pitch) {
    if (ball->isFalling()) {
        ball->fall();
        if (isCounting) {
            isCounting = false;
            clockTimer->stop();
        }
        return;
    }

    double ballPosition = calculateBallPosition(pitch);
    double platformWidth = platform->rect().width();

    if (ballPosition < -platformWidth / 2 || ballPosition > platformWidth / 2) {
        ball->fall();
        if (isCounting) {
            isCounting = false;
            clockTimer->stop();
        }
    } else {
        double angleRad = qDegreesToRadians(platform->rotation());
        double ballX = platform->x() + ballPosition * cos(angleRad);
        double ballY = platform->y() + ballPosition * sin(angleRad);

        ball->setPosition(ballX, ballY);
    }
}

/**
 * @brief Calculates the new ball position based on the pitch value.
 * @param pitch The pitch value.
 * @return The new ball position.
 */
double MainWindow::calculateBallPosition(double pitch) {
    double g = 9.81;
    double ballSpeed = g * sin(pitch * M_PI / 180.0);
    static double ballPosition = 0;

    ballPosition += ballSpeed * 0.1;
    return ballPosition;
}

/**
 * @brief Increases the platform width.
 */
void MainWindow::increasePlatformWidth()
{
    QRectF rect = platform->rect();
    rect.setWidth(rect.width() + 20);
    rect.moveCenter(QPointF(0, 0));
    platform->setRect(rect);
}

/**
 * @brief Decreases the platform width.
 */
void MainWindow::decreasePlatformWidth()
{
    QRectF rect = platform->rect();
    if (rect.width() > 20) {
        rect.setWidth(rect.width() - 20);
        rect.moveCenter(QPointF(0, 0));
        platform->setRect(rect);
    }
}

/**
 * @brief Updates the LED indicator based on the serial port status.
 * @param isOpen The status of the serial port.
 */
void MainWindow::updateLedIndicator(bool isOpen)
{
    if (isOpen) {
        ledIndicator->setStyleSheet("background-color: green;");
    } else {
        ledIndicator->setStyleSheet("background-color: red;");
    }
}

/**
 * @brief Applies the time scale to the charts.
 */
void MainWindow::applyTimeScale()
{
    qDebug() << "Applying time scale. Duration: " << chartDuration / 1000 << " seconds.";
    qint64 currentTime = QDateTime::currentMSecsSinceEpoch();
    QDateTime minTime = QDateTime::fromMSecsSinceEpoch(currentTime - chartDuration);
    QDateTime maxTime = QDateTime::fromMSecsSinceEpoch(currentTime);

    QDateTimeAxis *axisXRoll = qobject_cast<QDateTimeAxis*>(chartManager->getRollChart()->axes(Qt::Horizontal).first());
    if (axisXRoll) {
        axisXRoll->setMin(minTime);
        axisXRoll->setMax(maxTime);
    }

    QDateTimeAxis *axisXPitch = qobject_cast<QDateTimeAxis*>(chartManager->getPitchChart()->axes(Qt::Horizontal).first());
    if (axisXPitch) {
        axisXPitch->setMin(minTime);
        axisXPitch->setMax(maxTime);
    }

    chartManager->getRollChartView()->repaint();
    chartManager->getPitchChartView()->repaint();
}

/**
 * @brief Decreases the time scale of the charts.
 */
void MainWindow::decreaseTimeScale()
{
    if (chartDuration > 5 * 1000) { // Ensure the duration doesn't go below 5 seconds
        chartDuration -= 5 * 1000; // Decrease by 5 seconds
        applyTimeScale();
        qDebug() << "Time scale decreased. New duration: " << chartDuration / 1000 << " seconds.";
    } else {
        qDebug() << "Minimum time scale reached.";
    }
}

/**
 * @brief Increases the time scale of the charts.
 */
void MainWindow::increaseTimeScale()
{
    chartDuration += 5 * 1000; // Increase by 5 seconds
    applyTimeScale();
    qDebug() << "Time scale increased. New duration: " << chartDuration / 1000 << " seconds.";
}
