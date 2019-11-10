#pragma once

#include <QObject>

class TaskView;
class TrayIcon;
class MainWindow;
struct Task;

class Manager : public QObject
{
  Q_OBJECT

public:
  explicit Manager(const QString& fileName);
  ~Manager();

private:
  void showTaskView();
  void addTask(const Task& task);
  void showMainWindow();
  void quit();

  QScopedPointer<TrayIcon> tray_;
  QScopedPointer<TaskView> taskView_;
  QScopedPointer<MainWindow> mainWindow_;

  QString fileName_;
};
