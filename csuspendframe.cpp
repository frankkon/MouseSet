/*----------------------------------------------------------------
//Copyright (C) 2022 frankkon All rights reserved.
//This program comes with ABSOLUTELY NO WARRANTY.
//This is free software, and you are welcome to redistribute it
//under AGPL V3 conditions.

// 鼠标模拟点击工具 版权所有
// 文件名：csuspendframe.cpp
// 文件功能描述： 鼠标录制时的浮窗
// 作者：孔俊
// 时间：2022-10-07
// 修改描述：
//  1、2022-10-07 初稿完成
//----------------------------------------------------------------*/

#include <QtWidgets>
#include <QDesktopWidget>

#include "typedef.h"
#include "mainwindow.h"
#include "csuspendframe.h"
#include "ui_csuspendframe.h"

CSuspendFrame::CSuspendFrame(QWidget* parent) :
    QWidget(parent),
    ui(new Ui::CSuspendFrame),
    m_pMainWindow(nullptr),
    m_pTimerMove(new QTimer(this))
{
    ui->setupUi(this);
    ui->btnRecordStop->setText("停止录制");
    ui->btnRecordStop->setStyleSheet("color: rgb(255, 0, 0); background-color: rgb(0, 0, 0);");
    setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);

    connect(m_pTimerMove, SIGNAL(timeout()), this, SLOT(onMove()));

}

CSuspendFrame::CSuspendFrame(MainWindow* pMainWindow, QWidget* parent) :
    CSuspendFrame(parent)
{
    m_pMainWindow = pMainWindow;
    connect(ui->btnRecordStop, SIGNAL(clicked()), m_pMainWindow, SLOT(onTaskStopRecord()));
}

CSuspendFrame::~CSuspendFrame()
{
    SAFE_DELETE(m_pTimerMove);
    SAFE_DELETE(ui);
}

void CSuspendFrame::mouseMoveEvent(QMouseEvent* event)
{
    if (event->buttons() & Qt::LeftButton)
    {
        move(event->globalPos() - dragPosition);
        event->accept();
    }
}

void CSuspendFrame::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton)
    {
        dragPosition = event->globalPos() - frameGeometry().topLeft();
        event->accept();
    }
}

void CSuspendFrame::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event)
    QPainter painter(this);
    painter.drawPixmap(0, 0, QPixmap(":/images/stoprec2.png"));
}

void CSuspendFrame::moveFromCenterToRightBottom()
{

    QDesktopWidget* pDesktop = QApplication::desktop();
    move(pDesktop->width() / 2, pDesktop->height() / 2);

    //屏幕中心位置
    qint32 iSrcX = pDesktop->width() / 2;
    qint32 iSrcY = pDesktop->height() / 2;

    //屏幕右下角
    qint32 iDestX = pDesktop->width() - this->width() - PIXEL_TO_DESKTOP_RIGHT;
    qint32 iDestY = pDesktop->height() - this->height() - PIXEL_TO_DESKTOP_BOTTOM;

    //移动的步长
    m_iStepX = (iDestX - iSrcX) / (WINDOW_MOVE_DURATION_MS / WINDOW_MOVE_SPEED_MS); //500毫秒移动到位，10毫秒移动一次。
    m_iStepY = (iDestY - iSrcY) / (WINDOW_MOVE_DURATION_MS / WINDOW_MOVE_SPEED_MS); //500毫秒移动到位，10毫秒移动一次。

    qDebug() << "iSrcX=" << iSrcX << ",iSrcY=" << iSrcY
             << ",iDestX=" << iDestX << ",iDestY=" << iDestY
             << ",m_iStepX=" << m_iStepX << ",m_iStepY=" << m_iStepY << Qt::endl;

    m_pTimerMove->start(WINDOW_MOVE_SPEED_MS);
}

void CSuspendFrame::onMove()
{
    QDesktopWidget* pDesktop = QApplication::desktop();

    QPoint ptFrameCurrPos = mapToGlobal(QPoint(0, 0));
    qint32 iDestX = pDesktop->width() - this->width() - PIXEL_TO_DESKTOP_RIGHT;
    qint32 iToGoX = ptFrameCurrPos.x() + m_iStepX;
    qint32 iToGoY = ptFrameCurrPos.y() + m_iStepY;

    if (iToGoX > iDestX)
    {
        m_pTimerMove->stop();
        return;
    }
    else
    {
        move(iToGoX, iToGoY);
    }
}
