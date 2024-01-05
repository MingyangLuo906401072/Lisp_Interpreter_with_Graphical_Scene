#include <QDebug>
#include <QtTest/QtTest>
#include <QtWidgets>

#include "canvas_widget.hpp"
#include "main_window.hpp"
#include "message_widget.hpp"
#include "repl_widget.hpp"
#include "qgraphics_arc_item.hpp"

#include <iostream>

class TestGUI : public QObject {
  Q_OBJECT

public:
private slots:

  void initTestCase();
  void testREPLGood();
  void testREPLBad();
  void testREPLBad2Good();
  void testPoint();
  void testLine();
  void testArc();
  void testEnvRestore();
  void testMessage();
  void cleanupTestCase();
  void testCanvasAddGraphic();
  void testCanvasClear();
  void testCanvasDrawLine();
  void testCanvasDrawPoint();
  void testQGraphicsArcItemPainting();
  void testQGraphicsArcItemSpanAngle();
  void testQGraphicsArcItemStartAngle();
  void testQGraphicsArcItem();
  void testREPLWidgetCreation();
  void testGraphicsVector();
  void testStoreGraphics();
  void testQtInterpreterParsingAndEvaluating();
  void testQtInterpreterOperatorsAndFunctions();
  
private:
  MainWindow w;

  REPLWidget *repl;
  QLineEdit *replEdit;
  MessageWidget *message;
  QLineEdit *messageEdit;
  CanvasWidget *canvas;
  QGraphicsScene *scene;
};

void TestGUI::initTestCase() {

  repl = w.findChild<REPLWidget *>();
  QVERIFY2(repl, "Could not find REPLWidget instance in MainWindow instance.");

  replEdit = repl->findChild<QLineEdit *>();
  QVERIFY2(replEdit,
           "Could not find QLineEdit instance in REPLWidget instance.");

  message = w.findChild<MessageWidget *>();
  QVERIFY2(message,
           "Could not find MessageWidget instance in MainWindow instance.");

  messageEdit = message->findChild<QLineEdit *>();
  QVERIFY2(messageEdit,
           "Could not find QLineEdit instance in MessageWidget instance.");

  canvas = w.findChild<CanvasWidget *>();
  QVERIFY2(canvas,
           "Could not find CanvasWidget instance in MainWindow instance.");

  scene = canvas->findChild<QGraphicsScene *>();
  QVERIFY2(scene,
           "Could not find QGraphicsScene instance in CanvasWidget instance.");

    w.setMinimumSize(800,600);
  w.show();
  
}

void TestGUI::testREPLGood() {

  QVERIFY(repl && replEdit);
  QVERIFY(message && messageEdit);

  // send a string to the repl widget
  QTest::keyClicks(replEdit, "(define a 1)");
  QTest::keyClick(replEdit, Qt::Key_Return, Qt::NoModifier);

  // check message
  QVERIFY2(messageEdit->isReadOnly(),
           "Expected QLineEdit inside MessageWidget to be read-only.");
  QCOMPARE(messageEdit->text(), QString("(1)"));
}

void TestGUI::testREPLBad() {

  QVERIFY(repl && replEdit);
  QVERIFY(message && messageEdit);

  // send a string to the repl widget
  QTest::keyClicks(replEdit, "(foo)");
  QTest::keyClick(replEdit, Qt::Key_Return, Qt::NoModifier);

  // check message
  QVERIFY2(messageEdit->isReadOnly(),
           "Expected QLineEdit inside MessageWidget to be read-only.");
  QVERIFY2(messageEdit->text().startsWith("Error"), "Expected error message.");

  // check background color and selection
  QPalette p = messageEdit->palette();
  QCOMPARE(p.highlight().color(), QColor(Qt::red));
  QVERIFY2(messageEdit->selectedText().startsWith("Error"),
           "Expected error to be selected.");
}

