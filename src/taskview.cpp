#include "taskview.h"
#include "constants.h"
#include "task.h"

#include <QApplication>
#include <QBoxLayout>
#include <QContextMenuEvent>
#include <QDateTimeEdit>
#include <QLineEdit>
#include <QMenu>
#include <QScreen>
#include <QSettings>

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
    move((screenSize.width() - width()) / 2,
         (screenSize.height() - height()) / 2);
  }

  text_->setPlaceholderText(tr("Enter record text"));

  restoreState();
}

TaskView::~TaskView()
{
  saveState();
}

void TaskView::reset()
{
  date_->setDateTime(QDateTime::currentDateTime());
  text_->clear();
  text_->setFocus();
}

void TaskView::setVisible(bool visible)
{
  QSettings settings;
  settings.beginGroup(qs_taskViewGroup);

  if (!visible && settings.value(qs_geometry) != saveGeometry())
    settings.setValue(qs_geometry, saveGeometry());

  QWidget::setVisible(visible);

  if (visible && settings.contains(qs_geometry))
    restoreGeometry(settings.value(qs_geometry).toByteArray());

  settings.endGroup();
}

void TaskView::saveState()
{
  QSettings settings;
  settings.beginGroup(qs_taskViewGroup);
  settings.setValue(qs_geometry, saveGeometry());
  settings.setValue(qs_frameless, isFrameless());
  settings.endGroup();
}

void TaskView::restoreState()
{
  QSettings settings;
  settings.beginGroup(qs_taskViewGroup);
  if (settings.contains(qs_geometry))
    restoreGeometry(settings.value(qs_geometry).toByteArray());
  setFrameless(settings.value(qs_frameless, false).toBool());
  settings.endGroup();
}

bool TaskView::isFrameless() const
{
  return windowFlags() & Qt::FramelessWindowHint;
}

void TaskView::setFrameless(bool on)
{
  const auto visible = isVisible();
  setWindowFlag(Qt::FramelessWindowHint, on);
  setVisible(visible);
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

void TaskView::contextMenuEvent(QContextMenuEvent *event)
{
  QMenu menu;
  auto toggleFrame = menu.addAction(tr("Show window frame"));
  toggleFrame->setCheckable(true);
  toggleFrame->setChecked(!isFrameless());

  auto choice = menu.exec(event->globalPos());

  if (choice == toggleFrame)
    setFrameless(!toggleFrame->isChecked());
}
