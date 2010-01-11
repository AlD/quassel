/***************************************************************************
 *   Copyright (C) 2005-09 by the Quassel Project                          *
 *   devel@quassel-irc.org                                                 *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) version 3.                                           *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef CHATITEM_H_
#define CHATITEM_H_

#include <QAction>
#include <QGraphicsItem>
#include <QObject>

#include "chatlinemodel.h"
#include "chatscene.h"
#include "clickable.h"
#include "uistyle.h"
#include "qtui.h"

#include <QTextLayout>

class ChatItem : public QGraphicsItem {
protected:
  ChatItem(const qreal &width, const qreal &height, const QPointF &pos, QGraphicsItem *parent);

public:
  const QAbstractItemModel *model() const;
  int row() const;
  virtual ChatLineModel::ColumnType column() const = 0;
  inline ChatScene *chatScene() const { return qobject_cast<ChatScene *>(scene()); }

  inline QRectF boundingRect() const { return _boundingRect; }
  inline qreal width() const { return _boundingRect.width(); }
  inline qreal height() const { return _boundingRect.height(); }

  void initLayoutHelper(QTextLayout *layout, QTextOption::WrapMode, Qt::Alignment = Qt::AlignLeft) const;
  virtual inline void initLayout(QTextLayout *layout) const {
    initLayoutHelper(layout, QTextOption::NoWrap);
    doLayout(layout);
  }
  virtual void doLayout(QTextLayout *) const;
  virtual UiStyle::FormatList formatList() const;

  virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
  enum { Type = ChatScene::ChatItemType };
  virtual inline int type() const { return Type; }

  QVariant data(int role) const;

  // selection stuff, to be called by the scene
  QString selection() const;
  void clearSelection();
  void setFullSelection();
  void continueSelecting(const QPointF &pos);
  bool hasSelection() const;
  bool isPosOverSelection(const QPointF &pos) const;

  QList<QRectF> findWords(const QString &searchWord, Qt::CaseSensitivity caseSensitive);

  virtual void addActionsToMenu(QMenu *menu, const QPointF &itemPos);
  virtual void handleClick(const QPointF &pos, ChatScene::ClickMode);

protected:
  enum SelectionMode {
    NoSelection,
    PartialSelection,
    FullSelection
  };

  virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
  virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
  virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

  void paintBackground(QPainter *);
  QVector<QTextLayout::FormatRange> selectionFormats() const;
  virtual QVector<QTextLayout::FormatRange> additionalFormats() const;
  void overlayFormat(UiStyle::FormatList &fmtList, int start, int end, quint32 overlayFmt) const;

  inline qint16 selectionStart() const { return _selectionStart; }
  inline void setSelectionStart(qint16 start) { _selectionStart = start; }
  inline qint16 selectionEnd() const { return _selectionEnd; }
  inline void setSelectionEnd(qint16 end) { _selectionEnd = end; }
  inline SelectionMode selectionMode() const { return _selectionMode; }
  inline void setSelectionMode(SelectionMode mode) { _selectionMode = mode; }
  void setSelection(SelectionMode mode, qint16 selectionStart, qint16 selectionEnd);

  qint16 posToCursor(const QPointF &pos) const;

  // WARNING: setGeometry and setHeight should not be used without either:
  //  a) calling prepareGeometryChange() immediately before setColumns()
  //  b) calling Chatline::setPos() immediately afterwards
  inline void setGeometry(qreal width, qreal height) {
    _boundingRect.setWidth(width);
    _boundingRect.setHeight(height);
  }
  inline void setHeight(const qreal &height) {
    _boundingRect.setHeight(height);
  }
  inline void setWidth(const qreal &width) {
    _boundingRect.setWidth(width);
  }

private:
  // internal selection stuff
  void setSelection(int start, int length);

  QRectF _boundingRect;

  SelectionMode _selectionMode;
  qint16 _selectionStart, _selectionEnd;

  friend class ChatLine;
};

// ************************************************************
// TimestampChatItem
// ************************************************************

//! A ChatItem for the timestamp column
class TimestampChatItem : public ChatItem {
public:
  TimestampChatItem(const qreal &width, const qreal &height, QGraphicsItem *parent) : ChatItem(width, height, QPointF(0, 0), parent) {}
  enum { Type = ChatScene::TimestampChatItemType };
  virtual inline int type() const { return Type; }
  virtual inline ChatLineModel::ColumnType column() const { return ChatLineModel::TimestampColumn; }
};

// ************************************************************
// SenderChatItem
// ************************************************************
//! A ChatItem for the sender column
class SenderChatItem : public ChatItem {
public:
  SenderChatItem(const qreal &width, const qreal &height, const QPointF &pos, QGraphicsItem *parent) : ChatItem(width, height, pos, parent) {}
  virtual inline ChatLineModel::ColumnType column() const { return ChatLineModel::SenderColumn; }

protected:
  virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
  enum { Type = ChatScene::SenderChatItemType };
  virtual inline int type() const { return Type; }
  virtual inline void initLayout(QTextLayout *layout) const {
    initLayoutHelper(layout, QTextOption::ManualWrap, Qt::AlignRight);
    doLayout(layout);
  }
};

// ************************************************************
// ContentsChatItem
// ************************************************************
struct ContentsChatItemPrivate;

//! A ChatItem for the contents column
class ContentsChatItem : public ChatItem {
  Q_DECLARE_TR_FUNCTIONS(ContentsChatItem)

public:
  ContentsChatItem(const qreal &width, const QPointF &pos, QGraphicsItem *parent);
  ~ContentsChatItem();

  enum { Type = ChatScene::ContentsChatItemType };
  virtual inline int type() const { return Type; }

  inline ChatLineModel::ColumnType column() const { return ChatLineModel::ContentsColumn; }
  QFontMetricsF *fontMetrics() const;

protected:
  virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
  virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
  virtual void hoverMoveEvent(QGraphicsSceneHoverEvent *event);
  virtual void handleClick(const QPointF &pos, ChatScene::ClickMode clickMode);

  virtual void addActionsToMenu(QMenu *menu, const QPointF &itemPos);
  virtual void copyLinkToClipboard();

  virtual QVector<QTextLayout::FormatRange> additionalFormats() const;

  virtual inline void initLayout(QTextLayout *layout) const {
    initLayoutHelper(layout, QTextOption::WrapAtWordBoundaryOrAnywhere);
    doLayout(layout);
  }
  virtual void doLayout(QTextLayout *layout) const;
  virtual UiStyle::FormatList formatList() const;

private:
  class ActionProxy;
  class WrapColumnFinder;

  ContentsChatItemPrivate *_data;
  ContentsChatItemPrivate *privateData() const;

  Clickable clickableAt(const QPointF &pos) const;

  void endHoverMode();
  void showWebPreview(const Clickable &click);
  void clearWebPreview();

  qreal setGeometryByWidth(qreal w);
  friend class ChatLine;
  friend struct ContentsChatItemPrivate;

  QFontMetricsF *_fontMetrics;

  // we need a receiver for Action signals
  static ActionProxy _actionProxy;
};

struct ContentsChatItemPrivate {
  ContentsChatItem *contentsItem;
  ClickableList clickables;
  Clickable currentClickable;
  Clickable activeClickable;

  ContentsChatItemPrivate(const ClickableList &c, ContentsChatItem *parent) : contentsItem(parent), clickables(c) {}
};

class ContentsChatItem::WrapColumnFinder {
public:
  WrapColumnFinder(const ChatItem *parent);
  ~WrapColumnFinder();

  qint16 nextWrapColumn(qreal width);

private:
  const ChatItem *item;
  QTextLayout layout;
  QTextLine line;
  ChatLineModel::WrapList wrapList;
  qint16 wordidx;
  qint16 lineCount;
  qreal choppedTrailing;
};

//! Acts as a proxy for Action signals targetted at a ContentsChatItem
/** Since a ChatItem is not a QObject, hence cannot receive signals, we use a static ActionProxy
 *  as a receiver instead. This avoids having to handle ChatItem actions (e.g. context menu entries)
 *  outside the ChatItem.
 */
class ContentsChatItem::ActionProxy : public QObject {
  Q_OBJECT

public slots:
  inline void copyLinkToClipboard() { item()->copyLinkToClipboard(); }

private:
  /// Returns the ContentsChatItem that should receive the action event.
  /** For efficiency reasons, values are not checked for validity. You gotta make sure that you set the data() member
   *  in the Action correctly.
   *  @return The ChatItem from which the sending Action originated
   */
  inline ContentsChatItem *item() const {
    return static_cast<ContentsChatItem *>(qobject_cast<QAction *>(sender())->data().value<void *>());
  }
};

/*************************************************************************************************/

#endif