void TestGUI::testREPLBad2Good() {

  QVERIFY(repl && replEdit);
  QVERIFY(message && messageEdit);

  // send a string to the repl widget
  QTest::keyClicks(replEdit, "(foo)");
  QTest::keyClick(replEdit, Qt::Key_Return, Qt::NoModifier);

  // check message
  QVERIFY2(messageEdit->isReadOnly(),
           "Expected QLineEdit inside MessageWidget to be read-only.");
  QVERIFY2(messageEdit->text().startsWith("Error"), "Expected error message.");

  // check background color and selection
  QPalette p = messageEdit->palette();
  QCOMPARE(p.highlight().color(), QColor(Qt::red));
  QVERIFY2(messageEdit->selectedText().startsWith("Error"),
           "Expected error to be selected.");

  // send a string to the repl widget
  QTest::keyClicks(replEdit, "(define value 100)");
  QTest::keyClick(replEdit, Qt::Key_Return, Qt::NoModifier);

  // check message
  QVERIFY2(messageEdit->isReadOnly(),
           "Expected QLineEdit inside MessageWidget to be read-only.");
  QCOMPARE(messageEdit->text(), QString("(100)"));

  // check background color and selection
  p = messageEdit->palette();
  QVERIFY2(p.highlight().color() != QColor(Qt::red),
           "Did not expect red highlight on successful eval.");
  QVERIFY2(messageEdit->selectedText() == "",
           "Expected no selcted text on successful eval.");
}

void TestGUI::testPoint() {

  QVERIFY(repl && replEdit);
  QVERIFY(canvas && scene);

  // send a string to the repl widget
  QTest::keyClicks(replEdit, "(draw (point 0 0))");
  QTest::keyClick(replEdit, Qt::Key_Return, Qt::NoModifier);

  // check canvas
  QVERIFY2(scene->itemAt(QPointF(0, 0), QTransform()) != 0,
           "Expected a point in the scene. Not found.");
}

void TestGUI::testLine() {

  QVERIFY(repl && replEdit);
  QVERIFY(canvas && scene);

  // send a string to the repl widget
  QTest::keyClicks(replEdit, "(draw (line (point 10 0) (point 0 10)))");
  QTest::keyClick(replEdit, Qt::Key_Return, Qt::NoModifier);
  
  // check canvas
  QVERIFY2(scene->itemAt(QPointF(10, 0), QTransform()) != 0,
           "Expected a line in the scene. Not found.");
  QVERIFY2(scene->itemAt(QPointF(0, 10), QTransform()) != 0,
           "Expected a line in the scene. Not found.");
}

void TestGUI::testArc() {

  QVERIFY(repl && replEdit);
  QVERIFY(canvas && scene);

  // send a string to the repl widget
  QTest::keyClicks(replEdit, "(draw (arc (point 0 0) (point 100 0) pi))");
  QTest::keyClick(replEdit, Qt::Key_Return, Qt::NoModifier);

  // check canvas
  QVERIFY2(scene->itemAt(QPointF(100, 0), QTransform()) != 0,
           "Expected a point on the arc in the scene. Not found.");
  QVERIFY2(scene->itemAt(QPointF(-100, 0), QTransform()) != 0,
           "Expected a point on the arc in the scene. Not found.");

  QGraphicsItem * temp = scene->itemAt(QPointF(-20, -1), QTransform());
  qDebug() << temp;
  qDebug() << temp->boundingRect();
}

void TestGUI::testEnvRestore() {
  
  QVERIFY(repl && replEdit);
  QVERIFY(canvas && scene);

  QGraphicsItem * temp2 = scene->itemAt(QPointF(-20, 0), QTransform());
  qDebug() << temp2;
  
  // send a string to the repl widget
  QTest::keyClicks(replEdit, "(begin (draw (point -20 0)) (define pi 3))");
  QTest::keyClick(replEdit, Qt::Key_Return, Qt::NoModifier);

  // check canvas
  QGraphicsItem * temp = scene->itemAt(QPointF(-20, 0), QTransform());
  qDebug() << temp;
  
  QVERIFY2(scene->itemAt(QPointF(-20, 0), QTransform()) == 0,
           "Did not expected a point in the scene. One found.");
}

