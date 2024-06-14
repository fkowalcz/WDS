#include "SerialManager.h"
#include <QDateTime>
#include <QDebug> // Add this include

SerialManager::SerialManager(QObject *parent)
    : QObject(parent)
    , serial(new QSerialPort(this))
{
}

SerialManager::~SerialManager()
{
    if (serial->isOpen())
        serial->close();
}

void SerialManager::startReading(const QString &portName, qint32 baudRate)
{
    serial->setPortName(portName);
    serial->setBaudRate(baudRate);
    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setFlowControl(QSerialPort::NoFlowControl);

    if (serial->open(QIODevice::ReadOnly)) {
        connect(serial, &QSerialPort::readyRead, this, &SerialManager::readSerialData);
    } else {
        qDebug() << "Failed to open port!"; // qDebug() requires <QDebug> to be included
    }
}

void SerialManager::stopReading()
{
    if (serial->isOpen()) {
        serial->close();
    }
}

void SerialManager::readSerialData()
{
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
                    emit newData(rollValue, pitchValue);
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
}
