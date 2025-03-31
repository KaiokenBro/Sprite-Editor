#include "editorwindow.h"
#include "ui_editorwindow.h"
#include "previewwindow.h"

#include <QMouseEvent>
#include <QPainter>
#include <QFileDialog>
#include <QMessageBox>
#include <QString>

using std::min;
using std::max;

// Constructor
EditorWindow::EditorWindow(SaveLoadManager* saveLoadManager, FrameManager* frameManager, int width, int height, QWidget *parent) :
    QMainWindow(parent),            // Call base QMainWindow constructor
    ui(new Ui::EditorWindow),       // Initialize UI pointer
    spriteWidth(width),             // Store user-defined width
    spriteHeight(height)            // Store user-defined height
{

    // Set up UI components
    ui->setupUi(this);

    this->saveLoadManager = saveLoadManager;
    this->frameManager = frameManager;

    // Create the logical pixel grid (sprite image) using ARGB (supports transparency)
    sprite = QImage(spriteWidth, spriteHeight, QImage::Format_ARGB32);

    // Fill the image with a transparent background
    sprite.fill(QColor(255, 255, 255, 0));

    // Fix the QLabel size so it's always 500x500, regardless of sprite resolution
    ui->spriteLabel->setFixedSize(500, 500);

    // Draw initial sprite image on the QLabel
    updateCanvas();

    // Set up the label to handle mouse clicks
    ui->spriteLabel->installEventFilter(this);

    color = QColor::fromRgb(0, 0, 0, 255);

    //////////////////////////////////////////////////
    ////////   UI -> UI CONNECTIONS        //////////
    /////////////////////////////////////////////////

    // When invertButton clicked, invert pixel colors
    connect(ui->invertButton,
            &QPushButton::clicked,
            this,
            &EditorWindow::invertColor);

    // When drawButton clicked, enable drawing
    connect(ui->copyButton,
            &QPushButton::clicked,
            this,
            &EditorWindow::enableCopyColor);

    // When eraserButton clicked, enable erasing
    connect(ui->eraserButton,
            &QPushButton::clicked,
            this,
            &EditorWindow::enableEraser);

    // When drawButton clicked, enable drawing
    connect(ui->drawButton,
            &QPushButton::clicked,
            this,
            &EditorWindow::enableDrawing);

    // When the red value changes, change the current color's red value
    connect(ui->redSpinBox,
            &QSpinBox::valueChanged,
            this,
            &EditorWindow::redChanged);

    // When the blue value changes, change the current color's blue value
    connect(ui->blueSpinBox,
            &QSpinBox::valueChanged,
            this,
            &EditorWindow::blueChanged);

    // When the green value changes, change the current color's green value
    connect(ui->greenSpinBox,
            &QSpinBox::valueChanged,
            this,
            &EditorWindow::greenChanged);

    // When the alpha value changes, change the current color's alpha value
    connect(ui->alphaSpinBox,
            &QSpinBox::valueChanged,
            this,
            &EditorWindow::alphaChanged);

    // Emits a signal that changes the value of the redSpinBox
    connect(this,
            &EditorWindow::changeRedValue,
            ui->redSpinBox,
            &QSpinBox::setValue);

    // Emits a signal that changes the value of the greenSpinBox
    connect(this,
            &EditorWindow::changeGreenValue,
            ui->greenSpinBox,
            &QSpinBox::setValue);

    // Emits a signal that changes the value of the blueSpinBox
    connect(this,
            &EditorWindow::changeBlueValue,
            ui->blueSpinBox,
            &QSpinBox::setValue);

    // Emits a signal that changes the value of the alphaSpinBox
    connect(this,
            &EditorWindow::changeAlphaValue,
            ui->alphaSpinBox,
            &QSpinBox::setValue);

    //////////////////////////////////////////////////
    ////////   UI -> FRAMEMANAGER CONNECTIONS   //////
    /////////////////////////////////////////////////

    // When EditorWindow starts, add one frame to match the canvas
    connect(this,
            &EditorWindow::addOneFrame,
            frameManager,
            &FrameManager::addFrame);

    // When addFrameButton clicked, add new frame to frameManager
    connect(ui->addFrameButton,
            &QPushButton::clicked,
            frameManager,
            &FrameManager::addFrame);

    // When frameAdded to frameManager, addFrameToStack in editorWindow
    connect(frameManager,
            &FrameManager::frameAdded,
            this,
            &EditorWindow::addFrameToStack);

    // When deleteFrameButton clicked, deleteFrameFromStack in editorWindow
    connect(ui->deleteFrameButton,
            &QPushButton::clicked,
            this,
            &EditorWindow::deleteFrameFromStack);

    // When deleteFrame emits, deleteFrame in frameManager
    connect(this,
            &EditorWindow::deleteFrame,
            frameManager,
            &FrameManager::deleteFrame);

    // When updatePixelInFrame emits, updateFrame in frameManager
    connect(this,
            &EditorWindow::updatePixelInFrame,
            frameManager,
            &FrameManager::updateFrame);

    // When itemSelectionChanged in frameStackWidget, getSelectedFrame from editorWindow
    connect(ui->frameStackWidget,
            &QListWidget::itemSelectionChanged,
            this,
            &EditorWindow::getSelectedFrame);

    // When getPixels emits, getPixelsForFrame for frameManager
    connect(this,
            &EditorWindow::getPixels,
            frameManager,
            &FrameManager::getPixelsForFrame);

    // When frameManager foundFrame, switchCanvas in editorWindow
    connect(frameManager,
            &FrameManager::foundFrame,
            this,
            &EditorWindow::switchCanvas);

    // When duplicateFrameButton clicked, getSelectedFrameToCopy from editorWindow
    connect(ui->duplicateFrameButton,
            &QPushButton::clicked,
            this,
            &EditorWindow::getSelectedFrameToCopy);

    // When selectedFrameToCopy emits, copyFrame in frameManager
    connect(this,
            &EditorWindow::selectedFrameToCopy,
            frameManager,
            &FrameManager::copyFrame);

    // When rotateButton is clicked, get the frame to rotate
    connect(ui->rotateButton,
            &QPushButton::clicked,
            this,
            &EditorWindow::getSelectedFrameToRotate);

    // Pass the frame to model to rotate it
    connect(this,
            &EditorWindow::selectedFrameToRotate,
            frameManager,
            &FrameManager::rotate90Clockwise);

    //////////////////////////////////////////////////
    /////   UI -> SAVELOADMANAGER CONNECTIONS   /////
    /////////////////////////////////////////////////

    // When saveButton clicked,
    connect(ui->saveButton,
            &QPushButton::clicked,
            this,
            &EditorWindow::onSaveButtonClicked);

}

