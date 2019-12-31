#pragma once

#include <QSystemTrayIcon>

class Settings;

class QAction;

class TrayIcon : public QObject
{
  Q_OBJECT

public:
  TrayIcon();
  ~TrayIcon();

  void updateSettings(const Settings& settings);

signals:
  void addTaskRequested();
  void mainWindowRequested();
  void settingsRequested();
  void quitRequested();

private:
  QSystemTrayIcon* tray_;
  QAction* addRecordAction_;
};
