/**
 * @file frame.cpp
 * @brief Implementation of the Frame class, which represents a 2D pixel grid used in sprite frames.
 * @author Richard Nghiem
 * @date 03/31/2025
 */

#include "frame.h"

using std::swap;
using std::vector;

Frame::Frame() : height(0), width(0) {}

Frame::Frame(int height, int width) : height(height), width(width) {

    for (int i = 0; i < height; i++) {
        vector<QColor> pixelsRow;

        for(int j = 0; j < width; j++) {
            QColor newPixel(255, 255, 255, 0);
            pixelsRow.push_back(newPixel);
        }
        pixels.push_back(pixelsRow);
    }
}

void Frame::updateFrame(int rowIndex, int columnIndex, int red, int green, int blue, int alpha) {
    QColor* pixelToUpdate = &pixels.at(rowIndex).at(columnIndex);
    pixelToUpdate->setRed(red);
    pixelToUpdate->setBlue(blue);
    pixelToUpdate->setGreen(green);
    pixelToUpdate->setAlpha(alpha);
}

std::vector<std::vector<QColor>> Frame::getPixels() {
    return pixels;
}

int Frame::getHeight() {
    return height;
}

int Frame::getWidth() {
    return width;
}

void Frame::rotateFrame() {

    // Transpose
    for (int i = 0; i < height; i++) {
        for (int j = i + 1; j < width; j++) {
            swap(pixels.at(i).at(j), pixels.at(j).at(i));
        }
    }

    // Reverse each pixel in a row.
    for (int i = 0; i < height; i++) {
        int leftIndex = 0;
        int rightIndex = width - 1;
        while (leftIndex < rightIndex) {
            swap(pixels.at(i).at(leftIndex), pixels.at(i).at(rightIndex));
            leftIndex++;
            rightIndex--;
        }
    }
}
