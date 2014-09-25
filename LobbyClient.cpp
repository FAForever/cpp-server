#include "LobbyClient.h"

#include <QTime>

LobbyClient::LobbyClient(QTcpSocket* socket, QObject *parent) :
  QObject(parent), socket(socket)
{
  connect(socket,SIGNAL(readyRead()),SLOT(onReadyRead()));
  connect(socket,SIGNAL(error(QAbstractSocket::SocketError)),SLOT(onError(QAbstractSocket::SocketError)));
  connect(socket,SIGNAL(disconnected()),SLOT(onDisconnect()));

  state = HANDSHAKE;

  connect(&keep_alive,SIGNAL(timeout()),SLOT(keepAlive()));
}

void LobbyClient::send(QString command, Json data)
{
  qDebug() << getName() << "<<" << command << data;

  Json obj;

  obj["id"] = command;
  obj["data"] = data;

  //QByteArray buf = QJsonDocument(obj).toBinaryData();
  QByteArray buf = QJsonDocument(obj).toJson(QJsonDocument::Compact);

  quint32 size = buf.size();

  socket->write((char*)&size, 4);
  socket->write(buf);
}

void LobbyClient::onMessage(QString id, Json data)
{
  qDebug() << getName() << ">>" << id << data;

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

  /*if(address != "scfaf.net")
    // Are we spoofed?
  {
    Json resp;
    resp["success"] = false;
    resp["reason"] = "You seem to be connected over proxy.";

    send("handshake_resp", resp);
    disconnect();
  }
  else*/
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

      send("handshake_resp", resp);
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

  QString username = data["username"].toString();
  QString password = data["username"].toString();

  Json resp;
  resp["success"] = true;

  send("login_resp", resp);
}

void LobbyClient::onPlay(QString id, Json data)
{

}

void LobbyClient::keepAlive()
{
  Json kp;
  kp["time"] = QTime::currentTime().toString(Qt::ISODate);
  send("PING", kp);
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

    //Json obj = QJsonDocument::fromBinaryData(socket->read(size)).object();
    Json obj = QJsonDocument::fromJson(socket->read(size)).object();
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
