/*----------------------------------------------------------------
//Copyright (C) 2022 frankkon All rights reserved.
//This program comes with ABSOLUTELY NO WARRANTY.
//This is free software, and you are welcome to redistribute it
//under AGPL V3 conditions.

// 鼠标模拟点击工具 版权所有
// 文件名：csuspendframe.h
// 文件功能描述： 鼠标录制时的浮窗
// 作者：孔俊
// 时间：2022-10-07
// 修改描述：
//  1、2022-10-07 初稿完成
//----------------------------------------------------------------*/

#ifndef CSUSPENDFRAME_H
#define CSUSPENDFRAME_H

#include <QWidget>

namespace Ui
{
class CSuspendFrame;
}

class MainWindow;

class CSuspendFrame : public QWidget
{
    Q_OBJECT

public:
    explicit CSuspendFrame(QWidget* parent = nullptr);
    explicit CSuspendFrame(MainWindow* pMainWindow, QWidget* parent = nullptr);
    ~CSuspendFrame();

    void moveFromCenterToRightBottom();
protected:
    void mouseMoveEvent(QMouseEvent*);
    void mousePressEvent(QMouseEvent*);
    void paintEvent(QPaintEvent* event);

private slots:
    void onMove();

private:
    Ui::CSuspendFrame* ui;

    MainWindow* m_pMainWindow;
    QTimer* m_pTimerMove;
    qint32 m_iStepX; //X方向的移动步长
    qint32 m_iStepY; //Y方向的移动步长

    QPoint dragPosition;
};

#endif // CSUSPENDFRAME_H
