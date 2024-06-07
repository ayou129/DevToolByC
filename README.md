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
## .vscode 文件夹内容
.vscode/settings.json
~~~
{
    "files.associations": {
        "*.embeddedhtml": "html",
        "*.cjson": "jsonc",
        "*.wxss": "css",
        "*.wxs": "javascript",
        "*.md": "markdown",
        "qplaintextedit": "cpp",
        "qlineedit": "cpp",
        "string": "cpp",
        "array": "cpp",
        "atomic": "cpp",
        "bit": "cpp",
        "*.tcc": "cpp",
        "cctype": "cpp",
        "charconv": "cpp",
        "chrono": "cpp",
        "clocale": "cpp",
        "cmath": "cpp",
        "compare": "cpp",
        "concepts": "cpp",
        "condition_variable": "cpp",
        "cstdarg": "cpp",
        "cstddef": "cpp",
        "cstdint": "cpp",
        "cstdio": "cpp",
        "cstdlib": "cpp",
        "cstring": "cpp",
        "ctime": "cpp",
        "cwchar": "cpp",
        "cwctype": "cpp",
        "deque": "cpp",
        "list": "cpp",
        "map": "cpp",
        "set": "cpp",
        "unordered_map": "cpp",
        "vector": "cpp",
        "exception": "cpp",
        "expected": "cpp",
        "algorithm": "cpp",
        "functional": "cpp",
        "iterator": "cpp",
        "memory": "cpp",
        "memory_resource": "cpp",
        "numeric": "cpp",
        "optional": "cpp",
        "random": "cpp",
        "ratio": "cpp",
        "string_view": "cpp",
        "system_error": "cpp",
        "tuple": "cpp",
        "type_traits": "cpp",
        "utility": "cpp",
        "format": "cpp",
        "future": "cpp",
        "initializer_list": "cpp",
        "iomanip": "cpp",
        "iosfwd": "cpp",
        "iostream": "cpp",
        "istream": "cpp",
        "limits": "cpp",
        "mutex": "cpp",
        "new": "cpp",
        "numbers": "cpp",
        "ostream": "cpp",
        "semaphore": "cpp",
        "span": "cpp",
        "sstream": "cpp",
        "stdexcept": "cpp",
        "stdfloat": "cpp",
        "stop_token": "cpp",
        "streambuf": "cpp",
        "text_encoding": "cpp",
        "thread": "cpp",
        "cinttypes": "cpp",
        "typeinfo": "cpp",
        "variant": "cpp",
        "qregularexpression": "cpp",
        "qdebug": "cpp",
        "qstack": "cpp"
    },
    "C_Cpp.errorSquiggles": "disabled"
}
~~~
.vscode/launch.json
~~~
{
    "version": "0.2.0",
    "configurations": [
        {
            "name": "QT Launch",
            "type": "cppdbg",
            "request": "launch",
            "program": "${workspaceFolder}/build/cmake-Debug/DevTool.exe",
            "args": [],
            "stopAtEntry": false,
            "cwd": "${workspaceFolder}",
            "environment": [],
            "externalConsole": false,
            "MIMode": "gdb",
            "miDebuggerPath": "gdb.exe",
            "setupCommands": [
                {
                    "description": "Enable pretty-printing for gdb",
                    "text": "-enable-pretty-printing",
                    "ignoreFailures": true
                }
            ],
            "preLaunchTask": "Build",
            "internalConsoleOptions": "openOnSessionStart"
        }
    ]
}
~~~
.vscode/tasks.json
~~~
{
    "version": "2.0.0",
    "tasks": [
        {
            "label": "Create Build Directories",
            "type": "shell",
            "command": "mkdir -p ${workspaceFolder}/build/cmake-Debug",
            "group": "build"
        },
        {
            "label": "Build",
            "type": "shell",
            "command": "cmake.exe",
            "args": [
                "--build", "${workspaceFolder}/build/cmake-Debug",
                "--target", "all"
            ],
            "group": {
                "kind": "build",
                "isDefault": true
            },
            "problemMatcher": [
                "$gcc"
            ],
            "presentation": {
                "echo": true,
                "reveal": "always",
                "focus": false,
                "panel": "shared",
                "showReuseMessage": true,
                "clear": false
            },
            "dependsOn": ["Run DevTool"]
        },
        {
            "label": "Run DevTool",
            "type": "shell",
            "command": "${workspaceFolder}/build/cmake-Debug/DevTool.exe",
            "group": {
                "kind": "none",
                "isDefault": false
            },
            "problemMatcher": []
        }
    ]
}
~~~

.vscode/c_cpp_properties.json
~~~
{
    "configurations": [
        {
            "name": "Qt Configuration",
            "includePath": [
                "${workspaceFolder}/**",
                "C://Qt//6.7.1//mingw_64//include//**"
            ],
            "browse": {
                "path": [
                    "${workspaceFolder}",
                    "C://Qt//6.7.1//mingw_64//include"
                ]
            },
            "intelliSenseMode": "gcc-x64",
            "compilerPath": "C:/Qt/Tools/mingw1120_64/bin/gcc.exe",
            "configurationProvider": "ms-vscode.cmake-tools"
        }
    ],
    "version": 4
}
~~~

