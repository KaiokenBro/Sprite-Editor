/**
 * @file saveloadmanager.cpp
 * @brief Implementation of the SaveLoadManager class for saving and loading sprite data in the Sprite Editor.
 *
 * This file provides functionality for converting the internal sprite representation into JSON
 * and saving it to disk, as well as loading and reconstructing sprite frames from previously saved files.
 *
 * @date 03/31/2025
 */

#include "saveloadmanager.h"

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QColor>
#include <QDebug>

using std::vector;

SaveLoadManager::SaveLoadManager(QObject* parent) : QObject{parent} {}

bool SaveLoadManager::saveToFile(FrameManager& manager, QString filePath) {
    QJsonArray framesArray;

    // Loop through each frame in the FrameManager
    for (size_t i = 0; i < manager.frames.size(); ++i) {
        Frame frame = manager.frames[i];
        QJsonArray pixelArray;
        vector<vector<QColor>> pixels = frame.getPixels();

        // Loop through each row (y) of the frame
        for (int y = 0; y < frame.getHeight(); ++y) {

            // Loop through each column (x) in the row
            for (int x = 0; x < frame.getWidth(); ++x) {

                // Get the QColor at position (x, y)
                QColor color = pixels[y][x];

                // Create a JSON object for this pixel
                QJsonObject pixelObj;
                pixelObj["x"] = x;              // X-coordinate
                pixelObj["y"] = y;              // Y-coordinate
                pixelObj["r"] = color.red();    // Red component
                pixelObj["g"] = color.green();  // Green component
                pixelObj["b"] = color.blue();   // Blue component
                pixelObj["a"] = color.alpha();  // Alpha (transparency)
                pixelArray.append(pixelObj);
            }
        }

        // Create a JSON object to represent the entire frame
        QJsonObject frameObj;
        frameObj["index"] = static_cast<int>(i);  // Add the frame index
        frameObj["pixels"] = pixelArray;        // Store all pixels in this frame

        // Add the frame object to the top-level frames array
        framesArray.append(frameObj);
    }

    // Create the root JSON object and assign the frames array to it
    QJsonObject root;
    root["height"] = manager.frames[0].getHeight();
    root["width"] = manager.frames[0].getWidth();
    root["frames"] = framesArray;

    // Convert the root JSON object into a QJsonDocument
    QJsonDocument doc(root);

    // Open the output file in write-only mode
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly)) {
        qWarning() << "Failed to open file for writing:" << filePath;
        return false;   // Return false if file couldn't be opened
    }

    file.write(doc.toJson());
    file.close();
    return true;
}

bool SaveLoadManager::loadFromFile(FrameManager& manager, QString filePath) {
    QFile file(filePath);

    // If the file can't be opened, log an error and return false
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Failed to open file for reading:" << filePath;
        return false;
    }

    QByteArray data = file.readAll();
    file.close();
    QJsonDocument doc = QJsonDocument::fromJson(data);

    // If the top-level JSON is not an object, log an error and return false
    if (!doc.isObject()) {
        qWarning() << "Invalid JSON format.";
        return false;
    }

    // Extract the root JSON object and the array of frames
    QJsonObject root = doc.object();
    QJsonArray framesArray = root["frames"].toArray();

    int height = root["height"].toInt();
    int width = root["width"].toInt();
    manager.frames.clear();
    manager.height = height;
    manager.width = width;

    // Loop through each frame in the array
    for (const QJsonValue& frameVal : framesArray) {
        QJsonObject frameObj = frameVal.toObject();
        QJsonArray pixelArray = frameObj["pixels"].toArray();
        Frame newFrame(height, width);

        // Loop through each pixel and update the new frame
        for (const QJsonValue& pixelVal : pixelArray) {
            QJsonObject pixelObj = pixelVal.toObject();

            // Extract pixel properties
            int x = pixelObj["x"].toInt();
            int y = pixelObj["y"].toInt();
            int r = pixelObj["r"].toInt();
            int g = pixelObj["g"].toInt();
            int b = pixelObj["b"].toInt();
            int a = pixelObj["a"].toInt();

            // Update the pixel in the new frame at (x, y) with the RGBA values
            newFrame.updateFrame(y, x, r, g, b, a);
        }

        // Add the fully constructed frame to the FrameManager
        manager.addFrameJson(newFrame);
    }

    return true;
}
