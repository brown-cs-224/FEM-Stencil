#ifndef VIEW_H
#define VIEW_H

#include "engine/util/CommonIncludes.h"

#include <QGLWidget>
#include <QTime>
#include <QTimer>
#include <memory>

class Graphics;
class Camera;

/**
 * This is similar to your "CS1971FrontEnd" class. Here you will receive all of the input events
 * to forward to your game.
 *
 * @brief The View class
 */
class View : public QGLWidget
{
    Q_OBJECT

public:
    View(QWidget *parent);
    ~View();

private:
    static const int FRAMES_TO_AVERAGE = 30;

private:
    void initializeGL();
    void paintGL();
    void resizeGL(int w, int h);

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void keyRepeatEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

private:
    QWidget *m_window;
    QTime m_time;
    QTimer m_timer;
    bool m_captureMouse;

    float m_fps;
    int m_frameIndex;
    float m_frameTimes[FRAMES_TO_AVERAGE];

    Graphics* m_graphics;

    // TODO (Warmup 1): You might want to remove this after completing the lab
    std::shared_ptr<Camera> m_camera;

    // TODO (Warmup 1): Add an application variable to the View class

private slots:
    void tick();
};

#endif // VIEW_H

