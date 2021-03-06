#ifndef GATEITEM_H
#define GATEITEM_H

#include <QVector>
#include <QPainter>
#include <QTimer>
#include <QMap>
#include <QGraphicsItemGroup>
#include <iostream>
#include <cmath>
#include <QGraphicsScene>
#include <QGraphicsSceneContextMenuEvent>
#include "connection.h"

class LogicElement : public QGraphicsRectItem
{
public:
  enum ElementType {And, Or, Xor, Nand, Nor, Multiplexer, Demultiplexer,Adder, Subtractor, Decoder, Encoder,
                    JK, SR, D, T, Id, Not, In, Out, Clock};

  LogicElement(ElementType type,  QGraphicsItem* parent = nullptr);

  virtual bool addConnection(Connection* conn, QPointF point = QPointF(0,0)) = 0;
  virtual void removeConnection(Connection* conn) = 0;
  virtual void removeConnections() = 0;

  virtual QPointF getConnPosIn(Connection* conn);
  virtual QPointF getConnPosOut(Connection* conn);

  virtual void calculate() = 0;
  virtual bool getValue(Connection* conn = nullptr);

  ElementType elementType() const { return myElementType; }
protected:
  ElementType myElementType;
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
  bool addConnection(Connection* conn, QPointF) override;

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
  bool addConnection(Connection* conn, QPointF) override;

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
  bool addConnection(Connection* conn, QPointF) override;

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
  Or(int numOfInput = 4);

  void calculate() override;
  void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) override;
};

class Xor : public InnerGate
{
public:
  Xor(int numOfInput = 4);

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
};

class Id : public InnerGate
{
public:
  Id();

  void calculate() override;
  void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) override;
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
  Multiplexer(int numOfSelector = 2);

  bool addConnection(Connection* conn, QPointF point) override;
  void removeConnection(Connection *conn) override;

  void calculate() override;
  void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) override;
};


class Demultiplexer : public Plexer
{
public:
    Demultiplexer(int numOfSelector = 2);

    bool addConnection(Connection* conn, QPointF point) override;
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

    bool addConnection(Connection *conn, QPointF point) override;
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

    bool addConnection(Connection *conn, QPointF point) override;
    void removeConnection(Connection *conn) override;

    bool getValue(Connection *conn) override;
    void calculate() override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) override;

private:
    QVector<int> indexConnectionFrom;
    QVector<bool> myValues;
};

/***************************************************************************************************/

class FlipFlop : public LogicElement
{
public:
    FlipFlop(ElementType type, int numOfInput = 2);

    void removeConnections() override;
    void removeConnection(Connection* conn) override;

    QPointF getConnPosIn(Connection* conn) override;
    QPointF getConnPosOut(Connection* conn) override;

    bool getValue(Connection *conn) override;

protected:
    QVector<Connection*> connectionsTo;
    QVector<Connection*> connectionsFrom;
    Connection* clock;
    int numOfInput;
    int numOfOutput;
    QVector<int> indexConnectionFrom;
    QVector<bool> myValues;
};

class JK : public FlipFlop
{
public:
    JK();

    bool addConnection(Connection *conn, QPointF point) override;

    void calculate() override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) override;

};

class SR : public FlipFlop
{
public:
    SR();

    bool addConnection(Connection *conn, QPointF point) override;

    void calculate() override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) override;

};

class D : public FlipFlop
{
public:
    D();

    bool addConnection(Connection *conn, QPointF point) override;

    void calculate() override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) override;

};

class T : public FlipFlop
{
public:
    T();

    bool addConnection(Connection *conn, QPointF point) override;

    void calculate() override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) override;

};

/***************************************************************************************************/

class Arithmetic : public LogicElement
{
public:
    Arithmetic(ElementType type, int numOfIn);

    void removeConnections() override;
    void removeConnection(Connection* conn) override;
    bool addConnection(Connection *conn, QPointF point) override;

    QPointF getConnPosIn(Connection* conn) override;
    QPointF getConnPosOut(Connection* conn) override;

    bool getValue(Connection *conn) override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) override;

protected:
    QMap<int, Connection*> connectionsTo0;
    QMap<int, Connection*> connectionsTo1;
    Connection* connectionRestIn;
    QVector<Connection*> connectionsFrom;
    QVector<int> indexConnectionFrom;
    QVector<Connection*> connectionsRestOut;
    int numOfInput;
    int numOfOutput;
    QVector<int> myValues;
    bool cinValue;
};

class Adder : public Arithmetic
{
public:
    Adder(int numOfInput = 4);

    void calculate() override;
};

class Subtractor : public Arithmetic
{
public:
    Subtractor(int numOfInput = 4);

    void calculate() override;
};

#endif // GATEITEM_H
