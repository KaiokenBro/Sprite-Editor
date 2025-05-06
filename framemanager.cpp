/**
 * @file framemanager.cpp
 * @brief Implementation of the FrameManager class for managing sprite frames and pixel data.
 * @date 03/31/2025
 *
 * Checked by Nicholas Cicalese
 */

#include "framemanager.h"

using std::vector;

FrameManager::FrameManager(int height, int width, QObject* parent) :
    QObject(parent),
    height(height),
    width(width)
{}

void FrameManager::addFrameJson(Frame frameToAdd){
    frames.push_back(frameToAdd);
    emit frameAdded(frames.size());
}

void FrameManager::addFrame() {
    Frame frameToAdd(height, width);
    frames.push_back(frameToAdd);
    emit frameAdded(frames.size());
}

void FrameManager::deleteFrame(int frameIndex) {
    if (frames.size() > 1){
        frames.erase(frames.begin() + frameIndex);
    }
}

void FrameManager::copyFrame(int frameIndex) {
    frames.push_back(frames.at(frameIndex));
    emit frameAdded(frames.size());
}

void FrameManager::updateFrame(int frameIndex, int rowIndex, int columnIndex, int red, int green, int blue, int alpha) {
    frames.at(frameIndex).updateFrame(rowIndex, columnIndex, red, green, blue, alpha);
}

void FrameManager::getPixelsForFrame(int frameIndex) {
    emit foundFrame(frames.at(frameIndex).getPixels());
}

vector<Frame> FrameManager::sendFrames(){
    return frames;
}

void FrameManager::rotate90Clockwise(int frameIndex) {
    Frame* frameToRotate = &frames.at(frameIndex);
    frameToRotate->rotateFrame();
    emit foundFrame(frameToRotate->getPixels());
}
