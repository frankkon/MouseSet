/*----------------------------------------------------------------
//Copyright (C) 2022 frankkon All rights reserved.
//This program comes with ABSOLUTELY NO WARRANTY.
//This is free software, and you are welcome to redistribute it
//under AGPL V3 conditions.

// 鼠标模拟点击工具 版权所有
// 文件名：mianwindow.cpp
// 文件功能描述： 软件主界面
// 作者：孔俊
// 时间：2022-08-27
// 修改描述：
//  1、2022-08-27 初稿完成
//----------------------------------------------------------------*/

#include <QEvent>
#include <QMouseEvent>
#include <QtDebug>
#include <QMessageBox>
#include <QAction>
#include <QFileDialog>
#include <QMenu>
#include <QMenuBar>

#include "typedef.h"
#include "global.h"
#include "ctask.h"
#include "dlgaddtask.h"
#include "csuspendframe.h"
#include "ui_mainwindow.h"
#include "mainwindow.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_timerTask(nullptr)
    , m_timerLcd(nullptr)
    , m_bStartFlag(false)
    , m_iStage(0)
    , m_pointLastPostion(0, 0)
    , m_iLastTickTime(0)
    , m_uLastActionType(0)
    , m_bFirstClick(true)
    , m_pointLDLastPostion(0, 0)
    , m_iLastLDTickTime(0)
    , m_iLastTaskTickTime(0)
    , m_pFrame(nullptr)

{
    //界面初始化
    ui->setupUi(this);
    ui->btnTaskSave->setText("保存任务");
    ui->btnTaskLoad->setText("载入任务");
    ui->btnRecordStart->setText("录制鼠标");
    ui->btnRecordStop->setText("停止录制");
    ui->btnRecordStop->setDisabled(true);
    setWindowIcon(QIcon(":/images/appicon48.png"));
    setWindowTitle(APP_NAME);
    QPalette lcdpat = ui->lcdNumber->palette();
    lcdpat.setColor(QPalette::Normal, QPalette::WindowText, Qt::red);
    ui->lcdNumber->setSegmentStyle(QLCDNumber::Flat);
    ui->lcdNumber->setPalette(lcdpat);
    //layout()->setSizeConstraint(QLayout::SetFixedSize);

    //菜单初始化
    createMainMenu();
    createContextMenu();
    //信号槽连接
    connect(&m_timerTask, SIGNAL(timeout()), this, SLOT(onTaskDo()));
    connect(&m_timerLcd, SIGNAL(timeout()), this, SLOT(onLcdUpdate()));
    connect(ui->btnTaskAdd, SIGNAL(clicked()), this, SLOT(onTaskAdd()));
    connect(ui->btnTaskCopy, SIGNAL(clicked()), this, SLOT(onTaskCopy()));
    connect(ui->btnTaskStart, SIGNAL(clicked()), this, SLOT(onTaskStart()));
    connect(ui->btnTaskReset, SIGNAL(clicked()), this, SLOT(onTaskReset()));
    connect(ui->lstWgtTask, SIGNAL(itemDoubleClicked(QListWidgetItem*)),
            this, SLOT(onTaskRemove(QListWidgetItem*)));
    connect(ui->btnTaskSave, SIGNAL(clicked()), this, SLOT(onTasksSave()));
    connect(ui->btnTaskLoad, SIGNAL(clicked()), this, SLOT(onTasksLoad()));
    connect(ui->btnRecordStart, SIGNAL(clicked()), this, SLOT(onTaskStartRecord()));
    connect(ui->btnRecordStop, SIGNAL(clicked()), this, SLOT(onTaskStopRecord()));

}

MainWindow::~MainWindow()
{
    SAFE_DELETE(m_pFrame);
    SAFE_DELETE(ui);
}

void MainWindow::recordTask(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (HC_ACTION == nCode)
    {
        if (WM_LBUTTONDOWN == wParam)
        {
            qDebug() << "catch a left button down." << Qt::endl;
            processLeftButtonDown(wParam, lParam);
        }
        else if (WM_LBUTTONUP == wParam)
        {
            qDebug() << "catch a left button up." << Qt::endl;
            processLeftButtonUp(wParam, lParam);
        }
    }
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    if (m_bStartFlag)
    {
        int iRetCode = QMessageBox::warning(this, APP_NAME, "任务正在执行中，确定退出吗？",
                                            QMessageBox::Close | QMessageBox::No);
        if (QMessageBox::Close == iRetCode)
        {
            event->accept();
        }
        else
        {
            event->ignore();
        }
    }

}

