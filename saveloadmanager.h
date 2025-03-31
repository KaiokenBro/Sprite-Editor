#ifndef SAVELOADMANAGER_H
#define SAVELOADMANAGER_H

/**
 * @file saveloadmanager.cpp
 * @authors Victor Valdez Landa,
 *
 * @brief This file defines the implementation of the of svaing and loading the images of sprites from the Sprite Editor
 *
 * @date 03/23/2025
 */

#include "framemanager.h"

#include <QObject>
#include <QWidget>

class SaveLoadManager : public QObject {
    Q_OBJECT

public:

    explicit SaveLoadManager(QObject *parent = nullptr);

    bool saveToFile(FrameManager& manager, QString filePath);
    bool loadFromFile(FrameManager& manager, QString filePath);

};

#endif // SAVELOADMANAGER_H
