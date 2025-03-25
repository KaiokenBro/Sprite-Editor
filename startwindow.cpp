#include "startwindow.h"
#include "ui_startwindow.h"

#include <QIntValidator>

// Pattern
// connect(WhatIsEmittingTheSignal,
//        WhichSignalIsIt,
//        WhatHasASlot,
//        WhichSlotIsIt);

// Constructor
StartWindow::StartWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::StartWindow)
{
    ui->setupUi(this);

    // Create a validator to restrict input to integers between 16 and 512
    QIntValidator *validator = new QIntValidator(16, 512, this);

    // Apply validator to width and height input
    ui->widthLineEdit->setValidator(validator);
    ui->heightLineEdit->setValidator(validator);

    // Initially hide createFileBox
    ui->createFileBox->setVisible(false);

    // Initially disable setSizeButton
    ui->setSizeButton->setEnabled(false);

    // Initially disable the createButton
    ui->createButton->setEnabled(false);

    // When newButton is clicked, hide welcomeLabel
    connect(ui->newButton, &QPushButton::clicked, this, &StartWindow::hideWelcomeLabel);

    // When loadButton is clicked, hide welcomeLabel
    connect(ui->loadButton, &QPushButton::clicked, this, &StartWindow::hideWelcomeLabel);

    // When newButton is clicked, display createFileBox
    connect(ui->newButton, &QPushButton::clicked, this, &StartWindow::displayCreateFileBox);

    // When loadButton is clicked, hide createFileBox
    connect(ui->loadButton, &QPushButton::clicked, this, &StartWindow::hideCreateFileBox);

    // When widthLineEdit is changed, check to enable setSizeButton
    connect(ui->widthLineEdit, &QLineEdit::textChanged, this, &StartWindow::validateInputs);

    // When heightLineEdit is changed, check to enable setSizeButton
    connect(ui->heightLineEdit, &QLineEdit::textChanged, this, &StartWindow::validateInputs);

    // When setSizeButton is clicked, update statusLabel
    connect(ui->setSizeButton, &QPushButton::clicked, this, &StartWindow::onSetSizeButtonClicked);

    // When newButton is clicked, disable newButton
    connect(ui->newButton, &QPushButton::clicked, this, &StartWindow::disableNewButton);

    // When newButton is clicked, enable loadButton
    connect(ui->newButton, &QPushButton::clicked, this, &StartWindow::enableLoadButton);

    // When loadButton is clicked, disable loadButton
    connect(ui->loadButton, &QPushButton::clicked, this, &StartWindow::disableLoadButton);

    // When loadButton is clicked, enable newButton
    connect(ui->loadButton, &QPushButton::clicked, this, &StartWindow::enableNewButton);
}

// Destructor
StartWindow::~StartWindow()
{
    delete ui;
}

// Slot
void StartWindow::hideWelcomeLabel()
{
    ui->welcomeLabel->setVisible(false);
}

// Slot
void StartWindow::displayCreateFileBox()
{
    ui->createFileBox->setVisible(true);
}

// Slot
void StartWindow::hideCreateFileBox()
{
    ui->createFileBox->setVisible(false);
}

// Slot
void StartWindow::disableNewButton()
{
    ui->newButton->setEnabled(false);
}

// Slot
void StartWindow::enableNewButton()
{
    ui->newButton->setEnabled(true);
}

// Slot
void StartWindow::disableLoadButton()
{
    ui->loadButton->setEnabled(false);
}

// Slot
void StartWindow::enableLoadButton()
{
    ui->loadButton->setEnabled(true);
}

// Slot
void StartWindow::validateInputs()
{
    // Check if both fields have non-empty values
    bool validWidth = !ui->widthLineEdit->text().isEmpty();
    bool validHeight = !ui->heightLineEdit->text().isEmpty();

    // Enable the button only if both are filled
    ui->setSizeButton->setEnabled(validWidth && validHeight);
}

// Slot
void StartWindow::onSetSizeButtonClicked()
{
    int width = ui->widthLineEdit->text().toInt();
    int height = ui->heightLineEdit->text().toInt();

    // Check if both width and height are within the allowed range
    if (width >= 16 && width <= 512 && height >= 16 && height <= 512) {
        ui->statusLabel->setText("✅");
        ui->statusLabel->setStyleSheet("color: green; font-size: 18px;");

        // Enable the createButton since the input is valid
        ui->createButton->setEnabled(true);
    } else {
        ui->statusLabel->setText("❌");
        ui->statusLabel->setStyleSheet("color: red; font-size: 18px;");

        // Disable the createButton since the input is invalid
        ui->createButton->setEnabled(false);
    }
}
