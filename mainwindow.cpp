#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <thread>

#include <QMainWindow>
#include <QtNetwork/QTcpSocket>
#include <QPlainTextEdit>
#include <QLineEdit>
#include <QTimer>
#include <QVBoxLayout>
#include <QPushButton>
#include <QMessageBox>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , terminalOutput(new QTextEdit(this))
    , inputLine(new QLineEdit(this))
    , sshReadTimer(new QTimer(this))
{
    ui->setupUi(this);

    setWindowTitle("SSH Terminal");
    setGeometry(200, 200, 800, 600);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(terminalOutput);
    layout->addWidget(inputLine);

    QWidget *centralWidget = new QWidget(this);
    centralWidget->setLayout(layout);
    setCentralWidget(centralWidget);

    terminalOutput->setReadOnly(true);
    inputLine->setPlaceholderText("Enter command...");

    connect(inputLine, &QLineEdit::returnPressed, this, &MainWindow::onEnterPressed);

    sshReadTimer->setInterval(100); // 设置检查间隔，单位毫秒
    connect(sshReadTimer, &QTimer::timeout, this, &MainWindow::handleReadFromSSH);

    initializeSSH();
}

MainWindow::~MainWindow()
{
    // 关闭SSH读取定时器
    sshReadTimer->stop();

    // 清理SSH资源
    if (channel) {
        ssh_channel_close(channel);
        ssh_channel_free(channel);
    }
    if (session) {
        ssh_disconnect(session);
        ssh_free(session);
    }

    delete ui;
}

void MainWindow::appendTextWithANSI(const QString &text)
{
    terminalOutput->append(parseANSI(text));
}

void MainWindow::onEnterPressed()
{
    QString command = inputLine->text();
    if (!command.isEmpty()) {
        sendCommand(command);
        inputLine->clear();
    }
}

void MainWindow::initializeSSH()
{
    session = ssh_new();
    ssh_options_set(session, SSH_OPTIONS_HOST, "192.168.31.20");
    ssh_options_set(session, SSH_OPTIONS_USER, "liguoxin");

    int rc = ssh_connect(session);
    if (rc != SSH_OK) {
        log(ssh_get_error(session));
        return;
    }

    rc = ssh_userauth_password(session, nullptr, "li680910");
    if (rc != SSH_AUTH_SUCCESS) {
        log(ssh_get_error(session));
        ssh_disconnect(session);
        ssh_free(session);
        return;
    }

    channel = ssh_channel_new(session);
    if (channel == nullptr) {
        log(ssh_get_error(session));
        return;
    }

    rc = ssh_channel_open_session(channel);
    if (rc != SSH_OK) {
        log(ssh_get_error(session));
        ssh_channel_free(channel);
        return;
    }

    rc = ssh_channel_request_shell(channel);
    if (rc != SSH_OK) {
        log(ssh_get_error(session));
        ssh_channel_close(channel);
        ssh_channel_free(channel);
        return;
    }

    sshReadTimer->start();
}

QString MainWindow::parseANSI(const QString &text)
{
    QString parsedText = text;
    // 解析ANSI颜色码
    parsedText.replace("\033[31m", "<span style=\"color:red;\">");
    parsedText.replace("\033[32m", "<span style=\"color:green;\">");
    parsedText.replace("\033[0m", "</span>");
    return parsedText;
}

void MainWindow::sendCommand(const QString &command)
{
    QString commandWithNewline = command + "\n";
    ssh_channel_write(channel, commandWithNewline.toUtf8().data(), commandWithNewline.size());
}

void MainWindow::handleReadFromSSH()
{
    char buffer[256];
    int nbytes;

    while ((nbytes = ssh_channel_read_nonblocking(channel, buffer, sizeof(buffer), 0)) > 0) {
        QString data = QString::fromUtf8(buffer, nbytes);

        // 处理ANSI码，更新标题等
        if (data.contains("\033]0;")) { // 检测到标题更新
            int start = data.indexOf("\033]0;") + 4;
            int end = data.indexOf("\007", start);
            if (end != -1) {
                QString title = data.mid(start, end - start);
                qDebug() << "Title updated: " << title;
                setWindowTitle(title);
            }
        }

        appendTextWithANSI(data);
    }
}
