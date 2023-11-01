#include "main_window.hpp"
#include "message_widget.hpp"
#include "canvas_widget.hpp"
#include "repl_widget.hpp"

MainWindow::MainWindow(QWidget* parent) : MainWindow("", parent) {
    // This constructor serves as a default call to the parameterized constructor with an empty filename
}

MainWindow::MainWindow(std::string filename, QWidget* parent) : QWidget(parent) {
    layout = new QVBoxLayout(this);

    messageWidget = new MessageWidget();
    canvasWidget = new CanvasWidget();
    replWidget = new REPLWidget();

    layout->addWidget(messageWidget);
    layout->addWidget(canvasWidget);
    layout->addWidget(replWidget);

    setLayout(layout);

    // Open and read the file content if filename is not empty
    if (!filename.empty()) {
        std::ifstream file(filename);
        if (file.is_open()) {
            std::stringstream buffer;
            buffer << file.rdbuf();
            std::string fileContent = buffer.str();
            QObject::connect(&qtinterp, &QtInterpreter::drawGraphic, canvasWidget, &CanvasWidget::addGraphic);
            QObject::connect(&qtinterp, &QtInterpreter::clear, canvasWidget, &CanvasWidget::clear);
            qtinterp.parseAndEvaluate(fileContent.c_str());
            file.close();
        }
        else {
            QString errorMsg = "Error: Unable to open file: " + QString::fromStdString(filename);
            messageWidget->error(errorMsg);
        }
    }

    // Connect lineEntered from REPLWidget to QtInterpreter's parseAndEvaluate
    QObject::connect(replWidget, &REPLWidget::lineEntered, &qtinterp, &QtInterpreter::parseAndEvaluate);

    // Connect info and error signals from QtInterpreter to MessageWidget
    QObject::connect(&qtinterp, &QtInterpreter::info, messageWidget, &MessageWidget::info);
    QObject::connect(&qtinterp, &QtInterpreter::error, messageWidget, &MessageWidget::error);
    QObject::connect(&qtinterp, &QtInterpreter::clear, canvasWidget, &CanvasWidget::clear);
    QObject::connect(&qtinterp, &QtInterpreter::drawGraphic, canvasWidget, &CanvasWidget::addGraphic);

    /*QObject::connect(&qtinterp, SIGNAL(info(QString)), messageWidget, SLOT(info(QString)));
    QObject::connect(&qtinterp, SIGNAL(error(QString)), messageWidget, SLOT(error(QString)));
    QObject::connect(&qtinterp, SIGNAL(drawGraphic(QGraphicsItem*)), canvasWidget, SLOT(addGraphic(QGraphicsItem*)));
    QObject::connect(replWidget, SIGNAL(lineEntered(QString)), &qtinterp, SLOT(parseAndEvaluate(QString)));*/

}

