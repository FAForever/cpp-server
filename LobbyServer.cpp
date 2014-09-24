#include "LobbyServer.h"

#include <iostream>

LobbyServer::LobbyServer(QObject *parent) :
  QObject(parent),
  server(this)
{
  connect(&server,SIGNAL(newConnection()),SLOT(onClientConnecting()));
}

void LobbyServer::start(QHostAddress addr, quint16 port)
{
  qDebug() << "Binding to" << addr << ":" << port;
  assert(server.listen(addr, port));
}

void LobbyServer::onClientConnecting()
{
  while(server.hasPendingConnections())
  {
    LobbyClient* client = new LobbyClient(server.nextPendingConnection());
    clients.append(client);
    connect(client,SIGNAL(destroyed(QObject*)),SLOT(onClientDestroyed(QObject*)));
  }
}

void LobbyServer::onClientDestroyed(LobbyClient* client)
{

}
