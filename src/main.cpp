#include <QApplication>
#include "mainwindow.h"
#include <cstdlib>
#include <ctime>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    srand (static_cast <unsigned> (time(0)));
    // We cannot use w.showFullscreen() here because on Linux that creates the
    // window behind all other windows, so we have to set it to fullscreen after
    // it has been shown. 
    w.show();
    //w.setWindowState(w.windowState() | Qt::WindowFullScreen); // Comment out this line to have a windowed 800x600 game on startup.

    return a.exec();
}

