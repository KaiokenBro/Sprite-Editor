/**
 * @file main.cpp
 * @brief Entry point for the Sprite Editor application. Initializes Qt application and main UI components.
 * @date 03/31/2025
 */

#include "mainwindow.h"

#include <QApplication>

/**
 * @brief The main function initializes the Qt application and shows the MainWindow.
 * @param argc Command-line argument count.
 * @param argv Command-line argument values.
 * @return Exit code returned by the Qt application event loop.
 */
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
