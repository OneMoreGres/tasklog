#pragma once

#include <QByteArray>
#include <QString>

class FileWriter
{
public:
  explicit FileWriter(const QString &fileName);

  bool writeAt(qint64 pos, const QByteArray &data);

private:
  bool append(const QByteArray &data);
  bool insert(qint64 pos, const QByteArray &data);

  QString fileName_;
};
