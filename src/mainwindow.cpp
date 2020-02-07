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

  view_->installEventFilter(this);

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
  view_->setContextMenuPolicy(Qt::CustomContextMenu);
  connect(view_, &QTableView::customContextMenuRequested,  //
          this, &MainWindow::showContextMenu);

  {
    auto menu = new QMenu(tr("File"));
    {
      auto action = menu->addAction(tr("Filter"));
      action->setShortcut(QKeySequence("Ctrl+F"));
      connect(action, &QAction::triggered,  //
              this, &MainWindow::focusFilter);
    }
    {
      auto action = saveAs_ = menu->addAction(tr("Save as..."));
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

void MainWindow::showContextMenu()
{
  QMenu menu;
  menu.addAction(saveAs_);
  menu.exec(QCursor::pos());
}

bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
  auto consume = false;
  if (event->type() != QEvent::KeyPress)
    return consume;

  const auto casted = static_cast<QKeyEvent *>(event);
  const auto key = casted->key();
  if (watched == filter_) {
    if (key == Qt::Key_Down || key == Qt::Key_Up) {
      view_->setFocus();
      const auto isUp = key == Qt::Key_Up;
      const auto row = isUp ? proxy_->rowCount() - 1 : 0;
      view_->selectRow(row);
    } else if (casted->key() == Qt::Key_Escape) {
      filter_->clear();
    }
  } else if (watched == view_) {
    if (key == Qt::Key_Down || key == Qt::Key_Up) {
      const auto current = view_->currentIndex().row();
      const auto isUp = key == Qt::Key_Up;
      auto row = isUp ? current - 1 : current + 1;
      if (row < 0)
        row = proxy_->rowCount() - 1;
      else if (row >= proxy_->rowCount())
        row = 0;
      view_->selectRow(row);
      consume = true;
    }
    if (key == Qt::Key_Escape) {
      filter_->setFocus();
    }
  }

  return consume;
}
