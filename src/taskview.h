#pragma once

#include <QWidget>

class Task;

class QDateTimeEdit;
class QLineEdit;

class TaskView : public QWidget
{
  Q_OBJECT

public:
  explicit TaskView(QWidget *parent = nullptr);

  void reset();

signals:
  void taskAdded(const Task& task);

protected:
  void keyPressEvent(QKeyEvent* event) override;

private:
  QDateTimeEdit* date_;
  QLineEdit* text_;
};
