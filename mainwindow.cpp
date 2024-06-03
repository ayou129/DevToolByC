#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QVBoxLayout>
#include <QPushButton>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow),
    sshSession(ssh_new())
{
    ui->setupUi(this);

    textEdit = new QTextEdit(this);
    textEdit->setReadOnly(true);

    QPushButton *button = new QPushButton("Connect and Execute Command", this);
    connect(button, &QPushButton::clicked, this, &MainWindow::connectToServer);

    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(textEdit);
    layout->addWidget(button);

    QWidget *centralWidget = new QWidget(this);
    centralWidget->setLayout(layout);
    setCentralWidget(centralWidget);
}

MainWindow::~MainWindow()
{
    delete ui;
    ssh_free(sshSession);
}

void MainWindow::connectToServer() {
    ssh_options_set(sshSession, SSH_OPTIONS_HOST, "192.168.31.20");
    ssh_options_set(sshSession, SSH_OPTIONS_USER, "liguoxin");

    int rc = ssh_connect(sshSession);
    if (rc != SSH_OK) {
        QMessageBox::critical(this, "Error", "Failed to connect to the server");
        return;
    }

    rc = ssh_userauth_password(sshSession, nullptr, "li680910");
    if (rc != SSH_AUTH_SUCCESS) {
        QMessageBox::critical(this, "Error", "Failed to authenticate");
        ssh_disconnect(sshSession);
        return;
    }

    executeCommand("ls -la");
    ssh_disconnect(sshSession);
}

void MainWindow::executeCommand(const char *command) {
    ssh_channel channel = ssh_channel_new(sshSession);
    if (channel == nullptr) return;

    int rc = ssh_channel_open_session(channel);
    if (rc != SSH_OK) {
        ssh_channel_free(channel);
        return;
    }

    rc = ssh_channel_request_exec(channel, command);
    if (rc != SSH_OK) {
        ssh_channel_close(channel);
        ssh_channel_free(channel);
        return;
    }

    char buffer[256];
    int nbytes;
    while ((nbytes = ssh_channel_read(channel, buffer, sizeof(buffer), 0)) > 0) {
        textEdit->append(QString::fromUtf8(buffer, nbytes));
    }

    ssh_channel_close(channel);
    ssh_channel_free(channel);
}
