#include "mainwindow.h"
#include "editorwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QIntValidator>
#include <QMessageBox>
#include <QString>

// Constructor
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

    //////////////////////////////////////////////////
    ////////   UI -> UI CONNECTIONS        //////////
    /////////////////////////////////////////////////

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
            &MainWindow::openEditorWindowNew);

    // When loadButton is clicked, loadFile
    connect(ui->loadButton,
            &QPushButton::clicked,
            this,
            &MainWindow::loadFile);

    // When openButton is clicked, openEditorWindow
    connect(ui->openButton,
            &QPushButton::clicked,
            this,
            &MainWindow::openEditorWindowLoad);

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

    //////////////////////////////////////////////////
    ////////  UI -> SAVELOADMANAGER CONNECTIONS //////
    /////////////////////////////////////////////////

}

// Destructor
MainWindow::~MainWindow() {
    delete ui;
}

// Slot - UI
void MainWindow::hideWelcomeLabel() {
    ui->welcomeLabel->setVisible(false);
}

// Slot - UI
void MainWindow::displayCreateFileBox() {
    ui->createFileBox->setVisible(true);
}

// Slot - UI
void MainWindow::hideCreateFileBox() {
    ui->createFileBox->setVisible(false);
}

// Slot - UI
void MainWindow::disableNewButton() {
    ui->newButton->setEnabled(false);
}

// Slot - UI
void MainWindow::enableNewButton() {
    ui->newButton->setEnabled(true);
}

// Slot - UI
void MainWindow::disableLoadButton() {
    ui->loadButton->setEnabled(false);
}

// Slot - UI
void MainWindow::enableLoadButton() {
    ui->loadButton->setEnabled(true);
}

// Slot - UI
void MainWindow::validateInputs() {

    // Check if both fields have non-empty values
    bool validWidth = !ui->widthLineEdit->text().isEmpty();
    bool validHeight = !ui->heightLineEdit->text().isEmpty();

    // Enable the button only if both are filled
    ui->setSizeButton->setEnabled(validWidth && validHeight);
}

// Slot - Syncs Width and Height value box
void MainWindow::syncHeightToWidth(const QString &text) {
    if (ui->heightLineEdit->text() != text) {
        ui->heightLineEdit->setText(text);
    }
}

// Slot - Syncs Width and Height value box
void MainWindow::syncWidthToHeight(const QString &text) {
    if (ui->widthLineEdit->text() != text) {
        ui->widthLineEdit->setText(text);
    }
}

// Slot - Validates size of sprite
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
// Used for new file
void MainWindow::openEditorWindowNew() {

    int width = ui->widthLineEdit->text().toInt();
    int height = ui->heightLineEdit->text().toInt();

    editorWindow->reinitializeEditor(width, height);

    // Show the editor window
    editorWindow->show();

    // Close the main window
    this->close();
}

// Slot - Transitions to the editorwindow UI
// Used for loading file
void MainWindow::openEditorWindowLoad() {

    // Get file path from the label
    QString filePath = ui->filePathLabel->text();

    // Attempt to load from the file
    bool success = saveLoadManager->loadFromFile(*frameManager, filePath);

    // If loading failed, show an error and stop
    if (!success) {
        QMessageBox::warning(this, "Error", "Failed to load the file.");
        return;
    }

    // Use the first frame's size to reinitialize the editor
    int newWidth = frameManager->width;
    int newHeight = frameManager->height;

    // Use new method for initializing loaded data
    editorWindow->initializeFromLoadedFile(newWidth, newHeight);

    // Show the editor window
    editorWindow->show();

    // Close the main window
    this->close();
}

// Slot - Opens file manager for selecting .ssp files to load
void MainWindow::loadFile() {

    // Open a file dialog to select only .ssp files
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

        // Update the file path label
        ui->filePathLabel->setText(fileName);

        // Enable the "Open" button now that a valid file is selected
        ui->openButton->setEnabled(true);
    }

}
