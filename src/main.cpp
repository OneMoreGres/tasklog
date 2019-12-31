#include "debug.h"
#include "manager.h"

#include <QApplication>
#include <QCommandLineParser>
#include <QLockFile>
#include <QStandardPaths>

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  a.setApplicationName("TaskLog");
  a.setOrganizationName("Gres");
  a.setApplicationVersion(VERSION);

  a.setQuitOnLastWindowClosed(false);

  {
    QCommandLineParser parser;
    parser.setApplicationDescription(QObject::tr("Task journal helper"));
    parser.addHelpOption();
    parser.addVersionOption();

    parser.process(a);
  }

  const auto lockFileName =
      QStandardPaths::writableLocation(QStandardPaths::TempLocation) +
      QStringLiteral("/tasklog.lock");
  QLockFile lockFile(lockFileName);
  if (!lockFile.tryLock()) {
    LWARNING() << "Another instance is running. Lock file is busy.";
    return 0;
  }

  Manager manager;

  return a.exec();
}
