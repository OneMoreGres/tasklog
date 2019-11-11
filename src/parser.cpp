#include "parser.h"
#include "debug.h"
#include "task.h"

#include <QFile>

namespace
{
const auto listMark = QLatin1Char('*');

struct TaskExtractor {
  QVector<Task> tasks;
  QString current;
  void parseText()
  {
    if (current.startsWith(listMark))
      current = current.mid(1).trimmed();

    const auto spaceIndex = current.indexOf(QLatin1Char(' '));
    const auto dateString = current.left(spaceIndex);
    Task task;
    task.date = QDateTime::fromString(dateString, Qt::ISODate);

    if (!task.date.isValid())
      return;

    task.text = current.mid(spaceIndex);

    if (task.isValid())
      tasks.append(task);
  }
  void addText(const QString &text) { current += text; }
  void setText(const QString &text) { current = text; }
};
}  // namespace

Parser::Parser(const QString &fileName)
  : fileName_(fileName)
{
}

QVector<Task> Parser::loadAll() const
{
  QFile f(fileName_);
  if (!f.open(QFile::ReadWrite)) {
    LERROR() << "Failed to open file for reading" << LARG(fileName_)
             << LARG(f.errorString());
    return {};
  }

  TaskExtractor extractor;
  while (!f.atEnd()) {
    const auto line = QString::fromUtf8(f.readLine());

    if (line.isEmpty() || line.startsWith(listMark)) {
      extractor.parseText();
      extractor.setText(line);
      continue;
    }

    extractor.addText(line);
  }
  extractor.parseText();

  return extractor.tasks;
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
    LERROR() << "Failed to open file for writing" << LARG(fileName_)
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
