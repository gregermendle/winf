#include <windows.h>
#include <winuser.h>
#include <iostream>

std::string windowTitle(HWND hwnd)
{
  int length = GetWindowTextLength(hwnd);
  if(length == 0)
  {
    return "";
  }

  char *tempTitle = new char[length + 1];
  GetWindowText(hwnd, tempTitle, length + 1);
  std::string title(tempTitle);
  return title;
}

HWND findWindowByTitle(std::string titlePredicate)
{
  for(HWND hwnd = GetTopWindow(NULL); hwnd != NULL; hwnd = GetNextWindow(hwnd, GW_HWNDNEXT))
  {   
    std::string title = windowTitle(hwnd);
    if(
      IsWindowVisible(hwnd) && 
      title != "Program Manager" && 
      !title.empty() &&
      title.compare(titlePredicate) == 0
    )
    {
        return hwnd;
    }
  }

  return NULL;
}

bool isFullScreen(std::string titlePredicate)
{
    HWND windowHandle = findWindowByTitle(titlePredicate);
    MONITORINFO monitorInfo = { 0 };
    monitorInfo.cbSize = sizeof(MONITORINFO);
    GetMonitorInfo(MonitorFromWindow(windowHandle, MONITOR_DEFAULTTOPRIMARY), &monitorInfo);

    RECT windowRect;
    GetWindowRect(windowHandle, &windowRect);

    return windowRect.left == monitorInfo.rcMonitor.left
        && windowRect.right == monitorInfo.rcMonitor.right
        && windowRect.top == monitorInfo.rcMonitor.top
        && windowRect.bottom == monitorInfo.rcMonitor.bottom;
}
