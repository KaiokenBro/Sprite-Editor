#ifndef PREVIEWWINDOW_H
#define PREVIEWWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QImage>
#include <QPainter>
#include "frame.h"
#include "framemanager.h"

/**
 * @file previewwindow.h
 * @authors Samuel Garcia,
 *
 * @brief This is the header file for previewwindow.cpp
 *
 * @date 03/23/2025
 */

namespace Ui { class previewwindow; }

class PreviewWindow : public QMainWindow {
    Q_OBJECT

public:

    explicit PreviewWindow(int spriteHeight, int spriteWidth, FrameManager *frameManager, QWidget *parent = nullptr);

    ~PreviewWindow();

private:

    Ui::previewwindow *ui;

    //Actual height of the sprite
    int actualHeight;

    //Actual width of the sprite
    int actualWidth;

    //Blank QImage to be used to draw the sprite
    QImage sprite;

public slots:

    /*
     * Toggles the animation on to be looped through until it is stopped
     */
    void animation();

    /*
     * Shows the frame in the preview window
     * @param frame: the Frame to be displayed
     */
    void showFrame(Frame);

    /*
     * Loops through the frames and displays them in order until the button is clicked again stopping it.
     * @param animationBool: used to see if the animation cycle should keep running or not.
     */
    void animate(bool);

signals:
    /*
     * Signal sent to the frame manager to get the frames for animation
     */
    std::vector<Frame> getFrames();
};

#endif // PREVIEWWINDOW_H
