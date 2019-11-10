#pragma once

#include <QDateTime>
#include <QDebug>

struct Task {
  QString text;
  QDateTime date;
};

inline QDebug operator<<(QDebug s, const Task& v)
{
  s << "Task(" << v.date.toString(Qt::ISODate) << v.text << ")";
  return s;
}
