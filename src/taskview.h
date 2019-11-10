#pragma once

#include <QWidget>

class Task;

class QDateTimeEdit;
class QLineEdit;
class QCalendarWidget;

class TaskView : public QWidget
{
  Q_OBJECT

public:
  explicit TaskView(QWidget *parent = nullptr);
  ~TaskView();

  void reset();
  void setVisible(bool visible) override;
  bool eventFilter(QObject* watched, QEvent* event) override;

signals:
  void taskAdded(const Task& task);

protected:
  void keyPressEvent(QKeyEvent* event) override;
  void contextMenuEvent(QContextMenuEvent* event) override;

private:
  void saveState();
  void restoreState();
  void showCalendar();
  void applyCalendarDate();

  bool isFrameless() const;
  void setFrameless(bool on);

  QDateTimeEdit* date_;
  QLineEdit* text_;
  QCalendarWidget* calendar_;
};
