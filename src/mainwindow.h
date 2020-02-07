#pragma once

#include <QMainWindow>
#include <QModelIndex>

class TaskModel;
class QTableView;
class CompletableLineEdit;
class QSortFilterProxyModel;

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  MainWindow(TaskModel& model, QWidget* parent = nullptr);
  ~MainWindow();

  bool eventFilter(QObject* watched, QEvent* event) override;
  void setKeywords(const QStringList& words);

signals:
  void saveAs(const QString& fileName, const QModelIndexList& indexes);

private:
  void saveState();
  void restoreState();
  void applyFilter();
  void focusFilter();
  void promptSaveAs();
  void showContextMenu();

  QSortFilterProxyModel* proxy_;
  QAction* saveAs_{nullptr};
  QTableView* view_;
  CompletableLineEdit* filter_;
};
