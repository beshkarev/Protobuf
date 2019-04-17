#pragma once

#include <chrono>
#include <fstream>
#include <memory>

class FileWorker;

class Logger
{
public:
    Logger( );

    ~Logger( );

    Logger&
    operator<<( const std::string& str )
    {
        write( str );
        return *this;
    }

private:
    uint64_t get_timestamp( ) const;

    void write( const std::string& ss );

    static std::unique_ptr< FileWorker > m_worker;
};
