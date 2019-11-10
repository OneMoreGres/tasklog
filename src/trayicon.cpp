#include "trayicon.h"

#include <QMenu>
#include <QSystemTrayIcon>

TrayIcon::TrayIcon()
  : tray_(new QSystemTrayIcon(this))
{
  auto menu = new QMenu;
  {
    auto action = menu->addAction(tr("Add record"));
    connect(action, &QAction::triggered,  //
            this, &TrayIcon::addTaskRequested);
  }
  {
    auto action = menu->addAction(tr("Show existing"));
    connect(action, &QAction::triggered,  //
            this, &TrayIcon::mainWindowRequested);
  }
  menu->addSeparator();
  {
    auto action = menu->addAction(tr("Quit"));
    connect(action, &QAction::triggered,  //
            this, &TrayIcon::quitRequested);
  }

  tray_->setContextMenu(menu);
  tray_->setIcon(QIcon::fromTheme("edit-find"));
  tray_->show();
}

TrayIcon::~TrayIcon() = default;
