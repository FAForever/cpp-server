#ifndef LOBBYCLIENT_H
#define LOBBYCLIENT_H

#include <QObject>
#include <QTcpSocket>

class LobbyClient : public QObject
{
  Q_OBJECT
public:
  explicit LobbyClient(QTcpSocket* socket, QObject *parent = 0);

private:
  enum State
  {
    HANDSHAKE, LOGIN, PLAYING
  };

  QTcpSocket* connection;

  State state;

  void onMessage(QString message);

  QString getName();
signals:

public slots:

private slots:
  void onReadyRead();

  void onError(QAbstractSocket::SocketError error);
  void onDisconnect();
};

#endif // LOBBYCLIENT_H
