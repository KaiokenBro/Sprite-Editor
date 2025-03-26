#ifndef PREVIEWWINDOW_H
#define PREVIEWWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <iostream>

namespace Ui {
class previewwindow;
}

class previewwindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit previewwindow(QWidget *parent = nullptr);
    ~previewwindow();

public slots:
    void animation();
    void showFrame(int);
    void animate(bool, int);

private:
    Ui::previewwindow *ui;
};

#endif // PREVIEWWINDOW_H
