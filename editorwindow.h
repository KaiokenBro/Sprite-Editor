#ifndef EDITORWINDOW_H
#define EDITORWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class EditorWindow; }
QT_END_NAMESPACE

class EditorWindow : public QMainWindow {
    Q_OBJECT

public:

    // Constructor
    explicit EditorWindow(int width, int height, QWidget *parent = nullptr);

    // Destructor
    ~EditorWindow();

private:

    Ui::EditorWindow *ui;
    QImage sprite;

    int zoom = 10;
    int spriteWidth;
    int spriteHeight;

    bool isDrawing = false;
    bool isErasing = false;

    void updateCanvas();

protected:

    // Handles mouse drawing
    virtual bool eventFilter(QObject *watched, QEvent *event) override;

public slots:

    void animateClicked();
    void enableDrawing();
    void enableEraser();
    void redChanged(int value);
    void greenChanged(int value);
    void blueChanged(int value);
    void alphaChanged(int value);
};

#endif // EDITORWINDOW_H
