#include "parser.h"
#include "debug.h"
#include "filewriter.h"
#include "task.h"

#include <QFileInfo>

namespace
{
struct MdFormat {
  const QChar listMark = QLatin1Char('*');
  const QChar headingMark = QLatin1Char('#');
  const QString dateFormat = "yyyy.MM.dd";

  QVector<Task> tasks;
  QString currentText;
  QDate currentDate;
  bool isFirst{true};
  enum class Type { Date, Task, None };

  QVector<Task> loadTasks(const QString &fileName)
  {
    QFile f(fileName);
    if (!f.open(QFile::ReadWrite)) {
      LERROR() << QObject::tr("Failed to open file for reading")
               << LARG(fileName) << LARG(f.errorString());
      return {};
    }

    isFirst = true;
    while (!f.atEnd()) {
      const auto line = QString::fromUtf8(f.readLine()).trimmed();
      parseLine(line);
    }
    parseLine();

    return tasks;
  }

  bool appendTask(const QString &fileName, const Task &task)
  {
    SOFT_ASSERT(task.isValid(), return false)
    if (fileName.isEmpty()) {
      LERROR() << QObject::tr("File for saving task not set");
      return false;
    }

    tasks.clear();

    const auto addState = taskPosition(fileName, task);

    QByteArray data;
    if (addState.position == 0)
      data += QObject::tr("# Completed tasks list:\n").toUtf8();

    if (addState.mustWriteDate)
      data += QString("\n## " + task.date.toString(dateFormat) + "\n").toUtf8();

    const auto time = task.date.time();
    const auto timeString = time.msecsSinceStartOfDay() > 0
                                ? time.toString(Qt::ISODate) + QLatin1Char(' ')
                                : QString();
    const auto stringed = QStringLiteral("* ") + timeString + task.text;
    data += (QLatin1Char('\n') + splitLongText(stringed) + QLatin1Char('\n'))
                .toUtf8();

    FileWriter writer(fileName);
    return writer.writeAt(addState.position, data);
  }

  QStringList parseKeywords(const QString &text,
                            const QString &keywordPrefixes) const
  {
    if (keywordPrefixes.isEmpty())
      return {};

    QStringList result;
    const auto parts = text.split(' ', QString::SkipEmptyParts);
    for (const auto &part : parts) {
      if (part.size() < 2)
        continue;
      if (keywordPrefixes.contains(part[0]))
        result.append(part.mid(1));
    }
    return result;
  }

private:
  struct InsertState {
    int position{0};
    bool mustWriteDate{true};
  };
  InsertState taskPosition(const QString &fileName, const Task &task)
  {
    SOFT_ASSERT(!fileName.isEmpty(), return {});

    QFile f(fileName);
    InsertState result;
    if (f.open(QFile::ReadOnly)) {
      result.position = f.size();

      const auto taskDate = task.date.date();
      auto repeat = !f.atEnd();

      while (repeat) {
        repeat = !f.atEnd();
        const auto current = f.pos();
        const auto line =
            !f.atEnd() ? QString::fromUtf8(f.readLine()).trimmed() : QString();

        const auto parseResult = parseLine(line);
        if (parseResult == Type::None)
          continue;

        if (parseResult == Type::Date) {
          if (currentDate == taskDate) {
            result.mustWriteDate = false;
            result.position = current;
          }
          continue;
        }

        if (tasks.isEmpty())
          continue;
        const auto &last = tasks.last();
        if (last.date > task.date)
          break;
        result.mustWriteDate = last.date.date() != task.date.date();
        result.position = current;
      }

      f.close();
    }
    return result;
  }

  Type parseLine(const QString &line = {})
  {
    const auto separators = QVector<QChar>{listMark, headingMark};
    const auto isNewEntry = line.isEmpty() || separators.contains(line[0]);
    if (!isNewEntry) {
      if (!line.isEmpty() && !currentText.isEmpty())
        currentText += QLatin1Char(' ');
      currentText += line;
      return Type::None;
    }

    auto result = Type::None;

    if (!currentText.isEmpty())
      result = parseCurrent();

    currentText = line;
    return result;
  }

