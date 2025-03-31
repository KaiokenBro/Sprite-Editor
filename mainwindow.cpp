/**
 * @file mainwindow.cpp
 * @author Harrison Doppelt, Victor Valdez Landa
 *
 * @brief Implementation of the MainWindow class — the entry UI for creating or loading sprites.
 *
 * Provides logic for input validation, file loading, and transition to the EditorWindow.
 *
 * @date 03/31/2025
 *
 * Checked by Richard Nghiem
 */

#include "mainwindow.h"
#include "editorwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QIntValidator>
#include <QMessageBox>
#include <QString>

MainWindow::MainWindow(SaveLoadManager* saveLoadManager, FrameManager* frameManager, EditorWindow* editorWindow, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->editorWindow = editorWindow;
    this->frameManager = frameManager;
    this->saveLoadManager = saveLoadManager;

    // Create a validator to restrict input to integers between 1 and 64
    QIntValidator *validator = new QIntValidator(1, 64, this);
    ui->widthLineEdit->setValidator(validator);
    ui->heightLineEdit->setValidator(validator);

    ui->setSizeButton->setEnabled(false);
    ui->createButton->setEnabled(false);
    ui->openButton->setEnabled(false);

    ui->createFileBox->setStyleSheet(
        "QGroupBox {"
        "  border: 2px solid black;"
        "}"
        );

    // Connect width input to input validation
    connect(ui->widthLineEdit,
            &QLineEdit::textChanged,
            this,
            &MainWindow::validateInputs);

    // Connect height input to input validation
    connect(ui->heightLineEdit,
            &QLineEdit::textChanged,
            this,
            &MainWindow::validateInputs);

    // Connect "Set Size" button to validation and visual feedback
    connect(ui->setSizeButton,
            &QPushButton::clicked,
            this,
            &MainWindow::onSetSizeButtonClicked);

    // Connect "Create" button to opening a new sprite in EditorWindow
    connect(ui->createButton,
            &QPushButton::clicked,
            this,
            &MainWindow::openEditorWindowNew);

    // Connect "Load File" button to file dialog for loading .ssp file
    connect(ui->loadButton,
            &QPushButton::clicked,
            this,
            &MainWindow::loadFile);

    // Connect "Open" button to load the selected file and enter editor
    connect(ui->openButton,
            &QPushButton::clicked,
            this,
            &MainWindow::openEditorWindowLoad);

    // Keep height synchronized with width input
    connect(ui->widthLineEdit,
            &QLineEdit::textChanged,
            this,
            &MainWindow::syncHeightToWidth);

    // Keep width synchronized with height input
    connect(ui->heightLineEdit,
            &QLineEdit::textChanged,
            this,
            &MainWindow::syncWidthToHeight);

    // Invalidate confirmation when width is changed
    connect(ui->widthLineEdit,
            &QLineEdit::textChanged,
            this,
            &MainWindow::invalidateSizeConfirmation);

    // Invalidate confirmation when height is changed
    connect(ui->heightLineEdit,
            &QLineEdit::textChanged,
            this,
            &MainWindow::invalidateSizeConfirmation);

}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::disableLoadButton() {
    ui->loadButton->setEnabled(false);
}

void MainWindow::enableLoadButton() {
    ui->loadButton->setEnabled(true);
}

void MainWindow::invalidateSizeConfirmation() {
    ui->statusLabel->setText("❌");
    ui->statusLabel->setStyleSheet("color: red; font-size: 18px;");
    ui->createButton->setEnabled(false);
}

void MainWindow::validateInputs() {

    // Check if both fields have non-empty values
    bool validWidth = !ui->widthLineEdit->text().isEmpty();
    bool validHeight = !ui->heightLineEdit->text().isEmpty();

    // Enable the button only if both are filled
    ui->setSizeButton->setEnabled(validWidth && validHeight);
}

void MainWindow::syncHeightToWidth(const QString &text) {
    if (ui->heightLineEdit->text() != text) {
        ui->heightLineEdit->setText(text);
    }
}

void MainWindow::syncWidthToHeight(const QString &text) {
    if (ui->widthLineEdit->text() != text) {
        ui->widthLineEdit->setText(text);
    }
}

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

void MainWindow::openEditorWindowNew() {
    int width = ui->widthLineEdit->text().toInt();
    int height = ui->heightLineEdit->text().toInt();
    editorWindow->reinitializeEditor(width, height);
    editorWindow->show();
    this->close();
}

void MainWindow::openEditorWindowLoad() {
    QString filePath = ui->filePathLabel->text();
    bool success = saveLoadManager->loadFromFile(*frameManager, filePath);

    // If loading failed, show an error and stop
    if (!success) {
        QMessageBox::warning(this, "Error", "Failed to load the file.");
        return;
    }

    int newWidth = frameManager->width;
    int newHeight = frameManager->height;
    editorWindow->initializeFromLoadedFile(newWidth, newHeight);
    editorWindow->show();
    this->close();
}

void MainWindow::loadFile() {

    QString fileName = QFileDialog::getOpenFileName(
        this,                               // Parent widget
        "Open Sprite File",                 // Dialog title
        QDir::homePath(),                   // Default directory
        "Sprite Save Files (*.ssp)"         // File filter
        );

    // If the user selects a file
    if (!fileName.isEmpty()) {

        // Ensure the file has the .ssp extension
        if (!fileName.endsWith(".ssp", Qt::CaseInsensitive)) {
            QMessageBox::warning(this, "Invalid File", "Please select a valid .ssp file.");
            return;  // Reject invalid files
        }

        ui->filePathLabel->setText(fileName);
        ui->openButton->setEnabled(true);
    }
}
