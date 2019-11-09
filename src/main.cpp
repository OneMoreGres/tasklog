#include "manager.h"

#include <QApplication>

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  a.setApplicationName("TaskLog");
  a.setOrganizationName("Gres");
  a.setQuitOnLastWindowClosed(false);

  Manager manager;

  return a.exec();
}
