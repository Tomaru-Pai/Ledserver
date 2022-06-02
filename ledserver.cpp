#include "ledserver.h"

// Testabgabe
// Thomas Pail
// 2.6.2022

LedServer::LedServer(quint16 port, QObject *parent) : QObject(parent), m_port(port)
{
    m_gpio = new Gpio(this);
    m_server = new QTcpServer(this);

    connect(m_server, &QTcpServer::newConnection,
            this, &LedServer::myNewConnection);
}

void LedServer::start()
{
    m_server->listen(QHostAddress::Any, PORT);
    // Debug
    qDebug() << "Server lauscht auf: " << PORT;
}

// Client hat Verbindung zum Server aufgebaut
void LedServer::myNewConnection()
{
    // Verbindung zum Client über m_socket
    m_socket = m_server->nextPendingConnection();
    qDebug() << "Verbindung zum server hergestellt";

    m_socket->write("Bitte Zahl von 0 bis 15 eingeben: ");

    connect(m_socket, &QTcpSocket::disconnected, this, &LedServer::myClientDisconnect);
    connect(m_socket, &QTcpSocket::readyRead, this, &LedServer::myServerRead);
}

// Client hat Verbindung zum Server getrennt
void LedServer::myClientDisconnect()
{
    // Schalet LEDS aus
    m_gpio->set(0);
    m_socket->disconnectFromHost();
    qDebug() << "Verbindung zum Client getrennt";
}

// Client hat eine Zahl (0...15) zum Server gesendet
void LedServer::myServerRead()
{
    while (m_socket->bytesAvailable()) {
            // Gets Value from User
            QString message = m_socket->readAll();
            // Converts to integer
            int value = message.toInt();
            qDebug() << "Daten vom Client eingelangt: " << value;
            if (value >= 0 && value <= 15)
            {
                m_socket->write("OK\n");
                m_gpio->set(value);
            }
            m_socket->write("Bitte Zahl von 0 bis 15 eingeben: ");
        }
}
