/**
 * @file editorwindow.cpp
 * @brief This file contains the implementation of the EditorWindow class,
 * which provides the main interface for drawing, editing, saving, and managing
 * sprite frames in the sprite editor application.
 *
 * The EditorWindow is responsible for capturing user input (mouse events, button clicks),
 * performing canvas operations (draw, erase, invert, etc.), and communicating with the
 * FrameManager and SaveLoadManager to persist sprite state.
 *
 * @date 03/31/2025
 */

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
using std::vector;

EditorWindow::EditorWindow(SaveLoadManager* saveLoadManager, FrameManager* frameManager, int width, int height, QWidget* parent) :
    QMainWindow(parent),
    ui(new Ui::EditorWindow),
    spriteWidth(width),
    spriteHeight(height)
{
    ui->setupUi(this);

    this->saveLoadManager = saveLoadManager;
    this->frameManager = frameManager;

    // Create the logical pixel grid (sprite image) using ARGB (supports transparency)
    sprite = QImage(spriteWidth, spriteHeight, QImage::Format_ARGB32);

    // Set background color to dark gray
    sprite.fill(QColor(100, 100, 100, 50));

    // Fix the QLabel size so it's always 500x500, regardless of sprite resolution
    ui->spriteLabel->setFixedSize(500, 500);

    // Draw initial sprite image on the QLabel
    updateCanvas();

    // Set up the label to handle mouse clicks
    ui->spriteLabel->installEventFilter(this);

    color = QColor::fromRgb(0, 0, 0, 255);

    // Connect "Invert" button to inverting all pixel colors on the canvas
    connect(ui->invertButton,
            &QPushButton::clicked,
            this,
            &EditorWindow::invertColor
    );

    // Connect "Copy Color" button to enable color picking mode
    connect(ui->copyButton,
            &QPushButton::clicked,
            this,
            &EditorWindow::enableCopyColor
    );

    // Connect "Eraser" button to enable eraser mode
    connect(ui->eraserButton,
            &QPushButton::clicked,
            this,
            &EditorWindow::enableEraser
    );

    // Connect "Draw" button to enable drawing mode
    connect(ui->drawButton,
            &QPushButton::clicked,
            this,
            &EditorWindow::enableDrawing
    );

    // Connect red spinbox changes to update the current color's red component
    connect(ui->redSpinBox,
            &QSpinBox::valueChanged,
            this,
            &EditorWindow::redChanged
    );

    // Connect blue spinbox changes to update the current color's blue component
    connect(ui->blueSpinBox,
            &QSpinBox::valueChanged,
            this,
            &EditorWindow::blueChanged
    );

    // Connect green spinbox changes to update the current color's green component
    connect(ui->greenSpinBox,
            &QSpinBox::valueChanged,
            this,
            &EditorWindow::greenChanged
    );

    // Connect alpha spinbox changes to update the current color's transparency
    connect(ui->alphaSpinBox,
            &QSpinBox::valueChanged,
            this,
            &EditorWindow::alphaChanged
    );

    // Connect color picker mode to update red spinbox value
    connect(this,
            &EditorWindow::changeRedValue,
            ui->redSpinBox,
            &QSpinBox::setValue
    );

    // Connect color picker mode to update green spinbox value
    connect(this,
            &EditorWindow::changeGreenValue,
            ui->greenSpinBox,
            &QSpinBox::setValue
    );

    // Connect color picker mode to update blue spinbox value
    connect(this,
            &EditorWindow::changeBlueValue,
            ui->blueSpinBox,
            &QSpinBox::setValue
    );

    // Connect color picker mode to update alpha spinbox value
    connect(this,
            &EditorWindow::changeAlphaValue,
            ui->alphaSpinBox,
            &QSpinBox::setValue
    );

    // Connect startup signal to add one blank frame to match canvas
    connect(this,
            &EditorWindow::addOneFrame,
            frameManager,
            &FrameManager::addFrame
    );

    // Connect "Add Frame" button to appending a blank frame to the stack
    connect(ui->addFrameButton,
            &QPushButton::clicked,
            frameManager,
            &FrameManager::addFrame
    );

    // Connect frame added signal to update frame stack UI
    connect(frameManager,
            &FrameManager::frameAdded,
            this,
            &EditorWindow::addFrameToStack
    );

    // Connect "Delete Frame" button to delete the selected frame from UI
    connect(ui->deleteFrameButton,
            &QPushButton::clicked,
            this,
            &EditorWindow::deleteFrameFromStack
    );

    // Connect delete frame signal to frame manager to delete model data
    connect(this,
            &EditorWindow::deleteFrame,
            frameManager,
            &FrameManager::deleteFrame
    );

    // Connect pixel update signal to update the selected frame in the model
    connect(this,
            &EditorWindow::updatePixelInFrame,
            frameManager,
            &FrameManager::updateFrame
    );

    // Connect frame selection change to reloading that frame on the canvas
    connect(ui->frameStackWidget,
            &QListWidget::itemSelectionChanged,
            this,
            &EditorWindow::getSelectedFrame
    );

    // Connect frame pixel request to load pixels from the frame manager
    connect(this,
            &EditorWindow::getPixels,
            frameManager,
            &FrameManager::getPixelsForFrame
    );

    // Connect pixels loaded signal to repaint the canvas
    connect(frameManager,
            &FrameManager::foundFrame,
            this,
            &EditorWindow::switchCanvas
    );

    // Connect "Duplicate Frame" button to emit copy request
    connect(ui->duplicateFrameButton,
            &QPushButton::clicked,
            this,
            &EditorWindow::getSelectedFrameToCopy
    );

    // Connect copy request signal to frame manager's copy function
    connect(this,
            &EditorWindow::selectedFrameToCopy,
            frameManager,
            &FrameManager::copyFrame
    );

    // Connect "Rotate" button to emit rotation request for selected frame
    connect(ui->rotateButton,
            &QPushButton::clicked,
            this,
            &EditorWindow::getSelectedFrameToRotate
    );

    // Connect rotation signal to rotate the specified frame in model
    connect(this,
            &EditorWindow::selectedFrameToRotate,
            frameManager,
            &FrameManager::rotate90Clockwise
    );

    // Connect "Save" button to trigger file save dialog and operation
    connect(ui->saveButton,
            &QPushButton::clicked,
            this,
            &EditorWindow::onSaveButtonClicked
    );

}

