#pragma once

#include <QObject>

class QSystemTrayIcon;

class TrayIcon : public QObject
{
  Q_OBJECT

public:
  TrayIcon();
  ~TrayIcon();

signals:
  void addTaskRequested();
  void mainWindowRequested();
  void quitRequested();

private:
  QSystemTrayIcon *tray_;
};
