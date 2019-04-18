#pragma once

#include <QDir>
#include <QJsonDocument>
#include <QJsonObject>
#include <fstream>
#include <iomanip>
#include "src/logging/logger.h"

namespace ConfigKey
{
constexpr const char* SERVER_HOST_NAME = "server_hostname";
constexpr const char* CLIENT_HOST_NAME = "client_hostname";
constexpr const char* SERVER_PORT = "server_port";
constexpr const char* CLIENT_PORT = "client_port";
constexpr const char* CONFIG_NAME = "ProtobufConfig.json";
}  // namespace ConfigKey

class Configuration
{
public:
    enum class Connectivity
    {
        ONLINE,
        OFFLINE
    };

    Configuration( )
    {
        auto json_doc = read( );
        if ( json_doc.isObject( ) )
        {
            m_config = json_doc.object( );
        }
    }

    uint16_t
    get_server_port( ) const
    {
        auto it = m_config.find( ConfigKey::SERVER_PORT );
        if ( it != m_config.end( ) )
        {
            return static_cast< uint16_t >( it->toInt( ) );
        }

        Logger( ) << "Couldn't find server port.";
        return 0u;
    }

    QString
    get_server_host_name( ) const
    {
        auto it = m_config.find( ConfigKey::SERVER_HOST_NAME );
        if ( it != m_config.end( ) )
        {
            return it->toString( );
        }

        Logger( ) << "Couldn't find server host name.";
        return QString{};
    }

    QString
    get_client_host_name( )
    {
        auto it = m_config.find( ConfigKey::CLIENT_HOST_NAME );
        if ( it != m_config.end( ) )
        {
            return it->toString( );
        }

        Logger( ) << "Couldn't find client host name.";
        return QString{};
    }

    uint16_t
    get_client_port( ) const
    {
        auto it = m_config.find( ConfigKey::CLIENT_PORT );
        if ( it != m_config.end( ) )
        {
            return static_cast< uint16_t >( it->toInt( ) );
        }

        Logger( ) << "Couldn't find client port.";
        return 0u;
    }

    void
    set_connectivity( Connectivity mode )
    {
        m_conectivity_mode = mode;
    }

    Connectivity
    get_connectivity( )
    {
        return m_conectivity_mode;
    }

private:
    QJsonDocument
    read( )
    {
        auto path = QDir::currentPath( ) + "/" + ConfigKey::CONFIG_NAME;
        Logger( ) << "Read config by the path " << path.toStdString( );

        QFile jsonFile( path );
        jsonFile.open( QFile::ReadOnly );
        return QJsonDocument( ).fromJson( jsonFile.readAll( ) );
    }

    QJsonObject m_config;
    Connectivity m_conectivity_mode;
};
