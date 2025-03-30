#include "previewwindow.h"
#include "ui_previewwindow.h"

/**
 * @file previewwindow.cpp
 * @authors Samuel Garcia,
 *
 * @brief This file handles the preview window and the animation of the sprite.
 *
 * @date 03/23/2025
 */

PreviewWindow::PreviewWindow(int height, int width, FrameManager *frameManager, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::previewwindow)
    , actualHeight(height)
    , actualWidth(width)
    , sprite(QImage(actualWidth, actualHeight, QImage::Format_ARGB32))
{
    ui->setupUi(this);

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
    animate(animateBool);
}

void PreviewWindow::animate(bool animationBool) {
    std::vector<Frame> frames = emit getFrames();

    while (animationBool && this->isVisible()) {
        for (const Frame& frame : frames) {
            showFrame(frame);
        }
        animationBool = ui->animateButton->isChecked();
    }
}

void PreviewWindow::showFrame(Frame frame) {
    std::vector<std::vector<QColor>> pixels = frame.getPixels();

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
    int pixelSize = std::max(1, std::min(pixelWidth, pixelHeight));

    // Calculate the total size of the scaled sprite
    int totalWidth = pixelSize * actualWidth;
    int totalHeight = pixelSize * actualHeight;

    // Center the drawing on the QLabel
    int offsetX = (labelWidth - totalWidth) / 2;
    int offsetY = (labelHeight - totalHeight) / 2;

    // Create the visual canvas image (what the user sees)
    QPixmap canvas(labelWidth, labelHeight);

    // Set background color to White
    canvas.fill(Qt::white);

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
            // Get color at (x, y)
            QColor color = sprite.pixelColor(x, y);
            QRect rect(offsetX + x * pixelSize, offsetY + y * pixelSize, pixelSize, pixelSize);

            // Fill pixel
            painter.fillRect(rect, color);
        }

    }

    // Update the QLabel with the new canvas image
    ui->spriteLabel->setPixmap(canvas);
    QEventLoop loop;
    QTimer timer;
    QObject::connect(&timer, &QTimer::timeout, &loop, &QEventLoop::quit);
    timer.start(1000/ui->fpsSlider->sliderPosition());
    loop.exec();
}
