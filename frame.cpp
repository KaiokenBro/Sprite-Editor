#include "frame.h"

Frame::Frame() : width(0), height(0){}

Frame::Frame(int width, int height) : width(width), height(height){
    for(int i = 0; i < height; i++){
        std::vector<QColor> pixelsRow;
        for(int j = 0; j < width; j++){
            QColor blankPixel;
            pixelsRow.push_back(blankPixel);
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
