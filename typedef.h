/*----------------------------------------------------------------
//Copyright (C) 2022 frankkon All rights reserved.
//This program comes with ABSOLUTELY NO WARRANTY.
//This is free software, and you are welcome to redistribute it
//under AGPL V3 conditions.

// 鼠标模拟点击工具 版权所有
// 文件名：typedef.h
// 文件功能描述： 常量定义
// 作者：孔俊
// 时间：2022-08-27
// 修改描述：
//  1、2022-08-27 初稿完成
//----------------------------------------------------------------*/

#ifndef TYPEDEF_H
#define TYPEDEF_H

#include <QtGlobal>
#include <QString>

#define APP_NAME (tr("鼠标点击模拟器 v1.0"))

//应用保存的文件头信息
static const quint32 MOUSE_SET_FILE_HEAD = 0x8E62D878;

//任务说明文本的最大长度
static const qint32 MAX_COMMENTS_LENGTH = 20;

//鼠标双击的时，2次点击的最大允许像素点偏移
static const qint32 DBCLICK_MAX_PIXEL_OFFSET = 3;

//距离桌面右下角的像素偏移
static const qint32 PIXEL_TO_DESKTOP_RIGHT = 50;
static const qint32 PIXEL_TO_DESKTOP_BOTTOM = 50;

//停止录制浮窗动画速度(毫秒)
static const qint32 WINDOW_MOVE_SPEED_MS = 10;
static const qint32 WINDOW_MOVE_DURATION_MS = 500;
//任务类型枚举
enum TTaskType
{
    TASK_CLICK = 0,
    TASK_DOUBLE_CLICK = 1,
    TASK_LEFT_DOWN = 2,
    TASK_LEFT_UP = 3,
    TASK_KEY_INPUT = 4
};

//和任务类型相对应的任务描述字典
static QString G_TASK[5] = { "单击。", "双击。", "按下左键。", "左键松开。", "输入：" };

//区分插入任务时的位置
enum TInsertPosition
{
    ABOVE_POSITION = 0,
    BELOW_POSITION = 1,
    END_POSITION = 2
};

//安全堆内存释放
#define SAFE_DELETE(x)  do \
                        { \
                            if( (x) != nullptr ) \
                            { \
                                delete (x); \
                                (x) = nullptr; \
                            } \
                        }while(0);


#endif // TYPEDEF_H