//添加鼠标点击任务
void MainWindow::onTaskAdd()
{
    insertTaskbyPosition(END_POSITION);
}

//启动任务执行
void MainWindow::onTaskStart()
{
    if (m_bStartFlag)
    {
        //点击暂停模拟
        m_timerTask.stop();
        m_timerLcd.stop();
        ui->btnTaskStart->setText("开始模拟");
        ui->lcdNumber->display(0);
        m_bStartFlag = false;

    }
    else //点击开始模拟
    {
        //判断点击任务是否全部执行完成
        if (m_lstTasks.count() <= m_iStage)
        {
            QMessageBox::information(this, APP_NAME, "请先添加任务！");
            return;
        }

        //如果时从头开始执行，则将之前置灰的任务重新点亮
        if (0 == m_iStage)
        {
            qint32 idx = 0;
            while (idx < ui->lstWgtTask->count())
            {
                ui->lstWgtTask->item(idx)->setForeground(Qt::black);
                idx++;
            }
        }

        //根据选择，设置定时器触发时间：interval（分钟）
        qint32 interval = m_lstTasks[m_iStage].getInterval() * 1000;
        m_timerTask.stop();
        m_timerTask.start(interval);

        //初始化，每秒钟刷新一下LCD的显示内容
        qint32 totalNum = m_lstTasks[m_iStage].getInterval();
        ui->lcdNumber->display(totalNum);
        m_timerLcd.stop();
        m_timerLcd.start(1000);

        m_bStartFlag = true;
        ui->btnTaskStart->setText("暂停模拟");

        ui->btnTaskLoad->setDisabled(true);
        ui->btnRecordStart->setDisabled(true);
        ui->btnRecordStop->setDisabled(true);


    }
}

//重置全部任务
void MainWindow::onTaskReset()
{
    if (m_bStartFlag)
    {
        int iRetCode = QMessageBox::question(this, APP_NAME, "任务正在执行中，确定重置吗？");
        if (QMessageBox::No == iRetCode)
        {
            return;
        }
    }

    resetTask();
}

//双击删除不需要的鼠标任务
void MainWindow::onTaskRemove(QListWidgetItem* item)
{
    //如果在模拟点击任务执行中就不能删
    if (m_bStartFlag)
    {
        return;
    }

    //已经执行过的和正在执行的任务不能删除
    qint32 currRow = ui->lstWgtTask->currentRow();
    if (currRow <= m_iStage && 0 != m_iStage)
    {
        return;
    }

    //删除列表控件中的任务
    ui->lstWgtTask->removeItemWidget(item);
    SAFE_DELETE(item);  //必须delete,否则listwidget中会仍然留着

    //删除任务列表中的任务
    m_lstTasks.removeAt(currRow);

}

//先选中一个已有的任务，通过复制添加到任务列表最后
void MainWindow::onTaskCopy()
{
    QModelIndexList idxList = ui->lstWgtTask->selectionModel()->selectedIndexes();

    if (0 == idxList.count())
    {
        QMessageBox::information(this, APP_NAME, "没有可复制的任务，请先添加或选中任务，然后复制！");
        return;
    }

    std::stable_sort(idxList.begin(), idxList.end());

    qint32 idx = 0;
    QListWidgetItem* item = nullptr;
    while (idx < idxList.count())
    {
        //添加到任务列表控件
        item = ui->lstWgtTask->item(idxList[idx].row());
        ui->lstWgtTask->addItem(item->text());

        //添加到任务列表
        m_lstTasks.append(m_lstTasks[idxList[idx].row()]);
        idx++;
    }

}

