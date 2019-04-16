#include <QSslError>
#include <QSslSocket>
#include <QTcpSocket>
#include <cassert>
#include "Message.pb.h"
#include "socket.h"
#include "socket_impl.h"
#include "src/Logger.hpp"

constexpr uint32_t MAGIC_NUMBER = 12345;
constexpr int BUFFER_SIZE = 1024;

/// Encode a uint32 to big endian encoding
void
encodeToBigEndian( char* ptr, uint32_t data )
{
    ptr[ 0 ] = static_cast< char >( ( data & 0xFF000000 ) >> 24 );
    ptr[ 1 ] = static_cast< char >( ( data & 0x00FF0000 ) >> 16 );
    ptr[ 2 ] = static_cast< char >( ( data & 0x0000FF00 ) >> 8 );
    ptr[ 3 ] = static_cast< char >( ( data & 0x000000FF ) );
}

/// Decode a big endian to current uint32 endianess
uint32_t
decodeFromBigEndian( char* ptr )
{
    uint32_t data = static_cast< uint32_t >( ptr[ 0 ] );
    data = data << 8;
    data += static_cast< uint32_t >( ptr[ 1 ] );
    data = data << 8;
    data += static_cast< uint32_t >( ptr[ 2 ] );
    data = data << 8;
    data += static_cast< uint32_t >( ptr[ 3 ] );
    return data;
}
SocketImpl::SocketImpl( Socket* errorHandler, QObject* parent )
    : QObject( parent )
    , m_socket( new QSslSocket( this ) )
    , m_error_handler( errorHandler )
{
//    m_socket->setPeerVerifyMode( QSslSocket::VerifyNone );
    m_recv_buffer.reserve( BUFFER_SIZE );
    m_recv_buffer.resize( BUFFER_SIZE );

    QObject::connect( m_socket, &QTcpSocket::connected, this, &SocketImpl::connected );
    QObject::connect( m_socket, &QTcpSocket::disconnected, this, &SocketImpl::on_disconected );
//    QObject::connect( m_socket,
//                      static_cast< void ( QSslSocket::* )( const QList< QSslError >& ) >(
//                              &QSslSocket::sslErrors ),
//                      this,
//                      &SocketImpl::on_ssl_errors );
    QObject::connect( m_socket, &QTcpSocket::readyRead, this, &SocketImpl::on_ready_read );
}

SocketImpl::SocketImpl( QTcpSocket* socket )
    : m_socket( socket )
{
    m_recv_buffer.reserve( BUFFER_SIZE );
    m_recv_buffer.resize( BUFFER_SIZE );

    QObject::connect( m_socket, &QTcpSocket::readyRead, this, &SocketImpl::on_ready_read );
    QObject::connect( m_socket, &QTcpSocket::disconnected, this, &SocketImpl::on_disconected );
}

void
SocketImpl::connect_to_host( const QString& hostname, int port )
{
    m_socket->connectToHost( hostname, port );
    m_socket->waitForConnected(10000);
}

void
SocketImpl::disconect_from_host( )
{
    m_socket->disconnectFromHost( );
}

void
SocketImpl::on_ssl_errors( const QList< QSslError >& errors )
{
    QList< QSslError > toIgnore;
    for ( const QSslError& e : errors )
    {
        bool result = true;
        QMetaObject::invokeMethod( m_error_handler,
                                   "handleError",
                                   Qt::BlockingQueuedConnection,
                                   Q_RETURN_ARG( bool, result ),
                                   Q_ARG( QSslError, e ) );
        if ( !result )
        {
            disconect_from_host( );
            return;
        }
        toIgnore.push_back( e );
    }
//    m_socket->ignoreSslErrors( toIgnore );
}

void
SocketImpl::send_message( const proto::Message& message )
{
    //    const int message_size = 2 * sizeof( uint32_t ) + message.ByteSize( );
    //    m_send_buffer.resize( message_size );
    //    encodeToBigEndian( &m_send_buffer[ 0 ], MAGIC_NUMBER );
    //    encodeToBigEndian( &m_send_buffer[ 4 ], message.ByteSize( ) );
    //    const bool ok = message.SerializeToArray( &m_send_buffer[ 8 ], m_send_buffer.size( ) - 8
    //    );

    //    if ( ok )
    //    {
    //        Logger( ) << "Sending a message through the socket";
    //        m_socket->write( m_send_buffer.data( ), m_send_buffer.size( ) );
    //    }

    std::string str_message;
    message.SerializeToString( &str_message );
    m_send_buffer.resize( str_message.size( ) );
    memcpy( m_send_buffer.data( ), str_message.c_str( ), m_send_buffer.size( ) );

    Logger( ) << "Sending a message through the socket";
    m_socket->write( m_send_buffer.data( ), m_send_buffer.size( ) );
}

void
SocketImpl::on_ready_read( )
{
    m_recv_buffer.append( m_socket->readAll( ) );

    proto::Message message;
    message.ParseFromArray( m_recv_buffer.data( ), m_recv_buffer.size( ) );
    m_recv_buffer.clear( );

    emit message_received( message );
}

void
SocketImpl::on_disconected( )
{
    emit disconnected( m_socket->error( ), m_socket->errorString( ) );
}
