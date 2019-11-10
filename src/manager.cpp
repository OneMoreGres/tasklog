#include "manager.h"
#include "debug.h"
#include "mainwindow.h"
#include "parser.h"
#include "task.h"
#include "taskview.h"
#include "trayicon.h"

#include <QApplication>

Manager::Manager(const QString &fileName)
  : tray_(new TrayIcon)
  , taskView_(new TaskView)
  , mainWindow_(nullptr)
  , fileName_(fileName)
{
  connect(tray_.data(), &TrayIcon::addTaskRequested,  //
          this, &Manager::showTaskView);
  connect(tray_.data(), &TrayIcon::mainWindowRequested,  //
          this, &Manager::showMainWindow);
  connect(tray_.data(), &TrayIcon::quitRequested,  //
          this, &Manager::quit);

  taskView_->setWindowModality(Qt::WindowModal);

  connect(taskView_.data(), &TaskView::taskAdded,  //
          this, &Manager::addTask);
}

Manager::~Manager() = default;

void Manager::showTaskView()
{
  SOFT_ASSERT(taskView_, return );
  if (taskView_->isVisible()) {
    taskView_->raise();
    taskView_->activateWindow();
    return;
  }

  taskView_->reset();
  taskView_->show();
}

void Manager::addTask(const Task &task)
{
  SOFT_ASSERT(task.isValid(), return );
  Parser parser(fileName_);
  parser.append(task);
}

void Manager::showMainWindow()
{
  if (!mainWindow_)
    mainWindow_.reset(new MainWindow);

  mainWindow_->show();
}

void Manager::quit()
{
  QApplication::quit();
}
