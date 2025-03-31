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
    explicit EditorWindow(SaveLoadManager* saveLoadManager, FrameManager* frameManager, int width, int height, QWidget *parent = nullptr);

    // Destructor
    ~EditorWindow();

    // Setter
    void setSpriteWidth(int width);

    // Setter
    void setSpriteHeight(int height);

    void reinitializeEditor(int newWidth, int newHeight);
    void selectFirstFrame();
    void initializeFromLoadedFile(int width, int height);

private:

    Ui::EditorWindow *ui;
    FrameManager *frameManager;
    SaveLoadManager* saveLoadManager;
    QColor color;
    QImage sprite;
    int zoom = 10;
    int spriteWidth;
    int spriteHeight;
    bool isDrawing = false;
    bool isErasing = false;
    bool isGettingColor = false;
    bool mousePressed = false;
    void updateCanvas();
    void handleDrawingAction(int x, int y);
    int getCurrentFrameIndex();

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
    void getSelectedFrameToRotate();
    void invertColor();
    void onSaveButtonClicked();

signals:

    void addOneFrame();
    void changeRedValue(int value);
    void changeGreenValue(int value);
    void changeBlueValue(int value);
    void changeAlphaValue(int value);
    void deleteFrame(int frameIndex);
    void updatePixelInFrame(int frameIndex, int rowIndex, int columnIndex, int red, int green, int blue, int alpha);
    void getPixels(int frameIndex);
    void selectedFrameToCopy(int frameIndex);
    void selectedFrameToRotate(int frameIndex);

};

#endif // EDITORWINDOW_H
