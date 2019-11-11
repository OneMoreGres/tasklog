#include "mainwindow.h"
#include "constants.h"
#include "debug.h"
#include "taskmodel.h"

#include <QBoxLayout>
#include <QHeaderView>
#include <QSettings>
#include <QTableView>

MainWindow::MainWindow(TaskModel &model, QWidget *parent)
  : QMainWindow(parent)
  , model_(model)
  , view_(new QTableView(this))
{
  setCentralWidget(view_);

  view_->setModel(&model_);
  view_->horizontalHeader()->setStretchLastSection(true);
  view_->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);

  restoreState();
}

MainWindow::~MainWindow()
{
  saveState();
}

void MainWindow::saveState()
{
  QSettings settings;
  settings.beginGroup(qs_mainWindowGroup);
  settings.setValue(qs_geometry, saveGeometry());
  settings.endGroup();
}

void MainWindow::restoreState()
{
  QSettings settings;
  settings.beginGroup(qs_mainWindowGroup);
  if (settings.contains(qs_geometry))
    restoreGeometry(settings.value(qs_geometry).toByteArray());
  settings.endGroup();
}
