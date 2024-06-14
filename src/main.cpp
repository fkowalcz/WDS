#include "inc/mainwindow.h"
#include "ui_mainwindow.h"

#include "../inc/mainwindow.h"
#include <QApplication>

/**
 * @brief The main function for the application.
 *
 * This function initializes the QApplication, creates the MainWindow,
 * and starts the event loop.
 *
 * @param argc The number of command-line arguments.
 * @param argv The array of command-line arguments.
 * @return The exit status of the application.
 */
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
