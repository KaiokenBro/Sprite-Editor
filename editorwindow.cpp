#include "editorwindow.h"
#include "ui_editorwindow.h"
#include "previewwindow.h"

#include <QGraphicsScene>
#include <QGraphicsView>

// Constructor
EditorWindow::EditorWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::EditorWindow)
{
    ui->setupUi(this);
}

// Destructor
EditorWindow::~EditorWindow() {
    delete ui;
}

// Slot
void EditorWindow::animateClicked() {
    previewwindow *preview = new previewwindow(this);
    preview->show();
}
