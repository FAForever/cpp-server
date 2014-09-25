#ifndef LOBBYCLIENT_H
#define LOBBYCLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QJsonObject>
#include <QJsonDocument>

typedef QJsonObject Json;

class LobbyClient : public QObject
{
  Q_OBJECT
public:
  explicit LobbyClient(QTcpSocket* socket, QObject *parent = 0);

  void send(QString command, Json data);
private:
  enum State
  {
    HANDSHAKE, LOGIN, PLAYING
  };

  QTcpSocket* socket;

  State state;

  void onMessage(QString id, Json data);

  void onHandshake(QString id, Json data);
  void onLogin(QString id, Json data);
  void onPlay(QString id, Json data);

  void disconnect();

  QString getName();
signals:

public slots:

private slots:
  void onReadyRead();

  void onError(QAbstractSocket::SocketError error);
  void onDisconnect();
};



#endif // LOBBYCLIENT_H
