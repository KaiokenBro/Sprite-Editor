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

#include <QObject>
#include <QWidget>

class SaveLoadManager : public QObject {
    Q_OBJECT

public:

    explicit SaveLoadManager(QObject *parent = nullptr);

signals:


};

#endif // SAVELOADMANAGER_H
