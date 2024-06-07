#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextEdit>
#include <QLineEdit>
#include <QTimer>
#include <libssh/libssh.h>
#include <libssh/callbacks.h>
#include <iostream>

#include <QMap>
#include <QStack>
#include <QObject>
#include <QString>
// #include "TerminalWidget.h"

template<typename T>
void log(T&& message) {
    std::cerr << std::forward<T>(message) << std::endl;
}

class AnsiParser : public QObject {
    Q_OBJECT

public:
    AnsiParser(QObject *parent = nullptr);

signals:
    void newPlainText(const QString &text);
    void newFormattedText(const QString &html);
    void titleChanged(const QString &title);
    void clearScreen();

public slots:
    void processText(const QString &text);

private:
    enum class State {
        Ground,
        Escape,
        // EscapeIntermediate,
        CsiEntry,
        CsiIntermediate,
        CsiIgnore,
        CsiParam,
        // CsiSubParam,
        OscParam,
        // OscString,
        // OscTermination,
        // Ss3Entry,
        // Ss3Param,
        // Vt52Param,
        // DcsEntry,
        // DcsIgnore,
        // DcsIntermediate,
        // DcsParam,
        // DcsPassThrough,
        // SosPmApcString
    };
    // bool _processingLastCharacter;
    State currentState;
    QString buffer;
    QString _currentString;
    QStack<QString> openTags;

    QString formatStack;

    void handleInitialBackspaces();
    void processCharacter(const QChar &ch);
    void handleEscapeSequence(const QChar &ch);
    void handleCsiSequence(const QChar &ch);
    void handleCsiIntermediate(const QChar &ch);
    void handleCsiIgnore(const QChar &ch);
    void handleOscSequence(const QString &sequence);
    void handleOscTermination(const QChar &ch);
    void handleSs3Sequence(const QChar &ch);
    void handleVt52Param(const QChar &ch);
    void handleDcsSequence(const QChar &ch);
    void handleDcsIgnore(const QChar &ch);
    void handleDcsIntermediate(const QChar &ch);
    void handleDcsPassThrough(const QChar &ch);
    void handleSosPmApcString(const QChar &ch);
    void parseCsiSequence(const QString &seq);
    void handleControlSequence(const QString &seq);
    void clearScreenPart(const QString &seq);
    QString getColorFromCode(const QString &code);
    QString closeTag(const QString &tag);
    QString closeAllTags();
    QString completeHtmlTags(const QString &html);
};



class TerminalWidget : public QWidget {
    Q_OBJECT
public:
    QTextEdit *outputEdit;
    QLineEdit *inputLine;
    QString *title;
    explicit TerminalWidget(QWidget *parent = nullptr);
    QString getStyle(QString type);

signals:
    void commandEntered(const QString &command);
};



QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void appendTextWithANSI(const QString &text);
    QString parseANSI(const QString &text);
    void updateTitle(const QString &title);
    AnsiParser *ansiParser;
    // void resizeEvent(QResizeEvent* event);


private:
    Ui::MainWindow *ui;
    TerminalWidget *terminalWidget;
    QTimer* sshReadTimer;
    ssh_session session;
    ssh_channel channel;

    void initializeSSH();
    void sendCommand(const QString &command);
    // void clearTerminal();


private slots:
    void onEnterPressed(const QString &command);
    void handleReadFromSSH();
};


#endif // MAINWINDOW_H
