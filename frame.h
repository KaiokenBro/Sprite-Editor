#ifndef FRAME_H
#define FRAME_H

/**
 * @file frame.h
 * @author Richard Nghiem
 *
 * @brief Declares the Frame class, which represents a 2D grid of pixels (QColor) that make up a single sprite frame.
 *
 * A Frame contains a 2D vector of QColor objects, each representing a pixel with RGBA values.
 * Frames are used to store and manipulate individual layers of animation or static sprite images.
 *
 * @date 03/31/2025
 *
 * Checked by Harrison Doppelt
 */

#include <QColor>

using std::vector;

/**
 * @class Frame
 *
 * @brief Represents a single frame of a sprite composed of a grid of RGBA pixels.
 *
 * Provides methods for updating individual pixels, rotating the frame, and retrieving
 * pixel data and frame dimensions.
 */
class Frame {

private:

    /**
     * @brief 2D vector of pixels, where each element is a QColor with RGBA values.
     *
     * The outer vector represents rows; the inner vector represents columns.
     */
    vector<vector<QColor>> pixels;

    /**
     * @brief The height of the frame in pixels.
     */
    int height;

    /**
     * @brief The width of the frame in pixels.
     */
    int width;

public:

    /**
     * @brief Default constructor. Creates an empty frame with zero height and width.
     */
    Frame();

    /**
     * @brief Parameterized constructor. Initializes a frame with the specified height and width.
     *
     * All pixels are initialized to fully transparent white (RGBA: 255, 255, 255, 0).
     *
     * @param height The height of the frame (number of rows).
     * @param width The width of the frame (number of columns).
     */
    Frame(int height, int width);

    /**
     * @brief Updates a specific pixel in the frame with new RGBA values.
     * @param rowIndex The row index (y-coordinate) of the pixel to update.
     * @param columnIndex The column index (x-coordinate) of the pixel to update.
     * @param red Red component (0–255).
     * @param green Green component (0–255).
     * @param blue Blue component (0–255).
     * @param alpha Alpha (opacity) component (0–255).
     */
    void updateFrame(int rowIndex, int columnIndex, int red, int green, int blue, int alpha);

    /**
     * @brief Retrieves the full 2D pixel data of the frame.
     * @return A 2D vector of QColor objects representing all pixels in the frame.
     */
    vector<vector<QColor>> getPixels();

    /**
     * @brief Rotates the frame 90 degrees clockwise.
     *
     * This is done by transposing the pixel matrix and then reversing each row.
     * Note: This operation assumes the frame is square (height == width).
     */
    void rotateFrame();

    /**
     * @brief Gets the height of the frame.
     * @return The height in pixels.
     */
    int getHeight();

    /**
     * @brief Gets the width of the frame.
     * @return The width in pixels.
     */
    int getWidth();

};

#endif // FRAME_H
