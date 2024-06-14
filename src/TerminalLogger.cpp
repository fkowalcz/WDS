#include "TerminalLogger.h"
#include <QDateTime>

/**
 * @brief Constructs a TerminalLogger object.
 * @param plainTextEdit Pointer to the QPlainTextEdit widget where logs will be displayed.
 * @param parent The parent QObject.
 */
TerminalLogger::TerminalLogger(QPlainTextEdit *plainTextEdit, QObject *parent)
    : QObject(parent), plainTextEdit(plainTextEdit)
{
    // Ensure plainTextEdit is not null
    Q_ASSERT(plainTextEdit != nullptr);
}

/**
 * @brief Logs the pitch and roll measurements.
 *
 * This method logs the current pitch and roll measurements to the QPlainTextEdit widget
 * with a timestamp.
 *
 * @param pitch The pitch measurement.
 * @param roll The roll measurement.
 */
void TerminalLogger::logMeasurement(double pitch, double roll)
{
    QString timestamp = QDateTime::currentDateTime().toString("hh:mm:ss");
    QString logMessage = QString("%1 Pitch: %2 Roll: %3").arg(timestamp).arg(pitch).arg(roll);
    plainTextEdit->appendPlainText(logMessage);
}
