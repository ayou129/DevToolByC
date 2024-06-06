#include "mainwindow.h"
// #include "./ui_mainwindow.h"
#include <QVBoxLayout>
#include <QRegularExpression>
#include <QDebug>
#include <QStack>
#include <QString>

AnsiParser::AnsiParser(QObject *parent) : QObject(parent), currentState(State::Ground) {}
QString AnsiParser::completeHtmlTags(const QString &html) {
    QStack<QString> tags;
    QString result = html;
    static const QRegularExpression tagPattern(R"(<(/?)(\w+)[^>]*>)");  // 定义为静态常量
    QRegularExpression re(tagPattern);
    QRegularExpressionMatchIterator it = tagPattern.globalMatch(html); // 使用静态正则表达式


    while (it.hasNext()) {
        QRegularExpressionMatch match = it.next();
        QString tag = match.captured(0);    // 捕获整个标签
        QString tagName = match.captured(2); // 捕获标签名
        bool isClosingTag = !match.captured(1).isEmpty(); // 是否是闭合标签

        if (!isClosingTag) {
            // 如果是开标签，压入栈中
            tags.push(tagName);
        } else {
            // 如果是闭标签，从栈中弹出直到匹配的标签名
            if (!tags.isEmpty() && tags.top() == tagName) {
                tags.pop();
            }
        }
    }

    // 处理未闭合的标签，追加到结果字符串的末尾
    while (!tags.isEmpty()) {
        QString unclosedTag = tags.pop();
        result.append("</" + unclosedTag + ">");
    }

    return result;
}

void AnsiParser::processText(const QString &text) {
    qDebug() << "Processing text: " << text << static_cast<int>(currentState);  // 调试输出
    formatStack = text;
    size_t i = 0;
    handleInitialBackspaces();
    // qDebug() << "\n formatStack " << formatStack;
    _currentString.clear();
    currentState = State::Ground;
    buffer.clear();

    // .h 文件中已经定义类的成员 _currentString;

    while (i < formatStack.length()) {
        const QChar ch = formatStack.at(i);

        switch (currentState) {
            case State::Ground:
                if (ch == '\x1b') {
                    _currentString = completeHtmlTags(_currentString);

                    currentState = State::Escape;  // 进入转义状态
                } else {
                    _currentString += ch;
                }
                break;
            case State::Escape:
                if (ch == '[') {
                    currentState = State::CsiEntry;  // 开始CSI序列
                    buffer.clear();
                } else if (ch == ']') {
                    currentState = State::OscParam;  // 开始OSC序列
                    buffer.clear();
                } else if (ch == '>') {
                    // 处理键盘或其他设备的序列
                    currentState = State::CsiIgnore;
                } else {
                    currentState = State::Ground;
                }
                break;

            case State::CsiEntry:
            case State::CsiParam:
                buffer.append(ch);
                if (ch.isDigit() || ch == ';') {
                    // 继续收集参数
                } else if (ch == 'm' || ch == 'J' || ch == 'h' || ch == 'l') {
                    // 处理设置模式、颜色、格式等序列
                    handleControlSequence(buffer);
                    buffer.clear();
                    currentState = State::Ground;
                } else {
                    // 非结束字符，可能是中间字符，如`'`或`"`在某些复杂的CSI序列中
                    currentState = State::CsiIntermediate;
                }
                break;

            case State::CsiIntermediate:
                if (ch.isLetter()) {
                    // 处理中间状态结束，如从`CSI Intermediate`到`CSI End`
                    buffer.append(ch);
                    handleControlSequence(buffer);
                    buffer.clear();
                    currentState = State::Ground;
                } else {
                    // 继续收集中间状态的字符
                    buffer.append(ch);
                }
                break;
            case State::OscParam:
                if (ch == '\u0007') {  // OSC序列终止
                    handleOscSequence(buffer);
                    buffer.clear();
                    currentState = State::Ground;
                }else{
                    buffer.append(ch);
                }
                break;
            case State::CsiIgnore:
                // 当遇到无法识别或不期望处理的序列时，保持忽略状态直到序列结束
                if (ch.isLetter()) {
                    currentState = State::Ground;
                }
                break;
            default:
                _currentString += ch;
                break;
        }


        i++;
    }

    if (!_currentString.isEmpty()) {
        emit newPlainText(_currentString);  // 发射解析后的纯文本
    }
}

void AnsiParser::handleInitialBackspaces() {
    // static QRegularExpression promptRegex("(\\x1b\\[[^m]*m)?[%$#]\\s*$");
    static const QRegularExpression promptPattern(R"(\x1b\[7m%)");

    // 使用正则表达式移除匹配到的提示符模式
    formatStack.remove(promptPattern);

    formatStack = formatStack.trimmed();

    // 检查字符串长度足够，并且第二个和第三个字符是退格符
    if (formatStack.size() >= 2 && formatStack[1] == '\b') {
        formatStack.remove(0, 2);
    }
    // 替换 Windows 风格和旧 Mac 风格的换行符，最后替换为 HTML 换行标签
    formatStack.replace("\r\r\n", "<br></br>");
    formatStack.replace("\r\n", "<br></br>");
    formatStack.replace("\r", "<br></br>");    // 然后替换旧 Mac 风格的换行符
    formatStack.replace("\n", "<br></br>");  // 最后替换为 HTML 换行标签
}