void MainWindow::onTaskUpdate()
{
    int idx = ui->lstWgtTask->currentRow();

    //设置鼠标点击信息
    CTask* pTask = new CTask(m_lstTasks[idx].getInterval(),
                             m_lstTasks[idx].getTaskPos(),
                             m_lstTasks[idx].getTaskType(),
                             m_lstTasks[idx].getTaskComments());

    dlgAddTask* dlg = new dlgAddTask(pTask);
    qint32 res = dlg->exec();
    if (QDialog::Accepted == res)
    {
        QString sTaskInfo = QString::number(pTask->getInterval())
                            + "秒后，在坐标【X=" + QString::number(pTask->getTaskPos().x())
                            + "，Y=" + QString::number(pTask->getTaskPos().y()) + "】位置"
                            + G_TASK[pTask->getTaskType()]
                            + pTask->getTaskComments();

        QListWidgetItem* item = ui->lstWgtTask->item(idx);
        item->setText(sTaskInfo);
        m_lstTasks[idx] = *pTask;
        qDebug() << sTaskInfo << Qt::endl;
        qDebug() << "position:" << m_lstTasks[idx].getTaskPos()
                 << "task type:" << m_lstTasks[idx].getTaskType()
                 << "interval:" << m_lstTasks[idx].getInterval()
                 << "comments:" << m_lstTasks[idx].getTaskComments() << Qt::endl;
    }

    SAFE_DELETE(pTask);
    SAFE_DELETE(dlg);
}

void MainWindow::onTaskInsertAbove()
{
    if (isCurrentTaskDone())
    {
        return;
    }

    insertTaskbyPosition(ABOVE_POSITION);
}

void MainWindow::onTaskInsertBelow()
{
    if (isCurrentTaskDone())
    {
        return;
    }

    insertTaskbyPosition(BELOW_POSITION);
}

//任务等待间隔时间到触发鼠标事件
void MainWindow::onTaskDo()
{

    //执行鼠标操作任务
    m_lstTasks[m_iStage].doTask();

    //已经执行过的操作置灰
    ui->lstWgtTask->item(m_iStage)->setForeground(Qt::gray);

    //设置下一次点击
    m_iStage++;
    if (m_lstTasks.count() > m_iStage)
    {
        //重置LCD读秒
        qint32 totalNum = m_lstTasks[m_iStage].getInterval();
        ui->lcdNumber->display(totalNum);

        //设置新的触发间隔(毫秒)
        qint32 interval = m_lstTasks[m_iStage].getInterval() * 1000;
        m_timerTask.stop();
        m_timerTask.start(interval);
    }
    else  //任务全部执行完成，重置
    {
        m_iStage = 0;
        m_timerTask.stop();
        m_timerLcd.stop();
        ui->btnTaskStart->setText("开始模拟");
        ui->lcdNumber->display(0);
        m_bStartFlag = false;

        ui->btnTaskAdd->setEnabled(true);
        ui->btnTaskCopy->setEnabled(true);
        ui->btnTaskSave->setEnabled(true);
        ui->btnTaskLoad->setEnabled(true);
        ui->btnTaskStart->setEnabled(true);
        ui->btnRecordStart->setEnabled(true);
        ui->btnRecordStop->setDisabled(true);
    }
}

//LCD读秒
void MainWindow::onLcdUpdate()
{
    //倒计时读秒
    qint32 currentNum = ui->lcdNumber->intValue();
    ui->lcdNumber->display(currentNum - 1);
}

void MainWindow::onTasksSave()
{
    if (0 == m_lstTasks.count())
    {
        QMessageBox::information(this, APP_NAME, "没有可保存的任务，请先添加任务！");
        return;
    }

    QString sFileName = QFileDialog::getSaveFileName(this, "保存任务", ".", "Mouse Set(*.mst)");

    if (sFileName.isEmpty())
    {
        return;
    }

    QFile file(sFileName);
    if (!file.open(QIODevice::WriteOnly))
    {
        QMessageBox::warning(this, APP_NAME,
                             tr("无法写入文件 %1:\n%2.").arg(file.fileName(), file.errorString()));
        return;
    }

    QDataStream out(&file);
    out.setVersion(QDataStream::Qt_5_15);
    out << quint32(MOUSE_SET_FILE_HEAD);

    QApplication::setOverrideCursor(Qt::WaitCursor);
    for (int i = 0; i < m_lstTasks.count(); ++i)
    {
        out << m_lstTasks[i].getInterval()
            << m_lstTasks[i].getTaskPos()
            << m_lstTasks[i].getTaskType()
            << m_lstTasks[i].getTaskComments();
    }
    QApplication::restoreOverrideCursor();
}

