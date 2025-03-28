#include "frame.h"

Frame::Frame() : height(0), width(0){}

Frame::Frame(int height, int width) : height(height), width(width){
    for(int i = 0; i < height; i++){
        std::vector<QColor> pixelsRow;
        for(int j = 0; j < width; j++){
            QColor newPixel;
            // New pixels must be white and transparent.
            newPixel.setRed(255);
            newPixel.setBlue(255);
            newPixel.setGreen(255);
            newPixel.setAlpha(0);
            pixelsRow.push_back(newPixel);
        }
        pixels.push_back(pixelsRow);
    }
};

void Frame::updateFrame(int rowIndex, int columnIndex, int red, int green, int blue, int alpha){
    QColor* pixelToUpdate = &pixels.at(rowIndex).at(columnIndex);
    pixelToUpdate->setRed(red);
    pixelToUpdate->setBlue(blue);
    pixelToUpdate->setGreen(green);
    pixelToUpdate->setAlpha(alpha);
};

std::vector<std::vector<QColor>> Frame::getPixels() {
    return pixels;
}
