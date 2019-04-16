#include "file_worker.hpp"
#include "logger.h"

std::unique_ptr< FileWorker > Logger::m_worker( new FileWorker );

uint64_t
Logger::get_timestamp( ) const
{
    using namespace std::chrono;
    return duration_cast< milliseconds >( system_clock::now( ).time_since_epoch( ) ).count( );
}

void
Logger::write( const std::string& ss )
{
    auto& named_mutex = m_worker->get_mutex( );

    named_mutex.lock( );
    m_worker->get_ofstream( ) << "[" << std::to_string( get_timestamp( ) ) << "] " << ss
                              << std::endl;
    named_mutex.unlock( );
}
