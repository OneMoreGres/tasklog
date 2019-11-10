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
          this, &Manager::addTask);
  connect(tray_.data(), &TrayIcon::mainWindowRequested,  //
          this, &Manager::showMainWindow);
  connect(tray_.data(), &TrayIcon::quitRequested,  //
          this, &Manager::quit);
}

Manager::~Manager() = default;

void Manager::addTask()
{
  SOFT_ASSERT(taskView_, return );
  taskView_->clear();
  if (taskView_->exec() == QDialog::Rejected)
    return;

  const auto task = taskView_->task();
  if (!task)
    return;

  Parser parser(fileName_);
  parser.append(*task);
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
