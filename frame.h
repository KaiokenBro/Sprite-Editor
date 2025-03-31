#ifndef FRAME_H
#define FRAME_H

/**
 * @file frame.h
 *
 * @authors
 *
 * @brief A representation of a frame that contains pixels that can comprise an image.
 *
 * @date 03/30/2025
 *
 * Checked By
 *
 */

#include <QColor>

class Frame {

private:

    // The frame pixels. Each element is a row of pixels. Pixels are represented by QColor objects, which
    // have RGBA values.
    std::vector<std::vector<QColor>> pixels;

    // Height of the frame in pixels.
    int height;

    // Width of the frame in pixels.
    int width;

public:

    /**
     * @brief This method updates a pixel in the frame.
     * @param rowIndex - The row of the updated pixel.
     * @param columnIndex - The column of the updated pixel.
     * @param red - The red value for the updated pixel.
     * @param green - The green value for the updated pixel.
     * @param blue - The blue value for the updated pixel.
     * @param alpha - The alpha value for the updated pixel.
     */
    void updateFrame(int rowIndex, int columnIndex, int red, int green, int blue, int alpha);

    /**
     * @brief Default constructor to create an empty frame and sets the height and width to zero.
     */
    Frame();

    /**
     * @brief Constructor that creates an empty frame with specified height and width. The pixels are filled in with
     * a default QColor object.
     * @param width - The width of the frame in pixels.
     * @param height - The height of the frame in pixels.
     */
    Frame(int height, int width);

    /**
     * @brief Getter method for pxels.
     * @return The pixels vector.
     */
    std::vector<std::vector<QColor>> getPixels();

    /**
     * @brief Perform a 90 degrees clockwise rotation on all pixels. This is done by first transposing the pixels
     * and then reversing the order of pixels in each row. The frame must be a square (equal height and width).
     */
    void rotateFrame();

    /**
     * @brief getHeight - Method to get the height of a frame.
     * @return - returns the height of a frame.
     */
    int getHeight();

    /**
     * @brief getWidth - Methid ti get the width of a frame.
     * @return - returns the width of a frame.
     */
    int getWidth();

};

#endif // FRAME_H
