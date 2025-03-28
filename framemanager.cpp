#include "framemanager.h"

FrameManager::FrameManager(int height, int width, QObject *parent)
    : height(height),
    width(width),
    QObject(parent)
{}

// New function needed to add loaded frames from json
// void FrameManager::addFrame(Frame frameToAdd){
//     frames.push_back(frameToAdd);
//     emit frameAdded(frames.size());
// }

void FrameManager::addFrame(){
    Frame frameToAdd(height, width);
    frames.push_back(frameToAdd);
    emit frameAdded(frames.size());
}

void FrameManager::deleteFrame(int frameIndex){
    if(frames.size() > 1){
        frames.erase(frames.begin() + frameIndex);
    }
}

void FrameManager::copyFrame(int frameIndex){
    frames.push_back(frames.at(frameIndex));
    emit frameAdded(frames.size());
}

void FrameManager::updateFrame(int frameIndex, int rowIndex, int columnIndex, int red, int green, int blue, int alpha){
    frames.at(frameIndex).updateFrame(rowIndex, columnIndex, red, green, blue, alpha);
}

void FrameManager::getPixelsForFrame(int frameIndex) {
    emit foundFrame(frames.at(frameIndex).getPixels());
}