// Destructor
EditorWindow::~EditorWindow() {
    delete ui;
}

// Slot - UI
void EditorWindow::invertColor() {
    int frameIndex = getCurrentFrameIndex();
    for(int y = 0; y < spriteHeight; ++y){
        for(int x = 0; x < spriteWidth; ++x){
            QColor original = sprite.pixelColor(x, y);
            QColor inverted(
                255 - original.red(),
                255 - original.green(),
                255 - original.blue(),
                original.alpha()
            );

            sprite.setPixelColor(x, y, inverted);
            emit updatePixelInFrame(frameIndex, y, x, inverted.red(), inverted.green(), inverted.blue(), inverted.alpha());
        }
    }
    updateCanvas();
}

// Slot - UI
void EditorWindow::redChanged(int value) {
    color.setRed(value);
    ui->colorPreview->setStyleSheet("QLabel { background-color: " + color.name(QColor::HexArgb) + "; }");
}

// Slot - UI
void EditorWindow::blueChanged(int value) {
    color.setBlue(value);
    ui->colorPreview->setStyleSheet("QLabel { background-color: " + color.name(QColor::HexArgb) + "; }");
}

// Slot - UI
void EditorWindow::greenChanged(int value) {
    color.setGreen(value);
    ui->colorPreview->setStyleSheet("QLabel { background-color: " + color.name(QColor::HexArgb) + "; }");
}

// Slot - UI
void EditorWindow::alphaChanged(int value) {
    color.setAlpha(value);
    ui->colorPreview->setStyleSheet("QLabel { background-color: " + color.name(QColor::HexArgb) + "; }");
}

// Slot - UI
void EditorWindow::enableDrawing() {
    isDrawing = true;
    isErasing = false;
    isGettingColor = false;
}

// Slot - UI
void EditorWindow::enableEraser() {
    isErasing = true;
    isDrawing = false;
    isGettingColor = false;
}

// Slot - UI
void EditorWindow::enableCopyColor() {
    isGettingColor = true;
    isDrawing = false;
    isErasing = false;
}

// Slot - UI
void EditorWindow::animateClicked() {

    // Create preview window
    PreviewWindow *preview = new PreviewWindow(spriteHeight, spriteWidth, frameManager, this);

    // Show preview window
    preview->show();
}

