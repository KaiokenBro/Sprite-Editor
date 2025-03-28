#ifndef PREVIEWWINDOW_H
#define PREVIEWWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QImage>
#include <QPainter>
#include <vector>
#include <iostream>
#include "frame.h"
#include "framemanager.h"
#include "editorwindow.h"

namespace Ui {
class previewwindow;
}

class previewwindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit previewwindow(int spriteHeight, int spriteWidth, FrameManager *frameManager, QWidget *parent = nullptr);
    ~previewwindow();

public slots:
    void animation();
    void showFrame(Frame);
    void animate(bool, int);

signals:
    void getPixelsOfFrame(int frameIndex);

private:
    Ui::previewwindow *ui;
    int actualHeight;
    int actualWidth;
    FrameManager * frameManager;
    QImage sprite;
};

#endif // PREVIEWWINDOW_H
