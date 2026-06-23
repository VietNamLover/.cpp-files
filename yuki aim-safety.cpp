// yuki aim-safety.exe by VietNamLover (name by yuki aim [Clutterxmodz]).
// This is a safety version of yuki aim.exe, so you can run it on your real PC.

#include <Windows.h>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#pragma comment(lib, "winmm.lib")
#pragma comment(lib,"Msimg32.lib")

using namespace std;

struct VERTEX {
    float x, y, z;
};

struct EDGE {
    int vtx0, vtx1;
};

namespace _3D {
    void RotateX(VERTEX* v, float angle) {
        float tempY = v->y * cos(angle) - v->z * sin(angle);
        float tempZ = v->y * sin(angle) + v->z * cos(angle);
        v->y = tempY;
        v->z = tempZ;
    }

    void RotateY(VERTEX* v, float angle) {
        float tempX = v->x * cos(angle) + v->z * sin(angle);
        float tempZ = -v->x * sin(angle) + v->z * cos(angle);
        v->x = tempX;
        v->z = tempZ;
    }

    void RotateZ(VERTEX* v, float angle) {
        float tempX = v->x * cos(angle) - v->y * sin(angle);
        float tempY = v->x * sin(angle) + v->y * cos(angle);
        v->x = tempX;
        v->y = tempY;
    }

    void DrawEdge(HDC hdc, int x1, int y1, int x2, int y2, int size) {
        HPEN pen = CreatePen(PS_SOLID, 1, RGB(rand() % 255, rand() % 255, rand() % 255));
        HPEN oldPen = (HPEN)SelectObject(hdc, pen);
        MoveToEx(hdc, x1, y1, NULL);
        LineTo(hdc, x2, y2);
        SelectObject(hdc, oldPen);
        DeleteObject(pen);
    }
}

typedef union _RGBQUAD {
    COLORREF rgb;
    struct {
        BYTE b;
        BYTE g;
        BYTE r;
        BYTE Reserved;
    };
} _RGBQUAD, * PRGBQUAD;

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
        RGBQUAD rgb = {0};

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
    else if (green != length) {
        green++;
        return RGB(length, green, 0);
    }
    else if (blue != length) {
        blue++;
        return RGB(0, length, blue);
    }
    else {
        red = 0; green = 0; blue = 0;
        ifblue = true;
        return RGB(0, 0, 0);
    }
}

