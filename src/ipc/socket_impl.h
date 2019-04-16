#pragma once

#include <QAbstractSocket>

class Socket;
class QTcpSocket;

namespace proto
{
class Message;
}

///// This class wraps a QSslSocket and it's meant to run in its own thread
class SocketImpl : public QObject
{
    Q_OBJECT

public:
    SocketImpl( Socket* errorHandler, QObject* parent = nullptr );
    SocketImpl( QTcpSocket* socket );

public slots:
    void connect_to_host( const QString& hostname, int port );

    void disconect_from_host( );

    void send_message( const proto::Message& message );

    void on_ready_read( );

    void on_disconected( );

signals:
    void connected( );
    void disconnected( QAbstractSocket::SocketError error, const QString& errorString );
    void message_received( const proto::Message& message );
    void connection_failed( );

private:
    QTcpSocket* m_socket;
    Socket* m_error_handler;
};
