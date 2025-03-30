/**
 * @file main.cpp
 *
 * Team Members:
 * - Victor Valdez Landa (READY)
 * - Harrison Doppelt (READY)
 * - Charlie Jones (READY)
 * - Sam Garcia (READY)
 * - Richard Nghiem
 * - Nicholas Cicalese (READY)
 *
 * GitHub Usernames:
 * - KaiokenBro
 * - hdoppelt
 * - charlesj24240
 * - ncicalese
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

//

#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[]) {

    QApplication a(argc, argv);
    QApplication::setStyle("Fusion");

    SaveLoadManager saveLoadManager;

    // Placeholder dimensions; will be updated later based on user input
    int initialWidth = 1;
    int initialHeight = 1;
    FrameManager frameManager(initialHeight, initialWidth);

    // Pass SaveLoadManager and FrameManager to EditorWindow for editing and saving
    EditorWindow editorWindow(&saveLoadManager, &frameManager, initialWidth, initialHeight);

    MainWindow mainWindow(&saveLoadManager, &frameManager, &editorWindow);

    editorWindow.hide();
    mainWindow.show();
    return a.exec();
}
