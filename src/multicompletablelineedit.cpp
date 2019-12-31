#include "multicompletablelineedit.h"
#include "debug.h"

#include <QAbstractItemView>
#include <QCompleter>
#include <QScrollBar>
#include <QStringListModel>

CompletableLineEdit::CompletableLineEdit(QWidget *parent)
  : QLineEdit(parent)
  , completerModel_(new QStringListModel(this))
  , completer_(new QCompleter(completerModel_, this))
{
  completer_->setCaseSensitivity(Qt::CaseInsensitive);
  completer_->setWidget(this);
  connect(completer_, qOverload<const QString &>(&QCompleter::activated),  //
          this, &CompletableLineEdit::insertCompletion);
}

void CompletableLineEdit::setCompletions(const QStringList &words)
{
  SOFT_ASSERT(completerModel_, return );
  completerModel_->setStringList(words);
}

void CompletableLineEdit::keyPressEvent(QKeyEvent *event)
{
  QLineEdit::keyPressEvent(event);
  SOFT_ASSERT(completer_, return );
  SOFT_ASSERT(completer_, return );

  completer_->setCompletionPrefix(cursorWord());
  if (completer_->completionPrefix().isEmpty()) {
    completer_->popup()->hide();
    return;
  }

  auto rect = cursorRect();
  rect.setWidth(completer_->popup()->sizeHintForColumn(0) +
                completer_->popup()->verticalScrollBar()->sizeHint().width());

  completer_->complete(rect);
}

QPair<int, int> CompletableLineEdit::currentWordBoundaries() const
{
  const auto all = text();
  const auto cursorIndex = cursorPosition();
  const auto left =
      cursorIndex == 0 ? 0 : all.lastIndexOf(' ', cursorIndex - 1) + 1;
  auto right = all.indexOf(' ', cursorIndex);
  if (right == -1)
    right = all.size();
  return {left, right};
}

QString CompletableLineEdit::cursorWord() const
{
  const auto b = currentWordBoundaries();
  const auto all = text();
  const auto result = all.mid(b.first, b.second - b.first);
  return result;
}

void CompletableLineEdit::insertCompletion(const QString &completion)
{
  const auto b = currentWordBoundaries();
  const auto all = text();
  setText(all.left(b.first) + completion + all.mid(b.second));
}
