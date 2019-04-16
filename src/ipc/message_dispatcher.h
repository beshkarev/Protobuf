#pragma once

#include <map>
#include <memory>
#include <stdexcept>

#include "Message.pb.h"

class MessageProcessorBase
{
public:
    MessageProcessorBase( ) = default;
    virtual ~MessageProcessorBase( ) = default;

    MessageProcessorBase( const MessageProcessorBase& ) = delete;
    MessageProcessorBase& operator=( const MessageProcessorBase& ) = delete;

    virtual int64_t id( ) const = 0;

    virtual void process( const proto::Message& query ) = 0;
};

template < typename Query >
class ProtoMessageProcessor : public MessageProcessorBase
{
public:
    void
    process( const proto::Message& query ) override
    {
//        Query t_query;
//        if ( !t_query.ParseFromString( query.data( ) ) )
//        {
//            throw std::runtime_error( "Failed to parse query: " + query.ShortDebugString( ) );
//        }

        do_processing( query );
    }

private:
    virtual void do_processing( const Query& query ) = 0;
};

using MessageProcessorBasePtr = std::shared_ptr< MessageProcessorBase >;

class MessageDispatcher
{
    using DispatcherImplType = std::map< int64_t, MessageProcessorBasePtr >;

public:
    MessageDispatcher( ) = default;

    void add_processor( MessageProcessorBasePtr processor );

    void dispatch( const proto::Message& query );

    const DispatcherImplType& impl( ) const;

    static int64_t generate_id(const std::string &data);

private:
    DispatcherImplType mImpl;
};