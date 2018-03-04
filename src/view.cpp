#include "view.h"

#include "viewformat.h"
#include "engine/graphics/GraphicsDebug.h"
#include "engine/graphics/Graphics.h"
#include "engine/graphics/Camera.h"
#include "engine/graphics/Material.h"

#include <QApplication>
#include <QKeyEvent>

using namespace std;
using namespace glm;

View::View(QWidget *parent) : QGLWidget(ViewFormat(), parent),
    m_window(parent->parentWidget()),
    m_time(), m_timer(),
    m_captureMouse(true),
    m_fps(0), m_frameIndex(0),
    m_graphics(nullptr),
    m_camera(nullptr)
{
    /** SUPPORT CODE START **/

    // View needs all mouse move events, not just mouse drag events
    setMouseTracking(true);

    // Hide the cursor since this is a fullscreen app
    if(m_captureMouse) {
        QApplication::setOverrideCursor(Qt::BlankCursor);
    }
    else {
        QApplication::setOverrideCursor(Qt::ArrowCursor);
    }

    // View needs keyboard focus
    setFocusPolicy(Qt::StrongFocus);

    // The game loop is implemented using a timer
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(tick()));

    // Initialize frame times for last FRAMES_TO_AVERAGE frames
    for (int i = 0; i < FRAMES_TO_AVERAGE; i++) {
        m_frameTimes[i] = 0;
    }

    m_frameIndex = 0;

    /** SUPPORT CODE END **/
}

View::~View()
{
}

void View::initializeGL()
{
    /** SUPPORT CODE START **/

    // Initialize graphics object
    m_graphics = Graphics::getGlobalInstance();

    // Enable depth testing, so that objects are occluded based on depth instead of drawing order.
    m_graphics->enableDepthTest();

    // Enable back-face culling, meaning only the front side of every face is rendered.
    // Also specify that the front face is represented by vertices in counterclockwise order (this is
    // the default).
    m_graphics->enableBackfaceCulling();

    // Enable alpha blending, so that materials with an alpha value < 1 are not totally opaque.
    m_graphics->enableBlendTest();

    // Disable stencil test for now (students may change this for final projects)
    m_graphics->disableStencilTest();

    // Start a timer that will try to get 60 frames per second (the actual
    // frame rate depends on the operating system and other running programs)
    m_time.start();
    m_timer.start(1000 / 60);

    // Center the mouse, which is explained more in mouseMoveEvent() below.
    // This needs to be done here because the mouse may be initially outside
    // the fullscreen window and will not automatically receive mouse move
    // events. This occurs if there are two monitors and the mouse is on the
    // secondary monitor.
    QCursor::setPos(mapToGlobal(QPoint(width() / 2, height() / 2)));

    /** SUPPORT CODE END **/

    // TODO (Lab 1): Initialize camera

    // TODO (Lab 1): Initialize material

    // TODO (Warmup 1): Initialize application
}

void View::paintGL()
{
    /** SUPPORT CODE START **/

    m_graphics->setClearColor(glm::vec3(0, 0, 0));
    m_graphics->clearScreen(Graphics::CLEAR_FLAG::ALL);
    m_graphics->clearShader();
    m_graphics->setDefaultMaterial();

    /** SUPPORT CODE END **/


    // TODO (Lab 1): Call your game rendering code here

    // TODO (Warmup 1): Call your game rendering code here

    /** SUPPORT CODE START **/

#if GRAPHICS_DEBUG_LEVEL > 0
    m_graphics->printDebug();
    m_graphics->printShaderDebug();
    m_graphics->printFBODebug();
#endif

    /** SUPPORT CODE END **/
}

void View::resizeGL(int w, int h)
{
    /** SUPPORT CODE START **/

    m_graphics->setViewport(glm::vec2(0, 0), glm::vec2(w, h));

    /** SUPPORT CODE END **/

    // TODO (Lab 1): Resize the camera

    // TODO (Warmup 1): Resize the application
}

void View::mousePressEvent(QMouseEvent *event)
{
    // TODO (Warmup 1): Handle mouse press events
}

void View::mouseMoveEvent(QMouseEvent *event)
{
    /** SUPPORT CODE START **/

    // This starter code implements mouse capture, which gives the change in
    // mouse position since the last mouse movement. The mouse needs to be
    // recentered after every movement because it might otherwise run into
    // the edge of the screen, which would stop the user from moving further
    // in that direction. Note that it is important to check that deltaX and
    // deltaY are not zero before recentering the mouse, otherwise there will
    // be an infinite loop of mouse move events.
    int deltaX = event->x() - width() / 2;
    int deltaY = event->y() - height() / 2;

    if(m_captureMouse) {

        if (deltaX == 0 && deltaY == 0) {
            return;
        }

        QCursor::setPos(mapToGlobal(QPoint(width() / 2, height() / 2)));
    }

    /** SUPPORT CODE END **/

    // TODO (Lab 1): Handle mouse movements here

    // TODO (Warmup 1): Handle mouse movements here
}

void View::mouseReleaseEvent(QMouseEvent *event)
{
    // TODO (Warmup 1): Handle mouse release here
}

void View::wheelEvent(QWheelEvent *event)
{
    // TODO (Warmup 1): Handle mouse wheel events here
}

void View::keyPressEvent(QKeyEvent *event)
{
    /** SUPPORT CODE START **/

    // Don't remove this -- helper code for key repeat events
    if(event->isAutoRepeat()) {
        keyRepeatEvent(event);
        return;
    }

    // Feel free to remove this
    if (event->key() == Qt::Key_Escape) QApplication::quit();

    /** SUPPORT CODE END **/

    // TODO (Lab 1): Handle keyboard presses here

    // TODO (Warmup 1): Handle keyboard presses here
}

void View::keyRepeatEvent(QKeyEvent *event)
{
    // TODO (Warmup 1): Handle key repeats (happens when holding down keys)
}

void View::keyReleaseEvent(QKeyEvent *event)
{
    /** SUPPORT CODE START **/

    // Don't remove this -- helper code for key repeat events
    if(event->isAutoRepeat()) {
        return;
    }

    /** SUPPORT CODE END **/

    // TODO (Warmup 1): Handle key releases
}

void View::tick()
{
    /** SUPPORT CODE START **/

    // Get the number of seconds since the last tick (variable update rate)
    float seconds = m_time.restart() * 0.001f;

    m_frameTimes[m_frameIndex] = seconds;
    m_frameIndex = (m_frameIndex + 1) % FRAMES_TO_AVERAGE;
    m_fps = 0;
    for (int i = 0; i < FRAMES_TO_AVERAGE; i++) {
        m_fps += m_frameTimes[i];
    }
    m_fps /= FRAMES_TO_AVERAGE;
    m_fps = 1.f / m_fps;

    // Display fps
    QString title = "CS195U Engine";

    m_window->setWindowTitle(title + ", FPS: " + QString::number(m_fps, 'f', 3));

    /** SUPPORT CODE END **/


    // TODO (Warmup 1): Implement the game update here


    /** SUPPORT CODE START **/

    // Flag this view for repainting (Qt will call paintGL() soon after)
    update();

    /** SUPPORT CODE END **/
}
