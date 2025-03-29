#include "mainwindow.h"
#include "editorwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QIntValidator>

// Pattern
// connect(WhatIsEmittingTheSignal,
//        WhichSignalIsIt,
//        WhatHasASlot,
//        WhichSlotIsIt);

// Constructor
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , editorWindow(nullptr)
{
    ui->setupUi(this);

    // Create a validator to restrict input to integers between 1 and 64
    QIntValidator *validator = new QIntValidator(1, 64, this);

    // Apply validator to width and height input
    ui->widthLineEdit->setValidator(validator);
    ui->heightLineEdit->setValidator(validator);

    // Initially hide createFileBox
    ui->createFileBox->setVisible(false);

    // Initially disable setSizeButton
    ui->setSizeButton->setEnabled(false);

    // Initially disable the createButton
    ui->createButton->setEnabled(false);

    // Initially disable the openButton
    ui->openButton->setEnabled(false);

    // When newButton is clicked, hide welcomeLabel
    connect(ui->newButton,
            &QPushButton::clicked,
            this,
            &MainWindow::hideWelcomeLabel);

    // When loadButton is clicked, hide welcomeLabel
    connect(ui->loadButton,
            &QPushButton::clicked,
            this,
            &MainWindow::hideWelcomeLabel);

    // When newButton is clicked, display createFileBox
    connect(ui->newButton,
            &QPushButton::clicked,
            this,
            &MainWindow::displayCreateFileBox);

    // When loadButton is clicked, hide createFileBox
    connect(ui->loadButton,
            &QPushButton::clicked,
            this,
            &MainWindow::hideCreateFileBox);

    // When widthLineEdit is changed, check to enable setSizeButton
    connect(ui->widthLineEdit,
            &QLineEdit::textChanged,
            this,
            &MainWindow::validateInputs);

    // When heightLineEdit is changed, check to enable setSizeButton
    connect(ui->heightLineEdit,
            &QLineEdit::textChanged,
            this,
            &MainWindow::validateInputs);

    // When setSizeButton is clicked, update statusLabel
    connect(ui->setSizeButton,
            &QPushButton::clicked,
            this,
            &MainWindow::onSetSizeButtonClicked);

    // When newButton is clicked, disable newButton
    connect(ui->newButton,
            &QPushButton::clicked,
            this,
            &MainWindow::disableNewButton);

    // When newButton is clicked, enable loadButton
    connect(ui->newButton,
            &QPushButton::clicked,
            this,
            &MainWindow::enableLoadButton);

    // When loadButton is clicked, enable newButton
    connect(ui->loadButton,
            &QPushButton::clicked,
            this,
            &MainWindow::enableNewButton);

    // When createButton is clicked, openEditorWindow
    connect(ui->createButton,
            &QPushButton::clicked,
            this,
            &MainWindow::openEditorWindow);

    // When loadButton is clicked, loadFile
    connect(ui->loadButton,
            &QPushButton::clicked,
            this,
            &MainWindow::loadFile);

    // When openButton is clicked, openEditorWindow
    connect(ui->openButton,
            &QPushButton::clicked,
            this,
            &MainWindow::openEditorWindow);

    // When widthLineEdit's value is changed, update heightLineEdit's value to reflect the same
    connect(ui->widthLineEdit,
            &QLineEdit::textChanged,
            this,
            &MainWindow::syncHeightToWidth);

    // When heightLineEdit's value is changed, update widthLineEdit's value to reflect the same
    connect(ui->heightLineEdit,
            &QLineEdit::textChanged,
            this,
            &MainWindow::syncWidthToHeight);

}

// Destructor
MainWindow::~MainWindow() {
    delete ui;
}

// Slot
void MainWindow::hideWelcomeLabel() {
    ui->welcomeLabel->setVisible(false);
}

// Slot
void MainWindow::displayCreateFileBox() {
    ui->createFileBox->setVisible(true);
}

// Slot
void MainWindow::hideCreateFileBox() {
    ui->createFileBox->setVisible(false);
}

// Slot
void MainWindow::disableNewButton() {
    ui->newButton->setEnabled(false);
}

// Slot
void MainWindow::enableNewButton() {
    ui->newButton->setEnabled(true);
}

// Slot
void MainWindow::disableLoadButton() {
    ui->loadButton->setEnabled(false);
}

// Slot
void MainWindow::enableLoadButton() {
    ui->loadButton->setEnabled(true);
}

// Slot
void MainWindow::validateInputs() {

    // Check if both fields have non-empty values
    bool validWidth = !ui->widthLineEdit->text().isEmpty();
    bool validHeight = !ui->heightLineEdit->text().isEmpty();

    // Enable the button only if both are filled
    ui->setSizeButton->setEnabled(validWidth && validHeight);
}

// Slot
void MainWindow::onSetSizeButtonClicked() {
    int width = ui->widthLineEdit->text().toInt();
    int height = ui->heightLineEdit->text().toInt();

    // Check if both width and height are within the allowed range
    if (width >= 1 && width <= 64 && height >= 1 && height <= 64) {
        ui->statusLabel->setText("✅");
        ui->statusLabel->setStyleSheet("color: green; font-size: 18px;");

        // Enable the createButton since the input is valid
        ui->createButton->setEnabled(true);
    }

    else {
        ui->statusLabel->setText("❌");
        ui->statusLabel->setStyleSheet("color: red; font-size: 18px;");

        // Disable the createButton since the input is invalid
        ui->createButton->setEnabled(false);
    }
}

// Slot - Transitions to the editorwindow UI
void MainWindow::openEditorWindow() {

    int width = ui->widthLineEdit->text().toInt();
    int height = ui->heightLineEdit->text().toInt();

    if (!editorWindow) {

        // Create the editor window
        FrameManager *frameManager = new FrameManager(height, width, this);
        editorWindow = new EditorWindow(frameManager, width, height, nullptr);
        frameManager->setParent(editorWindow);
    }

    // Show the editor window
    editorWindow->show();

    // Close the main window
    this->close();
}

// Slot - Opens file manager for selecting files to load
void MainWindow::loadFile() {

    QString fileName
        = QFileDialog::getOpenFileName(this,
                                       "Open Sprite File",
                                       QDir::homePath(),
                                       "Image Files (*.png *.bmp *.jpg *.jpeg);;All Files (*.*)");

    if (!fileName.isEmpty()) {

        // Update the filePathLabel from the designer
        ui->filePathLabel->setText(fileName);

        // Enable the openButton
        ui->openButton->setEnabled(true);
    }

}

// Slot
void MainWindow::syncHeightToWidth(const QString &text) {
    if (ui->heightLineEdit->text() != text) {
        ui->heightLineEdit->setText(text);
    }
}

// Slot
void MainWindow::syncWidthToHeight(const QString &text) {
    if (ui->widthLineEdit->text() != text) {
        ui->widthLineEdit->setText(text);
    }
}
