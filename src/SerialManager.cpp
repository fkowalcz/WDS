#include "SerialManager.h"
#include <QSerialPortInfo>
#include <QDebug>

// CRC calculation function
uint16_t SerialManager::crc16_ccitt(const QByteArray &data) {
    uint16_t crc = 0xFFFF;
    for (char byte : data) {
        crc ^= static_cast<uint8_t>(byte) << 8;
        for (int i = 0; i < 8; i++) {
            if (crc & 0x8000) {
                crc = (crc << 1) ^ 0x1021;
            } else {
                crc = crc << 1;
            }
        }
    }
    return crc;
}

SerialManager::SerialManager(QObject *parent)
    : QObject(parent), serial(new QSerialPort(this))
{
    connect(serial, &QSerialPort::readyRead, this, &SerialManager::readSerialData);
}

SerialManager::~SerialManager()
{
    if (serial->isOpen()) {
        serial->close();
    }
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
        qDebug() << "Serial port opened successfully!";
    } else {
        qDebug() << "Failed to open port!";
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
    QByteArray data = serial->readAll();
    serialBuffer += data;

    while (serialBuffer.contains("\n\r")) {
        int endIndex = serialBuffer.indexOf("\n\r");
        QString line = serialBuffer.left(endIndex).trimmed();
        serialBuffer.remove(0, endIndex + 2);

        int crcIndex = line.lastIndexOf(' ');
        if (crcIndex != -1) {
            QString dataPart = line.left(crcIndex);
            QString crcPart = line.mid(crcIndex + 1);

            bool ok;
            uint16_t receivedCrc = crcPart.toUShort(&ok, 16);
            if (ok) {
                uint16_t calculatedCrc = crc16_ccitt(dataPart.toUtf8());
                if (calculatedCrc == receivedCrc) {
                    QStringList dataList = dataPart.split(' ');
                    if (dataList.size() == 2) {
                        bool ok1, ok2;
                        double rollValue = dataList[0].mid(1).toDouble(&ok1); // Skip 'b' at the beginning
                        double pitchValue = dataList[1].toDouble(&ok2);

                        if (ok1 && ok2) {
                            emit newData(rollValue, pitchValue);
                        }
                    }
                } else {
                    qDebug() << "CRC mismatch!";
                }
            } else {
                qDebug() << "Invalid CRC format!";
            }
        }
    }
}
