#pragma once

#include <QMainWindow>

class TaskModel;
class QTableView;
class QLineEdit;
class QSortFilterProxyModel;

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  MainWindow(TaskModel& model, QWidget* parent = nullptr);
  ~MainWindow();

private:
  void saveState();
  void restoreState();
  void applyFilter();

  TaskModel& model_;
  QSortFilterProxyModel* proxy_;
  QTableView* view_;
  QLineEdit* filter_;
};