EditorWindow::~EditorWindow() {
    delete ui;
}

void EditorWindow::invertColor() {
    int frameIndex = getCurrentFrameIndex();

    for (int y = 0; y < spriteHeight; ++y) {
        for (int x = 0; x < spriteWidth; ++x) {
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

void EditorWindow::redChanged(int value) {
    color.setRed(value);
    ui->colorPreview->setStyleSheet("QLabel { background-color: " + color.name(QColor::HexArgb) + "; }");
}

void EditorWindow::blueChanged(int value) {
    color.setBlue(value);
    ui->colorPreview->setStyleSheet("QLabel { background-color: " + color.name(QColor::HexArgb) + "; }");
}

void EditorWindow::greenChanged(int value) {
    color.setGreen(value);
    ui->colorPreview->setStyleSheet("QLabel { background-color: " + color.name(QColor::HexArgb) + "; }");
}

void EditorWindow::alphaChanged(int value) {
    color.setAlpha(value);
    ui->colorPreview->setStyleSheet("QLabel { background-color: " + color.name(QColor::HexArgb) + "; }");
}

void EditorWindow::enableDrawing() {
    isDrawing = true;
    isErasing = false;
    isGettingColor = false;
}

void EditorWindow::enableEraser() {
    isErasing = true;
    isDrawing = false;
    isGettingColor = false;
}

void EditorWindow::enableCopyColor() {
    isGettingColor = true;
    isDrawing = false;
    isErasing = false;
}

void EditorWindow::animateClicked() {
    PreviewWindow *preview = new PreviewWindow(frameManager, spriteHeight, spriteWidth, this);
    preview->show();
}

void EditorWindow::addFrameToStack(int frameNumber) {
    ui->frameStackWidget->addItem("Frame" + QString::number(frameNumber));
}

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
                QListWidgetItem* item = ui->frameStackWidget->item(i);
                item->setText("Frame " + QString::number(i + 1));
            }
        }
    }
}

void EditorWindow::switchCanvas(vector<vector<QColor>> pixels) {

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

    // Set background color to dark gray
    canvas.fill(QColor(100, 100, 100, 50));

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
            QColor color = sprite.pixelColor(x, y);
            QRect rect(offsetX + x * pixelSize, offsetY + y * pixelSize, pixelSize, pixelSize);
            painter.fillRect(rect, color);
            painter.setPen(Qt::gray);
            painter.drawRect(rect);
        }
    }
    ui->spriteLabel->setPixmap(canvas);
}

void EditorWindow::getSelectedFrame() {
    QListWidgetItem* selectedItem = ui->frameStackWidget->currentItem();

    if (selectedItem) {
        int frameIndex = ui->frameStackWidget->row(selectedItem);
        emit getPixels(frameIndex);
    }
}

void EditorWindow::getSelectedFrameToCopy() {
    QListWidgetItem* selectedItem = ui->frameStackWidget->currentItem();

    if (selectedItem) {
        int frameIndex = ui->frameStackWidget->row(selectedItem);
        emit selectedFrameToCopy(frameIndex);
    }
}