void MainWindow::onTasksLoad()
{
    QString sFileName = QFileDialog::getOpenFileName(this, "载入任务", ".", "Mouse Set(*.mst)");

    if (sFileName.isEmpty())
    {
        return;
    }

    QFile file(sFileName);
    if (!file.open(QIODevice::ReadOnly))
    {
        QMessageBox::warning(this, APP_NAME,
                             tr("无法读取文件 %1:\n%2.")
                             .arg(file.fileName(), file.errorString())
                            );
        return;
    }

    QDataStream in(&file);
    in.setVersion(QDataStream::Qt_5_15);
    quint32 iFileHead = 0;
    in >> iFileHead;
    if (iFileHead != MOUSE_SET_FILE_HEAD)
    {
        QMessageBox::warning(this, APP_NAME,
                             tr("要载入的文件不是本应用的文件."));
        return;
    }

    QApplication::setOverrideCursor(Qt::WaitCursor);
    qint32 iInterval = 0;
    QPoint pointTaskPos(0, 0);
    qint32 iTaskType = 0;
    QString sTaskComments{ "" };
    int idx = 0;

    resetTask();

    CTask task;
    while (!in.atEnd())
    {
        in >> iInterval >> pointTaskPos >> iTaskType >> sTaskComments;
        task.setInterval(iInterval);
        task.setTaskPos(pointTaskPos);
        task.setTaskType(iTaskType);
        task.setTaskComments(sTaskComments);
        ui->lstWgtTask->insertItem(idx, QString::number(task.getInterval())
                                   + "秒后，在坐标【X=" + QString::number(task.getTaskPos().x()) + "，Y="
                                   + QString::number(task.getTaskPos().y()) + "】位置"
                                   + G_TASK[task.getTaskType()]
                                   + task.getTaskComments());
        m_lstTasks.insert(idx, task);
        ++idx;
    }
    QApplication::restoreOverrideCursor();
}

void MainWindow::onTaskStartRecord()
{
    initRecordInfo();

    G_MOUSE_HOOK = SetWindowsHookExW(WH_MOUSE_LL, recordMouseTask, ModuleFromAddress((PVOID)recordMouseTask), 0);
    if (G_MOUSE_HOOK != NULL)
    {
        showSuspendFrame(true);
        ui->btnTaskAdd->setDisabled(true);
        ui->btnTaskCopy->setDisabled(true);
        ui->btnTaskSave->setDisabled(true);
        ui->btnTaskLoad->setDisabled(true);
        ui->btnTaskStart->setDisabled(true);
        ui->btnTaskReset->setDisabled(true);
        ui->btnRecordStart->setDisabled(true);
        ui->btnRecordStop->setEnabled(true);
        G_MAINWINDOW = this;
        qDebug() << "set hook successful." << Qt::endl;
    }
}

void MainWindow::onTaskStopRecord()
{
    BOOL ret = UnhookWindowsHookEx(G_MOUSE_HOOK);
    if (ret)
    {
        showSuspendFrame(false);
        ui->btnTaskAdd->setEnabled(true);
        ui->btnTaskCopy->setEnabled(true);
        ui->btnTaskSave->setEnabled(true);
        ui->btnTaskLoad->setEnabled(true);
        ui->btnTaskStart->setEnabled(true);
        ui->btnTaskReset->setEnabled(true);
        ui->btnRecordStart->setEnabled(true);
        ui->btnRecordStop->setDisabled(true);
        qDebug() << "unset hook successful." << Qt::endl;
    }
}

void MainWindow::onAbout()
{
    QMessageBox::about(this, APP_NAME,
                       tr("<h2>") + APP_NAME + tr("</h2>"
                                                  "<p>Copyright &copy; 2022 kongjun"
                                                  "<p>版权归孔俊所有"
                                                  "<p>有任何疑问，欢迎联系作者"
                                                  "<p>E-Mail：frankkon@sina.com"));
}

