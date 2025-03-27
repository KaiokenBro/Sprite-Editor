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
EditorWindow::EditorWindow(int width, int height, QWidget *parent)
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
            }

            else if (isErasing) {
                sprite.setPixelColor(x, y, QColor::fromRgb(255, 255, 255, 0)); // Default "erase to transparent"
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


