#pragma once

#include <QAbstractSocket>
#include <functional>

class QSslError;
class SocketImpl;
class MessageDispatcher;
class QTcpSocket;

namespace proto
{
class Message;
}

using SocketErrorHandler = std::function< bool( const QSslError& error ) >;

class Socket : public QObject
{
    Q_OBJECT

public:
    Socket( SocketErrorHandler&& errorHandler, MessageDispatcher& dispatcher );
    Socket( QTcpSocket* socket, MessageDispatcher& dispatcher );
    ~Socket( );

    QString get_host_name( ) const;

    int32_t get_port( ) const;

    /// Connect to the given hostname and port. This is invoked asynchronously
    void connect_to_host( const QString& hostname, uint32_t port );

    /// Disconnect from the host. This is invoked asynchronously
    void disconnect_from_host( );

public slots:
    /// Sends a message through this Socket. This is invoked asynchronously
    void send_message( const proto::Message& message );

    /// Private handler
    bool handle_error( const QSslError& error );

    void on_message_received( const proto::Message& message );

    /// Emitted when the socket connnected
    void connected( );

    void connect_to_host_again( );

    void disconnected( );

private:
    SocketErrorHandler m_error_handler;
    QString m_host_name;
    SocketImpl* m_socket_impl;
    MessageDispatcher& m_dispatcher;
    uint32_t m_port;
};