void TestGUI::testMessage(){

  MessageWidget message;

  QLineEdit * messageEdit = message.findChild<QLineEdit *>();
  QVERIFY2(messageEdit,
           "Could not find QLineEdit instance in MessageWidget instance.");
  
  // check message
  QVERIFY2(messageEdit->isReadOnly(),
           "Expected QLineEdit inside MessageWidget to be read-only.");
  QCOMPARE(messageEdit->text(), QString(""));
}

void TestGUI::cleanupTestCase() {

  QTest::qWait(5000);
  
}

void TestGUI::testCanvasAddGraphic() {
    QVERIFY(canvas && scene);

    // Create a QGraphicsItem to add to the canvas
    QGraphicsItem* item = new QGraphicsEllipseItem(0, 0, 50, 50);

    // Add the graphics item to the canvas
    canvas->addGraphic(item);

    // Check if the item is present in the scene
    QVERIFY2(scene->items().contains(item),
        "Expected graphics item to be present in the scene.");
}

void TestGUI::testCanvasClear() {
    QVERIFY(canvas && scene);

    // Create a QGraphicsItem and add it to the canvas
    QGraphicsItem* item = new QGraphicsEllipseItem(0, 0, 50, 50);
    canvas->addGraphic(item);

    // Clear the canvas
    canvas->clear();

    // Check if the scene is empty after clearing
    QVERIFY2(scene->items().isEmpty(), "Expected the scene to be empty after clearing.");
}

void TestGUI::testCanvasDrawLine() {
    QVERIFY(canvas && scene);

    // Send a string to the REPL widget to draw a line
    QTest::keyClicks(replEdit, "(draw (line (point 0 0) (point 50 50)))");
    QTest::keyClick(replEdit, Qt::Key_Return, Qt::NoModifier);

    // Check if the line is present in the scene
    QVERIFY2(scene->itemAt(QPointF(0, 0), QTransform()) != nullptr,
        "Expected a line in the scene. Not found.");
    QVERIFY2(scene->itemAt(QPointF(50, 50), QTransform()) != nullptr,
        "Expected a line in the scene. Not found.");
}

void TestGUI::testCanvasDrawPoint() {
    QVERIFY(canvas && scene);

    // Send a string to the REPL widget to draw a point
    QTest::keyClicks(replEdit, "(draw (point 20 20))");
    QTest::keyClick(replEdit, Qt::Key_Return, Qt::NoModifier);

    // Check if the point is present in the scene
    QVERIFY2(scene->itemAt(QPointF(20, 20), QTransform()) != nullptr,
        "Expected a point in the scene. Not found.");
}

void TestGUI::testQGraphicsArcItemPainting() {
    // Send a string to the REPL widget to draw an arc
    QTest::keyClicks(replEdit, "(draw (arc (point 0 0) (point 100 0) pi))");
    QTest::keyClick(replEdit, Qt::Key_Return, Qt::NoModifier);

    // Check if the arc is present in the scene
    QVERIFY2(scene->itemAt(QPointF(100, 0), QTransform()) != nullptr,
        "Expected a point on the arc in the scene. Not found.");
    QVERIFY2(scene->itemAt(QPointF(-100, 0), QTransform()) != nullptr,
        "Expected a point on the arc in the scene. Not found.");

    // Check if the arc is painted without errors
    QVERIFY2(QTest::qWaitForWindowExposed(&w), "Window not exposed");

    QTest::qWait(500); // Wait for the view to be displayed

    QGraphicsItem* temp = scene->itemAt(QPointF(-20, -1), QTransform());
    QVERIFY2(temp != nullptr, "Expected a point on the arc in the scene. Not found.");
}

