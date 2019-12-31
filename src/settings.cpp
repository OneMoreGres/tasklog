#include "settings.h"
#include "constants.h"

#include <QSettings>
#include <QStandardPaths>

Settings::Settings()
  : workingFileName_(
        QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) +
        QStringLiteral("/journal.md"))
  , addRecordHotkey_("Alt+F3")
  , keywordPrefixes_("#@")
{
}

void Settings::load()
{
  QSettings settings;

  const auto defaultFile = workingFileName_;
  workingFileName_ =
      settings.value(qs_workingFile, workingFileName_).toString();
  if (workingFileName_.isEmpty())
    workingFileName_ = defaultFile;

  addRecordHotkey_ =
      settings.value(qs_addHotkey, addRecordHotkey_.toString()).toString();

  keywordPrefixes_ =
      settings.value(qs_keywordPrefixes, keywordPrefixes_).toString();
}

void Settings::save()
{
  QSettings settings;
  settings.setValue(qs_workingFile, workingFileName_);
  settings.setValue(qs_addHotkey, addRecordHotkey_.toString());
  settings.setValue(qs_keywordPrefixes, keywordPrefixes_);
}

QString Settings::workingFileName() const
{
  return workingFileName_;
}

void Settings::setWorkingFileName(const QString &workingFileName)
{
  workingFileName_ = workingFileName;
}

QKeySequence Settings::addRecordHotkey() const
{
  return addRecordHotkey_;
}

void Settings::setAddRecordHotkey(const QKeySequence &addRecordHotkey)
{
  addRecordHotkey_ = addRecordHotkey;
}

QString Settings::keywordPrefixes() const
{
  return keywordPrefixes_;
}

void Settings::setKeywordPrefixes(const QString &keywordPrefixes)
{
  keywordPrefixes_ = keywordPrefixes;
}
