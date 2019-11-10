#include "debug.h"
#include "manager.h"

#include <QApplication>
#include <QCommandLineParser>
#include <QLockFile>
#include <QStandardPaths>

struct CmdLine {
  QString fileName;
};

CmdLine parseCmdLine(QApplication &a)
{
  QCommandLineParser parser;
  parser.setApplicationDescription(QObject::tr("Task journal helper"));
  parser.addHelpOption();
  parser.addVersionOption();

  parser.addPositionalArgument("journal",
                               QObject::tr("Journal file to maintain."));

  parser.process(a);

  CmdLine result;

  const auto args = parser.positionalArguments();
  if (!args.isEmpty()) {
    result.fileName = args.last();
  } else {
    result.fileName = "journal.md";
  }

  return result;
}

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  a.setApplicationName("TaskLog");
  a.setOrganizationName("Gres");
  QCoreApplication::setApplicationVersion("0.1");

  a.setQuitOnLastWindowClosed(false);

  const auto lockFileName =
      QStandardPaths::writableLocation(QStandardPaths::TempLocation) +
      QStringLiteral("/tasklog.lock");
  QLockFile lockFile(lockFileName);
  if (!lockFile.tryLock()) {
    LWARNING() << "Another instance is running. Lock file is busy.";
    return 0;
  }

  const auto cmdLine = parseCmdLine(a);
  Manager manager(cmdLine.fileName);

  return a.exec();
}
