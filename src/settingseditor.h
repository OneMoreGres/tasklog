#pragma once

#include <QWidget>

class QLineEdit;
class QToolButton;
class QKeySequenceEdit;

class SettingsEditor : public QWidget
{
  Q_OBJECT
public:
  explicit SettingsEditor(QWidget* parent = nullptr);

signals:
  void settingsChanged();

private:
  void showSettings();
  void saveSettings();
  void selectWorkingFile();

  QLineEdit* workingFile_;
  QLineEdit* keywordPrefixes_;
  QToolButton* selectWorkingFile_;
  QKeySequenceEdit* addRecordHotkey_;
};
