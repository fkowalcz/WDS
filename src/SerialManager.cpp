#include "SerialManager.h"
#include <QSerialPortInfo>
#include <QDebug>

/**
 * @brief Calculates the CRC-16-CCITT checksum for the given data.
 *
 * This method calculates the CRC-16-CCITT checksum for a given byte array.
 * The CRC-16-CCITT is a type of cyclic redundancy check used to detect errors
 * in data transmission. This implementation uses a polynomial of 0x1021 and
 * an initial value of 0xFFFF.
 *
 * @param data The data to calculate the checksum for.
 * @return The calculated checksum.
 */
uint16_t SerialManager::crc16_ccitt(const QByteArray &data) {
    uint16_t crc = 0xFFFF;  // Initialize the CRC to 0xFFFF
    for (char byte : data) {
        crc ^= static_cast<uint8_t>(byte) << 8;  // XOR byte into the upper 8 bits of crc

        // Process each bit in the byte
        for (int i = 0; i < 8; i++) {
            if (crc & 0x8000) {  // If the upper bit is set
                crc = (crc << 1) ^ 0x1021;  // Shift left and XOR with the polynomial 0x1021
            } else {
                crc = crc << 1;  // Otherwise, just shift left
            }
        }
    }
    return crc;  // Return the calculated CRC value
}

/**
 * @brief Constructs a SerialManager object.
 * @param parent The parent QObject.
 */
SerialManager::SerialManager(QObject *parent)
    : QObject(parent), serial(new QSerialPort(this))
{
    connect(serial, &QSerialPort::readyRead, this, &SerialManager::readSerialData);
}

/**
 * @brief Destructor for SerialManager.
 */
SerialManager::~SerialManager()
{
    if (serial->isOpen()) {
        serial->close();
    }
}

/**
 * @brief Starts reading data from the specified serial port.
 * @param portName The name of the serial port.
 * @param baudRate The baud rate for the serial communication.
 */
void SerialManager::startReading(const QString &portName, qint32 baudRate)
{
    serial->setPortName(portName);
    serial->setBaudRate(baudRate);
    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setFlowControl(QSerialPort::NoFlowControl);

    if (serial->open(QIODevice::ReadOnly)) {
        emit serialPortOpened(true); // Emit signal indicating port is open
        qDebug() << "Serial port opened successfully!";
    } else {
        emit serialPortOpened(false); // Emit signal indicating port failed to open
        qDebug() << "Failed to open port!";
    }
}

/**
 * @brief Stops reading data from the serial port.
 */
void SerialManager::stopReading()
{
    if (serial->isOpen()) {
        serial->close();
        emit serialPortOpened(false); // Emit signal indicating port is closed
    }
}

/**
 * @brief Reads data from the serial port.
 *
 * This method reads incoming data from the serial port, processes it,
 * and emits the newData signal if valid data is received.
 */
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