// Slot - UI
void EditorWindow::addFrameToStack(int frameNumber) {
    ui->frameStackWidget->addItem("Frame" + QString::number(frameNumber));
}

// Slot - UI
void EditorWindow::deleteFrameFromStack() {
    QListWidgetItem *selectedItem = ui->frameStackWidget->currentItem();

    if (selectedItem) {

        // There should always be at least 1 frame.
        if (ui->frameStackWidget->count() > 1) {

            int frameIndex = ui->frameStackWidget->row(selectedItem);
            delete ui->frameStackWidget->takeItem(frameIndex);
            emit deleteFrame(frameIndex);

            // Update the frame names upon successful deletion.
            for (int i = 0; i < ui->frameStackWidget->count(); ++i) {
                QListWidgetItem *item = ui->frameStackWidget->item(i);
                item->setText("Frame " + QString::number(i + 1));
            }
        }
    }
}

// Slot - UI
void EditorWindow::switchCanvas(std::vector<std::vector<QColor>> pixels) {

    // Dimensions of the QLabel display area
    int labelWidth = ui->spriteLabel->width();
    int labelHeight = ui->spriteLabel->height();

    // Calculate pixel size so the sprite fits inside the label
    int pixelWidth = labelWidth / spriteWidth;
    int pixelHeight = labelHeight / spriteHeight;
    int pixelSize = max(1, min(pixelWidth, pixelHeight));

    // Calculate the total size of the scaled sprite
    int totalWidth = pixelSize * spriteWidth;
    int totalHeight = pixelSize * spriteHeight;

    // Center the drawing on the QLabel
    int offsetX = (labelWidth - totalWidth) / 2;
    int offsetY = (labelHeight - totalHeight) / 2;

    // Create the visual canvas image (what the user sees)
    QPixmap canvas(labelWidth, labelHeight);

    // Set background color to White
    canvas.fill(Qt::white);

    QPainter painter(&canvas);

    QColor savedColor = color;

    for (int y = 0; y < spriteHeight; ++y) {

        for (int x = 0; x < spriteWidth; ++x) {
            color = pixels.at(y).at(x);
            sprite.setPixelColor(x, y, color);
        }
    }

    color = savedColor;

    // Draw each pixel from the logical sprite onto the canvas
    for (int y = 0; y < spriteHeight; ++y) {

        for (int x = 0; x < spriteWidth; ++x) {

            // Get color at (x, y)
            QColor color = sprite.pixelColor(x, y);
            QRect rect(offsetX + x * pixelSize, offsetY + y * pixelSize, pixelSize, pixelSize);

            // Fill pixel
            painter.fillRect(rect, color);

            // Draw grid
            painter.setPen(Qt::gray);
            painter.drawRect(rect);
        }
    }

    // Update the QLabel with the new canvas image
    ui->spriteLabel->setPixmap(canvas);
}

// Slot - UI
void EditorWindow::getSelectedFrame() {
    QListWidgetItem *selectedItem = ui->frameStackWidget->currentItem();

    if (selectedItem) {
        int frameIndex = ui->frameStackWidget->row(selectedItem);
        emit getPixels(frameIndex);
    }
}

// Slot - UI
void EditorWindow::getSelectedFrameToCopy() {
    QListWidgetItem *selectedItem = ui->frameStackWidget->currentItem();

    if (selectedItem) {
        int frameIndex = ui->frameStackWidget->row(selectedItem);
        emit selectedFrameToCopy(frameIndex);
    }
}

// Slot - UI
void EditorWindow::getSelectedFrameToRotate() {
    QListWidgetItem *selectedItem = ui->frameStackWidget->currentItem();
    int frameIndex;

    if (selectedItem) {
        frameIndex = ui->frameStackWidget->row(selectedItem);
    }

    else {
        frameIndex = 0;
    }

    emit selectedFrameToRotate(frameIndex);
}

// Method -
void EditorWindow::selectFirstFrame() {
    if (ui->frameStackWidget->count() > 0) {
        ui->frameStackWidget->setCurrentRow(0);
        emit getPixels(0);
    }
}

// Method - Setter
void EditorWindow::setSpriteWidth(int width) {
    spriteWidth = width;
}

// Method - Setter
void EditorWindow::setSpriteHeight(int height) {
    spriteHeight = height;
}

