#ifndef TERMINALLOGGER_H
#define TERMINALLOGGER_H

#include <QObject>
#include <QPlainTextEdit>

/**
 * @class TerminalLogger
 * @brief The TerminalLogger class logs measurements to a QPlainTextEdit widget.
 *
 * This class handles logging pitch and roll measurements to a text widget.
 */
class TerminalLogger : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief Constructs a TerminalLogger object.
     * @param plainTextEdit Pointer to the QPlainTextEdit widget where logs will be displayed.
     * @param parent The parent QObject.
     */
    TerminalLogger(QPlainTextEdit *plainTextEdit, QObject *parent = nullptr);

    /**
     * @brief Logs the pitch and roll measurements.
     * @param pitch The pitch measurement.
     * @param roll The roll measurement.
     */
    void logMeasurement(double pitch, double roll);

private:
    QPlainTextEdit *plainTextEdit; ///< Pointer to the QPlainTextEdit widget for displaying logs.
};

#endif // TERMINALLOGGER_H
