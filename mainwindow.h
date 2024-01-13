/*----------------------------------------------------------------
//Copyright (C) 2022 frankkon All rights reserved.
//This program comes with ABSOLUTELY NO WARRANTY.
//This is free software, and you are welcome to redistribute it
//under AGPL V3 conditions.

// 鼠标模拟点击工具 版权所有
// 文件名：mianwindow.h
// 文件功能描述： 软件主界面
// 作者：孔俊
// 时间：2022-08-27
// 修改描述：
//  1、2022-08-27 初稿完成
//----------------------------------------------------------------*/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>

#include <windows.h>

#include "ctask.h"

QT_BEGIN_NAMESPACE
namespace Ui
{
    class MainWindow;
}
QT_END_NAMESPACE

class QAction;
class QMenu;
class QPoint;
class QListWidgetItem;
class CSuspendFrame;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

    void recordTask(int nCode, WPARAM wParam, LPARAM lParam);

protected:
    virtual void closeEvent(QCloseEvent* event);

public slots:
    void onTaskStopRecord();

private slots:

    //任务执行
    void onTaskAdd();
    void onTaskCopy();
    void onTaskStart();
    void onTaskReset();

    //双击任务列表项，删除任务
    void onTaskRemove(QListWidgetItem* item);

    //任务列表上下文菜单
    void onTaskUpdate();
    void onTaskInsertAbove();
    void onTaskInsertBelow();

    //任务定时执行和LCD读秒
    void onTaskDo();
    void onLcdUpdate();

    //任务保存和载入
    void onTasksSave();
    void onTasksLoad();

    //录制鼠标事件Action
    void onTaskStartRecord();
//    void onTaskStopRecord();

    //帮助和版权信息Action
    void onAbout();
    void onHelp();

private:

    //主菜单初始化
    void createMainMenu();

    //任务列表上下文菜单初始化
    void createContextMenu();

    //添加任务
    void insertTaskbyPosition(int iPostion);

    //标志任务列表中的当前行是否已经执行过
    bool isCurrentTaskDone();

    //停止任务执行，清空任务信息
    void resetTask();

    //鼠标录制
    void processLeftButtonDown(WPARAM wParam, LPARAM lParam);
    void processLeftButtonUp(WPARAM wParam, LPARAM lParam);
    void initRecordInfo();
    void saveLastMouseInfo(int posX, int posY, WPARAM actionType);
    void showSuspendFrame(bool bShow);

private:
    Ui::MainWindow* ui;

    //鼠标点击间隔触发器
    QTimer m_timerTask;

    //读秒触发器
    QTimer m_timerLcd;

    //鼠标点击执行控制按钮
    bool m_bStartFlag;

    //鼠标点击任务（位置和时间间隔、类型、描述）列表
    QList< CTask > m_lstTasks;

    //当前鼠标点击执行到第几步
    qint32 m_iStage;

    //任务列表的右键菜单，用于修改和插入任务
    QAction* m_pActionUpdateTask;
    QAction* m_pActionInsertTaskAbove;
    QAction* m_pActionInsertTaskBelow;

    //主菜单：展示帮助、版权信息等
    QMenu* m_pMainMenu;
    QAction* m_pActionAbout;
    QAction* m_pActionHelp;

    //录制鼠标缓存信息
    QPoint m_pointLastPostion;
    DWORD m_iLastTickTime;  //用于集合左键松开，判断拖动
    WPARAM m_uLastActionType;
    bool m_bFirstClick;
    QPoint m_pointLDLastPostion;
    DWORD m_iLastLDTickTime;
    DWORD m_iLastTaskTickTime; //上个任务的tick时间，用来设置任务的时间间隔。单位：毫秒。

    //录制鼠标时的浮窗
    CSuspendFrame* m_pFrame;

};
#endif // MAINWINDOW_H



