#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDateTime>
#include <QtMath>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , serial(new QSerialPort(this))
    , chartManager(new ChartManager(this))
    , chartDuration(20 * 1000) // 20 seconds in milliseconds
    , isCounting(false)
{
    ui->setupUi(this);

    // Initialize timers
    animationTimer = new QTimer(this);
    connect(animationTimer, &QTimer::timeout, this, &MainWindow::updateAnimation);
    animationTimer->start(16); // 60 FPS (approx.)

    clockTimer = new QTimer(this);
    connect(clockTimer, &QTimer::timeout, this, &MainWindow::updateClock);

    connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::startCountdown);
    connect(ui->pushButton_5, &QPushButton::clicked, this, &MainWindow::decreasePlatformWidth); // Połączenie z przyciskiem zmniejszającym
    connect(ui->pushButton_6, &QPushButton::clicked, this, &MainWindow::increasePlatformWidth); // Połączenie z przyciskiem zwiększającym

    // Add chart views to layout
    ui->horizontalLayout->addWidget(chartManager->getRollChartView());
    ui->horizontalLayout_2->addWidget(chartManager->getPitchChartView());

    // Configure serial port
    serial->setPortName("/dev/ttyACM0"); // Set appropriate port name
    serial->setBaudRate(QSerialPort::Baud115200);
    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setFlowControl(QSerialPort::NoFlowControl);

    if (serial->open(QIODevice::ReadOnly)) {
        connect(animationTimer, &QTimer::timeout, this, &MainWindow::readSerialData); // Connect animation timer to readSerialData
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

    // Configure QGraphicsView
    ui->graphicsView_3->setRenderHint(QPainter::Antialiasing);
    ui->graphicsView_3->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView_3->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

MainWindow::~MainWindow()
{
    if (serial->isOpen())
        serial->close();
    delete ui;
}

void MainWindow::startCountdown() {
    ui->lcdNumber->display("00:00:00");
    startTime = QTime::currentTime();  // Zapisanie czasu startu
    isCounting = true;  // Ustawienie stanu licznika na true
    clockTimer->start(1000); // Start timer with 1000 ms interval (1 second)
}

void MainWindow::updateClock() {
    if (isCounting) {
        QTime currentTime = QTime::currentTime();
        int elapsedSeconds = startTime.secsTo(currentTime);
        QTime displayTime(0, 0);
        displayTime = displayTime.addSecs(elapsedSeconds);
        ui->lcdNumber->display(displayTime.toString("hh:mm:ss"));
    }
}

void MainWindow::updateAnimation() {
    // This method will be called at approximately 60 FPS
    updateBallPosition(0); // Pass 0 or another value if needed to update ball position
    platform->update();
    ball->update();
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
            chartManager->updateCharts(currentTime, rollValue, pitchValue);

            // Update QLabel
            ui->labelCurrentValue->setText(QString("Roll: %1, Pitch: %2").arg(rollValue).arg(pitchValue));

            // Update platform angle
            platform->setAngle(pitchValue);
            // Update ball position
            updateBallPosition(pitchValue);
        }
    }
}

void MainWindow::updateBallPosition(double pitch) {
    if (ball->isFalling()) {
        ball->fall();
        if (isCounting) {
            isCounting = false;  // Zatrzymanie licznika
            clockTimer->stop();  // Zatrzymanie timera czasu
        }
        return;
    }

    // Calculate ball position on the platform
    double ballPosition = calculateBallPosition(pitch);
    double platformWidth = platform->rect().width();

    // Check if the ball is on the platform
    if (ballPosition < -platformWidth / 2 || ballPosition > platformWidth / 2) {
        // Ball is off the platform, start falling
        ball->fall();
        if (isCounting) {
            isCounting = false;  // Zatrzymanie licznika
            clockTimer->stop();  // Zatrzymanie timera czasu
        }
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

    ballPosition += ballSpeed * 0.1; // Update position with some time step
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
                    chartManager->updateCharts(currentTime, rollValue, pitchValue);

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

void MainWindow::increasePlatformWidth()
{
    QRectF rect = platform->rect();
    rect.setWidth(rect.width() + 20); // Zwiększenie szerokości o 20 jednostek
    rect.moveCenter(QPointF(0, 0)); // Utrzymanie środka platformy w miejscu
    platform->setRect(rect);
}

void MainWindow::decreasePlatformWidth()
{
    QRectF rect = platform->rect();
    if (rect.width() > 20) { // Minimalna szerokość, aby uniknąć znikania platformy
        rect.setWidth(rect.width() - 20); // Zmniejszenie szerokości o 20 jednostek
        rect.moveCenter(QPointF(0, 0)); // Utrzymanie środka platformy w miejscu
        platform->setRect(rect);
    }
}
