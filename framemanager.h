#ifndef FRAMEMANAGER_H
#define FRAMEMANAGER_H

#include <QMainWindow>
#include <QObject>
#include "frame.h"

class FrameManager : public QObject {
    Q_OBJECT

public:

    // The existing frames in the frame manager.
    std::vector<Frame> frames;

    // Height of the frame in pixels
    int height;

    // Width of each frame in pixels
    int width;

public:

    explicit FrameManager(int height, int width, QObject *parent = nullptr);

public slots:

    // /*
    //  * This method adds an existing frame to the end of the frame manager.
    //  * @param frameToAdd - The frame to be added to the frame manager.
    //  */
    // void addFrame(Frame frameToAdd);

    /*
     * This method adds a new frame to the frame manager. The frame is always added to the end.
     * @param frameToAdd - The frame to be added to the frame manager.
     */
    void addFrame();

    void addFrameJson(Frame frame);

    /*
     * This method deletes the current frame selected from the frame manager.
     * @param frameIndex - The index of the frame to update.
     */
    void deleteFrame(int frameIndex);

    /*
     * This method copies the current frame selected and adds it to the end of the frame manager.
     * @param frameIndex - The index of the frame to update.
     */
    void copyFrame(int frameIndex);

    /*
     * This method updates a pixel in a specified frame.
     * @param frameIndex - The index of the frame to update.
     * @param rowIndex - The row of the updated pixel.
     * @param columnIndex - The column of the updated pixel.
     * @param red - The red value for the updated pixel.
     * @param green - The green value for the updated pixel.
     * @param blue - The blue value for the updated pixel.
     * @param alpha - The alpha value for the updated pixel.
     */
    void updateFrame(int frameIndex, int rowIndex, int columnIndex, int red, int green, int blue, int alpha);

    void getPixelsForFrame(int frameIndex);

    std::vector<Frame> sendFrames();

    /*
     * Rotates a frame given a frame index. The frame is expected to be a square (equal height and width).
     * @param frameIndex - The frame to rotate.
     */
    void rotate90Clockwise(int frameIndex);

signals:

    /*
     * Send stored pixels of a frame to the canvas.
     */
    void foundFrame(std::vector<std::vector<QColor>> pixels);

    /*
     * Let the UI know the number of existing frames.
     */
    void frameAdded(int framesCount);
};

#endif // FRAMEMANAGER_H
