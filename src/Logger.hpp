#pragma once

#include <chrono>
#include <fstream>
class Logger
{
public:
    friend Logger&
    operator<<( Logger&& logger, const std::string& ss )
    {
        std::ofstream of( "protobuf_log.txt", std::ios::app );
        of << "[" << std::to_string( get_timestamp( ) ) << "] " << ss << std::endl;
        return logger;
    }

private:
    static uint64_t
    get_timestamp( )
    {
        using namespace std::chrono;

        return duration_cast< milliseconds >( system_clock::now( ).time_since_epoch( ) ).count( );
    }
};
