#include "taskview.h"

#include "task.h"

TaskView::TaskView(QWidget *parent)
  : QDialog(parent)
{
}

void TaskView::clear()
{
}

std::optional<Task> TaskView::task() const
{
  return {};
}
