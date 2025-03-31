#ifndef SAVELOADMANAGER_H
#define SAVELOADMANAGER_H

/**
 * @file saveloadmanager.cpp
 * @authors Victor Valdez Landa, Harrison Dopplet
 *
 * @brief This file defines the implementation of the of svaing and loading the images of sprites from the Sprite Editor
 *
 * Checked by Victor Valdez Landa
 * @date 03/30/2025
 */

#include "framemanager.h"

#include <QObject>
#include <QWidget>

class SaveLoadManager : public QObject {
    Q_OBJECT

public:

    /**
     * @brief SaveLoadManager - Constructor for SaveLoadManager to be used in save/load purposes in the UI.
     * @param parent - QObject necessity for the use of objects for Qt.
     */
    explicit SaveLoadManager(QObject *parent = nullptr);

    /**
     * @brief saveToFile - Method to save the data that FrameManger has on the sprite editor,
     * compresses it into a Json Structure .ssp file.
     * @param manager - The FrameManager that holds the sprite data
     * @param filePath - the name of the file to be created
     * @return - returns true if the file was succesflly saved, false otherwise.
     */
    bool saveToFile(FrameManager& manager, QString filePath);

    /**
     * @brief loadFromFile - Deserializes a file to load in the data to the sprite editor's data of pixels.
     * @param manager - The FrameManager that holds the sprite data
     * @param filePath - The name of the file to be loaded
     * @return - returns true if the file was loaded in sucessfully, false otherwise.
     */
    bool loadFromFile(FrameManager& manager, QString filePath);

};

#endif // SAVELOADMANAGER_H
