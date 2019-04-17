#include "server_dispatcher.h"
#include "../message_dispatcher.h"
#include "src/mainwindow.h"

ServerDispatcher::ServerDispatcher( MainWindow& dispatcher )
    : m_dispatcher( dispatcher )
{
}

int64_t
ServerDispatcher::id( ) const
{
    return MessageDispatcher::generate_id( "server" );
}

void
ServerDispatcher::do_processing( const proto::Message& message )
{
    bool online = std::stoi( message.data( ) );
    m_dispatcher.on_conectivity_changed( online );
}
