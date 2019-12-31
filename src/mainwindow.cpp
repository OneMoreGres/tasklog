#include "mainwindow.h"
#include "constants.h"
#include "debug.h"
#include "taskmodel.h"

#include <QBoxLayout>
#include <QFileDialog>
#include <QHeaderView>
#include <QKeyEvent>
#include <QLineEdit>
#include <QMenuBar>
#include <QSettings>
#include <QSortFilterProxyModel>
#include <QTableView>

MainWindow::MainWindow(TaskModel &model, QWidget *parent)
  : QMainWindow(parent)
  , proxy_(new QSortFilterProxyModel(this))
  , view_(new QTableView(this))
  , filter_(new QLineEdit(this))
{
  setCentralWidget(new QWidget);
  auto layout = new QVBoxLayout(centralWidget());
  layout->addWidget(filter_);
  layout->addWidget(view_);

  filter_->setPlaceholderText(tr("Type to filter..."));
  filter_->installEventFilter(this);
  filter_->setFocus();

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

  {
    auto menu = new QMenu(tr("File"));
    {
      auto action = menu->addAction(tr("Filter"));
      action->setShortcut(QKeySequence("Ctrl+F"));
      connect(action, &QAction::triggered,  //
              this, &MainWindow::focusFilter);
    }
    {
      auto action = menu->addAction(tr("Save as..."));
      action->setShortcut(QKeySequence("Ctrl+S"));
      connect(action, &QAction::triggered,  //
              this, &MainWindow::promptSaveAs);
    }
    menuBar()->addMenu(menu);
  }

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

void MainWindow::focusFilter()
{
  filter_->setFocus();
}

void MainWindow::promptSaveAs()
{
  const auto fileName = QFileDialog::getSaveFileName(this, tr("Save file"));
  if (fileName.isEmpty())
    return;
  const auto selection = view_->selectionModel();
  SOFT_ASSERT(selection, return );
  const auto indexes = selection->selectedRows();
  QModelIndexList sourceIndexes;
  if (!indexes.isEmpty()) {
    sourceIndexes.reserve(indexes.size());
    for (const auto &i : indexes) {
      sourceIndexes.append(proxy_->mapToSource(i));
    }
  } else {
    sourceIndexes.reserve(proxy_->rowCount());
    for (auto i = 0, end = proxy_->rowCount(); i < end; ++i) {
      const auto index = proxy_->index(i, 0);
      sourceIndexes.append(proxy_->mapToSource(index));
    }
  }

  if (sourceIndexes.isEmpty())
    return;

  emit saveAs(fileName, sourceIndexes);
}

bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
  const auto consume = false;
  if (watched != filter_ || event->type() != QEvent::KeyPress)
    return consume;

  auto casted = static_cast<QKeyEvent *>(event);
  if (casted->key() == Qt::Key_Down) {
    view_->setFocus();
    view_->selectRow(0);
  } else if (casted->key() == Qt::Key_Escape) {
    filter_->clear();
  }

  return consume;
}
