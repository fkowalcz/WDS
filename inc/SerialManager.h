#ifndef SERIALMANAGER_H
#define SERIALMANAGER_H

#include <QObject>
#include <QSerialPort>

/**
 * @class SerialManager
 * @brief The SerialManager class handles serial communication.
 *
 * This class manages reading data from a serial port and emits signals
 * when new data is received or when the serial port is opened or closed.
 */
class SerialManager : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief Constructs a SerialManager object.
     * @param parent The parent object.
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
     * @brief Signal emitted when new data is received from the serial port.
     * @param rollValue The roll value from the data.
     * @param pitchValue The pitch value from the data.
     */
    void newData(double rollValue, double pitchValue);

    /**
     * @brief Signal emitted when the serial port is opened or closed.
     * @param isOpen Indicates if the serial port is open (true) or closed (false).
     */
    void serialPortOpened(bool isOpen);

private slots:
    /**
     * @brief Slot to read data from the serial port.
     */
    void readSerialData();

private:
    QSerialPort *serial;       ///< The serial port object.
    QString serialBuffer;      ///< Buffer to store incoming serial data.

    /**
     * @brief Computes the CRC-16-CCITT checksum for the given data.
     * @param data The data for which the checksum is computed.
     * @return The computed CRC-16-CCITT checksum.
     */
    uint16_t crc16_ccitt(const QByteArray &data);
};

#endif // SERIALMANAGER_H
