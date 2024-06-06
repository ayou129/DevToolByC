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

// #include "AnsiParser.cpp"
class TerminalWidget : public QTextEdit {
public:
    TerminalWidget(QWidget *parent = nullptr) : QTextEdit(parent) {
        // 获取项目路径
        // QString projectPath = QDir::currentPath();
        // QString fontPath = projectPath + "/fonts/Roboto-Bold.ttf";
        QTextOption option = document()->defaultTextOption();
        option.setFlags(option.flags() | QTextOption::AddSpaceForLineAndParagraphSeparators);
        option.setWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);
        document()->setDefaultTextOption(option);
        // // 加载字体
        // int fontId = QFontDatabase::addApplicationFont(fontPath);
        // if (fontId != -1) {
        //     QString family = QFontDatabase::applicationFontFamilies(fontId).at(0);

        //     QFont font(family, 12);
        //     setFont(font);  // 设置字体

        //     // QColor textBGColor = "white";
        //     // setTextBackgroundColor(textBGColor);
        // } else {
        //     qWarning() << "Failed to load font:" << fontPath;
        // }

        setReadOnly(true);

        // 设置全局样式表
        // setStyleSheet(
        //     "QTextEdit {"
        //     "}"
        //     );

        // 确保每个段落应用行高设置
        // QTextDocument *doc = document();
        // QTextCursor cursor(doc);
        // for (QTextBlock it = doc->begin(); it != doc->end(); it = it.next()) {
        //     QTextCursor textCursor(it);
        //     QTextBlockFormat textBlockFormat = it.blockFormat();
        //     textBlockFormat.setLineHeight(24, QTextBlockFormat::FixedHeight);  // 设置固定行高
        //     textCursor.setBlockFormat(textBlockFormat);

        //     // // QTextCursor cursor = textCursor();
        //     // QTextCharFormat format;
        //     // format.setVerticalAlignment(QTextCharFormat::AlignMiddle); // 垂直居中
        //     // textCursor.setCharFormat(format);
        // }

        // 将光标移动到文本末尾
        moveCursor(QTextCursor::End);

        QFont currentFont = font();
        qDebug() << "Font Family:" << currentFont.family();
        qDebug() << "Font Size:" << currentFont.pointSize();
    }
};


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , terminalOutput(new TerminalWidget(this))
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

    inputLine->setPlaceholderText("Enter command...");

    connect(inputLine, &QLineEdit::returnPressed, this, &MainWindow::onEnterPressed);

    sshReadTimer->setInterval(100); // 设置检查间隔，单位毫秒
    connect(sshReadTimer, &QTimer::timeout, this, &MainWindow::handleReadFromSSH);

    initializeSSH();

    ansiParser = new AnsiParser(this);  // 创建AnsiParser实例
    connect(ansiParser, &AnsiParser::newPlainText, this, &MainWindow::appendTextWithANSI);
    connect(ansiParser, &AnsiParser::newFormattedText, this, &MainWindow::appendTextWithANSI);
    connect(ansiParser, &AnsiParser::titleChanged, this, &MainWindow::setWindowTitle);
    connect(ansiParser, &AnsiParser::clearScreen, this, &MainWindow::clearTerminal);
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
    // terminalOutput->moveCursor(QTextCursor::End);
    terminalOutput->insertHtml(text);
    // terminalOutput->moveCursor(QTextCursor::End);
}

void MainWindow::onEnterPressed()
{
    QString command = inputLine->text();
    if (!command.isEmpty()) {
        sendCommand(command);
        inputLine->clear();
    }
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

void MainWindow::clearTerminal() {
    terminalOutput->clear();
}

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
