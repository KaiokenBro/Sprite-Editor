#include "editorwindow.h"
#include "ui_editorwindow.h"
#include "previewwindow.h"

#include <QMouseEvent>
#include <QPainter>

using std::min;
using std::max;

// Pattern
// connect(WhatIsEmittingTheSignal,
//        WhichSignalIsIt,
//        WhatHasASlot,
//        WhichSlotIsIt);

// Constructor
EditorWindow::EditorWindow(FrameManager *frameManager, int width, int height, QWidget *parent)
    : QMainWindow(parent)       // Call base QMainWindow constructor
    , ui(new Ui::EditorWindow)  // Initialize UI pointer
    , spriteWidth(width)        // Store user-defined width
    , spriteHeight(height)      // Store user-defined height
{

    // Set up UI components
    ui->setupUi(this);

    // Create the logical pixel grid (sprite image) using ARGB (supports transparency)
    sprite = QImage(spriteWidth, spriteHeight, QImage::Format_ARGB32);

    // Fill the image with a white background
    sprite.fill(Qt::white);

    // Fix the QLabel size so it's always 500x500, regardless of sprite resolution
    ui->spriteLabel->setFixedSize(500, 500);

    // Draw initial sprite image on the QLabel
    updateCanvas();

    // Set up the label to handle mouse clicks
    ui->spriteLabel->installEventFilter(this);

    // Add a frame that corresponds to the canvas to start with
    frameManager->addFrame();
    addFrameToStack(1);

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

    //When the red value changes, change the current color's red value
    connect(ui->redSpinBox,
            &QSpinBox::valueChanged,
            this,
            &EditorWindow::redChanged);

    //When the blue value changes, change the current color's blue value
    connect(ui->blueSpinBox,
            &QSpinBox::valueChanged,
            this,
            &EditorWindow::blueChanged);

    //When the green value changes, change the current color's green value
    connect(ui->greenSpinBox,
            &QSpinBox::valueChanged,
            this,
            &EditorWindow::greenChanged);

    //When the alpha value changes, change the current color's alpha value
    connect(ui->alphaSpinBox,
            &QSpinBox::valueChanged,
            this,
            &EditorWindow::alphaChanged);

    // Frame Manager related connects below

    connect(ui->addFrameButton,
            &QPushButton::clicked,
            frameManager,
            &FrameManager::addFrame);

    connect(frameManager,
            &FrameManager::frameAdded,
            this,
            &EditorWindow::addFrameToStack);

    connect(ui->deleteFrameButton,
            &QPushButton::clicked,
            this,
            &EditorWindow::deleteFrameFromStack);

    connect(this,
            &EditorWindow::deleteFrame,
            frameManager,
            &FrameManager::deleteFrame);

    connect(this,
            &EditorWindow::updatePixelInFrame,
            frameManager,
            &FrameManager::updateFrame);

    connect(ui->frameStackWidget,
            &QListWidget::itemSelectionChanged,
            this,
            &EditorWindow::getSelectedFrame);

    connect(this,
            &EditorWindow::getPixels,
            frameManager,
            &FrameManager::getPixelsForFrame);

    connect(frameManager,
            &FrameManager::foundFrame,
            this,
            &EditorWindow::switchCanvas);

    connect(ui->duplicateFrameButton,
            &QPushButton::clicked,
            this,
            &EditorWindow::getSelectedFrameToCopy);

    connect(this,
            &EditorWindow::selectedFrameToCopy,
            frameManager,
            &FrameManager::copyFrame);

    color = QColor::fromRgb(0, 0, 0, 255);
}

// Destructor
EditorWindow::~EditorWindow() {
    delete ui;
}

//Slot
void EditorWindow::redChanged(int value) {
    color.setRed(value);
    ui->colorPreview->setStyleSheet("QLabel { background-color: " + color.name(QColor::HexArgb) + "; }");
}

//Slot
void EditorWindow::blueChanged(int value) {
    color.setBlue(value);
    ui->colorPreview->setStyleSheet("QLabel { background-color: " + color.name(QColor::HexArgb) + "; }");
}

//Slot
void EditorWindow::greenChanged(int value) {
    color.setGreen(value);
    ui->colorPreview->setStyleSheet("QLabel { background-color: " + color.name(QColor::HexArgb) + "; }");
}

//Slot
void EditorWindow::alphaChanged(int value) {
    color.setAlpha(value);
    ui->colorPreview->setStyleSheet("QLabel { background-color: " + color.name(QColor::HexArgb) + "; }");
}

