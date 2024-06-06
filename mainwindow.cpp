#include "mainwindow.h"
#include "./ui_mainwindow.h"
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
#include <QDir>
// #include "TerminalWidget.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , terminalWidget(new TerminalWidget(this))
    , sshReadTimer(new QTimer(this))
{
    ui->setupUi(this);

    setStyleSheet(
        "MainWindow {"
            "background-color: #333; "
        "}"
        );

    // 设置窗口大小和标题
    setWindowTitle("Custom Terminal");
    resize(800, 600);
    setGeometry(200, 200, 800, 600);

    QVBoxLayout *layout = new QVBoxLayout;

    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(terminalWidget);
    layout->setSpacing(0);

    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    centralWidget->setLayout(layout);

    connect(terminalWidget, &TerminalWidget::commandEntered, this, &MainWindow::onEnterPressed);


    sshReadTimer->setInterval(100); // 设置检查间隔，单位毫秒
    connect(sshReadTimer, &QTimer::timeout, this, &MainWindow::handleReadFromSSH);

    initializeSSH();

    ansiParser = new AnsiParser(this);  // 创建AnsiParser实例
    connect(ansiParser, &AnsiParser::newPlainText, this, &MainWindow::appendTextWithANSI);
    connect(ansiParser, &AnsiParser::newFormattedText, this, &MainWindow::appendTextWithANSI);
    connect(ansiParser, &AnsiParser::titleChanged, this, &MainWindow::setWindowTitle);
    // connect(ansiParser, &AnsiParser::clearScreen, this, &MainWindow::clearTerminal);
}

void MainWindow::initializeSSH()
{
    QString authMethod = "miyao";

    session = ssh_new();
    if (authMethod == "mima") {

        ssh_options_set(session, SSH_OPTIONS_HOST, "192.168.31.20");
        ssh_options_set(session, SSH_OPTIONS_USER, "liguoxin");
    } else if (authMethod == "miyao") {

        ssh_options_set(session, SSH_OPTIONS_HOST, "43.136.67.170");
        ssh_options_set(session, SSH_OPTIONS_USER, "ubuntu");
    }

    int rc = ssh_connect(session);
    if (rc != SSH_OK) {
        qDebug() << ssh_get_error(session);
        return;
    }

    if (authMethod == "mima") {
        rc = ssh_userauth_password(session, nullptr, "li680910");
    } else if (authMethod == "miyao") {
        // 设置密钥文件路径
        ssh_key key = ssh_key_new();
        if (ssh_pki_import_privkey_file("C:/Users/li/.ssh/ylkj.pem", nullptr, nullptr, nullptr, &key) != SSH_OK) {
            qDebug() << "Failed to import private key:" << ssh_get_error(session);
            ssh_key_free(key);
            ssh_disconnect(session);
            ssh_free(session);
            return;
        }
        rc = ssh_userauth_publickey(session, nullptr, key);
        ssh_key_free(key);
    }

    if (rc != SSH_AUTH_SUCCESS) {
        qDebug() << ssh_get_error(session);
        ssh_disconnect(session);
        ssh_free(session);
        return;
    }

    channel = ssh_channel_new(session);
    if (channel == nullptr) {
        qDebug() << ssh_get_error(session);
        return;
    }

    ssh_set_blocking(session, 1); // 设置非阻塞模式

    rc = ssh_channel_open_session(channel);
    if (rc != SSH_OK) {
        qDebug() << ssh_get_error(session);
        ssh_channel_free(channel);
        return;
    }

    rc = ssh_channel_request_pty(channel); // 请求伪终端（PTY）
    if (rc != SSH_OK) {
        qDebug() << ssh_get_error(session);
        ssh_channel_close(channel);
        ssh_channel_free(channel);
        return;
    }

    rc = ssh_channel_request_shell(channel); // 请求交互式 shell
    if (rc != SSH_OK) {
        qDebug() << ssh_get_error(session);
        ssh_channel_close(channel);
        ssh_channel_free(channel);
        return;
    }

    qDebug() << "SSH session and channel successfully initialized";

    // 启动定时器以继续读取数据
    sshReadTimer->start();

    // 立即读取数据以捕获欢迎信息
    handleReadFromSSH();
}



void MainWindow::appendTextWithANSI(const QString &text)
{

    QString result = text;
    // result.replace(" ", "&nbsp;");
    qDebug() << "Appending text: " << text;
    // terminalWidget->moveCursor(QTextCursor::End);
    terminalWidget->outputEdit->insertHtml("<pre>" +text + "</pre>");
    // terminalWidget->moveCursor(QTextCursor::End);
}

void MainWindow::onEnterPressed(const QString &command)
{
    sendCommand(command);
    // QString command = inputLine->text();
    // if (!command.isEmpty()) {
    //     sendCommand(command);
    //     inputLine->clear();
    // }
}


void MainWindow::updateTitle(const QString &title)
{
    // 更新窗口标题
    // this->setWindowTitle(title);
    qDebug() << "Title updated: " << title;
}



void MainWindow::sendCommand(const QString &command)
{
    QString commandWithNewline = command + "\n";
    ssh_channel_write(channel, commandWithNewline.toUtf8().data(), commandWithNewline.size());
}


void MainWindow::handleReadFromSSH()
{
    char buffer[1024];  // 增加缓冲区大小
    int nbytes;
    // qDebug() << "Reading from SSH";

    if (channel == nullptr) {
        qDebug() << "Channel is null";
        sshReadTimer->stop();
        return;
    }

    if (!ssh_channel_is_open(channel)) {
        qDebug() << "Channel is not open";
        sshReadTimer->stop();
        return;
    }

    if (ssh_channel_is_eof(channel)) {
        qDebug() << "Channel reached EOF";
        sshReadTimer->stop();
        return;
    }

    // 使用非阻塞读取模式，处理所有已到达的数据
    while ((nbytes = ssh_channel_read_nonblocking(channel, buffer, sizeof(buffer), 0)) > 0) {
        QString data = QString::fromUtf8(buffer, nbytes);
        qDebug() << "Read data: " << data << "Bytes read: " << nbytes;
        ansiParser->processText(data);
    }

    if (nbytes == SSH_ERROR) {
        qDebug() << "Error reading from SSH channel: " << ssh_get_error(session);
    } else {
        // qDebug() << "No more data to read, retrying...";
    }
}

// void MainWindow::resizeEvent(QResizeEvent* event) {
//     QMainWindow::resizeEvent(event);
//     terminalWidget->layout()->activate();
//     qDebug() << "MainWindow resized to:" << event->size();  // 输出窗口新尺寸
//     qDebug() << "TerminalWidget size:" << terminalWidget->size();  // 输出终端控件的尺寸
// }

MainWindow::~MainWindow()
{
    sshReadTimer->stop();

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
