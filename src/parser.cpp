#include "parser.h"
#include "debug.h"
#include "task.h"

#include <QFile>

Parser::Parser(const QString &fileName)
  : fileName_(fileName)
{
}

QVector<Task> Parser::loadAll() const
{
  return {};
}

bool Parser::append(const Task &task)
{
  SOFT_ASSERT(task.isValid(), return false);
  if (fileName_.isEmpty()) {
    LERROR() << "File not set";
    return false;
  }

  QFile f(fileName_);
  if (!f.open(QFile::ReadWrite)) {
    LERROR() << "Failed to open file" << LARG(fileName_)
             << LARG(f.errorString());
    return false;
  }

  const auto addPos = f.size();
  if (!f.seek(addPos)) {
    LERROR() << "Failed to position at file" << LARG(fileName_)
             << LARG(f.errorString());
    return false;
  }

  if (addPos == 0) {
    const auto heading = QObject::tr("Completed tasks list:\n").toUtf8();
    const auto written = f.write(heading);
    if (written != heading.size()) {
      LERROR() << "Failed to write file heading" << LARG(fileName_)
               << LARG(f.errorString());
      return false;
    }
  }

  const auto stringed =
      (QStringLiteral("\n* ") + task.date.toString(Qt::ISODate) +
       QStringLiteral(" ") + task.text + QStringLiteral("\n"))
          .toUtf8();
  const auto written = f.write(stringed);
  if (written != stringed.size()) {
    LERROR() << "Failed to write record" << LARG(fileName_)
             << LARG(f.errorString());
    return false;
  }

  return true;
}
