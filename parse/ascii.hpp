#pragma once

enum AsciiChars : wchar_t
{
    NUL = 0x0, // 空字符
    SOH = 0x1, // 标题开始
    STX = 0x2, // 正文开始
    ETX = 0x3, // 正文结束
    EOT = 0x4, // 传输结束
    ENQ = 0x5, // 询问
    ACK = 0x6, // 确认
    BEL = 0x7, // 响铃
    BS = 0x8,  // 退格
    TAB = 0x9, // 水平制表符
    LF = 0xA,  // 换行
    VT = 0xB,  // 垂直制表符
    FF = 0xC,  // 换页
    CR = 0xD,  // 回车
    SO = 0xE,  // 锁定换码
    SI = 0xF,  // 解锁换码
    DLE = 0x10, // 数据链路转义
    DC1 = 0x11, // 设备控制1
    DC2 = 0x12, // 设备控制2
    DC3 = 0x13, // 设备控制3
    DC4 = 0x14, // 设备控制4
    NAK = 0x15, // 否定应答
    SYN = 0x16, // 同步空闲
    ETB = 0x17, // 传输块结束
    CAN = 0x18, // 取消
    EM = 0x19, // 媒介结束
    SUB = 0x1A, // 替补
    ESC = 0x1B, // 逃逸
    FS = 0x1C, // 文件分隔符
    GS = 0x1D, // 组分隔符
    RS = 0x1E, // 记录分隔符
    US = 0x1F, // 单元分隔符
    SPC = 0x20, // 空格
    DEL = 0x7F, // 删除
};
