#include "manager.h"
#include "debug.h"
#include "mainwindow.h"
#include "parser.h"
#include "task.h"
#include "taskmodel.h"
#include "taskview.h"
#include "trayicon.h"

#include <QApplication>

Manager::Manager(const QString &fileName)
  : tray_(new TrayIcon)
  , taskView_(new TaskView)
  , parser_(nullptr)
  , mainWindow_(nullptr)
  , taskModel_(nullptr)
{
  connect(tray_.data(), &TrayIcon::addTaskRequested,  //
          this, &Manager::showTaskView);
  connect(tray_.data(), &TrayIcon::mainWindowRequested,  //
          this, &Manager::showMainWindow);
  connect(tray_.data(), &TrayIcon::quitRequested,  //
          this, &Manager::quit);

  taskView_->setWindowModality(Qt::WindowModal);

  connect(taskView_.data(), &TaskView::taskAdded,  //
          this, &Manager::requestAddTask);

  updateSettings(fileName);
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

void Manager::showMainWindow()
{
  SOFT_ASSERT(parser_, return );
  if (!mainWindow_) {
    taskModel_.reset(new TaskModel);
    mainWindow_.reset(new MainWindow(*taskModel_));
    populateTasksModel();
  }

  mainWindow_->show();
}

void Manager::quit()
{
  QApplication::quit();
}


void Manager::updateSettings(const QString& fileName)
{
  parser_.reset(new Parser(fileName));
  connect(this, &Manager::requestAddTask,  //
          parser_.get(), &Parser::append);
  connect(this, &Manager::requestLoadAll,  //
          parser_.get(), &Parser::loadAll);

  connect(parser_.get(), &Parser::loaded,  //
          this, [this](const QVector<Task> &tasks) {
            if (taskModel_)
              taskModel_->setTasks(tasks);
          });
  connect(parser_.get(), &Parser::appended,  //
          this, [this](const Task &task) {
            if (taskModel_)
              taskModel_->addTask(task);
          });

  populateTasksModel();
}

void Manager::populateTasksModel()
{
  if (!taskModel_)
    return;

  requestLoadAll();
}
