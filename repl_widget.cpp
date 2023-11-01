#include "repl_widget.hpp"

REPLWidget::REPLWidget(QWidget* parent) : QWidget(parent) {
    layout = new QHBoxLayout(this);

    promptLabel = new QLabel("slisp>", this);
    layout->addWidget(promptLabel);

    lineEdit = new QLineEdit(this);
    layout->addWidget(lineEdit);

    setLayout(layout);

    connect(lineEdit, &QLineEdit::returnPressed, this, &REPLWidget::changed);
}

void REPLWidget::changed() {
    QString entry = lineEdit->text();
    if (!entry.isEmpty()) {
        emit lineEntered(entry); // Emit the lineEntered signal with the text from lineEdit
        lineEdit->clear(); // Clear the line edit after emitting the signal
    }
}



