#include <stdexcept>
#include "message_dispatcher.h"
#include "src/logging/logger.h"

void
MessageDispatcher::add_processor( MessageProcessorBasePtr processor )
{
    mImpl.emplace( processor->id( ), processor );
}

void
MessageDispatcher::dispatch( const proto::Message& query )
{
    auto it = mImpl.find( query.id( ) );

    if ( it == mImpl.end( ) )
    {
        Logger( ) << "Unknown message id: " + query.id( );
        return;
    }

    it->second->process( query );
}

const MessageDispatcher::DispatcherImplType&
MessageDispatcher::impl( ) const
{
    return mImpl;
}

int64_t
MessageDispatcher::generate_id( const std::string& data )
{
    std::hash< std::string > hasher;
    return hasher( data );
}
