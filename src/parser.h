#pragma once

#include <QString>

class Task;

class Parser
{
public:
  explicit Parser(const QString &fileName);

  void append(const Task &task);

private:
  QString fileName_;
};
