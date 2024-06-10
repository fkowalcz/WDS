#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDateTime>
#include <QtCharts>
#include <QSerialPort>

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

    // Inicjalizacja timera
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::updateClock);
    connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::startCountdown);

    // Konfiguracja wykresów
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

    // Dodanie widoków wykresów do layoutu
    ui->horizontalLayout->addWidget(rollChartView);
    ui->horizontalLayout_2->addWidget(pitchChartView);

    // Konfiguracja portu szeregowego
    serial->setPortName("/dev/ttyACM0"); // Ustaw odpowiednią nazwę portu
    serial->setBaudRate(QSerialPort::Baud115200);
    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setFlowControl(QSerialPort::NoFlowControl);

    if (serial->open(QIODevice::ReadOnly)) {
        connect(timer, &QTimer::timeout, this, &MainWindow::readSerialData); // Połącz timer z funkcją odczytu danych
        timer->start(100); // Uruchom timer z częstotliwością 100 ms (10 razy na sekundę)
    } else {
        qDebug() << "Failed to open port!";
    }
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
    timer->start(1000); // Uruchomienie timera, który emituje sygnał co 1000 ms (1 sekunda)
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

            // Aktualizacja QLabel
            ui->labelCurrentValue->setText(QString("Roll: %1, Pitch: %2").arg(rollValue).arg(pitchValue));
        }
    }

    // Aktualizacja osi czasu
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

    // Odśwież widoki wykresów
    rollChartView->repaint();
    pitchChartView->repaint();
}

void MainWindow::readSerialData() {
    // Odczytaj dostępne dane z portu szeregowego
    QByteArray data = serial->readAll();

    // Dodaj odczytane dane do bufora
    serialBuffer += data;

        // Szukaj paczek danych w buforze
        while (serialBuffer.contains("\n\r")) {
            // Znajdź początek i koniec paczki danych
            int startIndex = serialBuffer.indexOf('b');
            int endIndex = serialBuffer.indexOf("\n\r", startIndex);

            // Jeśli znaleziono poprawny początek i koniec paczki danych
            if (startIndex != -1 && endIndex != -1) {
                // Wyodrębnij paczkę danych
                QString package = serialBuffer.mid(startIndex, endIndex - startIndex + 2);

                // Usuń przetworzoną paczkę danych z bufora
                serialBuffer.remove(0, endIndex + 2);

                // Podziel paczkę danych na role i pitch
                QStringList dataList = package.trimmed().split(' ');
                if (dataList.size() == 2) {
                    bool ok1, ok2;
                    double rollValue = dataList[0].mid(1).toDouble(&ok1); // Pomijamy znak 'b' na początku
                    double pitchValue = dataList[1].toDouble(&ok2);

                    if (ok1 && ok2) {
                        // Dodaj odczytane dane do serii
                        qint64 currentTime = QDateTime::currentMSecsSinceEpoch();
                        rollSeries->append(currentTime, rollValue);
                        pitchSeries->append(currentTime, pitchValue);

                        // Aktualizacja QLabel
                        ui->labelCurrentValue->setText(QString("Roll: %1, Pitch: %2").arg(rollValue).arg(pitchValue));

                        // Wypisz kąty pitch i roll
                        qDebug() << "Roll:" << rollValue << "Pitch:" << pitchValue;
                    }
                }
            } else {
                // Jeśli nie znaleziono poprawnej paczki danych, usuń wszystko przed pierwszym znakiem 'b'
                int invalidStartIndex = serialBuffer.indexOf('b');
                if (invalidStartIndex != -1) {
                    serialBuffer.remove(0, invalidStartIndex);
                } else {
                    // Jeśli nie znaleziono znaku 'b', wyczyść bufor
                    serialBuffer.clear();
                }
            }
        }

        // Aktualizacja wykresów po dodaniu nowych danych
        updateCharts();
    }
