#include <QSslSocket>
#include <QThread>
#include "Message.pb.h"
#include "message_dispatcher.h"
#include "socket.h"
#include "socket_impl.h"
#include "src/Logger.hpp"

Socket::~Socket( )
{
}

Socket::Socket( SocketErrorHandler&& errorHandler, MessageDispatcher& dispatcher )
    : m_socket_impl( new SocketImpl( this ) )
    , m_error_handler( std::move( errorHandler ) )
    , m_dispatcher( dispatcher )
    , m_port( 0 )
{
    auto thread = new QThread( this );
    QObject::connect( thread, &QThread::finished, m_socket_impl, &SocketImpl::deleteLater );
    m_socket_impl->moveToThread( thread );

    QObject::connect(
            m_socket_impl, &SocketImpl::connected, this, &Socket::connected, Qt::QueuedConnection );
    QObject::connect( m_socket_impl,
                      &SocketImpl::disconnected,
                      this,
                      &Socket::disconnected,
                      Qt::QueuedConnection );
    QObject::connect( m_socket_impl,
                      &SocketImpl::message_received,
                      this,
                      &Socket::on_message_received,
                      Qt::QueuedConnection );

    thread->start( );
    qRegisterMetaType< proto::Message >( "proto::Message" );
}

Socket::Socket( QTcpSocket* socket, MessageDispatcher& dispatcher )
    : m_socket_impl( new SocketImpl( socket ) )
    , m_dispatcher( dispatcher )
{
    auto thread = new QThread( this );
    QObject::connect( thread, &QThread::finished, m_socket_impl, &SocketImpl::deleteLater );
    m_socket_impl->moveToThread( thread );

    QObject::connect( m_socket_impl,
                      &SocketImpl::message_received,
                      this,
                      &Socket::on_message_received,
                      Qt::QueuedConnection );

    thread->start( );
    qRegisterMetaType< proto::Message >( "proto::Message" );
}

QString
Socket::get_host_name( ) const
{
    return m_host_name;
}

int32_t
Socket::get_port( ) const
{
    return m_port;
}

void
Socket::connect_to_host( const QString& hostname, uint32_t port )
{
    m_host_name = hostname;
    m_port = port;
    QMetaObject::invokeMethod( m_socket_impl,
                               "connect_to_host",
                               Qt::QueuedConnection,
                               Q_ARG( QString, hostname ),
                               Q_ARG( int, port ) );
}

void
Socket::disconnect_from_host( )
{
    QMetaObject::invokeMethod( m_socket_impl, "disconect_from_host", Qt::QueuedConnection );
}

void
Socket::send_message( const proto::Message& message )
{
    QMetaObject::invokeMethod(
            m_socket_impl, "send_message", Qt::QueuedConnection, Q_ARG( proto::Message, message ) );
}

bool
Socket::handle_error( const QSslError& error )
{
    return m_error_handler( error );
}

void
Socket::on_message_received( const proto::Message& message )
{
    m_dispatcher.dispatch( message );
}

void
Socket::connected( )
{
    Logger( ) << "Connected.";
}
