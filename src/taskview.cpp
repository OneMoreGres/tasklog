#include "taskview.h"
#include "task.h"

#include <QApplication>
#include <QBoxLayout>
#include <QContextMenuEvent>
#include <QDateTimeEdit>
#include <QLineEdit>
#include <QScreen>

TaskView::TaskView(QWidget *parent)
  : QWidget(parent)
  , date_(new QDateTimeEdit(this))
  , text_(new QLineEdit(this))
{
  setWindowTitle(tr("Add record"));

  auto layout = new QHBoxLayout(this);
  layout->addWidget(date_);
  layout->addWidget(text_);

  date_->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

  if (const auto screen = QApplication::primaryScreen()) {
    const auto screenSize = screen->availableSize();
    resize(screenSize.width() / 2, 1);
  }

  text_->setPlaceholderText(tr("Enter record text"));
}

void TaskView::reset()
{
  date_->setDateTime(QDateTime::currentDateTime());
  text_->clear();
  text_->setFocus();
}

std::optional<Task> TaskView::task() const
{
  const auto task = Task{text_->text(), date_->dateTime()};
  if (!task.isValid())
    return {};

  return task;
}

void TaskView::keyPressEvent(QKeyEvent *event)
{
  if (event->key() == Qt::Key_Escape) {
    close();
  } else if (event->key() == Qt::Key_Return) {
    const auto task = Task{text_->text(), date_->dateTime()};
    if (task.isValid())
      emit taskAdded(task);
    close();
  }

  QWidget::keyPressEvent(event);
}
