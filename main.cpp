/**
 * @file main.cpp
 *
 * Team Members:
 * - Victor Valdez Landa
 * - Harrison Doppelt
 * - Charlie Jones
 * - Sam Garcia
 * - Richard Nghiem
 * - Nicholas Cicalese
 *
 * GitHub Usernames:
 * - KaiokenBro
 * - hdoppelt
 * - charlesj24240
 * - ncicalese
 * - SamuelFGarcia4
 * - Urichng
 *
 * Repository:
 * - https://github.com/UofU-CS3505/cs3505-assignment8-KaiokenBro
 *
 * Additional Features:
 * - Invert Colors
 * - Rotate Sprite
 * - Copy Pixel Color
 *
 * @date 03/31/2025
 */

#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    QApplication::setStyle("Fusion");
    SaveLoadManager saveLoadManager;

    // Placeholder, updated later based on user input
    int initialWidth = 1;
    int initialHeight = 1;

    FrameManager frameManager(initialHeight, initialWidth);
    EditorWindow editorWindow(&saveLoadManager, &frameManager, initialWidth, initialHeight);
    MainWindow mainWindow(&saveLoadManager, &frameManager, &editorWindow);
    editorWindow.hide();
    mainWindow.show();
    return a.exec();
}
