#ifndef TERMINALLOGGER_H
#define TERMINALLOGGER_H

#include <QObject>
#include <QPlainTextEdit>

class TerminalLogger : public QObject
{
    Q_OBJECT

public:
    TerminalLogger(QPlainTextEdit *plainTextEdit, QObject *parent = nullptr);
    void logMeasurement(double pitch, double roll);

private:
    QPlainTextEdit *plainTextEdit;
};

#endif // TERMINALLOGGER_H
