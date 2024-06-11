#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDateTime>
#include <QtCharts>
#include <QSerialPort>
#include "platform.h"
#include "ball.h"
#include <QtMath>

using namespace QtCharts;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , serial(new QSerialPort(this))
    , rollSeries(new QLineSeries())
    , pitchSeries(new QLineSeries())
    , rollChart(new QChart())
    , pitchChart(new QChart())
    , rollChartView(new QChartView(rollChart))
    , pitchChartView(new QChartView(pitchChart))
    , chartDuration(20 * 1000) // 20 seconds in milliseconds
{
    ui->setupUi(this);

    // Initialize timer
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::updateClock);
    connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::startCountdown);

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

    // Add chart views to layout
    ui->horizontalLayout->addWidget(rollChartView);
    ui->horizontalLayout_2->addWidget(pitchChartView);

    // Configure serial port
    serial->setPortName("/dev/ttyACM0"); // Set appropriate port name
    serial->setBaudRate(QSerialPort::Baud115200);
    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setFlowControl(QSerialPort::NoFlowControl);

    if (serial->open(QIODevice::ReadOnly)) {
        connect(timer, &QTimer::timeout, this, &MainWindow::readSerialData); // Connect timer to readSerialData
        timer->start(100); // Start timer with 100 ms interval (10 times per second)
    } else {
        qDebug() << "Failed to open port!";
    }

    // Initialize scene and items
    scene = new QGraphicsScene(0, 0, ui->graphicsView_3->width(), ui->graphicsView_3->height(), this);
    ui->graphicsView_3->setScene(scene);

    platform = new Platform();
    scene->addItem(platform);
    platform->setPos(ui->graphicsView_3->width() / 2, ui->graphicsView_3->height() / 2);

    ball = new Ball();
    scene->addItem(ball);
    ball->setPos(platform->x(), platform->y() - 10);
}

MainWindow::~MainWindow()
{
    if (serial->isOpen())
        serial->close();
    delete ui;
}

void MainWindow::startCountdown() {
    timer->stop();
    ui->lcdNumber->display("00:00:00");
    timer->start(1000); // Start timer with 1000 ms interval (1 second)
}

void MainWindow::updateClock() {
    QDateTime currentTime = QDateTime::currentDateTime();
    ui->lcdNumber->display(currentTime.toString("hh:mm:ss"));
}

void MainWindow::updateCharts() {
    qint64 currentTime = QDateTime::currentMSecsSinceEpoch();
    QByteArray data = serial->readAll();
    QString dataString = QString::fromStdString(data.toStdString());
    QStringList dataList = dataString.trimmed().split(' ');

    if (dataList.size() == 2) {
        bool ok1, ok2;
        double rollValue = dataList[0].toDouble(&ok1);
        double pitchValue = dataList[1].toDouble(&ok2);

        if (ok1 && ok2) {
            rollSeries->append(currentTime, rollValue);
            pitchSeries->append(currentTime, pitchValue);

            // Update QLabel
            ui->labelCurrentValue->setText(QString("Roll: %1, Pitch: %2").arg(rollValue).arg(pitchValue));

            // Update platform angle
            platform->setAngle(pitchValue);
            // Update ball position
            updateBallPosition(pitchValue);
        }
    }

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

void MainWindow::updateBallPosition(double pitch) {
    // Calculate ball position on the platform
    double ballPosition = calculateBallPosition(pitch);
    double platformWidth = platform->rect().width();

    // Check if the ball is on the platform
    if (ballPosition < -platformWidth / 2 || ballPosition > platformWidth / 2) {
        // Ball is off the platform, start falling
        ball->fall();
    } else {
        // Ball is on the platform
        // Calculate the position of the ball relative to the platform's rotation
        double angleRad = qDegreesToRadians(platform->rotation());
        double ballX = platform->x() + ballPosition * cos(angleRad);
        double ballY = platform->y() + ballPosition * sin(angleRad);

        // Update ball position relative to platform
        ball->setPosition(ballX, ballY);
    }
}




double MainWindow::calculateBallPosition(double pitch) {
    // Calculate ball position based on pitch angle
    double g = 9.81; // Gravity
    double ballSpeed = g * sin(pitch * M_PI / 180.0);
    static double ballPosition = 0;

    // Update position with some time step
    if (ballPosition >= -200 && ballPosition <= 200) {
        ballPosition += ballSpeed * 0.1;
    }
    return ballPosition;
}


void MainWindow::readSerialData() {
    // Read available data from the serial port
    QByteArray data = serial->readAll();

    // Add read data to buffer
    serialBuffer += data;

    // Search for data packets in the buffer
    while (serialBuffer.contains("\n\r")) {
        // Find the start and end of the data packet
        int startIndex = serialBuffer.indexOf('b');
        int endIndex = serialBuffer.indexOf("\n\r", startIndex);

        // If a valid start and end of the data packet are found
        if (startIndex != -1 && endIndex != -1) {
            // Extract the data packet
            QString package = serialBuffer.mid(startIndex, endIndex - startIndex + 2);

            // Remove the processed data packet from the buffer
            serialBuffer.remove(0, endIndex + 2);

            // Split the data packet into roll and pitch
            QStringList dataList = package.trimmed().split(' ');
            if (dataList.size() == 2) {
                bool ok1, ok2;
                double rollValue = dataList[0].mid(1).toDouble(&ok1); // Skip 'b' at the beginning
                double pitchValue = dataList[1].toDouble(&ok2);

                if (ok1 && ok2) {
                    // Add read data to series
                    qint64 currentTime = QDateTime::currentMSecsSinceEpoch();
                    rollSeries->append(currentTime, rollValue);
                    pitchSeries->append(currentTime, pitchValue);

                    // Update QLabel
                    ui->labelCurrentValue->setText(QString("Roll: %1, Pitch: %2").arg(rollValue).arg(pitchValue));

                    // Update platform angle and ball position
                    platform->setAngle(pitchValue);
                    updateBallPosition(pitchValue);

                    // Print pitch and roll angles
                    qDebug() << "Roll:" << rollValue << "Pitch:" << pitchValue;
                }
            }
        } else {
            // If a valid data packet is not found, remove everything before the first 'b' character
            int invalidStartIndex = serialBuffer.indexOf('b');
            if (invalidStartIndex != -1) {
                serialBuffer.remove(0, invalidStartIndex);
            } else {
                // If no 'b' character is found, clear the buffer
                serialBuffer.clear();
            }
        }
    }

    // Update charts after adding new data
    updateCharts();
}
