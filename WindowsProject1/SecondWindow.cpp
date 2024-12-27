#include "SecondWindow.h"
#include <vector>
using namespace std;
static POINT lastClickPos = { 0, 0 };
static vector<POINT> points;
LRESULT CALLBACK WndSecondProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // 分析菜单选择:
        switch (wmId)
        {
        case IDM_ABOUT:
            //DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        case ID_OPENFILE: {
            // 显示文件选择对话框
            OPENFILENAME ofn = {};
            TCHAR szFile[MAX_PATH] = {};

            ofn.lStructSize = sizeof(OPENFILENAME);
            ofn.hwndOwner = hWnd;
            ofn.lpstrFile = szFile;
            ofn.nMaxFile = MAX_PATH;
            ofn.lpstrFilter = TEXT("All Files\0*.*\0Text Files\0*.txt\0");
            ofn.nFilterIndex = 1;
            ofn.lpstrFileTitle = NULL;
            ofn.nMaxFileTitle = 0;
            ofn.lpstrInitialDir = NULL;
            ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

            if (GetOpenFileName(&ofn))
            {
                // 用户选择了文件，处理文件路径
                MessageBox(hWnd, szFile, TEXT("Selected File"), MB_OK);
            }
            break;
        }
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        // TODO: 在此处添加使用 hdc 的任何绘图代码...
		Rectangle(hdc, 20, 20, 200, 200);
        // 绘制一个椭圆
		Ellipse(hdc, 20, 20, 200, 200);
		// 让线条跟踪鼠标光标的位置，需要在WM_MOUSEMOVE消息中处理
		MoveToEx(hdc, 0, 0, NULL);
		LineTo(hdc, 200, 200);
        

        // 创建内存设备上下文
        HDC hdcMem = CreateCompatibleDC(hdc);
        HBITMAP hbmMem = CreateCompatibleBitmap(hdc, ps.rcPaint.right - ps.rcPaint.left, ps.rcPaint.bottom - ps.rcPaint.top);
        HGDIOBJ hOld = SelectObject(hdcMem, hbmMem);

        // 填充背景
        FillRect(hdcMem, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));

        // 绘制之前的所有线条
        for (size_t i = 1; i < points.size(); ++i) {
            MoveToEx(hdcMem, points[i - 1].x, points[i - 1].y, NULL);
            LineTo(hdcMem, points[i].x, points[i].y);
        }

        // 将内存设备上下文的内容复制到屏幕
        BitBlt(hdc, 0, 0, ps.rcPaint.right - ps.rcPaint.left, ps.rcPaint.bottom - ps.rcPaint.top, hdcMem, 0, 0, SRCCOPY);

        // 清理
        SelectObject(hdcMem, hOld);
        DeleteObject(hbmMem);
        DeleteDC(hdcMem);

        EndPaint(hWnd, &ps);
    }
    break;
    case WM_DESTROY:
		DestroyWindow(hWnd);
        break;
	case WM_LBUTTONDOWN: {
		// 鼠标左键按下时，更新线条的起点
        // 记录鼠标左键点击的位置
        lastClickPos.x = LOWORD(lParam);
        lastClickPos.y = HIWORD(lParam);
        points.push_back(lastClickPos);
		break;
	}
    case WM_MOUSEMOVE: {
		//// 鼠标移动时，更新线条的终点
		//HDC hdc = GetDC(hWnd);
		//MoveToEx(hdc, 0, 0, NULL);
		//LineTo(hdc, LOWORD(lParam), HIWORD(lParam));
		//ReleaseDC(hWnd, hdc);
		// 线条路径跟踪鼠标光标，还要显示线条
        POINT currentPos = { LOWORD(lParam), HIWORD(lParam) };
        if (wParam & MK_LBUTTON) {
            points.push_back(currentPos); // 记录终点
            InvalidateRect(hWnd, NULL, FALSE); // 触发重绘
        }
        else {
            InvalidateRect(hWnd, NULL, FALSE); // 触发重绘
            HDC hdc = GetDC(hWnd);
            POINT lastPoint = points.back();
            LineTo(hdc, LOWORD(lParam), HIWORD(lParam));
            LineTo(hdc, currentPos.x, currentPos.y);
        }
        
		break;
    }
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}


ATOM MySecondRegisterClass(HINSTANCE hInstance) {
    WNDCLASSEXW wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndSecondProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINDOWSPROJECT1));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = nullptr;
    wcex.lpszClassName = L"NewWindowClass";
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
    return RegisterClassExW(&wcex);
}
