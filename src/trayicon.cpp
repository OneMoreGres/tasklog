#include "trayicon.h"

#include <QSystemTrayIcon>

TrayIcon::TrayIcon()
  : tray_(new QSystemTrayIcon(this))
{
}

TrayIcon::~TrayIcon() = default;
