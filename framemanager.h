#ifndef FRAMEMANAGER_H
#define FRAMEMANAGER_H

/**
 * @file framemanager.h
 * @author Charlie Jones
 * @brief Declaration of the FrameManager class, which manages a collection of animation frames for a sprite.
 * @date 03/31/2025
 */

#include "frame.h"

#include <QMainWindow>
#include <QObject>

using std::vector;

/**
 * @class FrameManager
 *
 * @brief Manages a list of frames used in sprite animation.
 *
 * Responsible for creating, updating, duplicating, deleting, and rotating frames.
 */
class FrameManager : public QObject {
    Q_OBJECT

public:

    /**
     * @brief Constructs a FrameManager instance with initial frame dimensions.
     * @param height The height of each frame.
     * @param width The width of each frame.
     * @param parent Optional parent QObject.
     */
    explicit FrameManager(int height, int width, QObject* parent = nullptr);

    /**
     * @brief The list of frames currently managed.
     */
    vector<Frame> frames;

    /**
     * @brief The height of each frame in pixels.
     */
    int height;

    /**
     * @brief The width of each frame in pixels.
     */
    int width;

public slots:

    /**
     * @brief Adds a new blank frame to the end of the list using the current width and height.
     */
    void addFrame();

    /**
     * @brief Adds a deserialized frame to the frame list (used during JSON loading).
     * @param frame The frame to be added.
     */
    void addFrameJson(Frame frame);

    /**
     * @brief Deletes the frame at the given index if more than one frame exists.
     * @param frameIndex The index of the frame to delete.
     */
    void deleteFrame(int frameIndex);

    /**
     * @brief Duplicates the frame at the given index and appends it to the list.
     * @param frameIndex The index of the frame to copy.
     */
    void copyFrame(int frameIndex);

    /**
     * @brief Updates the pixel at (rowIndex, columnIndex) in a specified frame.
     * @param frameIndex Index of the frame to modify.
     * @param rowIndex Y-coordinate of the pixel.
     * @param columnIndex X-coordinate of the pixel.
     * @param red Red value of the new color (0–255).
     * @param green Green value of the new color (0–255).
     * @param blue Blue value of the new color (0–255).
     * @param alpha Alpha value of the new color (0–255).
     */
    void updateFrame(int frameIndex, int rowIndex, int columnIndex, int red, int green, int blue, int alpha);

    /**
     * @brief Emits the pixel data for the frame at the given index.
     * @param frameIndex The index of the frame to retrieve.
     */
    void getPixelsForFrame(int frameIndex);

    /**
     * @brief Returns all frames currently stored.
     * @return A vector containing all Frame objects.
     */
    vector<Frame> sendFrames();

    /**
     * @brief Rotates the specified frame 90 degrees clockwise.
     *
     * The frame must be square (height == width).
     *
     * @param frameIndex The index of the frame to rotate.
     */
    void rotate90Clockwise(int frameIndex);

signals:

    /**
     * @brief Signal emitted when a frame is requested for display.
     * @param pixels The 2D pixel data of the selected frame.
     */
    void foundFrame(vector<vector<QColor>> pixels);

    /**
     * @brief Signal emitted after a new frame has been added.
     * @param framesCount The total number of frames after the addition.
     */
    void frameAdded(int framesCount);
};

#endif // FRAMEMANAGER_H
