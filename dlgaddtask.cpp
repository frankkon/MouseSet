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

#include <QRegExpValidator>


#include "dlgaddtask.h"
#include "ui_dlgaddtask.h"
#include "ctask.h"
#include "typedef.h"

//在添加鼠标操作任务对话框与主窗体之间进行信息传递
extern CTask G_MouseTask;


dlgAddTask::dlgAddTask(QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::dlgAddTask)
    , m_pTask(nullptr)
    , m_ptPosition(0, 0)
{
    //界面初始化
    ui->setupUi(this);
    setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint);
    setWindowIcon(QIcon(":/images/appicon48.png"));
    setWindowTitle(APP_NAME);
    ui->btnBox->button(QDialogButtonBox::Ok)->setText("添加任务");
    ui->btnBox->button(QDialogButtonBox::Cancel)->setText("取消");

    //设置时间间隔输入框只能为大于0数字
    QRegExp regx("^[1-9][0-9]*$");
    QValidator* validator = new QRegExpValidator(regx, ui->lineEdit);
    ui->lineEdit->setValidator(validator);

    //设置说明信息最多可以输入20个字；
    ui->lEditComments->setMaxLength(MAX_COMMENTS_LENGTH);

    connect(ui->btnPositionGet, SIGNAL(clicked()), this, SLOT(onPositionGet()));
    connect(ui->btnBox, SIGNAL(accepted()), this, SLOT(onAccepted()));
    connect(ui->cmbBoxTaskType, SIGNAL(currentIndexChanged(int)), this, SLOT(onTaskTypeChanged(int)));

}

dlgAddTask::dlgAddTask(CTask* pTask, QWidget* parent)
    : dlgAddTask(parent)
{
    ui->btnBox->button(QDialogButtonBox::Ok)->setText("保存任务");

    m_pTask = pTask;
    ui->lineEdit->setText(QString::number(pTask->getInterval()));
    ui->labelPos->setText("秒后，点击坐标：X=" + QString::number(pTask->getTaskPos().x())
                          + "，Y=" + QString::number(pTask->getTaskPos().y()));
    ui->cmbBoxTaskType->setCurrentIndex(pTask->getTaskType());
    ui->lEditComments->setText(pTask->getTaskComments());
}


dlgAddTask::~dlgAddTask()
{
    SAFE_DELETE(m_pTask);
    SAFE_DELETE(ui);
}

CTask* dlgAddTask::getTaskInfo(void)
{
    return m_pTask;
}

void dlgAddTask::onPositionGet()
{
    //第一台显示器屏幕
    QScreen* screen0 = QApplication::screens().at(0);

    //当前鼠标坐标
    m_ptPosition = QCursor::pos(screen0);
    ui->labelPos->setText("秒后，点击坐标：X=" + QString::number(m_ptPosition.x())
                          + "，Y=" + QString::number(m_ptPosition.y()));
}


void dlgAddTask::onAccepted()
{
    //保存任务信息
    if (nullptr == m_pTask)
    {
        m_pTask = new CTask(this);
    }
    m_pTask->setTaskPos(m_ptPosition);
    m_pTask->setInterval(ui->lineEdit->text().toInt());
    m_pTask->setTaskType(ui->cmbBoxTaskType->currentIndex());
    m_pTask->setTaskComments(ui->lEditComments->text());
}

void dlgAddTask::onTaskTypeChanged(int idx)
{
    if (TASK_KEY_INPUT == idx)
    {
        ui->labelTaskComments->setText("输入内容：");
    }
    else
    {
        ui->labelTaskComments->setText("任务说明：");
    }
}

