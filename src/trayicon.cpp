#include "trayicon.h"
#include "globalaction.h"
#include "settings.h"

#include <QMenu>

TrayIcon::TrayIcon()
  : tray_(new QSystemTrayIcon(this))
  , addRecordAction_(nullptr)
{
  GlobalAction::init();

  auto menu = new QMenu;
  {
    auto action = menu->addAction(tr("Add record"));
    addRecordAction_ = action;
    connect(action, &QAction::triggered,  //
            this, &TrayIcon::addTaskRequested);
  }
  {
    auto action = menu->addAction(tr("Show existing"));
    connect(action, &QAction::triggered,  //
            this, &TrayIcon::mainWindowRequested);
  }
  {
    auto action = menu->addAction(tr("Settings"));
    connect(action, &QAction::triggered,  //
            this, &TrayIcon::settingsRequested);
  }
  menu->addSeparator();
  {
    auto action = menu->addAction(tr("Quit"));
    connect(action, &QAction::triggered,  //
            this, &TrayIcon::quitRequested);
  }

  tray_->setContextMenu(menu);
  tray_->setIcon(QIcon(QLatin1String(":icons/app.png")));
  tray_->show();
}

void TrayIcon::updateSettings(const Settings &settings)
{
  tray_->setToolTip(settings.workingFileName());

  if (!addRecordAction_->shortcut().isEmpty())
    GlobalAction::removeGlobal(addRecordAction_);
  if (settings.addRecordHotkey().isEmpty())
    return;
  addRecordAction_->setShortcut(settings.addRecordHotkey());
  GlobalAction::makeGlobal(addRecordAction_);
}

TrayIcon::~TrayIcon() = default;
