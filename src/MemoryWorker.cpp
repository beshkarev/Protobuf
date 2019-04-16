#include <fcntl.h>
#include <sys/ipc.h>
#include <sys/mman.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>
#include <cstring>
#include <sstream>
#include "Logger.hpp"
#include "MemoryWorker.h"

MemoryWorker::MemoryWorker( const std::string& name, uint32_t size )
    : m_usage_counter{std::make_unique< Counter >( "protobuf_counter" )}
    , m_name{name}
    , m_size{size}
{
    std::stringstream ss;
    ss << "Creating shared memory with name " << name << ".";
    Logger( ) << ss.str( );

    m_descriptor = shm_open( m_name.c_str( ), O_CREAT | O_RDWR, 0777 );
    if ( m_descriptor != -1 )
    {
        Logger( ) << "Shared memory created.";
    }
    else
    {
        Logger( ) << "Memory created with error.";
    }
}

MemoryWorker::~MemoryWorker( )
{
    if ( m_usage_counter->count( ) == 1 )
    {
        Logger( ) << "Unlinking shared memory.";

        if ( shm_unlink( m_name.c_str( ) ) == 0 )
        {
            Logger( ) << "Shared memory unlinked.";
        }
        else
        {
            Logger( ) << "Error occurence during memory unlinking.";
        }
    }
    else
    {
        std::stringstream ss;
        ss << "Count of memory usage is not equal to one, can't be unlinked."
           << std::to_string( m_usage_counter->count( ) );
        Logger( ) << ss.str( );
    }
}

MemoryWorker::Counter::Counter( const std::string& memory_name )
    : name{memory_name}
    , counter{0u}
    , descriptor{-1}
{
    ++counter;
    std::stringstream ss;
    ss << "Creating shared memory for counter with name " << name << ".";
    Logger( ) << ss.str( );

    descriptor = shm_open( name.c_str( ), O_CREAT | O_RDWR, 0777 );
    if ( descriptor != -1 )
    {
        Logger( ) << "Memory created.";

        auto counter_str{std::to_string( counter )};
        ftruncate( descriptor, counter_str.size( ) + 1 );
        char* addr = (char*)mmap(
                NULL, counter_str.size( ) + 1, PROT_WRITE | PROT_READ, MAP_SHARED, descriptor, 0 );
        memcpy( addr, counter_str.c_str( ), counter_str.size( ) );

        Logger( ) << "Counter wrote.";
    }
    else
    {
        Logger( ) << "Couldn't create memory.";
    }
}

MemoryWorker::Counter::~Counter( )
{
    --counter;
    if ( counter == 0 )
    {
        shm_unlink( name.c_str( ) );
    }
}

uint32_t
MemoryWorker::Counter::count( ) const
{
    return counter;
}
