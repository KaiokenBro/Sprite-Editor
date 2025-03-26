#ifndef FRAME_H
#define FRAME_H

#include <QColor>

class Frame
{
private:
    // The frame pixels. Each element is a row of pixels. Pixels are represented by QColor objects, which
    // have RGBA values.
    std::vector<std::vector<QColor>> pixels;
    // Width of the frame in pixels.
    int width;
    // Height of the frame in pixels.
    int height;
public:
    /*
     * Default constructor to create an empty frame and sets the height and width to zero.
     */
    Frame();

    /*
     * Constructor that creates an empty frame with specified height and width. The pixels are filled in with
     * a default QColor object.
     * @param width - The width of the frame in pixels.
     * @param height - The height of the frame in pixels.
     */
    Frame(int width, int height);

public slots:

    /*
     * This method updates a pixel in the frame.
     * @param rowIndex - The row of the updated pixel.
     * @param columnIndex - The column of the updated pixel.
     * @param red - The red value for the updated pixel.
     * @param green - The green value for the updated pixel.
     * @param blue - The blue value for the updated pixel.
     * @param alpha - The alpha value for the updated pixel.
     */
    void updateFrame(int rowIndex, int columnIndex, int red, int green, int blue, int alpha);
};

#endif // FRAME_H
