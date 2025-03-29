#ifndef EDITORWINDOW_H
#define EDITORWINDOW_H

#include <QMainWindow>
#include "framemanager.h"
#include "saveloadmanager.h"

QT_BEGIN_NAMESPACE
namespace Ui { class EditorWindow; }
QT_END_NAMESPACE

class EditorWindow : public QMainWindow {
    Q_OBJECT

public:

    // Constructor
    explicit EditorWindow(FrameManager *frameManager, int width, int height, QWidget *parent = nullptr);

    // Destructor
    ~EditorWindow();

private:

    FrameManager *frameManager;
    SaveLoadManager* saveLoadManager;

    Ui::EditorWindow *ui;
    QColor color;
    QImage sprite;

    int zoom = 10;
    int spriteWidth;
    int spriteHeight;

    bool isDrawing = false;
    bool isErasing = false;
    bool isGettingColor = false;

    void updateCanvas();

protected:

    // Handles mouse drawing
    virtual bool eventFilter(QObject *watched, QEvent *event) override;

public slots:

    void animateClicked();
    void enableDrawing();
    void enableEraser();
    void enableCopyColor();
    void redChanged(int value);
    void greenChanged(int value);
    void blueChanged(int value);
    void alphaChanged(int value);
    void addFrameToStack(int frameNumber);
    void deleteFrameFromStack();
    void switchCanvas(std::vector<std::vector<QColor>> pixels);
    void getSelectedFrame();
    void getSelectedFrameToCopy();

    void onSaveButtonClicked();

signals:

    void deleteFrame(int frameIndex);
    void updatePixelInFrame(int frameIndex, int rowIndex, int columnIndex, int red, int green, int blue, int alpha);
    void getPixels(int frameIndex);
    void selectedFrameToCopy(int frameIndex);

};

#endif // EDITORWINDOW_H
