#pragma once

#include <QObject>

class TaskView;
class TrayIcon;
class MainWindow;

class Manager : public QObject
{
  Q_OBJECT

public:
  Manager();
  ~Manager();

private:
  void addTask();
  void showMainWindow();
  void quit();

  QScopedPointer<TrayIcon> tray_;
  QScopedPointer<TaskView> taskView_;
  QScopedPointer<MainWindow> mainWindow_;

  QString fileName_;
};
