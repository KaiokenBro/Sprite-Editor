#include "editorwindow.h"
#include "ui_editorwindow.h"

#include <QGraphicsScene>
#include <QGraphicsView>

EditorWindow::EditorWindow(int width, int height, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::EditorWindow)
{
    ui->setupUi(this);

    // Create the scene and set it to the QGraphicsView
    QGraphicsScene *scene = new QGraphicsScene(0, 0, width, height, this);
    ui->graphicsView->setScene(scene);

    // Resize the QGraphicsView based on user-defined size
    ui->graphicsView->setFixedSize(width + 2, height + 2); // +2 for border
    scene->setSceneRect(0, 0, width, height);
}

EditorWindow::~EditorWindow()
{
    delete ui;
}
