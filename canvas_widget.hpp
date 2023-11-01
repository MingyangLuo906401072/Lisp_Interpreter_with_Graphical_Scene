#ifndef CANVAS_WIDGET_HPP
#define CANVAS_WIDGET_HPP

#include <QWidget>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QLayout>

class QGraphicsItem;
class QGraphicsScene;

class CanvasWidget: public QWidget{
  Q_OBJECT

public:

  CanvasWidget(QWidget * parent = nullptr);

public slots:

  void addGraphic(QGraphicsItem * item);
  void clear();

private:
	QGraphicsView* view;
    QGraphicsScene * scene;
	QVBoxLayout* layout;
};

#endif
