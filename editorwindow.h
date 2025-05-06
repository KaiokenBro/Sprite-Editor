#ifndef EDITORWINDOW_H
#define EDITORWINDOW_H

/**
 * @file editorwindow.h
 * @brief Declares the EditorWindow class for the main sprite editing interface.
 *
 * The EditorWindow is responsible for drawing, editing, coloring, and managing frames
 * in the sprite editor. It connects the UI with the underlying models and handles
 * real-time user interactions like mouse drawing, erasing, and color picking.
 *
 * @date 03/31/2025
 */

#include "framemanager.h"
#include "saveloadmanager.h"

#include <QMainWindow>

using std::vector;

QT_BEGIN_NAMESPACE
namespace Ui { class EditorWindow; }
QT_END_NAMESPACE

/**
 * @class EditorWindow
 *
 * @brief The main editor window where users draw, modify, animate, and manage sprite frames.
 */
class EditorWindow : public QMainWindow {
    Q_OBJECT

public:

    /**
     * @brief Constructs the EditorWindow with references to model managers.
     * @param saveLoadManager Handles save/load operations for frames.
     * @param frameManager Manages all sprite frames.
     * @param width Initial width of the sprite.
     * @param height Initial height of the sprite.
     * @param parent Optional parent widget.
     */
    explicit EditorWindow(SaveLoadManager* saveLoadManager, FrameManager* frameManager, int width, int height, QWidget* parent = nullptr);

    /**
     * @brief Destructor.
     */
    ~EditorWindow();

    /**
     * @brief Sets the logical width of the sprite.
     * @param width Width in pixels.
     */
    void setSpriteWidth(int width);

    /**
     * @brief Sets the logical height of the sprite.
     * @param height Height in pixels.
     */
    void setSpriteHeight(int height);

    /**
     * @brief Reinitializes the editor from scratch with new dimensions (used when creating a new sprite).
     * @param newWidth New sprite width.
     * @param newHeight New sprite height.
     */
    void reinitializeEditor(int newWidth, int newHeight);

    /**
     * @brief Initializes the editor window using data from a loaded file.
     * @param width Width from the loaded file.
     * @param height Height from the loaded file.
     */
    void initializeFromLoadedFile(int width, int height);

protected:

    /**
     * @brief Filters mouse events for the drawing canvas (spriteLabel).
     * @param watched The object receiving the event.
     * @param event The event to filter.
     * @return true if the event was handled; false otherwise.
     */
    virtual bool eventFilter(QObject* watched, QEvent* event) override;

private:

    /**
     * @brief Pointer to the UI layout.
     */
    Ui::EditorWindow* ui;

    /**
     * @brief Pointer to the frame manager model.
     */
    FrameManager* frameManager;

    /**
     * @brief Pointer to the save/load manager.
     */
    SaveLoadManager* saveLoadManager;

    /**
     * @brief Currently selected drawing color.
     */
    QColor color;

    /**
     * @brief Internal representation of the canvas image.
     */
    QImage sprite;

    /**
     * @brief Logical width of the sprite grid.
     */
    int spriteWidth;

    /**
     * @brief Logical height of the sprite grid.
     */
    int spriteHeight;

    /**
     * @brief Whether drawing mode is active.
     */
    bool isDrawing = false;

    /**
     * @brief Whether eraser mode is active.
     */
    bool isErasing = false;

    /**
     * @brief Whether color picker mode is active.
     */
    bool isGettingColor = false;

    /**
     * @brief Whether the mouse is currently pressed (for drawing).
     */
    bool mousePressed = false;

    /**
     * @brief Updates the canvas display to reflect the current sprite image.
     */
    void updateCanvas();

    /**
     * @brief Handles drawing, erasing, or color picking at a given coordinate.
     * @param x The X-coordinate in the sprite grid.
     * @param y The Y-coordinate in the sprite grid.
     */
    void handleDrawingAction(int x, int y);

    /**
     * @brief Returns the index of the currently selected frame.
     * @return The current frame index, or 0 if none is selected.
     */
    int getCurrentFrameIndex();

public slots:

    /**
     * @brief Opens the animation preview window.
     */
    void animateClicked();

    /**
     * @brief Enables drawing mode.
     */
    void enableDrawing();

    /**
     * @brief Enables erasing mode.
     */
    void enableEraser();

    /**
     * @brief Enables color picking mode.
     */
    void enableCopyColor();

    /**
     * @brief Updates the red channel of the selected color.
     * @param value Red component value.
     */
    void redChanged(int value);

    /**
     * @brief Updates the green channel of the selected color.
     * @param value Green component value.
     */
    void greenChanged(int value);

    /**
     * @brief Updates the blue channel of the selected color.
     * @param value Blue component value.
     */
    void blueChanged(int value);

    /**
     * @brief Updates the alpha channel (opacity) of the selected color.
     * @param value Alpha component value.
     */
    void alphaChanged(int value);

    /**
     * @brief Adds a new frame label to the UI frame stack.
     * @param frameNumber The frame number (1-based).
     */
    void addFrameToStack(int frameNumber);

    /**
     * @brief Deletes the currently selected frame from the frame stack.
     */
    void deleteFrameFromStack();

    /**
     * @brief Loads and displays the given pixel matrix on the canvas.
     * @param pixels A 2D vector of QColor pixels.
     */
    void switchCanvas(vector<vector<QColor>> pixels);

    /**
     * @brief Emits a request to retrieve the currently selected frame's pixel data.
     */
    void getSelectedFrame();

    /**
     * @brief Emits a request to copy the currently selected frame.
     */
    void getSelectedFrameToCopy();

    /**
     * @brief Emits a request to rotate the currently selected frame.
     */
    void getSelectedFrameToRotate();

    /**
     * @brief Inverts the pixel colors on the current canvas.
     */
    void invertColor();

    /**
     * @brief Triggered when the save button is clicked. Opens save dialog and writes to file.
     */
    void onSaveButtonClicked();

signals:

    /**
     * @brief Signal to add a new blank frame (used after initializing a new sprite).
     */
    void addOneFrame();

    /**
     * @brief Signal to update the red spinbox to a new value.
     * @param value Red component.
     */
    void changeRedValue(int value);

    /**
     * @brief Signal to update the green spinbox to a new value.
     * @param value Green component.
     */
    void changeGreenValue(int value);

    /**
     * @brief Signal to update the blue spinbox to a new value.
     * @param value Blue component.
     */
    void changeBlueValue(int value);

    /**
     * @brief Signal to update the alpha spinbox to a new value.
     * @param value Alpha component.
     */
    void changeAlphaValue(int value);

    /**
     * @brief Signal to delete a frame by its index.
     * @param frameIndex Index of the frame to delete.
     */
    void deleteFrame(int frameIndex);

    /**
     * @brief Signal to update a pixel in a specific frame.
     */
    void updatePixelInFrame(int frameIndex, int rowIndex, int columnIndex, int red, int green, int blue, int alpha);

    /**
     * @brief Signal to request the pixels of a specific frame.
     * @param frameIndex Index of the frame.
     */
    void getPixels(int frameIndex);

    /**
     * @brief Signal to request that a frame be copied.
     * @param frameIndex Index of the frame to copy.
     */
    void selectedFrameToCopy(int frameIndex);

    /**
     * @brief Signal to request that a frame be rotated 90° clockwise.
     * @param frameIndex Index of the frame to rotate.
     */
    void selectedFrameToRotate(int frameIndex);

};

#endif // EDITORWINDOW_H
