#ifndef PREVIEWWINDOW_H
#define PREVIEWWINDOW_H

/**
 * @file previewwindow.h
 * @brief Declares the PreviewWindow class, which displays animated previews of sprite frames.
 *
 * The PreviewWindow connects to the FrameManager to retrieve all current frames and
 * animates them at a configurable FPS. It also supports toggling between scaled preview
 * and actual pixel size.
 *
 * @date 03/31/2025
 */

#include "frame.h"
#include "framemanager.h"

#include <QMainWindow>
#include <QTimer>
#include <QImage>
#include <QPainter>

using std::vector;

namespace Ui { class previewwindow; }

/**
 * @class PreviewWindow
 *
 * @brief Provides a window to preview sprite animation in real time.
 *
 * This class is responsible for retrieving frames from FrameManager and displaying them
 * in a looped animation using QLabel and QPainter. It supports resolution scaling and FPS control.
 */
class PreviewWindow : public QMainWindow {
    Q_OBJECT

public:

    /**
     * @brief Constructs the PreviewWindow.
     * @param spriteHeight Actual height of each sprite frame in pixels.
     * @param spriteWidth Actual width of each sprite frame in pixels.
     * @param frameManager Pointer to the FrameManager instance for accessing frames.
     * @param parent Optional QWidget parent.
     */
    explicit PreviewWindow(FrameManager* frameManager, int spriteHeight, int spriteWidth, QWidget* parent = nullptr);

    /**
     * @brief Destructor for the PreveiwWindow.
     */
    ~PreviewWindow();

private:

    /**
     * @brief Pointer to the UI layout for the preview window.
     */
    Ui::previewwindow* ui;

    /**
     * @brief Height of the sprite in pixels.
     */
    int actualHeight;

    /**
     * @brief Width of the sprite in pixels.
     */
    int actualWidth;

    /**
     * @brief Internal QImage used to represent the current frame being drawn.
     */
    QImage sprite;

public slots:

    /**
     * @brief Starts or stops the animation based on user interaction.
     *
     * Continuously cycles through all frames while the animate toggle is active
     * and the preview window remains visible.
     */
    void animation();

    /**
     * @brief Renders a single frame in the preview display.
     * @param frame The Frame object to be rendered.
     */
    void showFrame(Frame);


signals:

    /**
     * @brief Requests the list of frames from FrameManager for animation playback.
     * @return A vector of all current frames in the project.
     */
    vector<Frame> getFrames();
};

#endif // PREVIEWWINDOW_H
