// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "win32_window.h"
#include <Dwmapi.h>

#include "resource.h"
#include "shellscalingapi.h"

#define CUSTOM_CLOSE_BUTTON

namespace
{

// the Windows DPI system is based on this
// constant for machines running at 100% scaling.
constexpr int kBaseDpi = 96;

constexpr LPCWSTR kClassName = L"CLASSNAME";

// Scale helper to convert logical scaler values to physical using passed in
// scale factor
int Scale(int source, double scale_factor)
{
  return static_cast<int>(source * scale_factor);
}

} // namespace

Win32Window::Win32Window() {}

Win32Window::~Win32Window() { Destroy(); }

bool Win32Window::CreateAndShow(const std::wstring &title, const Point &origin,
                                const Size &size)
{
  Destroy();

  WNDCLASS window_class = RegisterWindowClass();

  HMONITOR defaut_monitor =
      MonitorFromWindow(nullptr, MONITOR_DEFAULTTOPRIMARY);
  UINT dpi_x = 0, dpi_y = 0;
  DWORD dwStyle;
  GetDpiForMonitor(defaut_monitor, MDT_EFFECTIVE_DPI, &dpi_x, &dpi_y);

  double scale_factor = static_cast<double>(dpi_x) / kBaseDpi;

#ifndef CUSTOM_CLOSE_BUTTON
  dwStyle = WS_OVERLAPPEDWINDOW | WS_VISIBLE;
#else
  dwStyle = (WS_POPUP |
             WS_THICKFRAME |
             WS_VISIBLE) &
            ~WS_CAPTION;
#endif
  HWND window = CreateWindow(
      window_class.lpszClassName, NULL, //title.c_str(),
      dwStyle,
      Scale(origin.x, scale_factor),
      Scale(origin.y, scale_factor), Scale(size.width, scale_factor),
      Scale(size.height, scale_factor), nullptr, nullptr,
      window_class.hInstance, this);
  return window != nullptr;
}

WNDCLASS Win32Window::RegisterWindowClass()
{
  WNDCLASS window_class{};
  window_class.hCursor = LoadCursor(nullptr, IDC_ARROW);
  window_class.lpszClassName = kClassName;
  window_class.style = CS_HREDRAW | CS_VREDRAW;
  window_class.cbClsExtra = 0;
  window_class.cbWndExtra = 0;
  window_class.hInstance = GetModuleHandle(nullptr);
  window_class.hIcon =
      LoadIcon(window_class.hInstance, MAKEINTRESOURCE(IDI_APP_ICON));
  window_class.hbrBackground = 0;
  window_class.lpszMenuName = nullptr;
  window_class.lpfnWndProc = WndProc;
  RegisterClass(&window_class);
  return window_class;
}

LRESULT CALLBACK Win32Window::WndProc(HWND const window, UINT const message,
                                      WPARAM const wparam,
                                      LPARAM const lparam) noexcept
{
  if (message == WM_NCCREATE)
  {
    auto cs = reinterpret_cast<CREATESTRUCT *>(lparam);
    SetWindowLongPtr(window, GWLP_USERDATA,
                     reinterpret_cast<LONG_PTR>(cs->lpCreateParams));

    auto that = static_cast<Win32Window *>(cs->lpCreateParams);

    that->window_handle_ = window;
  }
  else if (Win32Window *that = GetThisFromHandle(window))
  {
    return that->MessageHandler(window, message, wparam, lparam);
  }

  return DefWindowProc(window, message, wparam, lparam);
}

LRESULT
Win32Window::MessageHandler(HWND hwnd, UINT const message, WPARAM const wparam,
                            LPARAM const lparam) noexcept
{
  auto window =
      reinterpret_cast<Win32Window *>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
  static RECT border_thickness;

  if (window == nullptr)
  {
    return 0;
  }

  switch (message)
  {
#ifdef CUSTOM_CLOSE_BUTTON
  case WM_CREATE:
  {
    //find border thickness
    SetRectEmpty(&border_thickness);
    if (GetWindowLongPtr(hwnd, GWL_STYLE) & WS_THICKFRAME)
    {
      AdjustWindowRectEx(&border_thickness, GetWindowLongPtr(hwnd, GWL_STYLE) & ~WS_CAPTION, FALSE, NULL);
      border_thickness.left *= -1;
      border_thickness.top *= -1;
    }
    else if (GetWindowLongPtr(hwnd, GWL_STYLE) & WS_BORDER)
    {
      SetRect(&border_thickness, 1, 1, 1, 1);
    }

    MARGINS margins = {0};
    DwmExtendFrameIntoClientArea(hwnd, &margins);
    SetWindowPos(hwnd, NULL, 0, 0, 0, 0, SWP_SHOWWINDOW | SWP_NOMOVE | SWP_NOSIZE | SWP_FRAMECHANGED);
    break;
  }
#endif
  case WM_CLOSE:
    printf("WM_CLOSE\n");
    break;
  case WM_DESTROY:
    window_handle_ = nullptr;
    Destroy();
    return 0;

  case WM_SIZE:
    RECT rect;
    GetClientRect(hwnd, &rect);
    if (child_content_ != nullptr)
    {
      // Size and position the child window.
      MoveWindow(child_content_, rect.left, rect.top, rect.right - rect.left,
                 rect.bottom - rect.top, TRUE);
    }
    return 0;

  case WM_ACTIVATE:
    if (child_content_ != nullptr)
    {
      SetFocus(child_content_);
    }
    return 0;
#ifdef CUSTOM_CLOSE_BUTTON
  case WM_NCACTIVATE:
    return 0;
  case WM_NCCALCSIZE:
    if (lparam)
    {
      NCCALCSIZE_PARAMS *sz = (NCCALCSIZE_PARAMS *)lparam;
      // sz->rgrc[0].top = 1;
      sz->rgrc[0].left += border_thickness.left;
      sz->rgrc[0].right -= border_thickness.right;
      sz->rgrc[0].bottom -= border_thickness.bottom;
      return 0;
    }
  case WM_NCHITTEST:
  {
    //do default processing, but allow resizing from top-border
    LRESULT result = DefWindowProc(hwnd, message, wparam, lparam);
    if (result == HTCLIENT)
    {
      POINT pt = {GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam)};
      ScreenToClient(hwnd, &pt);
      if (pt.y < border_thickness.top)
        return HTTOP;
    }
    return result;
  }
#endif
  // Messages that are directly forwarded to embedding.
  case WM_FONTCHANGE:
    SendMessage(child_content_, WM_FONTCHANGE, NULL, NULL);
    return 0;
  }

  return DefWindowProc(window_handle_, message, wparam, lparam);
}

void Win32Window::Destroy()
{
  if (window_handle_)
  {
    DestroyWindow(window_handle_);
    window_handle_ = nullptr;
  }

  UnregisterClass(kClassName, nullptr);
}

Win32Window *Win32Window::GetThisFromHandle(HWND const window) noexcept
{
  return reinterpret_cast<Win32Window *>(
      GetWindowLongPtr(window, GWLP_USERDATA));
}

void Win32Window::SetChildContent(HWND content)
{
  child_content_ = content;
  auto res = SetParent(content, window_handle_);
  RECT frame;
  GetClientRect(window_handle_, &frame);

  MoveWindow(content, frame.left, frame.top, frame.right - frame.left,
             frame.bottom - frame.top, true);

  SetFocus(child_content_);
}

HWND Win32Window::GetHandle() { return window_handle_; }