void MainWindow::onHelp()
{
    QMessageBox::about(this, APP_NAME,
                       tr("<h2>操作指导：</h2>"
                          "<p>1. 单击【添加任务】按钮，设置鼠标点击任务；"
                          "<p>2. 双击任务可以删除未执行任务；"
                          "<p>3. 右击任务修改或插入新任务；"
                          "<p>4. 选中一个或多个任务后，单击【复制任务】按钮，快速添加多个相同任务；"
                          "<p>5. 单击【录制鼠标】按钮，自动记录鼠标单击、双击、拖动任务；"
                          "<p>6. 单击【重置】按钮清空所有任务。"));
}

void MainWindow::createMainMenu()
{
    m_pActionAbout = new QAction("关于", this);
    m_pActionHelp = new QAction("帮助", this);
    m_pMainMenu = menuBar()->addMenu("&H帮助");
    m_pMainMenu->addAction(m_pActionHelp);
    m_pMainMenu->addAction(m_pActionAbout);

    connect(m_pActionAbout, SIGNAL(triggered()), this, SLOT(onAbout()));
    connect(m_pActionHelp, SIGNAL(triggered()), this, SLOT(onHelp()));
}

void MainWindow::createContextMenu()
{
    m_pActionUpdateTask = new QAction("修改任务信息", this);
    m_pActionInsertTaskAbove = new QAction("在上方插入任务", this);
    m_pActionInsertTaskBelow = new QAction("在下方插入任务", this);

    connect(m_pActionUpdateTask, SIGNAL(triggered()), this, SLOT(onTaskUpdate()));
    connect(m_pActionInsertTaskAbove, SIGNAL(triggered()), this, SLOT(onTaskInsertAbove()));
    connect(m_pActionInsertTaskBelow, SIGNAL(triggered()), this, SLOT(onTaskInsertBelow()));

    ui->lstWgtTask->addAction(m_pActionUpdateTask);
    ui->lstWgtTask->addAction(m_pActionInsertTaskAbove);
    ui->lstWgtTask->addAction(m_pActionInsertTaskBelow);
    ui->lstWgtTask->setContextMenuPolicy(Qt::ActionsContextMenu);
}

void MainWindow::insertTaskbyPosition(int iPostion)
{
    //设置鼠标点击信息
    dlgAddTask* dlg = new dlgAddTask();
    qint32 res = dlg->exec();
    if (QDialog::Accepted == res)
    {
        CTask* pTaskInfo = dlg->getTaskInfo();

        int idx = 0;
        if (ABOVE_POSITION == iPostion)
        {
            idx = ui->lstWgtTask->currentRow();
        }
        else if (BELOW_POSITION == iPostion)
        {
            idx = ui->lstWgtTask->currentRow() + 1;
        }
        else if (END_POSITION == iPostion)
        {
            idx = ui->lstWgtTask->count();
        }

        ui->lstWgtTask->insertItem(idx, QString::number(pTaskInfo->getInterval())
                                   + "秒后，在坐标【X=" + QString::number(pTaskInfo->getTaskPos().x()) + "，Y="
                                   + QString::number(pTaskInfo->getTaskPos().y()) + "】位置"
                                   + G_TASK[pTaskInfo->getTaskType()]
                                   + pTaskInfo->getTaskComments());
        m_lstTasks.insert(idx, *pTaskInfo);
    }

    SAFE_DELETE(dlg);
}

//检查当前位置的任务是否已经执行
bool MainWindow::isCurrentTaskDone()
{
    //如果任务在执行中，不能在已经完成的任务间插入新任务
    int idx = ui->lstWgtTask->currentRow();
    if (idx <= m_iStage && m_bStartFlag)
    {
        QMessageBox::information(this, APP_NAME, "当前位置的任务已经执行，暂时无法插入。\n请结束后再插入！");
        return true;
    }
    else
    {
        return false;
    }
}

