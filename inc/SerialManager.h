#ifndef SERIALMANAGER_H
#define SERIALMANAGER_H

#include <QObject>
#include <QSerialPort>

class SerialManager : public QObject
{
    Q_OBJECT

public:
    SerialManager(QObject *parent = nullptr);
    ~SerialManager();

    void startReading(const QString &portName, qint32 baudRate);
    void stopReading();

signals:
    void newData(double rollValue, double pitchValue);
    void serialPortOpened(bool isOpen); // Add this signal

private slots:
    void readSerialData();

private:
    QSerialPort *serial;
    QString serialBuffer;

    uint16_t crc16_ccitt(const QByteArray &data);
};

#endif // SERIALMANAGER_H
