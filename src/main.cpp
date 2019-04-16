#include <QApplication>
#include "Logger.hpp"
#include "MemoryWorker.h"
#include "mainwindow.h"
#include "src/ipc/tcp_server.h"

int
main( int argc, char* argv[] )
{
    QApplication a( argc, argv );
    MainWindow w;
    w.init( );
    w.show( );

    Logger( ) << "Starting application.";

    return a.exec( );
}
