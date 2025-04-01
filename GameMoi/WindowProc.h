#pragma once
#ifndef WINDOW_PROC_H
#define WINDOW_PROC_H

#include <windows.h>

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

#endif // WINDOW_PROC_H
