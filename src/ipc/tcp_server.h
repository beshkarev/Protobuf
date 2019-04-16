#pragma once

#include <QTcpServer>

class MessageDispatcher;

class TcpServer : public QTcpServer
{
    Q_OBJECT

public:
    TcpServer( const QString& host, uint32_t port, MessageDispatcher& dispatcher );
    void listen( );

    QTcpSocket* get_socket( );

public slots:
    void on_new_connection( );

private:
    QString m_host;
    uint32_t m_port;
    MessageDispatcher& m_dispatcher;
};
