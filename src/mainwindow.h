#pragma once

#include <QMainWindow>
#include <QModelIndex>

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

  bool eventFilter(QObject* watched, QEvent* event) override;

signals:
  void saveAs(const QString& fileName, const QModelIndexList& indexes);

private:
  void saveState();
  void restoreState();
  void applyFilter();
  void focusFilter();
  void promptSaveAs();

  QSortFilterProxyModel* proxy_;
  QTableView* view_;
  QLineEdit* filter_;
};
