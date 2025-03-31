/**
 * @file saveloadmanager.cpp
 * @authors Victor Valdez Landa, Harrison Doppelt
 *
 * @brief This file defines the implementation of the of saving and loading the images of sprites from the Sprite Editor
 *
 * Checked by Victor Valdez Landa
 * @date 03/30/2025
 */

#include "saveloadmanager.h"

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QColor>
#include <QDebug>

// Constructor
SaveLoadManager::SaveLoadManager(QObject *parent)
    : QObject{parent}
{}

// Method - Serialize (Object -> File)
// Saves all frames managed by FrameManager to a JSON file at the given file path.
bool SaveLoadManager::saveToFile(FrameManager& manager, QString filePath) {

    // Create a JSON array to hold all the serialized frames
    QJsonArray framesArray;

    // Loop through each frame in the FrameManager
    for (size_t i = 0; i < manager.frames.size(); ++i) {

        // Get the frame at index i
        Frame frame = manager.frames[i];

        // JSON array to hold this frame's pixel data
        QJsonArray pixelArray;

        // Get the 2D array of pixel colors
        std::vector<std::vector<QColor>> pixels = frame.getPixels();

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

                // Add this pixel object to the pixel array
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

    // Write the JSON document to the file
    file.write(doc.toJson());

    // Close the file
    file.close();

    // Successfully saved the file
    return true;
}

// Method - Deserialize (File -> Object)
// Loads all frames and pixel data from a JSON file into the FrameManager.
bool SaveLoadManager::loadFromFile(FrameManager& manager, QString filePath) {

    // Open the file at the specified path
    QFile file(filePath);

    // If the file can't be opened, log an error and return false
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Failed to open file for reading:" << filePath;
        return false;
    }

    // Read the entire file contents into a byte array
    QByteArray data = file.readAll();

    // Always close the file after reading
    file.close();

    // Convert the byte array into a JSON document
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

        // Get the frame object
        QJsonObject frameObj = frameVal.toObject();

        // Get the array of pixel objects
        QJsonArray pixelArray = frameObj["pixels"].toArray();

        // Create a new Frame with the given dimensions
        Frame newFrame(height, width);

        // Loop through each pixel and update the new frame
        for (const QJsonValue& pixelVal : pixelArray) {

            // Get pixel as JSON object
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

    // Return success
    return true;
}