void TestGUI::testQGraphicsArcItemSpanAngle() {
    QGraphicsArcItem* arc = new QGraphicsArcItem(0, 0, 100, 100);

    // Set the span angle to 45 degrees
    arc->setSpanAngle(45 * 16);

    // Add the arc to the canvas
    canvas->addGraphic(arc);

    // Check if the span angle is set correctly
    QCOMPARE(static_cast<int>(arc->spanAngle()), 45 * 16);
}

void TestGUI::testQGraphicsArcItemStartAngle() {
    QGraphicsArcItem* arc = new QGraphicsArcItem(0, 0, 100, 100);

    // Set the start angle to 90 degrees
    arc->setStartAngle(90 * 16);

    // Add the arc to the canvas
    canvas->addGraphic(arc);

    // Check if the start angle is set correctly
    QCOMPARE(static_cast<int>(arc->startAngle()), 90 * 16);
}

void TestGUI::testQGraphicsArcItem() {
    QGraphicsArcItem* arc = new QGraphicsArcItem(0, 0, 100, 100);

    QVERIFY2(arc != nullptr, "Failed to create QGraphicsArcItem");

    // Add the arc to the canvas
    canvas->addGraphic(arc);

    // Check if the arc is present in the scene
    QVERIFY2(scene->items().contains(arc),
        "Expected QGraphicsArcItem to be present in the scene.");
}

void TestGUI::testREPLWidgetCreation() {
    REPLWidget* replWidget = new REPLWidget();

    QVERIFY2(replWidget != nullptr, "Failed to create REPLWidget");
}

void TestGUI::testGraphicsVector() {
    Interpreter interpreter;
    std::istringstream expression("(begin (draw (point 0 0)) (draw (line (point 10 0) (point 0 10))) (draw (arc (point 0 0) (point 100 0) pi)))");
    QVERIFY(interpreter.parse(expression));
    interpreter.eval();
    const std::vector<Atom>& graphics = interpreter.getGraphicsVector();

    QCOMPARE(static_cast<int>(graphics.size()), 3);

    QCOMPARE(static_cast<int>(graphics[0].type), static_cast<int>(PointType));
    QCOMPARE(static_cast<int>(graphics[1].type), static_cast<int>(LineType));
    QCOMPARE(static_cast<int>(graphics[2].type), static_cast<int>(ArcType));
}

void TestGUI::testStoreGraphics() {
    Interpreter interpreter;
    std::istringstream expression("(begin "
        "(draw (point 0 0)) "
        "(draw (line (point 10 0) (point 0 10))) "
        "(draw (arc (point 0 0) (point 100 0) pi)) "
        "(draw (point 5 5)) "
        "(draw (line (point 0 0) (point 5 5))) "
        "(draw (arc (point 0 0) (point 50 0) (/ pi 2))))");
    QVERIFY(interpreter.parse(expression));
    interpreter.eval();
    const std::vector<Atom>& graphics = interpreter.getGraphicsVector();

    QCOMPARE(static_cast<int>(graphics.size()), 6);

    QCOMPARE(static_cast<int>(graphics[0].type), static_cast<int>(PointType));
    QCOMPARE(static_cast<int>(graphics[1].type), static_cast<int>(LineType));
    QCOMPARE(static_cast<int>(graphics[2].type), static_cast<int>(ArcType));
    QCOMPARE(static_cast<int>(graphics[3].type), static_cast<int>(PointType));
    QCOMPARE(static_cast<int>(graphics[4].type), static_cast<int>(LineType));
    QCOMPARE(static_cast<int>(graphics[5].type), static_cast<int>(ArcType));
}

