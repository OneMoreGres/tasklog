#pragma once

#include <QAbstractTableModel>

class Task;

class TaskModel : public QAbstractTableModel
{
  Q_OBJECT
public:
  enum class Column { Date, Name, Count };

  explicit TaskModel(QObject *parent = nullptr);
  ~TaskModel();

  void addTask(const Task &task);

  int rowCount(const QModelIndex &parent) const override;
  int columnCount(const QModelIndex &parent) const override;
  QVariant data(const QModelIndex &index, int role) const override;
  QVariant headerData(int section, Qt::Orientation orientation,
                      int role) const override;

private:
  QVector<Task> tasks_;
};
