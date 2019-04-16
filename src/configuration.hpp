#pragma once

#include <QDir>
#include <fstream>
#include <iomanip>
#include <nlohmann/json.hpp>
#include "src/logging/logger.h"

namespace ConfigKey
{
constexpr const char* HOST_NAME = "hostname";
constexpr const char* SERVER_PORT = "server_port";
constexpr const char* CLIENT_PORT = "client_port";
constexpr const char* CONFIG_NAME = "ProtobufConfig.json";
}  // namespace ConfigKey

class Configuration
{
public:
    Configuration( )
    {
        std::ifstream input(
                ( QDir::currentPath( ) + "/" + ConfigKey::CONFIG_NAME ).toStdString( ) );
        input >> m_config;
        input.close( );
    }

    uint32_t
    get_server_port( ) const
    {
        auto it = m_config.find( ConfigKey::SERVER_PORT );
        if ( it != m_config.end( ) )
        {
            return it->get< uint32_t >( );
        }

        Logger( ) << "Couldn't find server port.";
        return 0u;
    }

    QString
    get_host_name( ) const
    {
        auto it = m_config.find( ConfigKey::HOST_NAME );
        if ( it != m_config.end( ) )
        {
            return QString::fromStdString( it->get< std::string >( ) );
        }

        Logger( ) << "Couldn't find host name.";
        return QString{};
    }

    uint32_t
    get_client_port( ) const
    {
        auto it = m_config.find( ConfigKey::CLIENT_PORT );
        if ( it != m_config.end( ) )
        {
            return it->get< std::uint32_t >( );
        }

        Logger( ) << "Couldn't find client port.";
        return 0u;
    }

private:
    nlohmann::json m_config;
};
