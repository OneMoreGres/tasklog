#pragma once

#include <QObject>

class Task;

class Parser : public QObject
{
  Q_OBJECT
public:
  explicit Parser(const QString& fileName, const QString& keywordPrefixes);
  ~Parser();

  void loadAll();
  void append(const Task& task);
  void parseKeywords();

signals:
  void loaded(const QVector<Task>& tasks);
  void appended(const Task& task);
  void keywordsUpdated(const QStringList& words);

private:
  QString fileName_;
  QString keywordPrefixes_;
  QStringList keywords_;
};
