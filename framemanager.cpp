#include "framemanager.h"

FrameManager::FrameManager(Frame frameToAdd) : currentFrame(0) {
    frames.push_back(frameToAdd);
}

void FrameManager::addFrame(Frame frameToAdd){
    frames.push_back(frameToAdd);
}

void FrameManager::deleteFrame(){
    if(frames.size() > 1){
        frames.erase(frames.begin() + currentFrame);
    }
}

void FrameManager::copyFrame(){
    frames.push_back(frames.at(currentFrame));
}

void FrameManager::updateCurrentFrame(int newCurrentFrame){
    currentFrame = newCurrentFrame;
}

void FrameManager::updateFrame(int rowIndex, int columnIndex, int red, int green, int blue, int alpha){
    frames.at(currentFrame).updateFrame(rowIndex, columnIndex, red, green, blue, alpha);
    emit updatedFrame(frames.at(currentFrame).getPixels());
}

