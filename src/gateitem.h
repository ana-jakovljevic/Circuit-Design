#ifndef GATEITEM_H
#define GATEITEM_H

#include <QGraphicsPixmapItem>
#include <QVector>
#include <QPainter>
#include <QTimer>
#include <QWidget>
#include <QGraphicsItemGroup>


QT_BEGIN_NAMESPACE
class Connection;
class QPixmap;
class QGraphicsSceneContextMenuEvent;
class QMenu;
class QRectF;
QT_END_NAMESPACE

class LogicElement : public QGraphicsRectItem
{
public:
  enum ElementType {And, Or, Xor, Nand, Nor, Id, Not, In, Out, Clock, Multiplexer, Demultiplexer, Decoder, Encoder};
  enum ConnectionType { StartItem, EndItem };

  LogicElement(ElementType type,  QGraphicsItem* parent = nullptr);

  virtual void calculate() = 0;
  virtual bool getValue(Connection* conn = nullptr);

  virtual void removeConnections() = 0;
  virtual void removeConnection(Connection* conn) = 0;
  virtual bool addConnection(Connection* conn, ConnectionType type, QPointF point = QPointF(0,0)) = 0;

  virtual QPointF getConnPosIn(Connection* conn);
  virtual QPointF getConnPosOut(Connection* conn);

  ElementType elementType() const { return myElementType; }
  QPixmap image() const { return pixmap; }

protected:
  ElementType myElementType;
  QPixmap pixmap;
  bool myValue;
  QPen myPen;
};

/***************************************************************************************************/

class InputGate : public LogicElement
{
public:
  InputGate(ElementType type = LogicElement::ElementType::In);

  void removeConnections() override;
  void removeConnection(Connection *conn) override;
  bool addConnection(Connection* conn, ConnectionType type, QPointF) override;

  void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) override;
  void calculate() override;

  QVector<Connection*> connectionsFrom;
};

class ClockGate : public InputGate, public QObject
{
public:
    ClockGate();
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) override;

public slots:
    void calculate() override;
};

/***************************************************************************************************/

class OutputGate : public LogicElement
{
public:
  OutputGate( );

  void calculate() override;

  void removeConnections() override;
  void removeConnection(Connection *conn) override;
  bool addConnection(Connection* conn, ConnectionType type, QPointF) override;

  void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) override;
private:
  QVector<Connection*> connection;
};

/***************************************************************************************************/

class InnerGate : public LogicElement
{
public:
  InnerGate(ElementType type, int numOfInput);

  void removeConnections() override;
  void removeConnection(Connection *conn) override;
  bool addConnection(Connection* conn, ConnectionType type, QPointF) override;

  QPointF getConnPosIn(Connection* conn) override;
  QPointF getConnPosOut(Connection *conn) override;

  QVector<Connection*> connectionsTo;
  QVector<Connection*> connectionsFrom;

protected:
  int numOfInput;
};

/***************************************************************************************************/

class And : public InnerGate
{
public:
  And(int numOfInput = 2);

  void calculate() override;
  void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) override;
};

class Or : public InnerGate
{
public:
  Or(int numOfInput = 2);

  void calculate() override;
  void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) override;
};

class Xor : public InnerGate
{
public:
  Xor(int numOfInput = 2);

  void calculate() override;
  void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) override;
};

class Nor : public InnerGate
{
public:
  Nor(int numOfInput = 2);

  void calculate() override;
  void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) override;
};

class Nand : public InnerGate
{
public:
  Nand(int numOfInput = 2);

  void calculate() override;
  void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) override;
};

class Not : public InnerGate
{
public:
  Not();

  void calculate() override;
  void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) override;

  QPointF getConnPosIn(Connection*) override;
  QPointF getConnPosOut(Connection*) override;
};

class Id : public InnerGate
{
public:
  Id();

  void calculate() override;
  void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) override;

  QPointF getConnPosIn(Connection*) override;
  QPointF getConnPosOut(Connection*) override;
};

/***************************************************************************************************/

class Plexer : public LogicElement
{
public:
    Plexer(ElementType type, int numOfInput, int numOfOutput, int numOfSelector);

    void removeConnections() override;

    QPointF getConnPosIn(Connection* conn) override;
    QPointF getConnPosOut(Connection* conn) override;

protected:
    QVector<Connection*> connectionsSelector;
    QVector<Connection*> connectionsTo;
    QVector<Connection*> connectionsFrom;
    int numOfInput;
    int numOfOutput;
    int numOfSelector;
};

/***************************************************************************************************/

class Multiplexer : public Plexer
{
public:
  Multiplexer(int numOfInput = 4, int numOfSelector = 2);

  bool addConnection(Connection* conn, ConnectionType type, QPointF point) override;
  void removeConnection(Connection *conn) override;

  void calculate() override;
  void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) override;
};


class Demultiplexer : public Plexer
{
public:
    Demultiplexer(int numOfOutput = 4, int numOfSelector = 2);

    bool addConnection(Connection* conn, ConnectionType type, QPointF point) override;
    void removeConnection(Connection *conn) override;

    bool getValue(Connection *conn) override;
    void calculate() override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) override;

private:
    QVector<int> indexConnectionFrom;
    QVector<bool> myValues;
};

class Decoder : public Plexer
{
public:
    Decoder(int numOfInput = 2, int numOfOutput = 4);

    bool addConnection(Connection *conn, ConnectionType type, QPointF point) override;
    void removeConnection(Connection *conn) override;

    bool getValue(Connection *conn) override;
    void calculate() override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) override;

private:
    QVector<int> indexConnectionFrom;
    QVector<bool> myValues;
};

class Encoder : public Plexer
{
public:
    Encoder(int numOfInput = 4, int numOfOutput = 2);

    bool addConnection(Connection *conn, ConnectionType type, QPointF point) override;
    void removeConnection(Connection *conn) override;

    bool getValue(Connection *conn) override;
    void calculate() override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) override;

private:
    QVector<int> indexConnectionFrom;
    QVector<bool> myValues;
};

#endif // GATEITEM_H
