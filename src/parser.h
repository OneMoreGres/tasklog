#pragma once

#include <QObject>

class Task;

class Parser : public QObject
{
  Q_OBJECT
public:
  explicit Parser(const QString& fileName);
  ~Parser();

  void loadAll();
  void append(const Task& task);

signals:
  void loaded(const QVector<Task>& tasks);
  void appended(const Task& task);

private:
  QString fileName_;
};
