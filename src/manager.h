#pragma once

#include <QObject>
#include <QPointer>

class SettingsEditor;
class TaskView;
class TrayIcon;
class Parser;
class TaskModel;
class MainWindow;
struct Task;

class Manager : public QObject
{
  Q_OBJECT

public:
  Manager();
  ~Manager();

signals:
  void requestAddTask(const Task& task);
  void requestLoadAll();
  void requestParseKeywords();

private:
  void showTaskView();
  void showMainWindow();
  void showSettings();
  void quit();
  void updateSettings();
  void populateTasksModel();
  void destroyTasksModel();

  QScopedPointer<TrayIcon> tray_;
  QScopedPointer<TaskView> taskView_;
  QScopedPointer<Parser> parser_;
  QPointer<MainWindow> mainWindow_;
  QScopedPointer<TaskModel> taskModel_;
  QPointer<SettingsEditor> settingsEditor_;
};
