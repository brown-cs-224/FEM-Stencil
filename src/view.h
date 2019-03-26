#ifndef VIEW_H
#define VIEW_H

#include "simulation.h"
#include "graphics/camera.h"
#include "graphics/Shader.h"

#include <QGLWidget>
#include <QTime>
#include <QTimer>
#include <memory>
#include <unordered_map>

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

    Simulation m_sim;
    Camera m_camera;
    Shader *m_shader;

    std::unordered_map<int, bool> m_keys;

    int m_forward, m_sideways, m_vertical;

    int m_lastX, m_lastY;

    bool m_capture;

private slots:
    void tick();
};

#endif // VIEW_H