void MainWindow::resetTask()
{
    m_iStage = 0;
    m_timerTask.stop();
    m_timerLcd.stop();
    ui->btnTaskStart->setText("开始模拟");
    ui->lcdNumber->display(0);
    m_bStartFlag = false;
    m_lstTasks.clear();
    ui->lstWgtTask->clear();

    ui->btnTaskAdd->setEnabled(true);
    ui->btnTaskCopy->setEnabled(true);
    ui->btnTaskSave->setEnabled(true);
    ui->btnTaskLoad->setEnabled(true);
    ui->btnTaskStart->setEnabled(true);
    ui->btnTaskReset->setEnabled(true);
    ui->btnRecordStart->setEnabled(true);
    ui->btnRecordStop->setDisabled(true);

}

void MainWindow::processLeftButtonDown(WPARAM wParam, LPARAM lParam)
{
    //MOUSEHOOKSTRUCT* pMouseHook = (MOUSEHOOKSTRUCT*)lParam;
    MOUSEHOOKSTRUCT* pMouseHook = reinterpret_cast<MOUSEHOOKSTRUCT*>(lParam);
    DWORD iNowTickTime = GetTickCount();
    QPoint pointNowPosition(pMouseHook->pt.x, pMouseHook->pt.y);
    qDebug() << "X=" << pMouseHook->pt.x << ",Y=" << pMouseHook->pt.y << Qt::endl;

    if (m_bFirstClick)
    {
        //qDebug() << "first button down." << Qt::endl;
        //记录点击位置、时间间隔用于后面和鼠标松开的位置进行对比判断是拖动还是单击、双击。
        m_pointLDLastPostion.setX(pMouseHook->pt.x);
        m_pointLDLastPostion.setY(pMouseHook->pt.y);
        m_iLastLDTickTime = iNowTickTime;
        m_uLastActionType = WM_LBUTTONDOWN;
        m_bFirstClick = false;
    }
    else
    {
        //将上次和这次点击合并按照双击处理,根据验证，双击是在第二次按下时触发。
        //双击时，鼠标抖动容错，误差在3个像素以内。
        int x = pointNowPosition.x() - m_pointLastPostion.x();
        int y = pointNowPosition.y() - m_pointLastPostion.y();
        if ((abs(x) < DBCLICK_MAX_PIXEL_OFFSET && abs(y) < DBCLICK_MAX_PIXEL_OFFSET)
                && (iNowTickTime - m_iLastLDTickTime < GetDoubleClickTime()))
        {
            //记录双击事件
            qDebug() << "catch a double click." << Qt::endl;

            //添加到任务列表控件
            int iTaskInterval = (iNowTickTime - m_iLastTaskTickTime) / 1000; //毫秒转换为秒，取整
            ui->lstWgtTask->addItem(QString::number(iTaskInterval)
                                    + "秒后，在坐标【X=" + QString::number(pointNowPosition.x()) + "，Y="
                                    + QString::number(pointNowPosition.y()) + "】位置"
                                    + G_TASK[TASK_DOUBLE_CLICK]);

            //添加到任务列表
            CTask task;
            task.setTaskPos(pointNowPosition);
            task.setTaskType(TASK_DOUBLE_CLICK);
            task.setInterval(iTaskInterval);
            m_lstTasks.append(task);

            //更新上次任务时间，用于下个任务的间隔时间计算
            m_iLastTaskTickTime = iNowTickTime;

            //重置双击标志
            m_bFirstClick = true;
            m_pointLDLastPostion.setX(0);
            m_pointLDLastPostion.setY(0);
            m_iLastLDTickTime = 0;
            m_iLastTaskTickTime = GetTickCount();
        }
        else if (WM_LBUTTONUP == m_uLastActionType) //如果不满足双击时间间隔标准，则将上次的【松开】作为单击事件记录
        {
            //用Last信息记录单击事件
            qDebug() << "catch a click." << Qt::endl;

            //添加到任务列表
            CTask task;
            int iTaskInterval = (m_iLastTickTime - m_iLastTaskTickTime) / 1000; //毫秒转换为秒，取整
            task.setTaskPos(m_pointLastPostion);
            task.setTaskType(TASK_CLICK);
            task.setInterval(iTaskInterval);
            m_lstTasks.append(task);

            //添加到任务列表控件
            ui->lstWgtTask->addItem(QString::number(iTaskInterval)
                                    + "秒后，在坐标【X=" + QString::number(m_pointLastPostion.x()) + "，Y="
                                    + QString::number(m_pointLastPostion.y()) + "】位置"
                                    + G_TASK[TASK_CLICK]);

            //更新上次任务时间，用于下个任务的间隔时间计算
            m_iLastTaskTickTime = m_iLastTickTime;
        }

        //用当前信息作为新的“上次左键按下”信息。
        m_pointLDLastPostion.setX(pMouseHook->pt.x);
        m_pointLDLastPostion.setY(pMouseHook->pt.y);
        m_iLastLDTickTime = iNowTickTime;
    }

    //用当前信息作为新的“上次点击”信息。
    saveLastMouseInfo(pMouseHook->pt.x, pMouseHook->pt.y, wParam);
}

