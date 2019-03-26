#include "view.h"

#include "viewformat.h"

#include <QApplication>
#include <QKeyEvent>

#include <iostream>

using namespace std;

View::View(QWidget *parent) : QGLWidget(ViewFormat(), parent),
    m_window(parent->parentWidget()),
    m_time(), m_timer(),
    m_forward(), m_sideways(), m_vertical(),
    m_lastX(), m_lastY(),
    m_capture(false)
{
    // View needs all mouse move events, not just mouse drag events
    setMouseTracking(true);

    // Hide the cursor since this is a fullscreen app
    QApplication::setOverrideCursor(Qt::ArrowCursor);

    // View needs keyboard focus
    setFocusPolicy(Qt::StrongFocus);

    // The game loop is implemented using a timer
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(tick()));
}

View::~View()
{
    delete m_shader;
}

void View::initializeGL()
{
    glewExperimental = GL_TRUE;
    if(glewInit() != GLEW_OK) {
        std::cerr << "glew initialization failed" << std::endl;
    }
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glClearColor(0.1f, 0.1f, 0.1f, 1);


    m_shader = new Shader(":/shaders/shader.vert", ":/shaders/shader.frag");
    m_sim.init();

    // Should be setting euler angles all together in orientLook
    m_camera.orientLook(glm::vec4(0, 2, -7, 1), glm::vec4(0, -1.5f, 5, 0), glm::vec4(0, 1, 0, 0));
    m_camera.rotate(180, 0);

    m_time.start();
    m_timer.start(1000 / 60);
}

void View::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    m_shader->bind();
    glm::mat4x4 model = glm::mat4x4(1.f);
    glm::mat4x4 mvp = m_camera.getProjectionMatrix() * m_camera.getViewMatrix();
    m_shader->setUniform("m", model);
    m_shader->setUniform("vp", mvp);
    m_sim.draw(m_shader);
    m_shader->unbind();
}

void View::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
    m_camera.setAspectRatio(static_cast<float>(w) / h);
}

void View::mousePressEvent(QMouseEvent *event)
{
    m_capture = true;
    m_lastX = event->x();
    m_lastY = event->y();
}

void View::mouseMoveEvent(QMouseEvent *event)
{
    int deltaX = event->x() - m_lastX;
    int deltaY = event->y() - m_lastY;

    if(m_capture) {
        if(deltaX != 0 || deltaY != 0) {
            m_camera.rotate(deltaX, -deltaY);
        }
    }
    m_lastX = event->x();
    m_lastY = event->y();
}

void View::mouseReleaseEvent(QMouseEvent *)
{
    m_capture = false;
}

void View::wheelEvent(QWheelEvent *)
{

}

void View::keyPressEvent(QKeyEvent *event)
{
    // Don't remove this -- helper code for key repeat events
    if(event->isAutoRepeat()) {
        keyRepeatEvent(event);
        return;
    }

    // Feel free to remove this
    if (event->key() == Qt::Key_Escape) QApplication::quit();
    else if(event->key() == Qt::Key_T) m_sim.toggleWire();

    m_keys[event->key()] = true;
}

void View::keyRepeatEvent(QKeyEvent *)
{
}

void View::keyReleaseEvent(QKeyEvent *event)
{
    // Don't remove this -- helper code for key repeat events
    if(event->isAutoRepeat()) {
        return;
    }

    m_keys[event->key()] = false;
}

void View::tick()
{
    float seconds = m_time.restart() * 0.001f;
    m_sim.update(seconds);

    float moveSpeed = 0.1f;

    if(m_keys[Qt::Key_W]) {
        m_camera.translate(m_camera.getLook() * moveSpeed);
    }
    if(m_keys[Qt::Key_S]) {
        m_camera.translate(-m_camera.getLook() * moveSpeed);
    }

    if(m_keys[Qt::Key_A]) {
        m_camera.translate(-m_camera.getRight() * moveSpeed);
    }
    if(m_keys[Qt::Key_D]) {
        m_camera.translate(m_camera.getRight() * moveSpeed);
    }

    if(m_keys[Qt::Key_Q]) {
        m_camera.translate(-m_camera.getUp() * moveSpeed);
    }
    if(m_keys[Qt::Key_E]) {
        m_camera.translate(m_camera.getUp() * moveSpeed);
    }

    // Flag this view for repainting (Qt will call paintGL() soon after)
    update();
}
