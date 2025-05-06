#ifndef MAINWINDOW_H
#define MAINWINDOW_H

/**
 * @file mainwindow.h
 * @author Harrison Doppelt, Victor Valdez Landa
 *
 * @brief Declares the MainWindow class, which provides the initial UI for creating or loading sprite files.
 *
 * The MainWindow allows users to enter the dimensions for a new sprite, or load an existing sprite
 * from a `.ssp` file. It serves as the entry point before the EditorWindow is launched.
 *
 * @date 03/31/2025
 */

#include "editorwindow.h"

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

/**
 * @class MainWindow
 *
 * @brief The startup UI that lets users create a new sprite or load an existing one.
 *
 * Responsible for collecting width/height input, validating dimensions, triggering the editor window,
 * and handling file input for loading `.ssp` sprite files.
 */
class MainWindow : public QMainWindow {
    Q_OBJECT

public:

    /**
     * @brief Constructs a MainWindow instance.
     * @param saveLoadManager Pointer to shared SaveLoadManager for file I/O.
     * @param frameManager Pointer to shared FrameManager containing frame data.
     * @param editorWindow Pointer to the EditorWindow that will be launched.
     * @param parent Optional parent widget.
     */
    MainWindow(SaveLoadManager* saveLoadManager, FrameManager* frameManager, EditorWindow* editorWindow, QWidget *parent = nullptr);

    /**
     * @brief Destructor.
     */
    ~MainWindow();

private:

    /**
     * @brief Pointer to the auto-generated UI elements from Qt Designer.
     */
    Ui::MainWindow* ui;

    /**
     * @brief Pointer to the EditorWindow used after sprite creation or loading.
     */
    EditorWindow* editorWindow;

    /**
     * @brief Pointer to the FrameManager used to manage sprite frames.
     */
    FrameManager* frameManager;

    /**
     * @brief Pointer to the SaveLoadManager used to load and save `.ssp` files.
     */
    SaveLoadManager* saveLoadManager;

public slots:

    /**
     * @brief Validates user input in width/height fields and enables the size confirmation button.
     */
    void validateInputs();

    /**
     * @brief Called when the "Set Size" button is clicked.
     *
     * Verifies that width and height are within the allowed range.
     */
    void onSetSizeButtonClicked();

    /**
     * @brief Disables the "Load" button (used when switching modes).
     */
    void disableLoadButton();

    /**
     * @brief Enables the "Load" button.
     */
    void enableLoadButton();

    /**
     * @brief Opens the EditorWindow in "New Sprite" mode, using the input width and height.
     */
    void openEditorWindowNew();

    /**
     * @brief Opens the EditorWindow in "Load Sprite" mode after a file is selected and loaded.
     */
    void openEditorWindowLoad();

    /**
     * @brief Opens a file dialog to load a `.ssp` file and updates the internal state if successful.
     */
    void loadFile();

    /**
     * @brief Keeps the height input in sync with width input for square sprite creation.
     * @param text The current text value of the width field.
     */
    void syncHeightToWidth(const QString &text);

    /**
     * @brief Keeps the width input in sync with height input for square sprite creation.
     * @param text The current text value of the height field.
     */
    void syncWidthToHeight(const QString &text);

    /**
     * @brief Resets the size confirmation (✅ → ❌) if the user changes input after confirmation.
     */
    void invalidateSizeConfirmation();

signals:

    /**
     * @brief Emitted when a new sprite is successfully created.
     * @param width The sprite width.
     * @param height The sprite height.
     */
    void startNewSprite(int width, int height);

    /**
     * @brief Emitted when a sprite is loaded from file.
     */
    void openLoadedSprite();

};

#endif // MAINWINDOW_H
