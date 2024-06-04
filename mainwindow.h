#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork/QTcpSocket>
#include <QPlainTextEdit>
#include <QLineEdit>
#include <QTimer>
#include <libssh/libssh.h>
#include <libssh/callbacks.h>
#include <iostream>
#include <string>

template<typename T>
void log(T&& message) {
    std::cerr << std::forward<T>(message) << std::endl;
}

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void appendTextWithANSI(const QString &text);
    QString parseANSI(const QString &text);

private:
    Ui::MainWindow *ui;
    QTextEdit *terminalOutput;
    QLineEdit *inputLine;
    ssh_session session;
    ssh_channel channel;

    void initializeSSH();
    QTimer* sshReadTimer;
    void sendCommand(const QString &command);

private slots:
    void onEnterPressed();
    void handleReadFromSSH();
};


#endif // MAINWINDOW_H
