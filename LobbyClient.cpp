#include "LobbyClient.h"

#include <QDataStream>

LobbyClient::LobbyClient(QTcpSocket* socket, QObject *parent) :
  QObject(parent)
{
  connect(socket,SIGNAL(readyRead()),SLOT(onReadyRead()));
  connect(socket,SIGNAL(error(QAbstractSocket::SocketError)),SLOT(onError(QAbstractSocket::SocketError)));
  connect(socket,SIGNAL(disconnected()),SLOT(onDisconnect()));
}

void LobbyClient::onMessage(QString message)
{
  // TODO: Parsing
}

QString LobbyClient::getName()
{
  return "TODO";
}

void LobbyClient::onReadyRead()
{
  while(connection->bytesAvailable() >= 4)
  {
    quint32 size;
    connection->peek((char*)&size, 4);

    if(connection->bytesAvailable() < size+4)
      return;

    connection->read(4);

    onMessage(QString::fromUtf8(connection->read(size)));
  }
}

void LobbyClient::onError(QAbstractSocket::SocketError error)
{
  qDebug() << getName() << " error: " << error;
  deleteLater();
}

void LobbyClient::onDisconnect()
{
  qDebug() << getName() << " disconnected.";
  deleteLater();
}
