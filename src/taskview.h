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
  ~TaskView();

  void reset();
  void setVisible(bool visible) override;

signals:
  void taskAdded(const Task& task);

protected:
  void keyPressEvent(QKeyEvent* event) override;

private:
  void saveState();
  void restoreState();

  QDateTimeEdit* date_;
  QLineEdit* text_;
};
