/*----------------------------------------------------------------
//Copyright (C) 2022 frankkon All rights reserved.
//This program comes with ABSOLUTELY NO WARRANTY.
//This is free software, and you are welcome to redistribute it
//under AGPL V3 conditions.

// 鼠标模拟点击工具 版权所有
// 文件名：ctask.h
// 文件功能描述： 任务信息数据类
// 作者：孔俊
// 时间：2022-08-27
// 修改描述：
//  1、2022-08-27 初稿完成
//----------------------------------------------------------------*/

#ifndef CTASK_H
#define CTASK_H

#include <QObject>
#include <QPoint>

class CTask : public QObject
{
    Q_OBJECT
public:
    explicit CTask(QObject* parent = nullptr);

    CTask(qint32 iInterval, const QPoint& pointTaskPos, qint32 iTaskType, const QString& sTaskComments,
          QObject* parent = nullptr);

    explicit CTask(const CTask& task);

    CTask& operator=(const CTask& task);

    qint32 getInterval() const;
    QPoint getTaskPos() const;
    qint32 getTaskType() const;
    QString getTaskComments() const;

    void setInterval(qint32 iInterval);
    void setTaskPos(const QPoint& pointTaskPos);
    void setTaskType(qint32 iTaskType);
    void setTaskComments(const QString& sTaskComments);

    void doTask();


private:
    void inputWords(QString strWords);
    //void inputMouse(quint32 iActions);

private:
    //任务执行的时间间隔
    qint32 m_iInterval;

    //任务执行的位置
    QPoint m_pointTaskPos;

    //任务类型：1：单击，2：双击，3：左键按下，4：左键抬起（通过3和4两个动作组合实现“拖动”）
    qint32 m_iTaskType;

    //任务说明
    QString m_sTaskComments;

};

#endif // CTASK_H
