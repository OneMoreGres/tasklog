#include "filewriter.h"
#include "debug.h"

#include <QFileInfo>
#include <QSaveFile>

FileWriter::FileWriter(const QString &fileName)
  : fileName_(fileName)
{
}

bool FileWriter::writeAt(qint64 pos, const QByteArray &data)
{
  QFileInfo info(fileName_);
  if (!info.exists() || pos == info.size())
    return append(data);
  return insert(pos, data);
}

bool FileWriter::append(const QByteArray &data)
{
  QFile f(fileName_);
  if (!f.open(QFile::WriteOnly | QFile::Append)) {
    qDebug() << "Failed to open file for writing" << LARG(fileName_)
             << LARG(f.errorString());
    return false;
  }

  const auto written = f.write(data);
  if (written != data.size()) {
    LERROR() << "Failed to write record" << LARG(fileName_)
             << LARG(f.errorString());
    return false;
  }

  return true;
}

bool FileWriter::insert(qint64 pos, const QByteArray &data)
{
  QFile in(fileName_);
  if (!in.open(QFile::ReadOnly)) {
    qDebug() << "Failed to open file for reading" << LARG(fileName_)
             << LARG(in.errorString());
    return false;
  }

  QSaveFile writer(fileName_);
  if (!writer.open(QFile::WriteOnly)) {
    qDebug() << "Failed to open file for writing" << LARG(fileName_)
             << LARG(writer.errorString());
    return false;
  }

  writer.write(in.read(pos));
  writer.write(data);
  writer.write(in.readAll());
  in.close();

  auto result = writer.commit();
  if (!result) {
    qDebug() << "Failed to write file" << LARG(fileName_)
             << LARG(writer.errorString());
    return false;
  }

  return true;
}
