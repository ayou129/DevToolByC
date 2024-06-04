#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QMainWindow>
#include <QtNetwork/QTcpSocket>
#include <QPlainTextEdit>
#include <QLineEdit>
#include <QTimer>
#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <libssh/libssh.h>
#include <libssh/callbacks.h>
#include <QVBoxLayout>
#include <QPushButton>
#include <QMessageBox>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "DevTool_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }
    MainWindow w;
    w.show();
    return a.exec();
}