// Method -
void EditorWindow::initializeFromLoadedFile(int width, int height) {
    spriteWidth = width;
    spriteHeight = height;

    // Create and clear canvas image
    sprite = QImage(spriteWidth, spriteHeight, QImage::Format_ARGB32);
    sprite.fill(QColor(255, 255, 255, 0));

    // Populate frame list in UI
    ui->frameStackWidget->clear();
    for (size_t i = 0; i < frameManager->frames.size(); ++i) {
        addFrameToStack(i + 1);
    }

    // Trigger canvas to draw first frame
    ui->frameStackWidget->setCurrentRow(0);
    emit getPixels(0); // causes switchCanvas to be called
}

// Method - Used to reinitialize editorwindow size
// Called in mainwindow before opening the editorwindow
void EditorWindow::reinitializeEditor(int newWidth, int newHeight) {
    spriteWidth = newWidth;
    spriteHeight = newHeight;

    // Resize sprite image
    sprite = QImage(spriteWidth, spriteHeight, QImage::Format_ARGB32);
    sprite.fill(QColor(255, 255, 255, 0));

    // Clear frames and start fresh
    frameManager->frames.clear();

    frameManager->frames.clear();
    frameManager->height = newHeight;
    frameManager->width = newWidth;
    emit addOneFrame(); // Add 1 new blank frame

    updateCanvas(); // Redraw canvas
}

// Method - Redraws the canvas (spriteLabel) to reflect the current state of the sprite image.
// This ensures that any changes to pixel colors are visually updated on the screen.
void EditorWindow::updateCanvas() {

    // Dimensions of the QLabel display area
    int labelWidth = ui->spriteLabel->width();
    int labelHeight = ui->spriteLabel->height();

    // Calculate pixel size so the sprite fits inside the label
    int pixelWidth = labelWidth / spriteWidth;
    int pixelHeight = labelHeight / spriteHeight;
    int pixelSize = max(1, min(pixelWidth, pixelHeight));

    // Calculate the total size of the scaled sprite
    int totalWidth = pixelSize * spriteWidth;
    int totalHeight = pixelSize * spriteHeight;

    // Center the drawing on the QLabel
    int offsetX = (labelWidth - totalWidth) / 2;
    int offsetY = (labelHeight - totalHeight) / 2;

    // Create the visual canvas image (what the user sees)
    QPixmap canvas(labelWidth, labelHeight);

    // Set background color to White
    canvas.fill(Qt::white);

    QPainter painter(&canvas);

    // Draw each pixel from the logical sprite onto the canvas
    for (int y = 0; y < spriteHeight; ++y) {

        for (int x = 0; x < spriteWidth; ++x) {

            // Get color at (x, y)
            QColor color = sprite.pixelColor(x, y);
            QRect rect(offsetX + x * pixelSize, offsetY + y * pixelSize, pixelSize, pixelSize);

            // Fill pixel
            painter.fillRect(rect, color);

            // Draw grid
            painter.setPen(Qt::gray);
            painter.drawRect(rect);
        }

    }

    // Update the QLabel with the new canvas image
    ui->spriteLabel->setPixmap(canvas);
}

// Method - Filters events on the spriteLabel to handle mouse-based drawing, erasing, and color picking.
// Supports both click and drag interactions for intuitive user control.
bool EditorWindow::eventFilter(QObject *watched, QEvent *event) {

    // Only handle events for the spriteLabel (the drawing area)// Only handle events for the spriteLabel (the drawing area)
    if (watched == ui->spriteLabel) {

        // Get dimensions of the QLabel (visual canvas area)
        int labelWidth = ui->spriteLabel->width();
        int labelHeight = ui->spriteLabel->height();

        // Calculate the size of each "pixel" block to scale the sprite to fit the canvas
        int pixelWidth = labelWidth / spriteWidth;
        int pixelHeight = labelHeight / spriteHeight;
        int pixelSize = max(1, min(pixelWidth, pixelHeight));

        // Calculate the total area occupied by the scaled sprite
        int totalWidth = pixelSize * spriteWidth;
        int totalHeight = pixelSize * spriteHeight;

        // Calculate offsets to center the sprite in the QLabel
        int offsetX = (labelWidth - totalWidth) / 2;
        int offsetY = (labelHeight - totalHeight) / 2;

        // Lambda to convert screen coordinates to logical (x, y) in the sprite grid
        auto getXY = [&](const QPoint& pos, int& x, int& y) {
            x = (pos.x() - offsetX) / pixelSize;
            y = (pos.y() - offsetY) / pixelSize;
        };

        // Handle mouse button press (begin drawing or interaction)
        if (event->type() == QEvent::MouseButtonPress) {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
            int x, y;
            getXY(mouseEvent->pos(), x, y);

            // Start tracking mouse drag
            mousePressed = true;

            // Only process if (x, y) is within bounds
            if (x >= 0 && x < spriteWidth && y >= 0 && y < spriteHeight) {

                // Perform drawing/erasing/color-picking
                handleDrawingAction(x, y);

                // Redraw the canvas
                updateCanvas();
            }

            // Event handled
            return true;
        }

        // Handle mouse movement (continue drawing while dragging)
        else if (event->type() == QEvent::MouseMove && mousePressed) {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
            int x, y;
            getXY(mouseEvent->pos(), x, y);

            // Only process if (x, y) is within bounds
            if (x >= 0 && x < spriteWidth && y >= 0 && y < spriteHeight) {

                // Continue drawing
                handleDrawingAction(x, y);

                // Redraw canvas
                updateCanvas();
            }

            // Event handled
            return true;
        }

        // Handle mouse release (end drawing or dragging)
        else if (event->type() == QEvent::MouseButtonRelease) {

            // Stop tracking mouse drag
            mousePressed = false;

            // Event handled
            return true;
        }
    }

    // Pass unhandled events to the default implementation
    return QMainWindow::eventFilter(watched, event);
}

