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

    /**
     * @brief PreviewWindow - Constructor.
     * @param spriteHeight - the actual height of the sprite in pixels.
     * @param spriteWidth - the actual width of teh sprite in pixels.
     * @param frameManager - the pointer to the framemanager class to set up signals.
     * @param parent - QObject needed for the use of objects in qt.
     */

    explicit PreviewWindow(int spriteHeight, int spriteWidth, FrameManager *frameManager, QWidget *parent = nullptr);

    /**
     * @brief Destructor for the PreveiwWindow.
     */
    ~PreviewWindow();

private:

    //The preview window ui pointer
    Ui::previewwindow *ui;

    //Actual height of the sprite
    int actualHeight;

    //Actual width of the sprite
    int actualWidth;

    //Blank QImage to be used to draw the sprite
    QImage sprite;

public slots:

    /**
     * @brief Toggles the animation on to be looped through until it is stopped.
     */
    void animation();

    /**
     * @brief Shows the frame in the preview window.
     * @param frame - the Frame to be displayed.
     */
    void showFrame(Frame);


signals:

    /**
     * @brief Signal sent to the frame manager to get the frames for animation
     */
    std::vector<Frame> getFrames();
};

#endif // PREVIEWWINDOW_H
