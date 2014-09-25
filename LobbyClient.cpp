#include "LobbyClient.h"

#include <QDataStream>

LobbyClient::LobbyClient(QTcpSocket* socket, QObject *parent) :
  QObject(parent), socket(socket)
{
  connect(socket,SIGNAL(readyRead()),SLOT(onReadyRead()));
  connect(socket,SIGNAL(error(QAbstractSocket::SocketError)),SLOT(onError(QAbstractSocket::SocketError)));
  connect(socket,SIGNAL(disconnected()),SLOT(onDisconnect()));

  state = HANDSHAKE;
}

void LobbyClient::send(QString command, Json data)
{
  Json obj;

  obj["id"] = command;
  obj["data"] = data;

  QByteArray buf = QJsonDocument(obj).toBinaryData();

  quint32 size = buf.size();

  socket->write((char*)&size, 4);
  socket->write(buf);
}

void LobbyClient::onMessage(QString id, Json data)
{
  switch(state)
  {
  case HANDSHAKE:
    onHandshake(id, data);
    break;

  case LOGIN:
    onLogin(id, data);
    break;

  case PLAYING:
    onPlay(id, data);
    break;
  }
}

void LobbyClient::onHandshake(QString id, Json data)
{
  if(id != "handshake")
    // Garbage?
    disconnect();

  QString address = data["server_address"].toString();

  if(address != "scfaf.net")
    // Are we spoofed?
  {
    Json resp;
    resp["success"] = false;
    resp["reason"] = "You seem to be connected over proxy.";

    send("handshake_resp", resp);
    disconnect();
  }
  else
  {
    int proto_ver = data["protocol_version"].toInt();
    if(proto_ver != 0)
    {
      Json resp;
      resp["success"] = false;
      resp["reason"] = "Unhandled protocol version.";

      send("handshake_resp", resp);
      disconnect();
    }
    else
    {
      Json resp;
      resp["success"] = true;

      send("hanshake_resp", resp);
      state = LOGIN;
    }
  }
}

void LobbyClient::onLogin(QString id, Json data)
{
  if(id != "login")
    // Garbage?
  {
    Json resp;
    resp["success"] = false;
    resp["reason"] = "Bad command.";

    send("login_resp", resp);
    disconnect();
  }
  QString address = data["server_address"].toString();

}

void LobbyClient::onPlay(QString id, Json data)
{

}

void LobbyClient::disconnect()
{
  socket->close();
  deleteLater();
}

QString LobbyClient::getName()
{
  return "TODO";
}

void LobbyClient::onReadyRead()
{
  while(socket->bytesAvailable() >= 4)
  {
    quint32 size;
    socket->peek((char*)&size, 4);

    if(socket->bytesAvailable() < size+4)
      return;

    socket->read(4);

    Json obj = QJsonDocument::fromBinaryData(socket->read(size)).object();
    onMessage(obj["id"].toString(), obj["data"].toObject());
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
