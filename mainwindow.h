#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "editorwindow.h"

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:

    MainWindow(SaveLoadManager* saveLoadManager, FrameManager* frameManager, EditorWindow* editorWindow, QWidget *parent = nullptr);
    ~MainWindow();

private:

    Ui::MainWindow *ui;
    EditorWindow* editorWindow;
    FrameManager* frameManager;
    SaveLoadManager* saveLoadManager;

public slots:

    void hideWelcomeLabel();
    void displayCreateFileBox();
    void hideCreateFileBox();
    void validateInputs();
    void onSetSizeButtonClicked();
    void disableNewButton();
    void enableNewButton();
    void disableLoadButton();
    void enableLoadButton();
    void openEditorWindowNew();
    void openEditorWindowLoad();
    void loadFile();
    void syncHeightToWidth(const QString &text);
    void syncWidthToHeight(const QString &text);

signals:

    void startNewSprite(int width, int height);
    void openLoadedSprite();

};

#endif // MAINWINDOW_H
