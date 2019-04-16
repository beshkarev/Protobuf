#pragma once

#include <QMainWindow>
#include <memory>

namespace Ui
{
class MainWindow;
}

class MessageDispatcher;
class Socket;
class QSslError;
class TcpServer;
class Configuration;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow( QWidget* parent = 0 );
    ~MainWindow( );

    void init( );

    void on_text_editor_updated( const std::string& text );

    void on_new_server_connection( );

private slots:
    void send_message( );

private:
    bool on_ssl_error( const QSslError& error );

    Ui::MainWindow* ui;
    std::unique_ptr< Configuration > m_config;
    std::unique_ptr< MessageDispatcher > m_dispatcher;
    std::unique_ptr< Socket > m_socket;
    std::unique_ptr< Socket > m_server_socket;
    std::unique_ptr< TcpServer > m_server;
};
