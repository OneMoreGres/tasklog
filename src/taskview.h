#pragma once

#include <QDialog>

#include <optional>

class Task;

class QDateTimeEdit;
class QLineEdit;

class TaskView : public QDialog
{
  Q_OBJECT

public:
  explicit TaskView(QWidget *parent = nullptr);

  void reset();

  std::optional<Task> task() const;

private:
  QDateTimeEdit* date_;
  QLineEdit* text_;
};
