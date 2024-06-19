#include "TerminalLogger.h"
#include <QDateTime>

TerminalLogger::TerminalLogger(QPlainTextEdit *plainTextEdit, QObject *parent)
    : QObject(parent), plainTextEdit(plainTextEdit)
{
    // Ensure plainTextEdit is not null
    Q_ASSERT(plainTextEdit != nullptr);
}

void TerminalLogger::logMeasurement(double pitch, double roll)
{
    QString timestamp = QDateTime::currentDateTime().toString("hh:mm:ss");
    QString logMessage = QString("%1 Pitch: %2 Roll: %3").arg(timestamp).arg(pitch).arg(roll);
    plainTextEdit->appendPlainText(logMessage);
}
