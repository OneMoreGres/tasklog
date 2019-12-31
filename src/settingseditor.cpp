#include "settingseditor.h"
#include "settings.h"

#include <QDialogButtonBox>
#include <QFileDialog>
#include <QFormLayout>
#include <QKeySequenceEdit>
#include <QLineEdit>
#include <QToolButton>

SettingsEditor::SettingsEditor(QWidget *parent)
  : QWidget(parent)
  , workingFile_(new QLineEdit)
  , keywordPrefixes_(new QLineEdit)
  , selectWorkingFile_(new QToolButton())
  , addRecordHotkey_(new QKeySequenceEdit)
{
  setWindowTitle(tr("Settings"));

  auto layout = new QFormLayout(this);
  {
    auto subLayout = new QHBoxLayout;
    subLayout->addWidget(workingFile_);
    subLayout->addWidget(selectWorkingFile_);
    layout->addRow(tr("Working file:"), subLayout);

    selectWorkingFile_->setText("...");
    connect(selectWorkingFile_, &QToolButton::clicked,  //
            this, &SettingsEditor::selectWorkingFile);
  }

  layout->addRow(tr("Keyword prefixes:"), keywordPrefixes_);

  layout->addRow(tr("Add record hotkey:"), addRecordHotkey_);

  {
    using B = QDialogButtonBox::StandardButton;
    auto buttons = new QDialogButtonBox(B::Ok | B::Cancel);
    layout->addRow(buttons);
    connect(buttons, &QDialogButtonBox::accepted,  //
            this, &SettingsEditor::saveSettings);
    connect(buttons, &QDialogButtonBox::accepted,  //
            this, &SettingsEditor::close);
    connect(buttons, &QDialogButtonBox::rejected,  //
            this, &SettingsEditor::close);
  }
  showSettings();
}

void SettingsEditor::showSettings()
{
  Settings settings;
  settings.load();
  workingFile_->setText(settings.workingFileName());
  addRecordHotkey_->setKeySequence(settings.addRecordHotkey());
  keywordPrefixes_->setText(settings.keywordPrefixes());
}

void SettingsEditor::saveSettings()
{
  Settings settings;
  settings.setWorkingFileName(workingFile_->text());
  settings.setAddRecordHotkey(addRecordHotkey_->keySequence());
  settings.setKeywordPrefixes(keywordPrefixes_->text());
  settings.save();
  emit settingsChanged();
}

void SettingsEditor::selectWorkingFile()
{
  const auto fileName = QFileDialog::getSaveFileName();
  if (fileName.isEmpty())
    return;
  workingFile_->setText(fileName);
}
