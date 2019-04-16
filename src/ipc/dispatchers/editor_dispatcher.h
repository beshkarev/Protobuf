#pragma once

#include "Message.pb.h"
#include "src/ipc/message_dispatcher.h"

class MainWindow;

class EditorDispatcher : public ProtoMessageProcessor< proto::Message >
{
public:
    EditorDispatcher( MainWindow& dispatcher );

    int64_t id( ) const override;

private:
    void do_processing( const proto::Message& message ) override;

    MainWindow& m_dispatcher;
};

