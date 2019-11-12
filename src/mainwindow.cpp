#include "mainwindow.h"
#include "constants.h"
#include "debug.h"
#include "taskmodel.h"

#include <QBoxLayout>
#include <QHeaderView>
#include <QLineEdit>
#include <QSettings>
#include <QSortFilterProxyModel>
#include <QTableView>

MainWindow::MainWindow(TaskModel &model, QWidget *parent)
  : QMainWindow(parent)
  , model_(model)
  , proxy_(new QSortFilterProxyModel(this))
  , view_(new QTableView(this))
  , filter_(new QLineEdit(this))
{
  setCentralWidget(new QWidget);
  auto layout = new QVBoxLayout(centralWidget());
  layout->addWidget(filter_);
  layout->addWidget(view_);

  filter_->setPlaceholderText(tr("Type to filter..."));

  connect(filter_, &QLineEdit::textChanged,  //
          this, &MainWindow::applyFilter);

  proxy_->setFilterCaseSensitivity(Qt::CaseSensitivity::CaseInsensitive);
  proxy_->setDynamicSortFilter(true);
  proxy_->setFilterKeyColumn(int(TaskModel::Column::Name));
  proxy_->setSourceModel(&model);

  view_->setModel(proxy_);
  const auto dateSampleWidth =
      view_->fontMetrics().horizontalAdvance("2020:10:10:20:20:20");
  view_->horizontalHeader()->resizeSection(int(TaskModel::Column::Date),
                                           dateSampleWidth);
  view_->horizontalHeader()->setStretchLastSection(true);
  view_->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);
  view_->setSortingEnabled(true);
  view_->horizontalHeader()->setSortIndicator(int(TaskModel::Column::Date),
                                              Qt::AscendingOrder);

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

void MainWindow::applyFilter()
{
  const auto filter = filter_->text();
  proxy_->setFilterFixedString(filter);
}
