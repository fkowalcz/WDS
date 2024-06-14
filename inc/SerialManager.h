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

private slots:
    void readSerialData();

private:
    QSerialPort *serial;
    QString serialBuffer;
};

#endif // SERIALMANAGER_H
