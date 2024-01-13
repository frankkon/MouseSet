/*----------------------------------------------------------------
//Copyright (C) 2022 frankkon All rights reserved.
//This program comes with ABSOLUTELY NO WARRANTY.
//This is free software, and you are welcome to redistribute it
//under AGPL V3 conditions.

// 鼠标模拟点击工具 版权所有
// 文件名：global.cpp
// 文件功能描述： 全局函数定义
// 作者：孔俊
// 时间：2022-10-06
// 修改描述：
//  1、2022-10-06 初稿完成
//----------------------------------------------------------------*/

#include "global.h"

HHOOK G_MOUSE_HOOK = NULL;
MainWindow* G_MAINWINDOW = nullptr;

HMODULE ModuleFromAddress(PVOID pv)
{
    MEMORY_BASIC_INFORMATION mbi;
    if (::VirtualQuery(pv, &mbi, sizeof(mbi)) != 0)
    {
        return (HMODULE)mbi.AllocationBase;
    }
    else
    {
        return NULL;
    }
}

LRESULT CALLBACK recordMouseTask(int nCode, WPARAM wParam, LPARAM lParam)
{
    G_MAINWINDOW->recordTask(nCode, wParam, lParam);
    return CallNextHookEx(G_MOUSE_HOOK, nCode, wParam, lParam);
}

