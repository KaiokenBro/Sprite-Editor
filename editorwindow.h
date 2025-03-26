#ifndef EDITORWINDOW_H
#define EDITORWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class EditorWindow; }
QT_END_NAMESPACE

class EditorWindow : public QMainWindow {
    Q_OBJECT

public:

    explicit EditorWindow(QWidget *parent = nullptr);
    ~EditorWindow();

private:

    Ui::EditorWindow *ui;

public slots:

    void animateClicked();

};

#endif // EDITORWINDOW_H
