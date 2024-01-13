/*----------------------------------------------------------------
//Copyright (C) 2022 frankkon All rights reserved.
//This program comes with ABSOLUTELY NO WARRANTY.
//This is free software, and you are welcome to redistribute it
//under AGPL V3 conditions.

// 鼠标模拟点击工具 版权所有
// 文件名：global.h
// 文件功能描述： 全局函数定义
// 作者：孔俊
// 时间：2022-10-06
// 修改描述：
//  1、2022-10-06 初稿完成
//----------------------------------------------------------------*/

#ifndef GLOBAL_H
#define GLOBAL_H

#include <windows.h>
#include "mainwindow.h"

extern HHOOK G_MOUSE_HOOK;
extern MainWindow* G_MAINWINDOW;

HMODULE ModuleFromAddress(PVOID pv);
LRESULT CALLBACK recordMouseTask(int nCode, WPARAM wParam, LPARAM lParam);

#endif


