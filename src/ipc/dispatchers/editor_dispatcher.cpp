#include "editor_dispatcher.h"
#include "src/mainwindow.h"

EditorDispatcher::EditorDispatcher( MainWindow& dispatcher )
    : m_dispatcher( dispatcher )
{
}

int64_t
EditorDispatcher::id( ) const
{
    return MessageDispatcher::generate_id( "editor" );
}

void
EditorDispatcher::do_processing( const proto::Message& message )
{
    m_dispatcher.on_text_editor_updated( message.data( ) );
}
