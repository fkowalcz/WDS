#ifndef SERIALMANAGER_H
#define SERIALMANAGER_H

#include <QObject>
#include <QSerialPort>

/**
 * @class SerialManager
 * @brief The SerialManager class manages serial communication.
 *
 * This class handles opening, reading, and processing data from a serial port.
 */
class SerialManager : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief Constructs a SerialManager object.
     * @param parent The parent QObject.
     */
    SerialManager(QObject *parent = nullptr);

    /**
     * @brief Destructor for SerialManager.
     */
    ~SerialManager();

    /**
     * @brief Starts reading data from the specified serial port.
     * @param portName The name of the serial port.
     * @param baudRate The baud rate for the serial communication.
     */
    void startReading(const QString &portName, qint32 baudRate);

    /**
     * @brief Stops reading data from the serial port.
     */
    void stopReading();

signals:
    /**
     * @brief Emitted when new data is available from the serial port.
     * @param rollValue The roll value from the data.
     * @param pitchValue The pitch value from the data.
     */
    void newData(double rollValue, double pitchValue);

    /**
     * @brief Emitted when the serial port is opened or closed.
     * @param isOpen True if the serial port is opened, false otherwise.
     */
    void serialPortOpened(bool isOpen);

private slots:
    /**
     * @brief Reads data from the serial port.
     */
    void readSerialData();

private:
    QSerialPort *serial;       ///< Pointer to the QSerialPort object.
    QString serialBuffer;      ///< Buffer for storing incoming serial data.

    /**
     * @brief Calculates the CRC-16-CCITT checksum for the given data.
     * @param data The data to calculate the checksum for.
     * @return The calculated checksum.
     */
    uint16_t crc16_ccitt(const QByteArray &data);
};

#endif // SERIALMANAGER_H
