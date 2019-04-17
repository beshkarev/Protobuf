#pragma once

#include <QThread>
#include <QTimer>
#include <boost/interprocess/sync/named_mutex.hpp>
#include <fstream>
#include "logger.h"

class FileWorker : public QObject
{
    Q_OBJECT

    FileWorker( )
        : named_mutex( boost::interprocess::open_or_create_t{}, "protobuf_logger_mtx" )
        , file( "protobuf_log.txt", std::ios::app )
    {
        QObject::connect( &thread, &QThread::started, this, &FileWorker::start_timer );

        moveToThread( &thread );
        timer.moveToThread( &thread );
        thread.start( );
    }

public:
    std::ofstream&
    get_ofstream( )
    {
        return file;
    }

    boost::interprocess::named_mutex&
    get_mutex( )
    {
        return named_mutex;
    }

private slots:
    void
    flush( )
    {
        named_mutex.lock( );

        file.flush( );
        file.clear( );

        named_mutex.unlock( );
    }

    void
    start_timer( )
    {
        QObject::connect(
                &timer, &QTimer::timeout, this, &FileWorker::flush, Qt::QueuedConnection );

        timer.start( 5000 );
    }

private:
    boost::interprocess::named_mutex named_mutex;
    std::ofstream file;
    QTimer timer;
    QThread thread;

    friend Logger;
};
