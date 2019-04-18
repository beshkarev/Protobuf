#include "socket_impl.h"
#include <QTcpSocket>
#include <cassert>
#include "Message.pb.h"
#include "socket.h"
#include "src/logging/logger.h"

SocketImpl::SocketImpl( Socket* errorHandler, QObject* parent )
    : QObject( parent )
    , m_socket( new QTcpSocket( this ) )
    , m_error_handler( errorHandler )
{
    QObject::connect( m_socket, &QTcpSocket::connected, this, &SocketImpl::connected );
    QObject::connect( m_socket, &QTcpSocket::disconnected, this, &SocketImpl::on_disconected );
    QObject::connect( m_socket, &QTcpSocket::readyRead, this, &SocketImpl::on_ready_read );
}

SocketImpl::SocketImpl( QTcpSocket* socket )
    : m_socket( socket )
{
    QObject::connect( m_socket, &QTcpSocket::readyRead, this, &SocketImpl::on_ready_read );
    QObject::connect( m_socket, &QTcpSocket::disconnected, this, &SocketImpl::on_disconected );
}

void
SocketImpl::connect_to_host( const QString& hostname, int port )
{
    m_socket->connectToHost( hostname, port );

    if ( !m_socket->waitForConnected( ) )
    {
        emit connection_failed( );
    }
}

void
SocketImpl::disconect_from_host( )
{
    m_socket->disconnectFromHost( );
}

void
SocketImpl::send_message( const proto::Message& message )
{
    std::string str_message;
    message.SerializeToString( &str_message );

    std::vector< char > send_buffer( str_message.size( ) );
    memcpy( send_buffer.data( ), str_message.c_str( ), send_buffer.size( ) );

    Logger( ) << "Sending a message through the socket";
    m_socket->write( send_buffer.data( ), send_buffer.size( ) );
}

void
SocketImpl::on_ready_read( )
{
    auto recv_buffer = m_socket->readAll( );

    proto::Message message;
    message.ParseFromArray( recv_buffer.data( ), recv_buffer.size( ) );

    emit message_received( message );
}

void
SocketImpl::on_disconected( )
{
    emit disconnected( m_socket->error( ), m_socket->errorString( ) );
}
