#include "Message.pb.h"
#include "src/ipc/message_dispatcher.h"
#include "tcp_server.h"

TcpServer::TcpServer( const QString& host, uint32_t port, MessageDispatcher& dispatcher )
    : m_host( host )
    , m_port( port )
    , m_dispatcher( dispatcher )
{
}

void
TcpServer::listen( )
{
    QTcpServer::listen( QHostAddress( m_host ), m_port );

    QObject::connect( this, &QTcpServer::newConnection, this, &TcpServer::on_new_connection );
}

QTcpSocket*
TcpServer::get_socket( )
{
    return nextPendingConnection( );
}

void
TcpServer::on_new_connection( )
{
    proto::Message message;
    message.set_id( MessageDispatcher::generate_id( "server" ) );

    m_dispatcher.dispatch( message );
}
