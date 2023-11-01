#include "canvas_widget.hpp"

CanvasWidget::CanvasWidget(QWidget * parent): QWidget(parent){
    // Create a QGraphicsScene to hold the graphics items
    scene = new QGraphicsScene(this);

    // Create a QGraphicsView to display the scene
    view = new QGraphicsView(scene, this);

    // Set up the layout to fill the widget with the view
    layout = new QVBoxLayout(this);
    layout->addWidget(view);
}

void CanvasWidget::addGraphic(QGraphicsItem * item){
    // Add the given graphics item to the scene for display
    scene->addItem(item);
}

void CanvasWidget::clear() {
    // Remove all graphics items from the scene
    scene->clear();
}
