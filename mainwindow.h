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

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:

    Ui::MainWindow *ui;

    // Pointer to EditorWindow
    EditorWindow *editorWindow;

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
    void openEditorWindow();
    void loadFile();
    void syncHeightToWidth(const QString &text);
    void syncWidthToHeight(const QString &text);

};

#endif // MAINWINDOW_H
