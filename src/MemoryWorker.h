#pragma once

#include <memory>
#include <string>

class MemoryWorker
{
public:
    MemoryWorker( const std::string& name, uint32_t size );
    ~MemoryWorker( );

private:
    struct Counter
    {
        Counter( const std::string& memory_name );
        ~Counter( );
        uint32_t count( ) const;

    private:
        std::string name;
        uint32_t counter;
        int32_t descriptor;
    };

    std::unique_ptr< Counter > m_usage_counter;
    std::string m_name;
    uint32_t m_size;
    int32_t m_descriptor;
};
