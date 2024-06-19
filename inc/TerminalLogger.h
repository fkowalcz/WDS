#ifndef TERMINALLOGGER_H
#define TERMINALLOGGER_H

#include <QObject>
#include <QPlainTextEdit>

/**
 * @class TerminalLogger
 * @brief The TerminalLogger class logs pitch and roll measurements to a QPlainTextEdit widget.
 *
 * This class is responsible for displaying pitch and roll measurements in a
 * QPlainTextEdit widget. It provides a method to log measurements and ensures
 * that the widget is properly initialized.
 */
class TerminalLogger : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief Constructs a TerminalLogger object.
     * @param plainTextEdit The QPlainTextEdit widget where logs will be displayed.
     * @param parent The parent object.
     *
     * This constructor initializes the TerminalLogger with the given QPlainTextEdit
     * widget. The widget must be valid and not null.
     */
    TerminalLogger(QPlainTextEdit *plainTextEdit, QObject *parent = nullptr);

    /**
     * @brief Logs a measurement to the QPlainTextEdit widget.
     * @param pitch The pitch value to log.
     * @param roll The roll value to log.
     *
     * This method logs the given pitch and roll values along with a timestamp
     * to the QPlainTextEdit widget. Each log entry includes the current time
     * in the format "hh:mm:ss" followed by the pitch and roll values.
     */
    void logMeasurement(double pitch, double roll);

private:
    QPlainTextEdit *plainTextEdit; ///< The QPlainTextEdit widget where logs are displayed.
};

#endif // TERMINALLOGGER_H