DWORD WINAPI shader1(LPVOID lpvd)
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
        double angle = 0.0f;
        for (int x = 0; x < w; x++)
        {
            for (int y = 0; y < h; y++)
            {
                int index = y * w + x;

                FLOAT fx = ((double)(tan(x / 500.f - y / h * 0.1) + i / 5))*5;
                FLOAT fx2 = ((double)(tan(y / 500.f - x / w * 0.1) + i / 5))*5;
                FLOAT fx3 = ((double)(tan(x / 500.f - y / h * 0.1) + i / 5))*5;
                FLOAT fx4 = (fx + fx2 + fx3) * (fx + fx2 + fx3) * 5;

                rgbquadCopy = rgbquad[index];

                hslcolor = Colors::rgb2hsl(rgbquadCopy);
                hslcolor.h = fmod(fx4 / 300.f + y / h * .1f, 1.f);

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

DWORD WINAPI shader2(LPVOID lpvd)
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
        double angle = 0.0f;
        for (int x = 0; x < w; x++)
        {
            for (int y = 0; y < h; y++)
            {
                int index = y * w + x;

                int cx = abs(x - (w / 2));
                int cy = abs(y - (h / 2));
                int zx = cos(angle) * cx - sin(angle) * cy;
                int zy = sin(angle) * cx + cos(angle) * cy;
                int fx = (zx + i) ^ (zy + i);

                rgbquadCopy = rgbquad[index];

                hslcolor = Colors::rgb2hsl(rgbquadCopy);
                hslcolor.h = fmod(fx / 300.f + y / h * .1f, 1.f);

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

DWORD WINAPI Tesseract(LPVOID lpvd)
{
    HDC dc = GetDC(NULL);
    int w = GetSystemMetrics(0);
    int h = GetSystemMetrics(1);

    float size = (float)(w + h) / 10;

    float cx = size;
    float cy = size;

    int xdv = 10;
    int ydv = 10;

    float angleX = 0.01;
    float angleY = 0.01;
    float angleZ = 0.01;

    float d = 60.0f;

    VERTEX vtx[] =
    {
        {size, 0, 0},
        {size, size, 0},
        {0, size, 0},
        {0, 0, 0},
        {size, 0, size},
        {size, size, size},
        {0, size, size},
        {0, 0, size},
        {size - d,  d,      d},
        {size - d,  size - d,  d},
        {d,      size - d,  d},
        {d,      d,      d},
        {size - d,  d,      size - d},
        {size - d,  size - d,  size - d},
        {d,      size - d,  size - d},
        {d,      d,      size - d}
    };

    EDGE edges[] =
    {
        {0, 1},
        {1, 2},
        {2, 3},
        {3, 0},
        {0, 4},
        {1, 5},
        {2, 6},
        {3, 7},
        {4, 5},
        {5, 6},
        {6, 7},
        {7, 4},
        {8,   9},
        {9,  10},
        {10, 11},
        {11,  8},
        {8,  12},
        {9,  13},
        {10, 14},
        {11, 15},
        {12, 13},
        {13, 14},
        {14, 15},
        {15, 12},
        {0,  8},
        {1,  9},
        {2, 10},
        {3, 11},
        {4, 12},
        {5, 13},
        {6, 14},
        {7, 15}
    };

    int index = rand() % 4;
    int totvtx = sizeof(vtx) / sizeof(vtx[0]);
    int totedg = sizeof(edges) / sizeof(edges[0]);

    while (true)
    {
        dc = GetDC(NULL);

        for (int i = 0; i < totvtx; i++)
        {
            _3D::RotateX(&vtx[i], angleX);
            _3D::RotateY(&vtx[i], angleY);
            _3D::RotateZ(&vtx[i], angleZ);
        }

        for (int i = 0; i < totedg; i++)
        {
            _3D::DrawEdge(dc,
                (int)(vtx[edges[i].vtx0].x + cx), (int)(vtx[edges[i].vtx0].y + cy),
                (int)(vtx[edges[i].vtx1].x + cx), (int)(vtx[edges[i].vtx1].y + cy), 20);
        }

        Sleep(40);

        cx += xdv;
        cy += ydv;

        if (cx > w - (size / 2) || cx < -(size / 2))
        {
            xdv *= -1;
            index = rand() % 4;
        }

        if (cy > h - (size / 2) || cy < -(size / 2))
        {
            ydv *= -1;
            index = rand() % 4;
        }
        ReleaseDC(0, dc);
    }

    return 0x00;
}

DWORD WINAPI blur1(LPVOID lpParam) {
    HDC hdc;
    int sw = GetSystemMetrics(0), sh = GetSystemMetrics(1);
    while (1) {
        hdc = GetDC(0); 
        HDC hdcMem = CreateCompatibleDC(hdc);
        HBITMAP screenshot = CreateCompatibleBitmap(hdc, sw, sh);
        SelectObject(hdcMem, screenshot);
        BitBlt(hdcMem, 0, 0, sw, sh, hdc, 0, 0, SRCCOPY);
        for (int i = 0; i < 30; i++) {
            StretchBlt(hdcMem, rand() % 10, rand() % 10, sw - 20, sh - 20, hdcMem, 0, 0, sw, sh, SRCCOPY);
        }
        BLENDFUNCTION blend = { AC_SRC_OVER, 0, 50, 0 };
        AlphaBlend(hdc, 0, 0, sw, sh, hdcMem, 0, 0, sw, sh, blend);
        ReleaseDC(0, hdc);
        DeleteObject(screenshot); 
        DeleteDC(hdcMem); 
        DeleteDC(hdc);
        Sleep(10);
    }
}

DWORD WINAPI shader3(LPVOID lpParam) {
    HDC hdcScreen = GetDC(0);
    HDC hdcMem = CreateCompatibleDC(hdcScreen);
    INT w = GetSystemMetrics(0), h = GetSystemMetrics(1);
    BITMAPINFO bmi = { 0 };
    PRGBQUAD rgbScreen = { 0 };
    bmi.bmiHeader.biSize = sizeof(BITMAPINFO);
    bmi.bmiHeader.biBitCount = 32;
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biWidth = w;
    bmi.bmiHeader.biHeight = h;

    HBITMAP hbmTemp = CreateDIBSection(hdcScreen, &bmi, DIB_RGB_COLORS, (void**)&rgbScreen, 0, 0);
    SelectObject(hdcMem, hbmTemp);
    
    for (;;) {
        hdcScreen = GetDC(0);
        BitBlt(hdcMem, 0, 0, w, h, hdcScreen, 0, 0, SRCCOPY);
        for (INT i = 0; i < w * h; i++) {
            INT x = i % w, y = i / w;

            int cx = x - (w / 2);
            int cy = y - (h / 2);

            int zx = (cx * cx);
            int zy = (cy * cy);

            int di = 128 + i;

            int fx = di + (di * (int)(sin(sqrt((float)(zx + zy)) / 32.0)));
            rgbScreen[i].r += 15;
            rgbScreen[i].g = fx;
            rgbScreen[i].b += 45;
        }
        BitBlt(hdcScreen, 0, 0, w, h, hdcMem, 0, 0, SRCCOPY);
        ReleaseDC(NULL, hdcScreen); 
        DeleteDC(hdcScreen);
    }
}

DWORD WINAPI glitc(LPVOID lpParam) {
    while (1) {
        HDC hdc = GetDC(0);
        int w = GetSystemMetrics(0);
        int h = GetSystemMetrics(1);
        BitBlt(hdc, rand() % 666, rand() % 666, w, h, hdc, rand() % 666, rand() % 666, NOTSRCCOPY);
        Sleep(100);
        ReleaseDC(0, hdc);
    }
}

DWORD WINAPI sq(LPVOID lpParam) {
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
        int top_x = 0 + x;
        int top_y = 0 + y;
        int bottom_x = 100 + x;
        int bottom_y = 100 + y;
        HBRUSH brush = CreateSolidBrush(Hue(239));
        SelectObject(hdc, brush);
        Rectangle(hdc, top_x, top_y, bottom_x, bottom_y);
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
        Sleep(10);
        DeleteObject(brush);
        ReleaseDC(0, hdc);
    }
}

DWORD WINAPI shake(LPVOID lpvd)
{
    while (1) {
        HDC hdc = GetDC(0);
        int w = GetSystemMetrics(0);
        int h = GetSystemMetrics(1);
        BitBlt(hdc, rand() % 2, rand() % 2, w, h, hdc, rand() % 2, rand() % 2, SRCCOPY);
        Sleep(1);
        ReleaseDC(0, hdc);
    }
}

DWORD WINAPI darker(LPVOID lpvd)
{
    HDC hdc;
    int w = GetSystemMetrics(0);
    int h = GetSystemMetrics(1);
    while (1) {
        hdc = GetDC(0);
        BitBlt(hdc, rand() % 2, rand() % 2, w, h, hdc, rand() % 2, rand() % 2, SRCAND);
        Sleep(100);
        ReleaseDC(0, hdc);
        if ((rand() % 100 + 1) % 67 == 0) InvalidateRect(0, 0, 0);
    }
}

DWORD WINAPI textz1(LPVOID lpvd)
{
    int x = GetSystemMetrics(0); 
    int y = GetSystemMetrics(1);
    LPCSTR text = 0;
    LPCSTR text1 = 0;
    LPCSTR text2 = 0;
    LPCSTR text3 = 0;
    LPCSTR text4 = 0;

    while (1)
    {
        HDC hdc = GetDC(0);
        text = "yuki aim.exe";
        text1 = "VietNamLover";
        text2 = "name by yuki aim (Clutterxmodz)";
        text3 = "R.I.P Windows!";
        text4 = "You need to reinstall Windows soon!";
        SetBkColor(hdc, RGB(rand() % 255, rand() % 255, rand() % 255));
        SetTextColor(hdc, RGB(rand() % 255, rand() % 255, rand() % 255));
        TextOutA(hdc, rand() % x, rand() % y, text, strlen(text));
        TextOutA(hdc, rand() % x, rand() % y, text1, strlen(text1));
        TextOutA(hdc, rand() % x, rand() % y, text2, strlen(text2));
        TextOutA(hdc, rand() % x, rand() % y, text3, strlen(text3));
        TextOutA(hdc, rand() % x, rand() % y, text4, strlen(text4));
        ReleaseDC(0, hdc);
        Sleep(10);
    }
}

DWORD WINAPI plgblur(LPVOID lpParam)
{
    HDC hdc = GetDC(0);
    HDC hdcCopy = CreateCompatibleDC(hdc);
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

    bmp = CreateDIBSection(hdc, &bmpi, DIB_RGB_COLORS, NULL, 0, 0);
    SelectObject(hdcCopy, bmp);

    blur.BlendOp = AC_SRC_OVER;
    blur.BlendFlags = 0;
    blur.AlphaFormat = 0;
    blur.SourceConstantAlpha = 10;
    RECT wRect;
    POINT wPt[3];
    while (1)
    {
        hdc = GetDC(0);
        GetWindowRect(GetDesktopWindow(), &wRect);
        wPt[0].x = wRect.left + 100;
        wPt[0].y = wRect.top - 100;
        wPt[1].x = wRect.right + 100;
        wPt[1].y = wRect.top + 0;
        wPt[2].x = wRect.left - 0;
        wPt[2].y = wRect.bottom - 0;
        PlgBlt(hdcCopy, wPt, hdc, wRect.left, wRect.top, wRect.right - wRect.left, wRect.bottom - wRect.top, 0, 0, 0);
        AlphaBlend(hdc, 0, 0, w, h, hdcCopy, 0, 0, w, h, blur);
        ReleaseDC(0, hdc);
    }
}

VOID WINAPI sound1() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[8000 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>((t >> 3 * t >> 16) | (t >> 3 * t * (t >> 3 * t >> 4 >> 5) & t >> 24) | (t >> 3 * (t >> 2)));

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI sound2() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[8000 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>(t + t >> 7 >> t | t * (20 & t | t >> 3 & t >> 1 | t >> 2 >> 3 >> 5));

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI sound3() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[8000 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>(t & 4096 ? t / 2 * (t ^ t % 255) | t >> 3 : t / 8 | (t & 8192 ? 4 * t : t));

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI sound4() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[8000 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>(t >> 1) * (t >> 9 | t >> 3) | (t >> 12 & t >> 9) * (t >> 1);

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
        buffer[t] = static_cast<char>(((t * (((114 >> (t >> 25) % 4 & 19) + 19)) / 8 | t / 10) & t / 1 - (t >> 14) % 5 + 14) - t / 191 + 98);

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI sound6() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[8000 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>(((t * (((114 >> (t >> 25) % 4 & 19) + 19)) / 8 | t / 10) & t / 1 - (t >> 14) % 5 + 14) - t / 191 + 98);

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
        buffer[t] = static_cast<char>(3 * (t >> 8 | t - t | t));

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

DWORD WINAPI msg(LPVOID lpParam)
{
    while (1)
    {
        MessageBoxW(NULL,
            L"R.I.P WINDOWS!",
            L"yuki aim.exe",
            MB_OK | MB_ICONWARNING);

        Sleep(1000);
    }

    return 0;
}

DWORD WINAPI theerror(LPVOID lpParam) {
    MessageBoxA(NULL, "Goodbye bro!", "yuki aim.exe", MB_ICONINFORMATION);
    return 0;
}

int CALLBACK WinMain(
    HINSTANCE hInstance, HINSTANCE hPrevInstance,
    LPSTR     lpCmdLine, int       nCmdShow
)
{
    srand((unsigned)time(NULL));
    
    if (MessageBoxW(NULL, L"This safety version of yuki aim.exe won't delete windows, run?", L"yuki aim-safety.exe by VietNamLover (name by yuki aim [Clutterxmodz])", MB_YESNO | MB_ICONEXCLAMATION) == IDNO)
    {
        ExitProcess(0);
    }
    else
    {
        if (MessageBoxW(NULL, L"Warning: NOT FOR EPILEPSY!", L"yuki aim.exe - Last Warning", MB_YESNO | MB_ICONEXCLAMATION) == IDNO)
        {
            ExitProcess(0);
        }
        else
        {
		    HANDLE msgthread = CreateThread(0, 0, msg, 0, 0, 0);
            Sleep(10);
            sound1();
            HANDLE thread1 = CreateThread(0, 0, shader1, 0, 0, 0);
            Sleep(30000);
            TerminateThread(thread1, 0);
            CloseHandle(thread1);
            InvalidateRect(0, 0, 0);
            Sleep(100);
            
            sound2();
            HANDLE thread2 = CreateThread(0, 0, shader2, 0, 0, 0);
            HANDLE thread2dot1 = CreateThread(0, 0, Tesseract, 0, 0, 0);
            Sleep(30000);
            TerminateThread(thread2, 0);
            CloseHandle(thread2);
            InvalidateRect(0, 0, 0);
            
            sound3();
            HANDLE thread3 = CreateThread(0, 0, blur1, 0, 0, 0);
            Sleep(30000);
            TerminateThread(thread3, 0);
            CloseHandle(thread3);
            InvalidateRect(0, 0, 0);
            
            sound4();
            HANDLE thread4 = CreateThread(0, 0, shader3, 0, 0, 0);
            Sleep(30000);
            TerminateThread(thread4, 0);
            CloseHandle(thread4);
            InvalidateRect(0, 0, 0);
            
            sound5();
            HANDLE thread5 = CreateThread(0, 0, glitc, 0, 0, 0);
            HANDLE thread5dot1 = CreateThread(0, 0, sq, 0, 0, 0);
            Sleep(30000);
            TerminateThread(thread5, 0);
            CloseHandle(thread5);
            TerminateThread(thread5dot1, 0);
            CloseHandle(thread5dot1);
            InvalidateRect(0, 0, 0);
            Sleep(10);
            
            sound6();
            HANDLE thread6 = CreateThread(0, 0, shake, 0, 0, 0);
            HANDLE thread6dot1 = CreateThread(0, 0, darker, 0, 0, 0);
            HANDLE thread6dot2 = CreateThread(0, 0, textz1, 0, 0, 0);
            Sleep(30000);
            TerminateThread(thread6, 0);
            CloseHandle(thread6);
            TerminateThread(thread6dot1, 0);
            CloseHandle(thread6dot1);
            InvalidateRect(0, 0, 0);
            
            sound7();
            HANDLE thread7 = CreateThread(0, 0, plgblur, 0, 0, 0);
            Sleep(30000);
            TerminateThread(thread2dot1, 0);
            CloseHandle(thread2dot1);
            TerminateThread(thread6dot2, 0);
            CloseHandle(thread6dot2);
            TerminateThread(thread7, 0);
            CloseHandle(thread7);
            InvalidateRect(0, 0, 0);
            Sleep(1000);
            
            CreateThread(0, 0, theerror, 0, 0, 0);
            Sleep(1500);
        }
    }
    return 0;
}