void EditorWindow::getSelectedFrameToRotate() {
    QListWidgetItem* selectedItem = ui->frameStackWidget->currentItem();
    int frameIndex;

    if (selectedItem) {
        frameIndex = ui->frameStackWidget->row(selectedItem);
    }

    else {
        frameIndex = 0;
    }

    emit selectedFrameToRotate(frameIndex);
}

void EditorWindow::setSpriteWidth(int width) {
    spriteWidth = width;
}

void EditorWindow::setSpriteHeight(int height) {
    spriteHeight = height;
}

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

void EditorWindow::reinitializeEditor(int newWidth, int newHeight) {
    spriteWidth = newWidth;
    spriteHeight = newHeight;

    // Resize sprite image
    sprite = QImage(spriteWidth, spriteHeight, QImage::Format_ARGB32);
    sprite.fill(QColor(255, 255, 255, 0));

    frameManager->frames.clear();
    frameManager->height = newHeight;
    frameManager->width = newWidth;
    emit addOneFrame(); // Add 1 new blank frame

    updateCanvas();
}

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

    // Set background color to dark gray
    canvas.fill(QColor(100, 100, 100, 50));
    QPainter painter(&canvas);

    // Draw each pixel from the logical sprite onto the canvas
    for (int y = 0; y < spriteHeight; ++y) {
        for (int x = 0; x < spriteWidth; ++x) {
            QColor color = sprite.pixelColor(x, y);
            QRect rect(offsetX + x * pixelSize, offsetY + y * pixelSize, pixelSize, pixelSize);
            painter.fillRect(rect, color);
            painter.setPen(Qt::gray);
            painter.drawRect(rect);
        }
    }

    ui->spriteLabel->setPixmap(canvas);
}

bool EditorWindow::eventFilter(QObject* watched, QEvent* event) {

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
            QMouseEvent* mouseEvent = static_cast<QMouseEvent *>(event);
            int x, y;
            getXY(mouseEvent->pos(), x, y);

            // Start tracking mouse drag
            mousePressed = true;

            // Only process if (x, y) is within bounds
            if (x >= 0 && x < spriteWidth && y >= 0 && y < spriteHeight) {
                handleDrawingAction(x, y);
                updateCanvas();
            }
            return true;
        }

        // Handle mouse movement (continue drawing while dragging)
        else if (event->type() == QEvent::MouseMove && mousePressed) {
            QMouseEvent* mouseEvent = static_cast<QMouseEvent *>(event);
            int x, y;
            getXY(mouseEvent->pos(), x, y);

            // Only process if (x, y) is within bounds
            if (x >= 0 && x < spriteWidth && y >= 0 && y < spriteHeight) {
                handleDrawingAction(x, y);
                updateCanvas();
            }
            return true;
        }

        // Handle mouse release (end drawing or dragging)
        else if (event->type() == QEvent::MouseButtonRelease) {
            mousePressed = false;
            return true;
        }
    }

    // Pass unhandled events to the default implementation
    return QMainWindow::eventFilter(watched, event);
}

void EditorWindow::handleDrawingAction(int x, int y) {

    // If drawing mode is active
    if (isDrawing) {

        // Set the pixel at (x, y) to the current selected color
        sprite.setPixelColor(x, y, color);

        int frameIndex = getCurrentFrameIndex();
        emit updatePixelInFrame(frameIndex, y, x, color.red(), color.green(), color.blue(), color.alpha());
    }

    // If eraser mode is active
    else if (isErasing) {

        // Set the pixel at (x, y) to a fully transparent color (erased)
        sprite.setPixelColor(x, y, QColor(255, 255, 255, 0));

        int frameIndex = getCurrentFrameIndex();
        emit updatePixelInFrame(frameIndex, y, x, 255, 255, 255, 0);
    }

    // If color picker mode is active
    else if (isGettingColor) {
        color = sprite.pixelColor(x, y);
        emit changeRedValue(color.red());
        emit changeGreenValue(color.green());
        emit changeBlueValue(color.blue());
        emit changeAlphaValue(color.alpha());
    }
}

int EditorWindow::getCurrentFrameIndex() {
    QListWidgetItem* selectedItem = ui->frameStackWidget->currentItem();

    // If an item is selected, return its index (row number) in the list.
    // If no item is selected (e.g., at startup), default to frame index 0.
    return selectedItem ? ui->frameStackWidget->row(selectedItem) : 0;
}

void EditorWindow::onSaveButtonClicked() {

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

        if (success) {
            QMessageBox::information(
                this,                       // Parent widget
                "Success",                  // Dialog title
                "File saved successfully!"  // Message
                );
        }

        else {
            QMessageBox::warning(
                this,                       // Parent widget
                "Error",                    // Dialog title
                "Failed to save the file."  // Message
                );
        }
    }
}
