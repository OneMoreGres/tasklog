#include "manager.h"
#include "debug.h"
#include "mainwindow.h"
#include "parser.h"
#include "settings.h"
#include "settingseditor.h"
#include "task.h"
#include "taskmodel.h"
#include "taskview.h"
#include "trayicon.h"

#include <QApplication>
#include <QThread>

Manager::Manager()
  : tray_(new TrayIcon)
  , taskView_(new TaskView)
  , parser_(nullptr)
  , mainWindow_(nullptr)
  , taskModel_(nullptr)
  , worker_(new QThread(this))
{
  qRegisterMetaType<Task>();
  qRegisterMetaType<QVector<Task>>();
  connect(tray_.data(), &TrayIcon::addTaskRequested,  //
          this, &Manager::showTaskView);
  connect(tray_.data(), &TrayIcon::mainWindowRequested,  //
          this, &Manager::showMainWindow);
  connect(tray_.data(), &TrayIcon::settingsRequested,  //
          this, &Manager::showSettings);
  connect(tray_.data(), &TrayIcon::quitRequested,  //
          this, &Manager::quit);

  taskView_->setWindowModality(Qt::WindowModal);

  connect(taskView_.data(), &TaskView::taskAdded,  //
          this, &Manager::requestAddTask);

  worker_->start();

  updateSettings();
}

Manager::~Manager()
{
  worker_->quit();
  const auto timeoutMs = 5000;
  if (!worker_->wait(timeoutMs))
    worker_->terminate();
}

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
  if (!mainWindow_) {
    taskModel_.reset(new TaskModel);
    mainWindow_ = new MainWindow(*taskModel_);
    mainWindow_->setAttribute(Qt::WA_DeleteOnClose);
    reloadTasksAndKeywords();
    connect(mainWindow_, &MainWindow::destroyed,  //
            this, &Manager::destroyTasksModel);
    connect(mainWindow_, &MainWindow::saveAs,  //
            this, &Manager::saveAs);
  }

  mainWindow_->show();
}

void Manager::showSettings()
{
  if (!settingsEditor_) {
    settingsEditor_ = new SettingsEditor;
    settingsEditor_->setAttribute(Qt::WA_DeleteOnClose);
    connect(settingsEditor_, &SettingsEditor::settingsChanged,  //
            this, &Manager::updateSettings);
  }

  settingsEditor_->show();
}

void Manager::quit()
{
  QApplication::quit();
}

void Manager::saveAs(const QString &fileName, const QModelIndexList &indexes)
{
  SOFT_ASSERT(taskModel_, return );
  QVector<Task> tasks;
  Parser parser(fileName, {});
  for (const auto &i : indexes) {
    const auto task = taskModel_->task(i);
    if (task.isValid())
      parser.append(task);
  }
}

void Manager::updateSettings()
{
  Settings settings;
  settings.load();

  parser_.reset(
      new Parser(settings.workingFileName(), settings.keywordPrefixes()));
  connect(this, &Manager::requestAddTask,  //
          parser_.get(), &Parser::append);
  connect(this, &Manager::reloadTasksAndKeywords,  //
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
  connect(parser_.get(), &Parser::keywordsUpdated,  //
          this, [this](const QStringList &words) {
            if (mainWindow_)
              mainWindow_->setKeywords(words);
            SOFT_ASSERT(taskView_, return );
            taskView_->setKeywords(words);
          });
  parser_->moveToThread(worker_);

  SOFT_ASSERT(tray_, return );
  tray_->updateSettings(settings);

  reloadTasksAndKeywords();
}

void Manager::destroyTasksModel()
{
  taskModel_.reset();
}
