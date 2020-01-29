#include "debug.h"
#include "manager.h"

#include <QApplication>
#include <QCommandLineParser>
#include <QLibraryInfo>
#include <QLockFile>
#include <QStandardPaths>
#include <QTranslator>

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  a.setApplicationName("TaskLog");
  a.setOrganizationName("Gres");
  a.setApplicationVersion(VERSION);

  a.setQuitOnLastWindowClosed(false);

  {
    auto translator = new QTranslator;
    const auto paths = QStringList{
        {},
        QLatin1String("translations"),
        QLibraryInfo::location(QLibraryInfo::TranslationsPath),
#ifdef Q_OS_LINUX
        qgetenv("APPDIR") + QLatin1String("/translations"),  // appimage
#endif
    };
    for (const auto &path : paths) {
      if (translator->load(QLocale(), QStringLiteral("tasklog"),
                           QStringLiteral("_"), path)) {
        a.installTranslator(translator);
        break;
      }
    }
  }

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
    LWARNING() << "Another instance is running. Lock file is busy." << lockFileName;
    return 0;
  }

  Manager manager;

  return a.exec();
}
