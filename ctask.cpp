/*----------------------------------------------------------------
//Copyright (C) 2022 frankkon All rights reserved.
//This program comes with ABSOLUTELY NO WARRANTY.
//This is free software, and you are welcome to redistribute it
//under AGPL V3 conditions.

// 鼠标模拟点击工具 版权所有
// 文件名：ctask.cpp
// 文件功能描述： 任务信息数据类
// 作者：孔俊
// 时间：2022-08-27
// 修改描述：
//  1、2022-08-27 初稿完成
//----------------------------------------------------------------*/

#include <windows.h>
#include <QDesktopWidget>
#include <QApplication>
#include <QDebug>

#include "ctask.h"
#include "typedef.h"


CTask::CTask(QObject* parent)
    : QObject{ parent }
    , m_iInterval(0)
    , m_pointTaskPos(0, 0)
    , m_iTaskType(TASK_CLICK)
    , m_sTaskComments("")
{
}

CTask::CTask(qint32 iInterval, const QPoint& pointTaskPos, qint32 iTaskType, const QString& sTaskComments, QObject* parent)
    : QObject{ parent }
    , m_iInterval(iInterval)
    , m_pointTaskPos(pointTaskPos)
    , m_iTaskType(iTaskType)
    , m_sTaskComments(sTaskComments)
{
}

CTask::CTask(const CTask& task)
    : QObject{}
    , m_iInterval(task.m_iInterval)
    , m_pointTaskPos(task.m_pointTaskPos)
    , m_iTaskType(task.m_iTaskType)
    , m_sTaskComments(task.m_sTaskComments)
{
}

CTask& CTask::operator=(const CTask& task)
{
    m_iInterval = task.m_iInterval;
    m_pointTaskPos = task.m_pointTaskPos;
    m_iTaskType = task.m_iTaskType;
    m_sTaskComments = task.m_sTaskComments;

    return *this;
}

qint32 CTask::getInterval() const
{
    return m_iInterval;
}

QPoint CTask::getTaskPos() const
{
    return m_pointTaskPos;
}

qint32 CTask::getTaskType() const
{
    return m_iTaskType;
}

QString CTask::getTaskComments() const
{
    return m_sTaskComments;
}

void CTask::setInterval(qint32 iInterval)
{
    m_iInterval = iInterval;
}

void CTask::setTaskPos(const QPoint& pointTaskPos)
{
    m_pointTaskPos = pointTaskPos;
}

void CTask::setTaskType(qint32 iTaskType)
{
    m_iTaskType = iTaskType;
}

void CTask::setTaskComments(const QString& sTaskComments)
{
    m_sTaskComments = sTaskComments;
}

void CTask::doTask()
{
    //将鼠标移动到目标位置
    SetCursorPos(m_pointTaskPos.x(), m_pointTaskPos.y());

    switch (m_iTaskType)
    {
        case TASK_CLICK:
        {
            mouse_event(MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
            break;
        }
        case TASK_DOUBLE_CLICK:
        {
            mouse_event(MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
            mouse_event(MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
            break;
        }
        case TASK_LEFT_DOWN:
        {
            mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
            break;
        }
        case TASK_LEFT_UP:
        {
            mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
            break;
        }
        case TASK_KEY_INPUT:
        {
            //先鼠标点击一下，激活窗口然后输入文字。
            mouse_event(MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
            inputWords(m_sTaskComments);
            break;
        }
        default:
            break;
    }
}

void CTask::inputWords(QString strWords)
{
    INPUT inputs[2] = {};

    qint32 idx = 0;
    while (idx < strWords.length())
    {
        ZeroMemory(inputs, sizeof(inputs));
        inputs[0].type = INPUT_KEYBOARD;
        inputs[0].ki.wVk = 0;
        inputs[0].ki.wScan = m_sTaskComments.at(idx).unicode();
        inputs[0].ki.dwFlags = KEYEVENTF_UNICODE;

        inputs[1].type = INPUT_KEYBOARD;
        inputs[1].ki.wVk = 0;
        inputs[1].ki.wScan = m_sTaskComments.at(idx).unicode();
        inputs[1].ki.dwFlags = KEYEVENTF_UNICODE | KEYEVENTF_KEYUP;

        UINT uSent = SendInput(ARRAYSIZE(inputs), inputs, sizeof(INPUT));
        if (uSent != ARRAYSIZE(inputs))
        {
            qDebug() << "键盘输入失败。应输入字符：" << m_sTaskComments.at(idx) << Qt::endl;
        }
        ++idx;
    }
}

//void CTask::inputMouse(quint32 iActions)
//{
//    INPUT input = {};
//    ZeroMemory(&input, sizeof(input));
//    input.type = INPUT_MOUSE;
//    input.mi.dwFlags = iActions;
//    SendInput(1, &input, sizeof(INPUT));
//}