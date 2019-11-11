#pragma once

#include <QMainWindow>

class TaskModel;
class QTableView;

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  MainWindow(TaskModel& model, QWidget* parent = nullptr);
  ~MainWindow();

private:
  void saveState();
  void restoreState();

  TaskModel& model_;
  QTableView* view_;
};
