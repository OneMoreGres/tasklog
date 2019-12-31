#pragma once

#include <QKeySequence>

class Settings
{
public:
  Settings();

  void load();
  void save();

  QString workingFileName() const;
  void setWorkingFileName(const QString &workingFileName);

  QKeySequence addRecordHotkey() const;
  void setAddRecordHotkey(const QKeySequence &addRecordHotkey);

  QString keywordPrefixes() const;
  void setKeywordPrefixes(const QString &keywordPrefixes);

private:
  QString workingFileName_;
  QKeySequence addRecordHotkey_;
  QString keywordPrefixes_;
};
