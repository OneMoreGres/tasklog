#pragma once

#include <QDialog>

#include <optional>

class Task;

class TaskView : public QDialog
{
  Q_OBJECT

public:
  explicit TaskView(QWidget *parent = nullptr);

  void clear();

  std::optional<Task> task() const;
};
