#pragma once

#include <QLineEdit>

class QStringListModel;

class CompletableLineEdit final : public QLineEdit
{
  Q_OBJECT
public:
  explicit CompletableLineEdit(QWidget* parent);

  void setCompletions(const QStringList& words);

protected:
  void keyPressEvent(QKeyEvent* event) override;

private:
  QPair<int, int> currentWordBoundaries() const;
  QString cursorWord() const;
  void insertCompletion(const QString& completion);

  QStringListModel* completerModel_;
  QCompleter* completer_;
};
