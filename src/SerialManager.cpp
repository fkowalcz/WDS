#include "SerialManager.h"
#include <QSerialPortInfo>
#include <QDebug>

/**
 * @brief Computes the CRC-16-CCITT checksum for the given data.
 * @param data The data for which the checksum is computed.
 * @return The computed CRC-16-CCITT checksum.
 *
 * The CRC-16-CCITT (Cyclic Redundancy Check) is a common error-detecting code
 * used to detect accidental changes to raw data. This function calculates the
 * CRC for the provided data using the CCITT polynomial 0x1021.
 *
 * The calculation is performed by iterating over each byte of the input data
 * and updating the CRC for each bit in the byte. The CRC is initially set to
 * 0xFFFF. For each byte, the CRC is XORed with the byte shifted left by 8 bits.
 * Then, for each of the 8 bits, if the highest bit of the CRC is set, the CRC
 * is shifted left by 1 and XORed with the polynomial 0x1021. Otherwise, the
 * CRC is just shifted left by 1. This process ensures that all bits of the
 * input data contribute to the final CRC value.
 */
uint16_t SerialManager::crc16_ccitt(const QByteArray &data) {
    uint16_t crc = 0xFFFF; // Initial CRC value
    for (char byte : data) {
        crc ^= static_cast<uint8_t>(byte) << 8; // XOR CRC with byte shifted left by 8 bits
        for (int i = 0; i < 8; i++) {
            if (crc & 0x8000) { // If the highest bit is set
                crc = (crc << 1) ^ 0x1021; // Shift left and XOR with polynomial
            } else {
                crc = crc << 1; // Just shift left
            }
        }
    }
    return crc; // Return the computed CRC value
}

/**
 * @brief Constructs a SerialManager object.
 * @param parent The parent object.
 *
 * This constructor initializes the SerialManager object by creating a new
 * QSerialPort instance and connecting the readyRead signal of the serial port
 * to the readSerialData slot. This setup allows the SerialManager to handle
 * incoming serial data.
 */
SerialManager::SerialManager(QObject *parent)
    : QObject(parent), serial(new QSerialPort(this))
{
    connect(serial, &QSerialPort::readyRead, this, &SerialManager::readSerialData);
}

/**
 * @brief Destructor for SerialManager.
 *
 * This destructor ensures that the serial port is properly closed if it is
 * still open when the SerialManager object is destroyed.
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
 *
 * This method configures the serial port with the specified port name and
 * baud rate, sets the data format to 8 data bits, no parity, one stop bit, and
 * no flow control. It then attempts to open the serial port in read-only mode.
 * If the port is successfully opened, the serialPortOpened signal is emitted
 * with a value of true. Otherwise, the signal is emitted with a value of false.
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
 *
 * This method closes the serial port if it is open and emits the serialPortOpened
 * signal with a value of false to indicate that the port is closed.
 */
void SerialManager::stopReading()
{
    if (serial->isOpen()) {
        serial->close();
        emit serialPortOpened(false); // Emit signal indicating port is closed
    }
}

/**
 * @brief Slot to read data from the serial port.
 *
 * This slot is called whenever there is new data available on the serial port.
 * It reads all available data from the serial port and appends it to the serial
 * buffer. It then processes complete lines of data terminated by "\n\r". Each
 * line of data is expected to contain two space-separated values followed by a
 * CRC checksum. The data part and the CRC part are extracted and the CRC is
 * verified. If the CRC is valid, the roll and pitch values are extracted and
 * emitted using the newData signal.
 */
void SerialManager::readSerialData()
{
    QByteArray data = serial->readAll(); // Read all available data
    serialBuffer += data; // Append data to the serial buffer

    while (serialBuffer.contains("\n\r")) { // Process complete lines of data
        int endIndex = serialBuffer.indexOf("\n\r");
        QString line = serialBuffer.left(endIndex).trimmed(); // Extract a complete line
        serialBuffer.remove(0, endIndex + 2); // Remove the processed line from the buffer

        int crcIndex = line.lastIndexOf(' '); // Find the last space, separating data and CRC
        if (crcIndex != -1) {
            QString dataPart = line.left(crcIndex); // Extract the data part
            QString crcPart = line.mid(crcIndex + 1); // Extract the CRC part

            bool ok;
            uint16_t receivedCrc = crcPart.toUShort(&ok, 16); // Convert CRC from hex string to integer
            if (ok) {
                uint16_t calculatedCrc = crc16_ccitt(dataPart.toUtf8()); // Calculate CRC for the data part
                if (calculatedCrc == receivedCrc) { // Check if calculated CRC matches received CRC
                    QStringList dataList = dataPart.split(' '); // Split data part into roll and pitch values
                    if (dataList.size() == 2) {
                        bool ok1, ok2;
                        double rollValue = dataList[0].mid(1).toDouble(&ok1); // Extract roll value (skip 'b' at the beginning)
                        double pitchValue = dataList[1].toDouble(&ok2); // Extract pitch value

                        if (ok1 && ok2) { // Check if both values were converted successfully
                            emit newData(rollValue, pitchValue); // Emit newData signal with the extracted values
                        }
                    }
                } else {
                    qDebug() << "CRC mismatch!"; // Log CRC mismatch
                }
            } else {
                qDebug() << "Invalid CRC format!"; // Log invalid CRC format
            }
        }
    }
}
