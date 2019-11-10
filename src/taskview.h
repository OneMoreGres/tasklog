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
  void contextMenuEvent(QContextMenuEvent* event) override;

private:
  void saveState();
  void restoreState();

  bool isFrameless() const;
  void setFrameless(bool on);

  QDateTimeEdit* date_;
  QLineEdit* text_;
};
