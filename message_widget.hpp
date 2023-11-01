#ifndef MESSAGE_WINDOW_HPP
#define MESSAGE_WINDOW_HPP

#include <QLabel>
#include <QString>
#include <QWidget>
#include <QLineEdit>

class QLineEdit;

class MessageWidget : public QWidget {
  Q_OBJECT

public:
  MessageWidget(QWidget *parent = nullptr);

public slots:

  void info(QString message);

  void error(QString message);

  void clear();

private:
	QLabel* messageLabel;
	QLineEdit* messageLineEdit;
};

#endif
