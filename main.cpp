/**
 * @file main.cpp
 *
 * Team Members:
 * - Victor Valdez Landa
 * - Harrison Doppelt (READY)
 * - Charlie Jones
 * - Sam Garcia
 * - Richard Nghiem
 * - Nicholas Cicalese
 *
 * GitHub Usernames:
 * - KaiokenBro
 * - hdoppelt
 * -
 * -
 * -
 *
 * Repository:
 * - https://github.com/UofU-CS3505/cs3505-assignment8-KaiokenBro
 *
 * Creative Element:
 * -
 *
 * @date 03/23/2025
 */

#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    // Eventually add an instance of teh Model here like this, Model m;
    // Then do this, MainWindow w(&m);
    MainWindow w;
    w.show();
    return a.exec();
}
