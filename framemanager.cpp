#include "framemanager.h"

// Constructor
FrameManager::FrameManager(int height, int width, QObject *parent) :
    QObject(parent),
    height(height),
    width(width)
{}

// New function needed to add loaded frames from json
void FrameManager::addFrameJson(Frame frameToAdd){
    frames.push_back(frameToAdd);
    emit frameAdded(frames.size());
}

// Adds frame to frameManager
void FrameManager::addFrame() {
    Frame frameToAdd(height, width);
    frames.push_back(frameToAdd);
    emit frameAdded(frames.size());
}

// Deletes frame from frameManager
void FrameManager::deleteFrame(int frameIndex) {
    if (frames.size() > 1){
        frames.erase(frames.begin() + frameIndex);
    }
}

// Copies frame to frameManager
void FrameManager::copyFrame(int frameIndex) {
    frames.push_back(frames.at(frameIndex));
    emit frameAdded(frames.size());
}

// Updates frame in frameManager
void FrameManager::updateFrame(int frameIndex, int rowIndex, int columnIndex, int red, int green, int blue, int alpha) {
    frames.at(frameIndex).updateFrame(rowIndex, columnIndex, red, green, blue, alpha);
}

// Get pixels for frame in frameManager
void FrameManager::getPixelsForFrame(int frameIndex) {
    emit foundFrame(frames.at(frameIndex).getPixels());
}
