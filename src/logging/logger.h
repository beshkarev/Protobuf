#pragma once

#include <chrono>
#include <fstream>
#include <memory>

class FileWorker;

class Logger
{
public:
    friend Logger&
    operator<<( Logger&& logger, const std::string& ss )
    {
        logger.write( ss );
        return logger;
    }

private:
    uint64_t get_timestamp( ) const;

    void write( const std::string& ss );

    static std::unique_ptr< FileWorker > m_worker;
};
