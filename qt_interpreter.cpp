#include "qt_interpreter.hpp"

#include <string>
#include <sstream>
#include <iostream>
#include <cmath>

#include <QBrush>
#include <QDebug>

#include "qgraphics_arc_item.hpp"

#include "interpreter_semantic_error.hpp"

const double PI = atan2(0, -1);

QtInterpreter::QtInterpreter(QObject * parent): QObject(parent){
}

const std::vector<Atom>& QtInterpreter::getGraphicsVector() const {
    return graphics;
}

void QtInterpreter::parseAndEvaluate(QString entry) {
    bool c = false;
    try {
        std::string s = entry.toStdString();
        std::istringstream stream(s);
        if (parse(stream)) {
            try {
                Expression result = eval();
                // emit the result as an info message
                std::stringstream resultStream;
                resultStream << result;
                std::string resultString = resultStream.str();
                QString qResultString = "(" + QString::fromStdString(resultString) + ")";
                emit info(qResultString);
            }
            catch (const InterpreterSemanticError& e) {
                QString em = QString::fromStdString("Error: evaluation failed");
                c = true;
                emit error(em);
            }
            if (!graphics.empty()) {
                for (const auto& graphic : graphics) {
                    if (graphic.type == PointType) {
                        // Handle PointType graphic
                        QGraphicsEllipseItem* point = new QGraphicsEllipseItem(
                            graphic.value.point_value.x,
                            graphic.value.point_value.y,
                            2, 2
                        );
                        point->setBrush(Qt::black);
                        emit drawGraphic(point);
                    }
                    else if (graphic.type == LineType) {
                        // Handle LineType graphic
                        QGraphicsLineItem* line = new QGraphicsLineItem(
                            graphic.value.line_value.first.x,
                            graphic.value.line_value.first.y,
                            graphic.value.line_value.second.x,
                            graphic.value.line_value.second.y
                        );

                        emit drawGraphic(line);
                    }
                    else if (graphic.type == ArcType) {
                        // Handle ArcType graphic
                        QGraphicsArcItem* arc = new QGraphicsArcItem(
                            graphic.value.arc_value.center.x - std::max(std::abs(graphic.value.arc_value.center.x - graphic.value.arc_value.start.x),
                                std::abs(graphic.value.arc_value.start.y - graphic.value.arc_value.center.y)),
                            graphic.value.arc_value.center.y - std::max(std::abs(graphic.value.arc_value.center.x - graphic.value.arc_value.start.x),
                                std::abs(graphic.value.arc_value.start.y - graphic.value.arc_value.center.y)),
                            2 * std::max(std::abs(graphic.value.arc_value.center.x - graphic.value.arc_value.start.x), 
                                std::abs(graphic.value.arc_value.start.y - graphic.value.arc_value.center.y)),
                            2 * std::max(std::abs(graphic.value.arc_value.center.x - graphic.value.arc_value.start.x),
                                std::abs(graphic.value.arc_value.start.y - graphic.value.arc_value.center.y)),
                            nullptr
                        );
                       
                        double angleInRadians = std::atan(std::abs(graphic.value.arc_value.start.y - graphic.value.arc_value.center.y) /
                            std::abs(graphic.value.arc_value.center.x - graphic.value.arc_value.start.x));

                        // Convert the angle from radians to degrees
                        double angleInDegrees = 16 * angleInRadians * (180.0 / PI);

                        double spanInDegrees = 16 * graphic.value.arc_value.span * (180.0 / PI);

                        // Set the start and span angle in degrees 
                        arc->setSpanAngle(spanInDegrees);
                        arc->setStartAngle(angleInDegrees);

                        emit drawGraphic(arc);
                    }
                }
            }
        }
        else {
            QString em = "Error: parsing failed";
            c = true;
            emit error(em);
        }
    }
    catch (const InterpreterSemanticError& e) {
        QString em = "Error: " + QString::fromStdString("parsing/evaluation failed");
        c = true;
        emit error(em);
    }
    if (c) {
        emit clear();
    }
}


