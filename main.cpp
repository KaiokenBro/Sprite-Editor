/**
 * @file main.cpp
 *
 * Team Members:
 * - Victor Valdez Landa (READY)
 * - Harrison Doppelt (READY)
 * - Charlie Jones (READY)
 * - Sam Garcia (READY)
 * - Richard Nghiem
 * - Nicholas Cicalese
 *
 * GitHub Usernames:
 * - KaiokenBro
 * - hdoppelt
 * - charlesj24240
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

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    MainWindow w;
    QApplication::setStyle("Fusion");
    w.show();
    return a.exec();
}
