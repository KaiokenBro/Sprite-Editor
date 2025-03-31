/**
 * @file previewwindow.cpp
 * @author Samuel Garcia
 *
 * @brief Implements the logic for the PreviewWindow class which renders animated previews
 * of sprite frames at user-defined speed and resolution.
 *
 * @date 03/31/2025
 */

#include "previewwindow.h"
#include "ui_previewwindow.h"

using std::min;
using std::max;
using std::vector;

PreviewWindow::PreviewWindow(FrameManager* frameManager, int height, int width, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::previewwindow),
    actualHeight(height),
    actualWidth(width),
    sprite(QImage(actualWidth, actualHeight, QImage::Format_ARGB32))
{
    ui->setupUi(this);

    // Connect signal to fetch all frames from FrameManager when animation starts
    connect(this,
            &PreviewWindow::getFrames,
            frameManager,
            &FrameManager::sendFrames);

}

PreviewWindow::~PreviewWindow() {
    delete ui;
}

void PreviewWindow::animation() {
    bool animateBool = ui->animateButton->isChecked();
    vector<Frame> frames = emit getFrames();

    while (animateBool && this->isVisible()) {
        for (const Frame& frame : frames) {
            showFrame(frame);
        }
        animateBool = ui->animateButton->isChecked();
    }
}

void PreviewWindow::showFrame(Frame frame) {
    vector<vector<QColor>> pixels = frame.getPixels();

    // Dimensions of the QLabel display area
    int labelWidth = ui->spriteLabel->width();
    int labelHeight = ui->spriteLabel->height();

    // Sets the size to pixel size of the radio button is checked.
    bool actualSize = ui->actualSizeRadio->isChecked();

    if (actualSize) {
        labelWidth = actualWidth;
        labelHeight = actualHeight;
    }

    // Calculate pixel size so the sprite fits inside the label
    int pixelWidth = labelWidth / actualWidth;
    int pixelHeight = labelHeight / actualHeight;
    int pixelSize = max(1, min(pixelWidth, pixelHeight));

    // Calculate the total size of the scaled sprite
    int totalWidth = pixelSize * actualWidth;
    int totalHeight = pixelSize * actualHeight;

    // Center the drawing on the QLabel
    int offsetX = (labelWidth - totalWidth) / 2;
    int offsetY = (labelHeight - totalHeight) / 2;

    // Create the visual canvas image (what the user sees)
    QPixmap canvas(labelWidth, labelHeight);

    // Set background color to dark gray
    canvas.fill(QColor(100, 100, 100, 50));

    QPainter painter(&canvas);

    for (int y = 0; y < actualHeight; ++y) {
        for (int x = 0; x < actualWidth; ++x) {
            QColor color = pixels.at(y).at(x);
            sprite.setPixelColor(x, y, color);
        }
    }

    // Draw each pixel from the logical sprite onto the canvas
    for (int y = 0; y < actualHeight; ++y) {
        for (int x = 0; x < actualWidth; ++x) {
            QColor color = sprite.pixelColor(x, y);
            QRect rect(offsetX + x * pixelSize, offsetY + y * pixelSize, pixelSize, pixelSize);
            painter.fillRect(rect, color);
        }
    }

    ui->spriteLabel->setPixmap(canvas);

    // Timer to wait 1/FPS seconds before drawing next image
    QEventLoop loop;
    QTimer timer;
    QObject::connect(&timer, &QTimer::timeout, &loop, &QEventLoop::quit);
    timer.start(1000/ui->fpsSlider->sliderPosition());
    loop.exec();
}
