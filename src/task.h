#pragma once

#include <QDateTime>
#include <QDebug>

struct Task {
  QString text;
  QDateTime date;

  bool isValid() const { return date.isValid() && !text.isEmpty(); }
};

inline QDebug operator<<(QDebug s, const Task& v)
{
  s << "Task(" << v.date.toString(Qt::ISODate) << v.text << ")";
  return s;
}

Q_DECLARE_METATYPE(Task)
Q_DECLARE_METATYPE(QVector<Task>)
