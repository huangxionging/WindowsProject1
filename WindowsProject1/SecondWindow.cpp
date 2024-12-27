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
        // �����˵�ѡ��:
        switch (wmId)
        {
        case IDM_ABOUT:
            //DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        case ID_OPENFILE: {
            // ��ʾ�ļ�ѡ��Ի���
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
                // �û�ѡ�����ļ��������ļ�·��
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
        // TODO: �ڴ˴����ʹ�� hdc ���κλ�ͼ����...
		Rectangle(hdc, 20, 20, 200, 200);
        // ����һ����Բ
		Ellipse(hdc, 20, 20, 200, 200);
		// ����������������λ�ã���Ҫ��WM_MOUSEMOVE��Ϣ�д���
		MoveToEx(hdc, 0, 0, NULL);
		LineTo(hdc, 200, 200);
        

        // �����ڴ��豸������
        HDC hdcMem = CreateCompatibleDC(hdc);
        HBITMAP hbmMem = CreateCompatibleBitmap(hdc, ps.rcPaint.right - ps.rcPaint.left, ps.rcPaint.bottom - ps.rcPaint.top);
        HGDIOBJ hOld = SelectObject(hdcMem, hbmMem);

        // ��䱳��
        FillRect(hdcMem, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));

        // ����֮ǰ����������
        for (size_t i = 1; i < points.size(); ++i) {
            MoveToEx(hdcMem, points[i - 1].x, points[i - 1].y, NULL);
            LineTo(hdcMem, points[i].x, points[i].y);
        }

        // ���ڴ��豸�����ĵ����ݸ��Ƶ���Ļ
        BitBlt(hdc, 0, 0, ps.rcPaint.right - ps.rcPaint.left, ps.rcPaint.bottom - ps.rcPaint.top, hdcMem, 0, 0, SRCCOPY);

        // ����
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
		// ����������ʱ���������������
        // ��¼�����������λ��
        lastClickPos.x = LOWORD(lParam);
        lastClickPos.y = HIWORD(lParam);
        points.push_back(lastClickPos);
		break;
	}
    case WM_MOUSEMOVE: {
		//// ����ƶ�ʱ�������������յ�
		//HDC hdc = GetDC(hWnd);
		//MoveToEx(hdc, 0, 0, NULL);
		//LineTo(hdc, LOWORD(lParam), HIWORD(lParam));
		//ReleaseDC(hWnd, hdc);
		// ����·����������꣬��Ҫ��ʾ����
        POINT currentPos = { LOWORD(lParam), HIWORD(lParam) };
        if (wParam & MK_LBUTTON) {
            points.push_back(currentPos); // ��¼�յ�
            InvalidateRect(hWnd, NULL, FALSE); // �����ػ�
        }
        else {
            InvalidateRect(hWnd, NULL, FALSE); // �����ػ�
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
