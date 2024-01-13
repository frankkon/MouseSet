/*----------------------------------------------------------------
//Copyright (C) 2022 frankkon All rights reserved.
//This program comes with ABSOLUTELY NO WARRANTY.
//This is free software, and you are welcome to redistribute it
//under AGPL V3 conditions.

// 鼠标模拟点击工具 版权所有
// 文件名：dlgaddtask.h
// 文件功能描述： 添加鼠标点击任务
// 作者：孔俊
// 时间：2022-08-27
// 修改描述：
//  1、2022-08-27 初稿完成
//----------------------------------------------------------------*/

#ifndef DLGADDTASK_H
#define DLGADDTASK_H

#include <QDialog>

class CTask;

namespace Ui
{
class dlgAddTask;
}

class dlgAddTask : public QDialog
{
    Q_OBJECT

public:
    explicit dlgAddTask(QWidget* parent = nullptr);
    dlgAddTask(CTask* pTask, QWidget* parent = nullptr);
    ~dlgAddTask();

    CTask* getTaskInfo(void);

private slots:
    void onPositionGet();
    void onAccepted();
    void onTaskTypeChanged(int idx);

private:
    Ui::dlgAddTask* ui;

    //保存任务信息
    CTask* m_pTask;

    //任务位置信息
    QPoint m_ptPosition;

};

#endif // DLGADDTASK_H




