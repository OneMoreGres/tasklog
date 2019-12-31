#include "taskmodel.h"
#include "debug.h"
#include "task.h"

TaskModel::TaskModel(QObject *parent)
  : QAbstractTableModel(parent)
{
}

void TaskModel::setTasks(const QVector<Task> &tasks)
{
  beginResetModel();
  tasks_ = tasks;
  endResetModel();
}

TaskModel::~TaskModel() = default;

void TaskModel::addTask(const Task &task)
{
  const auto row = tasks_.size();
  beginInsertRows({}, row, row);
  tasks_.append(task);
  endInsertRows();
}

Task TaskModel::task(const QModelIndex &index) const
{
  if (index.row() >= 0 && index.row() < tasks_.size()) {
    return tasks_[index.row()];
  }
  return {};
}

int TaskModel::rowCount(const QModelIndex & /*parent*/) const
{
  return tasks_.size();
}

int TaskModel::columnCount(const QModelIndex & /*parent*/) const
{
  return int(Column::Count);
}

QVariant TaskModel::data(const QModelIndex &index, int role) const
{
  if (role != Qt::DisplayRole || !index.isValid())
    return {};

  SOFT_ASSERT(index.row() < tasks_.size(), return {});

  const auto &task = tasks_[index.row()];

  switch (Column(index.column())) {
    case Column::Date: return task.date;
    case Column::Name: return task.text;
  }
  return {};
}

QVariant TaskModel::headerData(int section, Qt::Orientation orientation,
                               int role) const
{
  if (role != Qt::DisplayRole)
    return QAbstractTableModel::headerData(section, orientation, role);

  if (orientation == Qt::Vertical)
    return section + 1;

  switch (Column(section)) {
    case Column::Date: return tr("Date");
    case Column::Name: return tr("Name");
  }
  return {};
}
