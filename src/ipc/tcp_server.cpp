#include "tcp_server.h"
#include "Message.pb.h"
#include "src/ipc/message_dispatcher.h"

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
    message.set_data( "1" );

    m_dispatcher.dispatch( message );
}
