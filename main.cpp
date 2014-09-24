#include <QCoreApplication>

#include "LobbyServer.h"

int main(int argc, char *argv[])
{
  QCoreApplication a(argc, argv);

  LobbyServer server;

  server.start();

  return a.exec();
}
