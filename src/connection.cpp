#include "connection.h"
#include "gateitem.h"
#include <QPainter>
#include <QPropertyAnimation>
#include <QPen>

Connection::Connection(LogicElement* startItem, LogicElement* endItem, QGraphicsItem* parent)
  : QGraphicsLineItem(parent), QObject(), myStartItem(startItem), myEndItem(endItem)
{
  setFlag(QGraphicsItem::ItemIsSelectable, true);
  setPen(QPen(myColor, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));

  /*
  QPropertyAnimation animation(this, "pos");
  animation.setStartValue(0);
  animation.setEndValue(5000);
  animation.setDuration(5000);
  animation.start();
  */
}

void Connection::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
  QPainterPath path;

  QPen myPen;
  if(startItem()->getValue(this))
      myPen.setColor(Qt::green);
  else
      myPen.setColor(Qt::red);

  QPointF breakPoint;
  QPointF afterBreakPoint;

  QPointF pointStart = mapFromItem(myStartItem, myStartItem->getConnPosOut(this));
  QPointF pointEnd = mapFromItem(myEndItem, myEndItem->getConnPosIn(this));

  qreal breakCoord;

  if(abs(pointStart.rx() - pointEnd.rx()) >= abs(pointStart.ry() - pointEnd.ry()))
  {
       breakCoord = pointStart.rx() + 1.0/3.0 * (pointEnd.rx() - pointStart.rx());
       breakPoint = QPointF(breakCoord, pointStart.ry());
       afterBreakPoint = QPointF(breakCoord, pointEnd.ry());
  }
  else
  {
       breakCoord = pointStart.ry() + 0.2 * (pointEnd.ry() - pointStart.ry());
       breakPoint = QPointF(pointStart.rx(), breakCoord);
       afterBreakPoint = QPointF(pointEnd.rx(), breakCoord);
  }

  painter->setPen(myPen);
  path.moveTo(pointStart);
  path.lineTo(breakPoint);
  path.lineTo(afterBreakPoint);
  path.lineTo(pointEnd);

  painter->drawPath(path);
  this->myPath = path;
}


QPainterPath Connection::shape() const
{
  return this->myPath;
}

