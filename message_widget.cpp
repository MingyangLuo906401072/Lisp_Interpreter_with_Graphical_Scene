#include <QLayout>
#include <QLabel>

#include "message_widget.hpp"

MessageWidget::MessageWidget(QWidget* parent) : QWidget(parent) {
    // Create a layout for the widget
    QHBoxLayout* layout = new QHBoxLayout(this);

    // Create a label to display "Message:"
    messageLabel = new QLabel("Message:", this);
    layout->addWidget(messageLabel);

    // Change from QTextEdit to QLineEdit
    messageLineEdit = new QLineEdit(this);
    messageLineEdit->setReadOnly(true); // Set it to read-only to display text

    messageLineEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed); // Set size policy

    layout->addWidget(messageLineEdit);

    // Set the layout for the widget
    setLayout(layout);
}

void MessageWidget::info(QString message) {
    messageLineEdit->setStyleSheet("background-color: white; color: black;"); // Set the style to resemble QTextEdit
    messageLineEdit->setText(message);
}

void MessageWidget::error(QString message) {
    // Set the text color and highlight color
    QPalette palette = messageLineEdit->palette();
    palette.setColor(QPalette::Highlight, Qt::red); // Set the highlight color
    palette.setColor(QPalette::HighlightedText, Qt::white); // Set the text color in the highlight
    messageLineEdit->setPalette(palette);

    // Set the text and ensure selection and focus
    messageLineEdit->setText(message);
    messageLineEdit->selectAll();
    messageLineEdit->setFocus();
}

void MessageWidget::clear() {
    messageLineEdit->clear();
}
