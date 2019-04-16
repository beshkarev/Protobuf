#include <QSslError>
#include <QTcpSocket>
#include <QWidget>
#include "src/logging/logger.h"
#include "mainwindow.h"
#include "src/configuration.hpp"
#include "src/ipc/dispatchers/editor_dispatcher.h"
#include "src/ipc/dispatchers/server_dispatcher.h"
#include "src/ipc/message_dispatcher.h"
#include "src/ipc/socket.h"
#include "src/ipc/tcp_server.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow( QWidget* parent )
    : QMainWindow( parent )
    , ui( new Ui::MainWindow )
    , m_config( new Configuration( ) )
    , m_dispatcher( new MessageDispatcher )
    , m_socket( new Socket( [&]( const QSslError& error ) { return on_ssl_error( error ); },
                            *m_dispatcher ) )
    , m_server( new TcpServer(
              m_config->get_host_name( ), m_config->get_server_port( ), *m_dispatcher ) )
{
    ui->setupUi( this );

    setFixedSize( size( ) );

    QObject::connect( ui->send_button, &QPushButton::released, this, &MainWindow::send_message );
}

MainWindow::~MainWindow( )
{
    delete ui;

    if ( !m_socket )
    {
        Logger( ) << "Socket is nullptr.";
    }
    else
    {
        m_socket->disconnect_from_host( );
    }

    Logger( ) << "Application shutdown.\n";
}

void
MainWindow::init( )
{
    if ( !m_dispatcher )
    {
        Logger( ) << "Dispatcher is nullptr. Initialization failed.";
        return;
    }

    if ( !m_socket )
    {
        Logger( ) << "Socket is nullptr.";
        return;
    }

    if ( !m_server )
    {
        Logger( ) << "Server is nullptr.";
        return;
    }

    m_dispatcher->add_processor( std::make_shared< EditorDispatcher >( *this ) );
    m_dispatcher->add_processor( std::make_shared< ServerDispatcher >( *this ) );

    m_socket->connect_to_host( m_config->get_host_name( ), m_config->get_client_port( ) );
    m_server->listen( );
}

void
MainWindow::on_text_editor_updated( const std::string& text )
{
    ui->editor->setText( QString::fromStdString( text ) );
}

void
MainWindow::on_new_server_connection( )
{
    auto socket = m_server->get_socket( );

    m_server_socket.reset( new Socket( socket, *m_dispatcher ) );
}

void
MainWindow::send_message( )
{
    const auto& str = ui->editor->toPlainText( );

    if ( str.isEmpty( ) )
    {
        Logger( ) << "Text editor is empty, nothing to send.";
        return;
    }

    if ( !m_server_socket )
    {
        Logger( ) << "Server Socket is nullptr.";
        return;
    }

    proto::Message message;
    message.set_id( MessageDispatcher::generate_id( "editor" ) );
    message.set_data( str.toStdString( ) );

    m_server_socket->send_message( message );
}

bool
MainWindow::on_ssl_error( const QSslError& error )
{
    // Skip all errors. Here we can handle all ssl errors.

    Logger( ) << "****************************************";
    Logger( ) << "Ssl error connecting to " + m_socket->get_host_name( ).toStdString( ) + " port "
                         + std::to_string( m_socket->get_port( ) );
    Logger( ) << "Error: " + error.errorString( ).toStdString( );
    Logger( ) << "****************************************";

    return true;
}