// Method - Handles drawing, erasing, or color picking on the canvas based on the current mode.
// Called from mouse event handlers when the user interacts with the sprite grid.
void EditorWindow::handleDrawingAction(int x, int y) {

    // If drawing mode is active
    if (isDrawing) {

        // Set the pixel at (x, y) to the current selected color
        sprite.setPixelColor(x, y, color);

        // Get the current frame index selected in the frame stack
        int frameIndex = getCurrentFrameIndex();

        // Notify the frame manager to update the pixel in the current frame
        emit updatePixelInFrame(frameIndex, y, x, color.red(), color.green(), color.blue(), color.alpha());
    }

    // If eraser mode is active
    else if (isErasing) {

        // Set the pixel at (x, y) to a fully transparent color (erased)
        sprite.setPixelColor(x, y, QColor(255, 255, 255, 0));

        // Get the current frame index selected in the frame stack
        int frameIndex = getCurrentFrameIndex();

        // Notify the frame manager to update the pixel in the current frame as erased
        emit updatePixelInFrame(frameIndex, y, x, 255, 255, 255, 0);
    }

    // If color picker mode is active
    else if (isGettingColor) {

        // Get the color at (x, y) from the sprite
        color = sprite.pixelColor(x, y);

        // Update the spin boxes and color preview UI with the selected color values
        emit changeRedValue(color.red());
        emit changeGreenValue(color.green());
        emit changeBlueValue(color.blue());
        emit changeAlphaValue(color.alpha());
    }
}

// Helper Method - Retrieves the index of the currently selected frame in the frame stack.
// Returns 0 if no frame is selected (default to the first frame).
int EditorWindow::getCurrentFrameIndex() {

    // Get the currently selected item in the frameStackWidget (the list of frames)
    QListWidgetItem *selectedItem = ui->frameStackWidget->currentItem();

    // If an item is selected, return its index (row number) in the list.
    // If no item is selected (e.g., at startup), default to frame index 0.
    return selectedItem ? ui->frameStackWidget->row(selectedItem) : 0;
}

// Slot - Triggered when the Save button is clicked in the EditorWindow
void EditorWindow::onSaveButtonClicked() {

    // Open a file save dialog and let the user pick a path to save the JSON file.
    // The dialog title is "Save Sprite File", and it filters for .json files.
    QString filePath = QFileDialog::getSaveFileName(
        this,                           // Parent widget (EditorWindow)
        "Save Sprite File",             // Title of the dialog
        "",                             // Default directory (empty = current)
        "Sprite Save Files (*.ssp)"     // File filter
        );

    // Check if the user selected a file (didn't cancel the dialog)
    if (!filePath.isEmpty()) {

        // Ensure the file has a .ssp extension
        if (!filePath.endsWith(".ssp", Qt::CaseInsensitive)) {
            filePath += ".ssp";
        }

        bool success = saveLoadManager->saveToFile(*frameManager, filePath);

        // If the save was successful, show a success message
        if (success) {
            QMessageBox::information(
                this,                       // Parent widget
                "Success",                  // Dialog title
                "File saved successfully!"  // Message
                );
        }

        // If the save failed, show an error message
        else {
            QMessageBox::warning(
                this,                       // Parent widget
                "Error",                    // Dialog title
                "Failed to save the file."  // Message
                );
        }
    }
}
