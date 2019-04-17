#include "logger.h"
#include "file_worker.hpp"

std::unique_ptr< FileWorker > Logger::m_worker( new FileWorker );

Logger::Logger( )
{
    m_worker->get_mutex( ).lock( );
    write( "[" + std::to_string( get_timestamp( ) ) + "] " );
}

Logger::~Logger( )
{
    write( "\n" );
    m_worker->get_mutex( ).unlock( );
}

uint64_t
Logger::get_timestamp( ) const
{
    using namespace std::chrono;
    return duration_cast< milliseconds >( system_clock::now( ).time_since_epoch( ) ).count( );
}

void
Logger::write( const std::string& log )
{
    m_worker->get_ofstream( ) << log;
}
