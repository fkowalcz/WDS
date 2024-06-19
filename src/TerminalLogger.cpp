#include "TerminalLogger.h"
#include <QDateTime>

/**
 * @brief Constructs a TerminalLogger object.
 * @param plainTextEdit The QPlainTextEdit widget where logs will be displayed.
 * @param parent The parent object.
 *
 * This constructor initializes the TerminalLogger with the given QPlainTextEdit
 * widget. The widget must be valid and not null.
 */
TerminalLogger::TerminalLogger(QPlainTextEdit *plainTextEdit, QObject *parent)
    : QObject(parent), plainTextEdit(plainTextEdit)
{
    // Ensure plainTextEdit is not null
    Q_ASSERT(plainTextEdit != nullptr);
}

/**
 * @brief Logs a measurement to the QPlainTextEdit widget.
 * @param pitch The pitch value to log.
 * @param roll The roll value to log.
 *
 * This method logs the given pitch and roll values along with a timestamp
 * to the QPlainTextEdit widget. Each log entry includes the current time
 * in the format "hh:mm:ss" followed by the pitch and roll values.
 */
void TerminalLogger::logMeasurement(double pitch, double roll)
{
    // Get the current timestamp in "hh:mm:ss" format
    QString timestamp = QDateTime::currentDateTime().toString("hh:mm:ss");

    // Create a log message with the format "timestamp Pitch: pitch Roll: roll"
    QString logMessage = QString("%1 Pitch: %2 Roll: %3")
                         .arg(timestamp)
                         .arg(pitch)
                         .arg(roll);

    // Append the log message to the plain text edit widget
    plainTextEdit->appendPlainText(logMessage);
}
