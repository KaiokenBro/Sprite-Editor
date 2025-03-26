#ifndef FRAMEMANAGER_H
#define FRAMEMANAGER_H

#include <QMainWindow>
#include <QObject>
#include "frame.h"

class FrameManager : public QObject
{
    Q_OBJECT
private:
    // The index for the current frame in the frame manager.
    int currentFrame;
    // The existing frames in the frame manager.
    std::vector<Frame> frames;

public:
    /*
     * The constructor to create a frame manger. This constructor requires one frame to be added because the
     * frame manager must have one existing frame.
     * @param frameToAdd - A frame added to the frame manager.
     */
    FrameManager(Frame frameToAdd);
public slots:
    /*
     * This method adds an existing frame to the end of the frame manager.
     * @param frameToAdd - The frame to be added to the frame manager.
     */
    void addFrame(Frame frameToAdd);

    /*
     * This method deletes the current frame selected from the frame manager.
     */
    void deleteFrame();

    /*
     * This method copies the current frame selected and adds it to the end of the frame manager.
     */
    void copyFrame();

    /*
     * This method updates the index of the current frame selected.
     * @param newCurrentFrame - An int representing the index of the new current frame.
     */
    void updateCurrentFrame(int newCurrentFrame);

    /*
     * This method updates a pixel in the current frame.
     * @param rowIndex - The row of the updated pixel.
     * @param columnIndex - The column of the updated pixel.
     * @param red - The red value for the updated pixel.
     * @param green - The green value for the updated pixel.
     * @param blue - The blue value for the updated pixel.
     * @param alpha - The alpha value for the updated pixel.
     */
    void updateFrame(int rowIndex, int columnIndex, int red, int green, int blue, int alpha);

signals:
    /*
     * Signal for the UI to reflect changes in the frame manager.
     */
    void updatedFrame();
};

#endif // FRAMEMANAGER_H
