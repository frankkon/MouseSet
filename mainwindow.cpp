#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QEvent>
#include <QMouseEvent>
#include <QtDebug>
#include <windows.h>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_bStartFlag(false)
{
    ui->setupUi(this);

    connect(&m_timer, SIGNAL(timeout()), this, SLOT(on_timer()));
    connect(&m_timerLcd, SIGNAL(timeout()), this, SLOT(on_timerLcd()));

    //调色板
    QPalette lcdpat = ui->lcdNumber->palette();
    /*设置颜色，整体背景颜色 颜色蓝色,此函数的第一个参数可以设置多种。如文本、按钮按钮文字、多种*/
    lcdpat.setColor(QPalette::Normal,QPalette::WindowText,Qt::red);
    //设置当前窗口的调色板
    ui->lcdNumber->setSegmentStyle(QLCDNumber::Flat);
    ui->lcdNumber->setPalette(lcdpat);

}

MainWindow::~MainWindow()
{
    delete ui;
}




void MainWindow::on_pushButton_2_clicked()
{
    QScreen *screen0 = QApplication::screens().at(0);
    pos0 = QCursor::pos(screen0);
    ui->label->setText("鼠标位置：X=" + QString::number(pos0.x()) + " Y=" + QString::number(pos0.y()));
}

void MainWindow::on_pushButton_clicked()
{
    if(m_bStartFlag)
    {
        m_timer.stop();
        m_timerLcd.stop();
        ui->pushButton->setText("开始模拟");
        ui->lcdNumber->display(0);
        m_bStartFlag = false;

    }
    else
    {
        //根据选择，设置定时器触发时间：interval（分钟）
        int interval = ui->comboBox->currentText().toShort() * 1000 * 60;
        m_timer.stop();
        m_timer.start(interval);

        //初始化，每秒钟刷新一下LCD的显示内容
        int totalNum = ui->comboBox->currentText().toShort() * 60;
        ui->lcdNumber->display(totalNum);
        m_timerLcd.stop();
        m_timerLcd.start(1000);

        m_bStartFlag = true;
        ui->pushButton->setText("暂停模拟");

    }
}

void MainWindow::on_timer()
{

    SetCursorPos(pos0.x(), pos0.y());

    //暂停
    mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
    mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);

    //播放
    mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
    mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);

    //重置LCD读秒
    int totalNum = ui->comboBox->currentText().toShort() * 60;
    ui->lcdNumber->display(totalNum);
}

void MainWindow::on_timerLcd()
{
    //倒计时读秒
    int currentNum = ui->lcdNumber->intValue();
    ui->lcdNumber->display(currentNum - 1);
}