void MainWindow::processLeftButtonUp(WPARAM wParam, LPARAM lParam)
{
    //MOUSEHOOKSTRUCT* pMouseHook = (MOUSEHOOKSTRUCT*)lParam;
    MOUSEHOOKSTRUCT* pMouseHook = reinterpret_cast<MOUSEHOOKSTRUCT*>(lParam);
    DWORD iNowTickTime = GetTickCount();
    QPoint pointNowPosition(pMouseHook->pt.x, pMouseHook->pt.y);

    if (WM_LBUTTONDOWN == m_uLastActionType && m_pointLastPostion != pointNowPosition) //拖动事件
    {
        //记录拖动事件
        qDebug() << "catch a drag." << Qt::endl;

        //拖动记录为2个分解动作：按下和松开
        //1.记录按下
        CTask task;
        int iTaskInterval = (m_iLastTickTime - m_iLastTaskTickTime) / 1000; //毫秒转换为秒，取整
        task.setTaskPos(m_pointLastPostion);
        task.setTaskType(TASK_LEFT_DOWN);
        task.setInterval(iTaskInterval);
        m_lstTasks.append(task);

        ui->lstWgtTask->addItem(QString::number(iTaskInterval)
                                + "秒后，在坐标【X=" + QString::number(m_pointLastPostion.x())
                                + "，Y=" + QString::number(m_pointLastPostion.y()) + "】位置"
                                + G_TASK[TASK_LEFT_DOWN]);

        //2.记录松开
        iTaskInterval = (iNowTickTime - m_iLastTickTime) / 1000; //毫秒转换为秒，取整
        task.setTaskPos(pointNowPosition);
        task.setTaskType(TASK_LEFT_UP);
        task.setInterval(iTaskInterval);
        m_lstTasks.append(task);
        ui->lstWgtTask->addItem(QString::number(iTaskInterval)
                                + "秒后，在坐标【X=" + QString::number(pointNowPosition.x()) + "，Y="
                                + QString::number(pointNowPosition.y()) + "】位置"
                                + G_TASK[TASK_LEFT_UP]);

        //更新上次任务时间，用于下个任务的间隔时间计算
        m_iLastTaskTickTime = iNowTickTime;

        //拖动之后，重置双击的判断
        m_bFirstClick = true;
    }

    //用当前信息作为新的“上次点击”信息。
    saveLastMouseInfo(pMouseHook->pt.x, pMouseHook->pt.y, wParam);

}

void MainWindow::initRecordInfo()
{
    m_pointLastPostion.setX(0);
    m_pointLastPostion.setY(0);
    m_iLastTickTime = GetTickCount();
    m_uLastActionType = 0;
    m_iLastTaskTickTime = m_iLastTickTime;
    m_bFirstClick = true;
}

void MainWindow::saveLastMouseInfo(int posX, int posY, WPARAM actionType)
{
    m_pointLastPostion.setX(posX);
    m_pointLastPostion.setY(posY);
    m_iLastTickTime = GetTickCount();
    m_uLastActionType = actionType;
}

void MainWindow::showSuspendFrame(bool bShow)
{
    if (bShow)
    {
        if (nullptr == m_pFrame)
        {
            m_pFrame = new CSuspendFrame(this);
        }
        setWindowState(Qt::WindowMinimized);
        m_pFrame->show();
        m_pFrame->moveFromCenterToRightBottom();
    }
    else
    {
        m_pFrame->hide();
        setWindowState(Qt::WindowActive);
    }
}



