#include "previewwindow.h"
#include "ui_previewwindow.h"

previewwindow::previewwindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::previewwindow)
{
    ui->setupUi(this);
}

previewwindow::~previewwindow()
{
    delete ui;
}

void previewwindow::animation()
{
    bool animateBool = ui->animateButton->isChecked();
    int fps = ui->fpsSlider->value();
    animate(animateBool, fps);

}

void previewwindow::animate(bool animate, int fps)
{
    int frame = 0;
    while(animate){
        animate = ui->animateButton->isChecked();
        fps = ui->fpsSlider->value();
        std::cout << animate << std::endl;
        std::cout << fps << std::endl;
        QTimer::singleShot(1000/fps, this, [this, frame] () {showFrame(frame); });
        animate = false;
    }
}
void previewwindow::showFrame(int frame)
{
    std::cout << "showFrame" << std::endl;
}
