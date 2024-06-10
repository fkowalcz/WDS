#include "inc/mainwindow.h"
#include "ui_mainwindow.h"

//#include "../inc/mainwindow.h" // Dodajemy plik nagłówkowy MainWindow
//#include <QApplication>

//int main(int argc, char *argv[])
//{
//    QApplication a(argc, argv);
//    MainWindow w;
//    w.show();
//    return a.exec();
//}


#include "../inc/mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
