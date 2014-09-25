#ifndef LOBBYSERVER_H
#define LOBBYSERVER_H

#include <QObject>
#include <QTcpServer>
#include <QList>

#include <set>
#include "preamble.h"

#include "LobbyClient.h"

class LobbyServer : public QObject
{
  Q_OBJECT
public:
  explicit LobbyServer(QObject *parent = 0);

  QTcpServer server;
  set<LobbyClient*> clients;

  void start(QHostAddress addr = QHostAddress::Any, quint16 port = 8001);
signals:

public slots:

private slots:
  void onClientConnecting();
  void onClientDestroyed(QObject* client);
};

#endif // LOBBYSERVER_H
