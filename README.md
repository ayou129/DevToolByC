错误命令操作
~~~
123\u001B[?1l\u001B>\u001B[?2004l\r\r\n\u001B]2;123\u0007\u001B]1;123\u0007zsh: command not found: 123\r\n\u001B[1m\u001B[7m%\u001B[27m\u001B[1m\u001B[0m
~~~


VIM 操作
~~~
\u001B[2;1H▽\u001B[6n\u001B[2;1H  \u001B[3;1H\u001BPzz\u001B\\\u001B[0%m\u001B[6n\u001B[3;1H           \u001B[1;1H\u001B[>c\u001B]10;?\u0007\u001B]11;?\u0007\u001B[2;1H\u001B[34m#1717648738\u001B[m\u001B[2;12H\u001B[K\u001B[3;1Hls\u001B[3;3H\u001B[K\u001B[4;1H\u001B[34m#1717648745\u001B[m\r\nll\r\n\u001B[34m#1717649123\u001B[m\r\npwd\r\n\u001B[34m#1717649517\u001B[m\r\nll\r\n\u001B[34m#1717650474\u001B[m\r\ncat ~/.ssh/authorized_keys\r\n\u001B[34m#1717651472\u001B[m\r\nset +o history;\r\n\u001B[34m#1717652494\u001B[m\r\nls\r\n\u001B[34m#1717652500\u001B[m\r\ncd ~\r\n\u001B[34m#1717652501\u001B[m\r\nls\r\n\u001B[34m#1717652503\u001B[m\r\nll\r\n\u001B[34m#1717657191\u001B[m\r\nls\u001B[24;63H1,1\u001B[11CTop\u001B[1;1H\u001B[?25h
~~~

其他补充
~~~
   // // 加载字体
    // QString projectPath = QDir::currentPath();
    // QString fontPath = projectPath + "/fonts/Roboto-Bold.ttf";

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

    // outputEdit->setReadOnly(true);


    // // 将光标移动到文本末尾
    // outputEdit->moveCursor(QTextCursor::End);

    // QFont currentFont = font();
    // qDebug() << "Font Family:" << currentFont.family();
    // qDebug() << "Font Size:" << currentFont.pointSize();
	
	
	    // outputEdit->setStyleSheet("QTextEdit { "
    //                           "background-color: #333; "
    //                           "color: #fff; "
    //                           "font-family: 'Monospace'; "
    //                           "font-size: 12pt; "
    //                           "border: none; "
    //                           "margin: 0; "
    //                           "padding: 0; }");

    // inputLine = new QLineEdit(this);
    // inputLine->setStyleSheet("QLineEdit { "
    //                          "background-color: #333; "
    //                          "color: #fff; "
    //                          "font-family: 'Monospace'; "
    //                          "font-size: 12pt; "
    //                          "border: none; "
    //                          "margin: 0; "
    //                          "padding: 0; }");


~~~