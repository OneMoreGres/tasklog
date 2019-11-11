#pragma once

#include <QString>

class Task;

class Parser
{
public:
  explicit Parser(const QString &fileName);

  QVector<Task> loadAll() const;

  bool append(const Task &task);

private:
  QString fileName_;
};