  Type parseCurrent()
  {
    const auto isFirst = this->isFirst;
    this->isFirst = false;
    auto start = 0;
    const auto separators =
        QVector<QChar>{listMark, headingMark, QLatin1Char(' ')};
    for (const auto end = currentText.size(); start < end; ++start) {
      if (!separators.contains(currentText[start]))
        break;
    }

    const auto space = currentText.indexOf(QLatin1Char(' '), start);
    const auto firstWord = currentText.mid(start, space - start);

    if (currentText.startsWith(headingMark)) {
      currentDate = parseDate(firstWord);
      if (!currentDate.isValid() && !isFirst)
        LDEBUG() << "Ignored heading" << currentText;
      return currentDate.isValid() ? Type::Date : Type::None;
    }

    Task task;
    auto timeParsed = false;
    std::tie(task.date, timeParsed) = parseDateTime(firstWord);
    currentDate = task.date.date();
    task.text =
        timeParsed ? currentText.mid(space + 1) : currentText.mid(start);

    if (!task.isValid()) {
      LDEBUG() << "Ignored task entry" << currentText;
      return Type::None;
    }

    tasks.append(task);
    return Type::Task;
  }

  QDate parseDate(const QString &text) const
  {
    auto date = QDate::fromString(text, Qt::ISODate);
    if (!date.isValid())
      date = QDate::fromString(text, dateFormat);
    if (!date.isValid())
      date = QDate::fromString(text, "dd.MM.yyyy");
    return date;
  }

  std::pair<QDateTime, bool> parseDateTime(const QString &text) const
  {
    auto dateTime = QDateTime::fromString(text, Qt::ISODate);
    if (dateTime.isValid())
      return {dateTime, true};

    dateTime.setDate(currentDate);
    auto time = QTime::fromString(text, Qt::ISODate);
    if (!time.isValid())
      time = QTime::fromString(text, "hh:mm");
    dateTime.setTime(time);

    return {dateTime, time.isValid()};
  }

  QString splitLongText(const QString &text)
  {
    const auto maxLen = 80;
    const auto size = text.size();
    if (size <= maxLen)
      return text;

    QString result;
    auto endIndex = 0;
    auto startIndex = 0;
    while (endIndex != -1) {
      auto next = text.indexOf(QLatin1Char(' '), endIndex + 1);
      if (next == -1) {
        result += text.mid(startIndex);
        break;
      }
      if (next - startIndex > 80) {
        result +=
            text.mid(startIndex, endIndex - startIndex) + QLatin1Char('\n');
        startIndex = endIndex + 1;
        continue;
      }
      endIndex = next;
    }
    return result;
  }
};
}  // namespace

Parser::Parser(const QString &fileName, const QString &keywordPrefixes)
  : fileName_(fileName)
  , keywordPrefixes_(keywordPrefixes)
{
}

Parser::~Parser() = default;

void Parser::loadAll()
{
  MdFormat format;
  const auto result = format.loadTasks(fileName_);
  emit loaded(result);
}

void Parser::append(const Task &task)
{
  MdFormat format;
  const auto result = format.appendTask(fileName_, task);
  if (!result)
    return;

  emit appended(task);

  auto keywords = format.parseKeywords(task.text, keywordPrefixes_);
  auto added = false;
  for (const auto &i : keywords) {
    if (keywords_.contains(i))
      continue;
    auto it = std::lower_bound(keywords_.begin(), keywords_.end(), i);
    keywords_.insert(it, i);
    added = true;
  }

  if (added)
    emit keywordsUpdated(keywords_);
}

void Parser::parseKeywords()
{
  MdFormat format;
  const auto tasks = format.loadTasks(fileName_);

  keywords_.clear();

  for (const auto &task : tasks) {
    const auto words = format.parseKeywords(task.text, keywordPrefixes_);
    for (const auto &word : words) {
      if (!keywords_.contains(word))
        keywords_.append(word);
    }
  }

  std::sort(keywords_.begin(), keywords_.end());
  emit keywordsUpdated(keywords_);
}