void TestGUI::testQtInterpreterParsingAndEvaluating() {
    QtInterpreter qtInterpreter;

    qtInterpreter.parseAndEvaluate("(draw (point 10 10))");
    QCOMPARE(static_cast<int>(qtInterpreter.getGraphicsVector().size()), 1);

    qtInterpreter.parseAndEvaluate("(draw (line (point 0 0) (point 20 20)))");
    QCOMPARE(static_cast<int>(qtInterpreter.getGraphicsVector().size()), 2);

    qtInterpreter.parseAndEvaluate("(define x)");
    QCOMPARE(static_cast<int>(qtInterpreter.getGraphicsVector().size()), 2);

    qtInterpreter.parseAndEvaluate("(+ 1 2)");
    QCOMPARE(static_cast<int>(qtInterpreter.getGraphicsVector().size()), 2);

    qtInterpreter.parseAndEvaluate("(unknown-command)");
    QCOMPARE(static_cast<int>(qtInterpreter.getGraphicsVector().size()), 2);

    qtInterpreter.parseAndEvaluate("(+ 1 2) (define x) (unknown-command)");
    QCOMPARE(static_cast<int>(qtInterpreter.getGraphicsVector().size()), 2);

    qtInterpreter.parseAndEvaluate("(cl)");
    QCOMPARE(static_cast<int>(qtInterpreter.getGraphicsVector().size()), 2);

    qtInterpreter.parseAndEvaluate("(");
    QCOMPARE(static_cast<int>(qtInterpreter.getGraphicsVector().size()), 2);

    qtInterpreter.parseAndEvaluate("(draw (point 5 5))");
    QCOMPARE(static_cast<int>(qtInterpreter.getGraphicsVector().size()), 3);
}

void TestGUI::testQtInterpreterOperatorsAndFunctions() {
    QtInterpreter qtInterpreter;

    qtInterpreter.parseAndEvaluate("(and True False)");
    QCOMPARE(static_cast<int>(qtInterpreter.getGraphicsVector().size()), 0);

    qtInterpreter.parseAndEvaluate("(or True False)");
    QCOMPARE(static_cast<int>(qtInterpreter.getGraphicsVector().size()), 0);

    qtInterpreter.parseAndEvaluate("(not True)");
    QCOMPARE(static_cast<int>(qtInterpreter.getGraphicsVector().size()), 0);

    qtInterpreter.parseAndEvaluate("(draw (point (sin pi) (cos pi)))");
    QCOMPARE(static_cast<int>(qtInterpreter.getGraphicsVector().size()), 1);

    qtInterpreter.parseAndEvaluate("(draw (point (sin pi) (arctan 1 1)))");
    QCOMPARE(static_cast<int>(qtInterpreter.getGraphicsVector().size()), 2);

    qtInterpreter.parseAndEvaluate("(draw (point (cos pi) (arctan 1 1)))");
    QCOMPARE(static_cast<int>(qtInterpreter.getGraphicsVector().size()), 3);

    qtInterpreter.parseAndEvaluate("(draw (point 10 10))");
    QCOMPARE(static_cast<int>(qtInterpreter.getGraphicsVector().size()), 4);

    qtInterpreter.parseAndEvaluate("(draw (line (point 0 0) (point 20 20)))");
    QCOMPARE(static_cast<int>(qtInterpreter.getGraphicsVector().size()), 5);

    qtInterpreter.parseAndEvaluate("(draw (arc (point 5 5) (point 10 0) (* 2 pi)))");
    QCOMPARE(static_cast<int>(qtInterpreter.getGraphicsVector().size()), 6);

    qtInterpreter.parseAndEvaluate("(define radius 5)");
    QCOMPARE(static_cast<int>(qtInterpreter.getGraphicsVector().size()), 6);

    qtInterpreter.parseAndEvaluate("(draw (point (* 2 radius) (* 3 radius)))");
    QCOMPARE(static_cast<int>(qtInterpreter.getGraphicsVector().size()), 7);
}

QTEST_MAIN(TestGUI)
#include "test_gui.moc"
