#ifndef SAVELOADMANAGER_H
#define SAVELOADMANAGER_H

/**
 * @file saveloadmanager.cpp
 * @authors Victor Valdez Landa
 * @brief Declaration of the SaveLoadManager class used for saving and loading sprite data in the Sprite Editor.
 * @date 03/31/2025
 *
 * Checked by Samuel Garcia
 */

#include "framemanager.h"

#include <QObject>
#include <QWidget>

/**
 * @class SaveLoadManager
 *
 * @brief Handles serialization and deserialization of sprite data from the FrameManager to and from .ssp files.
 *
 * This class is responsible for converting the internal frame data into a JSON-based format
 * and writing it to disk, or for reading JSON sprite data from a file and updating the frame manager accordingly.
 */
class SaveLoadManager : public QObject {
    Q_OBJECT

public:

    /**
     * @brief Constructs a SaveLoadManager instance.
     * @param parent Optional parent QObject.
     */
    explicit SaveLoadManager(QObject *parent = nullptr);

    /**
     * @brief Saves all frames managed by the given FrameManager to a JSON-based .ssp file.
     * @param manager Reference to the FrameManager containing all frame data to save.
     * @param filePath The target file path where the .ssp file will be written.
     * @return true if the file was saved successfully; false otherwise.
     */
    bool saveToFile(FrameManager& manager, QString filePath);

    /**
     * @brief Loads sprite data from a JSON-based .ssp file and populates the given FrameManager.
     * @param manager Reference to the FrameManager where loaded frames will be stored.
     * @param filePath Path to the .ssp file to load.
     * @return true if the file was loaded successfully; false otherwise.
     */
    bool loadFromFile(FrameManager& manager, QString filePath);

};

#endif // SAVELOADMANAGER_H
