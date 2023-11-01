#ifndef REPL_WIDGET_HPP
#define REPL_WIDGET_HPP

#include <QWidget>
#include <QLineEdit>
#include <QString>
#include <QVector>
#include <QLabel>
#include <QLayout>
#include <QKeyEvent>

class REPLWidget: public QWidget{
Q_OBJECT

public:

  REPLWidget(QWidget * parent = nullptr);

signals:
  void lineEntered(QString entry);

private slots:

  void changed();

private:
    QHBoxLayout* layout;
    QLineEdit* lineEdit;
    QLabel* promptLabel;
};

#endif
