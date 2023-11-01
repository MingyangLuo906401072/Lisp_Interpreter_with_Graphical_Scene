#ifndef MAIN_WINDOW_HPP
#define MAIN_WINDOW_HPP

#include <string>
#include <QWidget>
#include <QLayout>
#include <QLabel>
#include <fstream>
#include <sstream>
#include "qt_interpreter.hpp"

class MessageWidget;
class CanvasWidget;
class REPLWidget;

class MainWindow : public QWidget {
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    MainWindow(std::string filename, QWidget* parent = nullptr);

private:
    QtInterpreter qtinterp;
    QVBoxLayout* layout;
    MessageWidget* messageWidget;
    CanvasWidget* canvasWidget;
    REPLWidget* replWidget;
};

#endif