// Slot
void EditorWindow::enableDrawing() {
    isDrawing = true;
    isErasing = false;
    isGettingColor = false;
}

// Slot
void EditorWindow::enableEraser() {
    isErasing = true;
    isDrawing = false;
    isGettingColor = false;
}

//Slot
void EditorWindow::enableCopyColor() {
    isGettingColor = true;
    isDrawing = false;
    isErasing = false;
}

// Slot
void EditorWindow::animateClicked() {

    // Create preview window
    previewwindow *preview = new previewwindow(this);

    // Show preview window
    preview->show();
}

// Method - Canvas redraw function
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

// Method
bool EditorWindow::eventFilter(QObject *watched, QEvent *event) {

    // Only respond to mouse presses on the spriteLabel
    if (watched == ui->spriteLabel && event->type() == QEvent::MouseButtonPress) {

        // Convert event
        QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);

        int labelWidth = ui->spriteLabel->width();
        int labelHeight = ui->spriteLabel->height();

        int pixelWidth = labelWidth / spriteWidth;
        int pixelHeight = labelHeight / spriteHeight;
        int pixelSize = max(1, min(pixelWidth, pixelHeight));

        int totalWidth = pixelSize * spriteWidth;
        int totalHeight = pixelSize * spriteHeight;

        int offsetX = (labelWidth - totalWidth) / 2;
        int offsetY = (labelHeight - totalHeight) / 2;

        // Convert screen position to logical (x, y) in sprite grid
        int x = (mouseEvent->pos().x() - offsetX) / pixelSize;
        int y = (mouseEvent->pos().y() - offsetY) / pixelSize;

        if (x >= 0 && x < spriteWidth && y >= 0 && y < spriteHeight) {

            if (isDrawing) {
                sprite.setPixelColor(x, y, color); // Replace with selected color later
                // Update frame in frame manager.
                QListWidgetItem *selectedItem = ui->frameStackWidget->currentItem();
                if (selectedItem) {
                    int frameIndex = ui->frameStackWidget->row(selectedItem);
                    emit updatePixelInFrame(frameIndex, y, x, color.red(), color.green(), color.blue(), color.alpha());
                }
                else {
                    int frameIndex = 0;
                    emit updatePixelInFrame(frameIndex, y, x, color.red(), color.green(), color.blue(), color.alpha());
                }
            }

            else if (isErasing) {
                sprite.setPixelColor(x, y, QColor::fromRgb(255, 255, 255, 0)); // Default "erase to transparent"
                // Update frame in frame manager.
                QListWidgetItem *selectedItem = ui->frameStackWidget->currentItem();
                if (selectedItem) {
                    int frameIndex = ui->frameStackWidget->row(selectedItem);
                    emit updatePixelInFrame(frameIndex, y, x, 255, 255, 255, 0);
                }
                else {
                    int frameIndex = 0;
                    emit updatePixelInFrame(frameIndex, y, x, 255, 255, 255, 0);
                }
            }

            else if (isGettingColor) {
                color = sprite.pixelColor(x, y);

                //Set correct values in UI (Should we do this with signal/slots somehow?)
                ui->blueSpinBox->setValue(color.blue());
                ui->redSpinBox->setValue(color.red());
                ui->greenSpinBox->setValue(color.green());
                ui->alphaSpinBox->setValue(color.alpha());
            }

            updateCanvas();
        }

        // Event handled
        return true;
    }

    // Default behavior
    return QMainWindow::eventFilter(watched, event);
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
            emit deleteFrame(frameIndex);
            delete ui->frameStackWidget->takeItem(frameIndex);

            // Update the frame names upon successful deletion.
            for (int i = 0; i < ui->frameStackWidget->count(); ++i) {
                QListWidgetItem *item = ui->frameStackWidget->item(i);
                item->setText("Frame " + QString::number(i + 1));
            }
        }
    }
}

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

void EditorWindow::getSelectedFrame() {
    QListWidgetItem *selectedItem = ui->frameStackWidget->currentItem();
    if (selectedItem) {
        int frameIndex = ui->frameStackWidget->row(selectedItem);
        emit getPixels(frameIndex);
    }
}

void EditorWindow::getSelectedFrameToCopy() {
    QListWidgetItem *selectedItem = ui->frameStackWidget->currentItem();
    if (selectedItem) {
        int frameIndex = ui->frameStackWidget->row(selectedItem);
        emit selectedFrameToCopy(frameIndex);
    }
}
