#include "mainwindow.h"
// #include "./ui_mainwindow.h"
#include <QVBoxLayout>
#include <QRegularExpression>
#include <QDebug>
#include <QStack>
#include <QString>
#include <QPalette>
#include <QScrollBar>
#include <QTextCursor>
#include <QTextBlock>
#include <QTextBlockFormat>
#include <QFontDatabase>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QDir>
#include <QLabel>


TerminalWidget::TerminalWidget(QWidget *parent) : QWidget(parent) {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0, 0, 0, 0);  // 确保没有外部边距

    outputEdit = new QTextEdit(this);
    outputEdit->setReadOnly(true);
    outputEdit->setWordWrapMode(QTextOption::WordWrap);
    outputEdit->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    outputEdit->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    outputEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    outputEdit->setStyleSheet("QTextEdit { background-color: #333; color: #fff; font-family: 'Monospace'; font-size: 12pt; border: none; margin: 0; padding: 0; }");

    mainLayout->addWidget(outputEdit, 1);  // 让 outputEdit 在垂直方向上扩展

    QHBoxLayout *inputLayout = new QHBoxLayout;
    inputLayout->setSpacing(0);
    inputLayout->setContentsMargins(0, 0, 0, 0);

    QLabel *label = new QLabel("Command:");
    label->setStyleSheet("QLabel { color: #ABB2BF; padding-right: 5px; }");

    inputLine = new QLineEdit(this);
    inputLine->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    inputLine->setStyleSheet("QLineEdit { background-color: #222; color: #fff; font-family: 'Monospace'; font-size: 12pt; border: none; margin: 0; padding: 0; }");

    inputLayout->addWidget(label);
    inputLayout->addWidget(inputLine, 1);

    mainLayout->addLayout(inputLayout);  // 将输入布局添加到主布局

    setLayout(mainLayout);  // 设置布局

    connect(inputLine, &QLineEdit::returnPressed, this, [this]() {
        emit commandEntered(inputLine->text());
        inputLine->clear();
    });
}