void AnsiParser::parseCsiSequence(const QString &seq) {
    QStringList parts = seq.split(';');
    QString htmlOutput;  // 用于构建最终的 HTML 字符串

    // qDebug() << "\n handleControlSequence ANSI color code: " << seq << "\n" << parts  << "\n";

    for (const QString &part : parts) {
        int code = part.split('m')[0].toInt();
        switch (code) {
            case 0:  // 重置所有模式
                // QString closedTags;
                // while (!openTags.isEmpty()) {
                //     closedTags += openTags.pop();
                // }
                // return closedTags;
                // htmlOutput += closeAllTags();
                break;
            case 1:  // 粗体
                htmlOutput += "<b>";
                break;
            case 22:  // 关闭粗体
                htmlOutput += "</b>";
                break;
            case 3:  // 斜体
                htmlOutput += "<i>";
                break;
            case 23:  // 关闭斜体
                htmlOutput += "</i>";
                break;
            case 4:  // 下划线
                htmlOutput += "<u>";
                break;
            case 24:  // 关闭下划线
                htmlOutput +=  "</u>";
                break;
            case 7:  // 反向（反色）
                htmlOutput += "<span style=\"filter: invert(100%);\">";
                break;
            case 27:  // 关闭反向
                htmlOutput += "</span>";
                break;
            case 9:  // 删除线
                htmlOutput += "<s>";
                break;
            case 29:  // 关闭删除线
                htmlOutput += "</s>";
                break;
            default:
                if (part.startsWith("3") || part.startsWith("4")) {  // 前景色和背景色
                    QString color = getColorFromCode(part);
                    QString style = part.startsWith("3") ? "color" : "background-color";
                    htmlOutput += QString("<span style=\"%1:%2;\">").arg(style, color);
                }
                break;
        }
    }

    // 确保所有打开的标签都被正确闭合
    // while (!openTags.isEmpty()) {
    //     htmlOutput += openTags.pop();
    // }

    // 追加到当前字符串
    _currentString += htmlOutput;
    // qDebug() << "Processed HTML: " << htmlOutput;
}



QString AnsiParser::getColorFromCode(const QString &code) {
    static const QMap<QString, QString> colorMap = {
        {"30", "black"}, {"31", "red"}, {"32", "green"}, {"33", "yellow"},
        {"34", "#FF851A"}, {"35", "magenta"}, {"36", "cyan"}, {"37", "white"},
        {"40", "black"}, {"41", "red"}, {"42", "green"}, {"43", "yellow"},
        {"44", "blue"}, {"45", "magenta"}, {"46", "cyan"}, {"47", "white"}
    };


    QString numericCode = code;
    int endIndex = numericCode.indexOf('m');
    if (endIndex != -1) {
        numericCode = numericCode.left(endIndex);
    }

    // Lookup the color or return "inherit" if not found
    QString color = colorMap.value(numericCode, "inherit");
    if (color == "inherit") {
        qDebug() << "Unrecognized ANSI color code: " << code;
    }

    return color;
}

void AnsiParser::handleControlSequence(const QString &seq) {
    if (seq.endsWith('m')) {
        // 文本属性设置，如颜色、粗体等
        parseCsiSequence(seq);
    } else if (seq.endsWith('J')) {
        // 清屏操作
        clearScreenPart(seq);
    } else if (seq.endsWith('h') || seq.endsWith('l')) {
        // 模式设置或重置，如键盘模式、回显模式等
        qDebug() << "Mode set/reset sequence: " << seq;
    }
}

void AnsiParser::clearScreenPart(const QString &sequence) {
    // 分析序列中的数字，确定清屏范围
    int index = sequence.indexOf('J');
    QString numberPart = sequence.mid(2, index - 2); // 假设序列格式是 "\x1b[2J"
    int mode = numberPart.isEmpty() ? 0 : numberPart.toInt();

    switch (mode) {
    case 0:
        // 清除从光标位置到屏幕末尾的内容
        // emit clearScreenFromCursorToEnd();
        qDebug() << "Cleared screen from cursor to end.";
        break;
    case 1:
        // 清除从屏幕开始到光标位置的内容
        // emit clearScreenFromStartToCursor();
        qDebug() << "Cleared screen from start to cursor.";
        break;
    case 2:
        // 清除整个屏幕
        // emit clearEntireScreen();
        qDebug() << "Cleared entire screen.";
        break;
    default:
        qDebug() << "Unsupported clear screen mode: " << mode;
        break;
    }
}


void AnsiParser::handleOscSequence(const QString &sequence){
    int semicolonIndex = buffer.indexOf(';');
    if (semicolonIndex != -1) {
        int oscCode = buffer.left(semicolonIndex).toInt();
        QString command = buffer.mid(semicolonIndex + 1);

        switch (oscCode) {
        case 0: // 设置图标和窗口标题
        case 2: // 设置窗口标题
            emit titleChanged(command);
            break;
        // 可以根据需要处理更多的OSC命令
        default:
            qDebug() << "Unsupported OSC command:" << oscCode << "with data:" << command;
            break;
        }
    }
}

