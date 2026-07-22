#include <windows.h>
#include <cmath>
#include <windowsx.h>
#include <math.h>
#include <iostream>
#include <tchar.h>
#include <time.h>
#include <vector>
#include <algorithm>

#pragma comment(lib, "msimg32.lib")
#pragma comment(lib, "winmm.lib")
#define _USE_MATH_DEFINES 1

#ifndef M_PI
#define M_PI 3.14159265358979323846264338327950288
#endif

typedef struct
{
    float x;
    float y;
    float z;
} VERTEX;

typedef struct
{
    int vtx0;
    int vtx1;
} EDGE;

typedef RGBTRIPLE* PRGBTRIPLE;

namespace _3D
{
    VOID RotateX(VERTEX* vtx, float angle)
    {
        float y = vtx->y;
        float z = vtx->z;
        vtx->y = cos(angle) * y - sin(angle) * z;
        vtx->z = sin(angle) * y + cos(angle) * z;
    }

    VOID RotateY(VERTEX* vtx, float angle)
    {
        float x = vtx->x;
        float z = vtx->z;
        vtx->x = cos(angle) * x + sin(angle) * z;
        vtx->z = -sin(angle) * x + cos(angle) * z;
    }

    VOID RotateZ(VERTEX* vtx, float angle)
    {
        float x = vtx->x;
        float y = vtx->y;
        vtx->x = cos(angle) * x - sin(angle) * y;
        vtx->y = sin(angle) * x + cos(angle) * y;
    }

    void DrawEdge(HDC dc, LPCWSTR icon, int x0, int y0, int x1, int y1, int r)
    {
        int dx = abs(x1 - x0);
        int dy = -abs(y1 - y0);

        int sx = (x0 < x1) ? 1 : -1;
        int sy = (y0 < y1) ? 1 : -1;

        int error = dx + dy;

        int i = 0;

        while (true)
        {
            if (i == 0)
            {
                DrawIcon(dc, x0, y0, LoadIconW(NULL, icon));
                i = 10;
            }
            else
            {
                i--;
            }

            if (x0 == x1 && y0 == y1)
            {
                break;
            }

            int e2 = 2 * error;

            if (e2 >= dy)
            {
                if (x0 == x1)
                {
                    break;
                }

                error += dy;
                x0 += sx;
            }

            if (e2 <= dx)
            {
                if (y0 == y1)
                {
                    break;
                }

                error += dx;
                y0 += sy;
            }
        }
    }
}

DWORD WINAPI Tesseract(LPVOID lpvd)
{
    HDC dc = GetDC(NULL);
    HDC dcCopy = CreateCompatibleDC(dc);
    int w = GetSystemMetrics(0);
    int h = GetSystemMetrics(1);

    float size = (w + h) / 10.0f;

    int cx = (int)size;
    int cy = (int)size;

    int xdv = 10;
    int ydv = 10;

    float angleX = 0.01;
    float angleY = 0.01;
    float angleZ = 0.01;

    int d = 60;

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
        {(float)(size - d),  (float)d,      (float)d},
        {(float)(size - d),  (float)(size - d),  (float)d},
        {(float)d,      (float)(size - d),  (float)d},
        {(float)d,      (float)d,      (float)d},
        {(float)(size - d),  (float)d,      (float)(size - d)},
        {(float)(size - d),  (float)(size - d),  (float)(size - d)},
        {(float)d,      (float)(size - d),  (float)(size - d)},
        {(float)d,      (float)d,      (float)(size - d)}
    };

    EDGE edges[] =
    {
        {0, 1}, {1, 2}, {2, 3}, {3, 0},
        {0, 4}, {1, 5}, {2, 6}, {3, 7},
        {4, 5}, {5, 6}, {6, 7}, {7, 4},
        {8, 9}, {9, 10}, {10, 11}, {11, 8},
        {8, 12}, {9, 13}, {10, 14}, {11, 15},
        {12, 13}, {13, 14}, {14, 15}, {15, 12},
        {0, 8}, {1, 9}, {2, 10}, {3, 11},
        {4, 12}, {5, 13}, {6, 14}, {7, 15}
    };

    LPCWSTR icons[] = { L"", L"", L"", L"" };
    icons[0] = MAKEINTRESOURCEW(32514);
    icons[1] = MAKEINTRESOURCEW(32517);
    icons[2] = MAKEINTRESOURCEW(32514);
    icons[3] = MAKEINTRESOURCEW(32516);

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
            _3D::DrawEdge(dc, icons[index],
                (int)(vtx[edges[i].vtx0].x + cx), (int)(vtx[edges[i].vtx0].y + cy),
                (int)(vtx[edges[i].vtx1].x + cx), (int)(vtx[edges[i].vtx1].y + cy), 20);
        }

        Sleep(40);

        cx += xdv;
        cy += ydv;

        if (cx > w - (int)(size / 2) || cx < -(int)(size / 2))
        {
            xdv *= -1;
            index = rand() % 4;
        }

        if (cy > h - (int)(size / 2) || cy < -(int)(size / 2))
        {
            ydv *= -1;
            index = rand() % 4;
        }
        ReleaseDC(0, dc);
    }

    return 0x00;
}

COLORREF COLORHSL(int length) {
    double h = fmod(length, 360.0);
    double s = 1.0;
    double l = 0.5;

    double c = (1.0 - fabs(2.0 * l - 1.0)) * s;
    double x = c * (1.0 - fabs(fmod(h / 60.0, 2.0) - 1.0));
    double m = l - c / 2.0;

    double r1, g1, b1;
    if (h < 60) {
        r1 = c;
        g1 = x;
        b1 = 0;
    }
    else if (h < 120) {
        r1 = x;
        g1 = c;
        b1 = 0;
    }
    else if (h < 180) {
        r1 = 0;
        g1 = c;
        b1 = x;
    }
    else if (h < 240) {
        r1 = 0;
        g1 = x;
        b1 = c;
    }
    else if (h < 300) {
        r1 = x;
        g1 = 0;
        b1 = c;
    }
    else {
        r1 = c;
        g1 = 0;
        b1 = x;
    }

    int red = static_cast<int>((r1 + m) * 255);
    int green = static_cast<int>((g1 + m) * 255);
    int blue = static_cast<int>((b1 + m) * 255);

    return RGB(red, green, blue);
}

typedef union COLOR {
    COLORREF rgb;
    struct {
        BYTE blue;
        BYTE green;
        BYTE red;
    };
} COLOR;

COLORREF HSV(float h) {
    float r, g, b;
    int i = (int)(h * 6.0f);
    float f = h * 6.0f - i;
    float p = 0.0f;
    float q = 1.0f - f;
    float tt = f;

    switch (i % 6) {
    case 0: r = 1; g = tt; b = p; break;
    case 1: r = q; g = 1; b = p; break;
    case 2: r = p; g = 1; b = tt; break;
    case 3: r = p; g = q; b = 1; break;
    case 4: r = tt; g = p; b = 1; break;
    case 5: r = 1; g = p; b = q; break;
    }

    return RGB((int)(r * 255), (int)(g * 255), (int)(b * 255));
}

static ULONGLONG n, r;
int randy() { return n = r, n ^= 0x8ebf635bee3c6d25, n ^= n << 5 | n >> 26, n *= 0xf3e05ca5c43e376b, r = n, n & 0x7fffffff; }

typedef union _RGBQUAD {
    COLORREF rgb;
    struct {
        BYTE b;
        BYTE g;
        BYTE r;
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

        FLOAT rgbMin = std::min(std::min(_r, _g), _b);
        FLOAT rgbMax = std::max(std::max(_r, _g), _b);

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

int red, green, blue;
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

VOID WINAPI sound1() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 11025, 11025, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[11025 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>(167 * (t >> 9 | t) ^ (t & t >> 9 ^ t >> 5));

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
        if (-t != 0) buffer[t] = static_cast<char>(t >> t + ((t ^ 46 & 3) + 4) | t ^ t / ((t ^ 46 & 3) + 4));

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
        if (-t != 0) buffer[t] = static_cast<char>(t >> 6 & t * 6 & t >> 7 ^ t >> 4 & t + (t >> 6 & -t) | t >> 4);

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
        if (-t != 0) buffer[t] = static_cast<char>(t * (((t >> 7) | (t >> 12)) & 15) & 126);

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI sound5() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 60000, 60000, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[60000 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        if (-t != 0) buffer[t] = static_cast<char>(100 * (4 * t >> (t >> 13) % 4 | (6 % t >> 11)) & 100 * (4 * t >> (t >> 13) % 4 + (8 & t >> 12)));

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI sound6() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 60000, 60000, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[60000 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        if (-t != 0) buffer[t] = static_cast<char>(t >> 5 & t >> 8 | (4 * t >> (t >> 13) % 4 + (6 & t >> 11)) >> (t & 8 >> 8 | t) >> 5);

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
        if (-t != 0) buffer[t] = static_cast<char>(t * (0xFFF998 >> (t >> 9 & 14) & 15) | t >> 2);

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
        if (-t != 0) buffer[t] = static_cast<char>(t * t + (1 * (t >> 9 & t >> 8)) & 128);

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI sound9() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 9000, 9000, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[9000 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        if (-t != 0) buffer[t] = static_cast<char>(((t >> 8) * t & t << 4));

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI sound10() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 22050, 22050, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[22050 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        if (-t != 0) buffer[t] = static_cast<char>(t * (t * 34 | t >> 7));

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI sound11() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 22050, 22050, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[22050 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        if (-t != 0) buffer[t] = static_cast<char>(t + t & (t >> 7) ^ t & t >> 8 | t >> 4);

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI sound12() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[8000 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        if (-t != 0) buffer[t] = static_cast<char>(t >> t >> ((t >> 11 | t % 16 * t >> 8) & 8));

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI sound13() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 11025, 11025, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[11025 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        if (-t != 0) buffer[t] = static_cast<char>(t ^ t * (2 + (t >> 8 & t >> 9)) & 128);

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI sound14() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[8000 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        if (-t != 0) buffer[t] = static_cast<char>(t * (t ? 1 : 3) * (t >> 7));

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI lastsound() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 15000, 15000, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[15000 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        if (-t != 0) buffer[t] = static_cast<char>(t * (9));

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

DWORD WINAPI shader1(LPVOID lpParam) {
    int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
    RGBQUAD* data = (RGBQUAD*)VirtualAlloc(0, (w * h + w) * sizeof(RGBQUAD), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    int rgb = 0xFF000000;
    int v = 0;
    float radius = 17.4f;
    double angle = 0;
    HDC hdc, hdcMem; HBITMAP hbm;
    for (int i = 0;; i++, i %= 6) {
        hdc = GetDC(0); hdcMem = CreateCompatibleDC(hdc); hbm = CreateBitmap(w, h, 1, 32, data);
        SelectObject(hdcMem, hbm);
        BitBlt(hdcMem, 0, 0, w, h, hdc, 0, 0, SRCERASE); GetBitmapBits(hbm, w * h * 8, data);
        for (int i = 0; w * h > i; i++) {
            int x = i % w, y = i / h;
            if (i % h == 0 && rand() % 100 == 0)  v = 1 + rand() % 6;
            rgb ^= (int)(size_t)data + (x * y);
            ((BYTE*)(data + i))[v] = rgb;
        }
        float x = cos(angle) * radius, y = sin(angle) * radius;
        SetBitmapBits(hbm, w * h * 4, data); BitBlt(hdc, 0, 0, w, h, hdcMem, (int)x, (int)y, SRCERASE);
        DeleteObject(hbm); DeleteObject(hdcMem);
        DeleteObject(hdc);
        angle = fmod(angle + M_PI / radius, M_PI * radius);
    }
}

DWORD WINAPI curicon(LPVOID lpParam) {
    while (1) {
        HDC hdc = GetDC(0);
        int x = GetSystemMetrics(SM_CXSCREEN);
        int y = GetSystemMetrics(SM_CYSCREEN);
        DrawIconEx(hdc, rand() % x, rand() % y, LoadCursor(0, IDC_WAIT), (rand() % 5 + 1) * GetSystemMetrics(SM_CXICON), (rand() % 5 + 1) * GetSystemMetrics(SM_CYICON), 0, NULL, DI_NORMAL);
        DrawIconEx(hdc, rand() % x, rand() % y, LoadCursor(0, IDC_APPSTARTING), (rand() % 5 + 1) * GetSystemMetrics(SM_CXICON), (rand() % 5 + 1) * GetSystemMetrics(SM_CYICON), 0, NULL, DI_NORMAL);
        DrawIconEx(hdc, rand() % x, rand() % y, LoadCursor(0, IDC_ICON), (rand() % 5 + 1) * GetSystemMetrics(SM_CXICON), (rand() % 5 + 1) * GetSystemMetrics(SM_CYICON), 0, NULL, DI_NORMAL);
        DrawIconEx(hdc, rand() % x, rand() % y, LoadCursor(0, IDC_ARROW), (rand() % 5 + 1) * GetSystemMetrics(SM_CXICON), (rand() % 5 + 1) * GetSystemMetrics(SM_CYICON), 0, NULL, DI_NORMAL);
        DrawIconEx(hdc, rand() % x, rand() % y, LoadCursor(0, IDC_CROSS), (rand() % 5 + 1) * GetSystemMetrics(SM_CXICON), (rand() % 5 + 1) * GetSystemMetrics(SM_CYICON), 0, NULL, DI_NORMAL);
        ReleaseDC(0, hdc);
        Sleep(1);
    }
}

DWORD WINAPI cur2(LPVOID lpParam) {
    POINT cursor;
    while (1) {
        HDC hdc = GetDC(NULL);
        HICON hIcon7 = LoadCursor(NULL, IDC_NO);
        GetCursorPos(&cursor);
        int X = cursor.x - rand() % 3 + 1;
        int Y = cursor.y - rand() % 3 + 1;
        SetCursorPos(X, Y);
        DrawIcon(hdc, cursor.x - 18, cursor.y - 18, hIcon7);
    }
    return(1);
}

DWORD WINAPI shader2(LPVOID lpvd) {
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

                int fx = (int)((i * 8) + (i ^ 8) * log((double)(x * x | x ^ i * y)));

                rgbquadCopy = rgbquad[index];

                hslcolor = Colors::rgb2hsl(rgbquadCopy);
                hslcolor.h = fmod((float)fx / 400.f + y / (float)screenHeight * .2f, 1.f);

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

DWORD WINAPI prgbtrip2(LPVOID lpParam) {
    HDC desk = GetDC(0); HWND wnd = GetDesktopWindow();
    int sw = GetSystemMetrics(0), sh = GetSystemMetrics(1);
    BITMAPINFO bmi = { 40, sw, sh, 1, 24 };
    PRGBTRIPLE rgbtriple;
    for (;;) {
        desk = GetDC(0);
        HDC deskMem = CreateCompatibleDC(desk);
        HBITMAP scr = CreateDIBSection(desk, &bmi, 0, (void**)&rgbtriple, 0, 0);
        SelectObject(deskMem, scr);
        BitBlt(deskMem, 0, 0, sw, sh, desk, 0, 0, NOTSRCCOPY);
        for (int i = 0; i < sw * sh; i++) {
            int x = i % sw, y = i / sh, t = y + y * x;
            rgbtriple[i].rgbtRed += GetRValue(x ^ y);
            rgbtriple[i].rgbtGreen += GetGValue(x ^ y);
            rgbtriple[i].rgbtBlue += GetBValue(x ^ y);
        }
        BitBlt(desk, 0, 0, sw, sh, deskMem, 0, 0, NOTSRCERASE);
        ReleaseDC(wnd, desk);
        DeleteDC(desk); DeleteDC(deskMem); DeleteObject(scr);
    }
}

DWORD WINAPI shader4(LPVOID lpvd)
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

                int fx = (int)((i ^ 8) + (i * 8) * log((double)(x ^ i + i + y & x + i * y ^ i)));

                rgbquadCopy = rgbquad[index];

                hslcolor = Colors::rgb2hsl(rgbquadCopy);
                hslcolor.h = fmod((float)fx / 400.f + y / (float)screenHeight * .2f, 1.f);

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

                int fx = (int)((i ^ 4) + (i * 4) + (i & 4) * (x * y + i * 8 ^ x + y));

                rgbquadCopy = rgbquad[index];

                hslcolor = Colors::rgb2hsl(rgbquadCopy);
                hslcolor.h = fmod((float)fx / 400.f + y / (float)screenHeight * .2f, 1.f);

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

LPCWSTR generateRandomUnicodeString(int len)
{
    wchar_t* ustr = new wchar_t[len + 1];

    for (int i = 0; i < len; i++) {
        ustr[i] = (rand() % 256) + 1024;
    }
    ustr[len] = L'\0';
    return ustr;
}

DWORD WINAPI msg(LPVOID lpParam) {
    while (1) {
        MessageBoxW(NULL, L"Logan_CZE and VietNamLover are here to destroy this PC!", L"Logan_CZE.exe", MB_ICONERROR);
    }
    return 0;
}

DWORD WINAPI shader6(LPVOID lpvd)
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
                int t = (y ^ y) * (x ^ x);

                rgbquad[index].rgbRed += x ^ y;
                rgbquad[index].rgbGreen += x & i;
                rgbquad[index].rgbBlue += t - i;
            }
        }

        i++;
        StretchBlt(hdc, 0, 0, w, h, hdcCopy, 0, 0, w, h, SRCCOPY);
        ReleaseDC(NULL, hdc); DeleteDC(hdc);
    }

    return 0x00;
}

DWORD WINAPI smthidk(LPVOID lpParam) {
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
        HBRUSH brush = CreateSolidBrush(RGB(rand() % 255, rand() % 255, rand() % 255));
        SelectObject(hdc, brush);
        DrawIconEx(hdc, top_x, top_y, LoadIcon(0, IDI_QUESTION), 3 * GetSystemMetrics(SM_CXICON), 3 * GetSystemMetrics(SM_CYICON), 0, NULL, DI_NORMAL);
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

DWORD WINAPI shader7(LPVOID lpvd)
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

                int fx = (int)((i ^ 4) + (i * 4) * sqrt((double)(x & y | i & x - (x * y))));

                rgbquadCopy = rgbquad[index];

                hslcolor = Colors::rgb2hsl(rgbquadCopy);
                hslcolor.h = fmod((float)fx / 400.f + y / (float)screenHeight * .2f, 1.f);

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

        RGBQUAD rgbquadCopy;

        for (int x = 0; x < screenWidth; x++)
        {
            for (int y = 0; y < screenHeight; y++)
            {
                int index = y * screenWidth + x;

                int fx = (int)((i ^ 4) + (i * 4) * log2((double)(x ^ y + x + i * y | x ^ y)));

                rgbquadCopy = rgbquad[index];

                hslcolor = Colors::rgb2hsl(rgbquadCopy);
                hslcolor.h = fmod((float)fx / 400.f + y / (float)screenHeight * .2f, 1.f);

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

DWORD WINAPI shader9(LPVOID lpvd)
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

                int fx = (int)((i ^ 4) + (i * 4) * log2((double)(x ^ x | x & i + y)));

                rgbquadCopy = rgbquad[index];

                hslcolor = Colors::rgb2hsl(rgbquadCopy);
                hslcolor.h = fmod((float)fx / 400.f + y / (float)screenHeight * .2f, 1.f);

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

        HBRUSH brush = CreateHatchBrush(rand() % 7, Hue(239));
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
            INT x = i % w, y = i / w;
            rgbScreen[i].rgb ^= (x + x ^ i) ^ (y * y & i) | x ^ y;
        }
        BitBlt(hdcScreen, 0, 0, w, h, hdcMem, 0, 0, SRCCOPY);
        ReleaseDC(NULL, hdcScreen); DeleteDC(hdcScreen);
    }
}

DWORD WINAPI rgbtrain(LPVOID lpParam) {
    int w = GetSystemMetrics(0);
    int h = GetSystemMetrics(1);
    while (1) {
        HDC hdc = GetDC(0);
        HBRUSH brush = CreateHatchBrush(rand() % 7, RGB(rand() % 255, rand() % 255, rand() % 255));
        SelectObject(hdc, brush);
        BitBlt(hdc, 0, 0, w, h, hdc, -30, 30, (DWORD)0x1900AC010E);
        BitBlt(hdc, 0, 0, w, h, hdc, w - 30, -h + 30, (DWORD)0x1900AC010E);
        DeleteObject(brush);
        ReleaseDC(0, hdc);
    }
}

DWORD WINAPI shk(LPVOID lpParam) {
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
                int fx = (int)((i - 4) + (i + 4) * sinf(15)) * (x & y)| (x + y);

                rgbquadCopy = rgbquad[index];

                hslcolor = Colors::rgb2hsl(rgbquadCopy);
                hslcolor.h = fmod((float)fx / 400.f + y / (float)screenHeight * .2f, 1.f);

                rgbquad[index] = Colors::hsl2rgb(hslcolor);
            }
        }

        i++;

        StretchBlt(hdc, 0, 0, screenWidth, screenHeight, hdcCopy, 0, 0, screenWidth, screenHeight, SRCCOPY);
        ReleaseDC(NULL, hdc);
        DeleteDC(hdc);
        Sleep(10);
    }

    return 0x00;
}

DWORD WINAPI invcc(LPVOID lpParam) {
    HDC hdc = GetDC(0);
    int sw = GetSystemMetrics(0), sh = GetSystemMetrics(1), size = 100, radius = 227;
    while (1) {
        hdc = GetDC(0);
        int origX1 = rand() % sw, origY1 = rand() % sh;
        int origX2 = -20 + rand() % (sw + 20), origY2 = -20 + rand() % (sh + 20);
        int origX3 = -30 + rand() % (sw + 30), origY3 = -30 + rand() % (sh + 30);
        int origX4 = -20 + rand() % (sw + 20), origY4 = -20 + rand() % (sh + 20);
        int origX5 = rand() % sw, origY5 = rand() % sh;
        for (int angle = 0; angle < 1081; angle += 12) {
            int x1 = radius * cos(angle * M_PI / 180.0) + origX1, y1 = radius * sin(angle * M_PI / 180.0) + origY1;
            int x2 = radius * cos(angle * M_PI / 180.0) + origX2, y2 = radius * sin(angle * M_PI / 180.0) + origY2;
            int x3 = radius * cos(angle * M_PI / 180.0) + origX3, y3 = radius * sin(angle * M_PI / 180.0) + origY3;
            int x4 = radius * cos(angle * M_PI / 180.0) + origX4, y4 = radius * sin(angle * M_PI / 180.0) + origY4;
            int x5 = radius * cos(angle * M_PI / 180.0) + origX5, y5 = radius * sin(angle * M_PI / 180.0) + origY5;
            if (angle < 720) size += 20; if (angle > 720) size -= 39;
            HRGN circle1 = CreateEllipticRgn(x1, y1, x1 - size, y1 - size);
            HRGN circle2 = CreateEllipticRgn(x2, y2, x2 - size, y2 - size);
            HRGN circle3 = CreateEllipticRgn(x3, y3, x3 - size, y3 - size);
            HRGN circle4 = CreateEllipticRgn(x4, y4, x4 - size, y4 - size);
            HRGN circle5 = CreateEllipticRgn(x5, y5, x5 - size, y5 - size);
            InvertRgn(hdc, circle1); InvertRgn(hdc, circle2); InvertRgn(hdc, circle3); InvertRgn(hdc, circle4); InvertRgn(hdc, circle5);
            Sleep(26);
        }
        ReleaseDC(0, hdc);
    }
}

DWORD WINAPI shader12(LPVOID lpParam) {
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
            rgbScreen[i].g += 255;
            rgbScreen[i].rgb += (x ^ y) ^ i | (x * y);
        }
        BitBlt(hdcScreen, 0, 0, w, h, hdcMem, 0, 0, SRCCOPY);
        ReleaseDC(NULL, hdcScreen); DeleteDC(hdcScreen);
    }
}

DWORD WINAPI payload13(LPVOID lpParam)
{
    HDC hdc = 0;
    int sx = 0, sy = 0;
    int rx = 0, ry = 0;

    while (1)
    {
        hdc = GetWindowDC(GetDesktopWindow());
        sx = GetSystemMetrics(SM_CXSCREEN);
        sy = GetSystemMetrics(SM_CYSCREEN);
        rx = GetSystemMetrics(SM_CXSCREEN);
        ry = GetSystemMetrics(SM_CYSCREEN);
        HBRUSH brush = CreateHatchBrush(rand() % 7, RGB(rand() % 225, rand() % 225, rand() % 225));
        SelectObject(hdc, brush);
        PatBlt(hdc, 0, 0, sx, sy, PATINVERT);
        PatBlt(hdc, 0, 0, rx, ry, PATINVERT);
        BitBlt(hdc, 0, 0, sx, sy, hdc, rx, ry, SRCCOPY);
        HBRUSH brush2 = CreateSolidBrush(RGB(rand() % 225, rand() % 225, rand() % 225));
        SelectObject(hdc, brush2);
        PatBlt(hdc, 0, 0, sx, sy, PATINVERT);
        PatBlt(hdc, 0, 0, rx, ry, PATINVERT);
        BitBlt(hdc, 0, 0, sx, sy, hdc, rx, ry, SRCCOPY);
        DeleteObject(brush);
        DeleteObject(brush2);
        ReleaseDC(0, hdc);
        Sleep(100);
    }
}

DWORD WINAPI textout(LPVOID lpvd)
{
    int x = GetSystemMetrics(0);
    int y = GetSystemMetrics(1);
    LPCSTR text = 0;
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
        text = "Logan_CZE.exe";
        text1 = "we aren't scared of you!";
        text2 = "try to do something, it will always fail miserably.";
        text3 = "you are pathetic.";
        text4 = "come on, loser.";
        text5 = "you made your choice.";
        text6 = "it's time to say goodbye";
        SetTextColor(hdc, Hue(239));
        HFONT font = CreateFontA(rand() % 100, rand() % 100, 0, 0, FW_THIN, 0, rand() % 1, 0, ANSI_CHARSET, 0, 0, 0, 0, "Cascadia Code");
        SelectObject(hdc, font);
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
        DeleteObject(font);
        ReleaseDC(0, hdc);
    }
}

DWORD WINAPI payload14(LPVOID lpParam) {
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
        BitBlt(dcCopy, 1, 1, w, h, hdc, -4, -4, SRCCOPY);
        BitBlt(dcCopy, 1, 1, w, h, hdc, 4, 4, SRCCOPY);
        AlphaBlend(hdc, 0, 0, w, h, dcCopy, 0, 0, w, h, blur);
        ReleaseDC(0, hdc);
    }
    return 0x00;
}

int main()
{
    if (MessageBoxW(NULL, L"Do you want to run this malware?!", L"Logan_CZE-safety.exe by VietNamLover", MB_YESNO | MB_ICONEXCLAMATION) == IDNO)
    {
        ExitProcess(0);
    }
    else
    {
        if (MessageBoxW(NULL, L"it will make sure that your device is crying for help and screaming of pain! the device is gonna be unbootable/unuseable! are you sure you want to run this? this is your last chance to stop.", L"Logan_CZE-safety.exe", MB_YESNO | MB_ICONEXCLAMATION) == IDNO)
        {
            ExitProcess(0);
        }
        else
        {
            Sleep(3000);
            HANDLE threadmsg = CreateThread(0, 0, msg, 0, 0, 0);
            Sleep(2000);
            HANDLE tessr = CreateThread(0, 0, Tesseract, 0, 0, 0);
            HANDLE threadsmth = CreateThread(0, 0, smthidk, 0, 0, 0);
            HANDLE thread1 = CreateThread(0, 0, shader1, 0, 0, 0);
            sound1();
            Sleep(30000);
            TerminateThread(thread1, 0);
            CloseHandle(thread1);
            InvalidateRect(0, 0, 0);
            HANDLE thread2 = CreateThread(0, 0, shader2, 0, 0, 0);
            sound2();
            Sleep(30000);
            TerminateThread(thread2, 0);
            CloseHandle(thread2);
            InvalidateRect(0, 0, 0);
            HANDLE thread3 = CreateThread(0, 0, prgbtrip2, 0, 0, 0);
            sound3();
            Sleep(30000);
            TerminateThread(thread3, 0);
            CloseHandle(thread3);
            InvalidateRect(0, 0, 0);
            HANDLE thread4 = CreateThread(0, 0, shader4, 0, 0, 0);
            sound4();
            Sleep(30000);
            TerminateThread(thread4, 0);
            CloseHandle(thread4);
            InvalidateRect(0, 0, 0);
            HANDLE thread5 = CreateThread(0, 0, shader5, 0, 0, 0);
            sound5();
            Sleep(30000);
            TerminateThread(thread5, 0);
            CloseHandle(thread5);
            InvalidateRect(0, 0, 0);
            HANDLE threadhexa = CreateThread(0, 0, hexagon, 0, 0, 0);
            HANDLE thread6 = CreateThread(0, 0, shader6, 0, 0, 0);
            sound6();
            Sleep(30000);
            TerminateThread(thread6, 0);
            CloseHandle(thread6);
            InvalidateRect(0, 0, 0);
            HANDLE thread7 = CreateThread(0, 0, shader7, 0, 0, 0);
            sound7();
            Sleep(30000);
            TerminateThread(thread7, 0);
            CloseHandle(thread7);
            InvalidateRect(0, 0, 0);
            HANDLE thread8 = CreateThread(0, 0, shader8, 0, 0, 0);
            sound8();
            Sleep(30000);
            TerminateThread(thread8, 0);
            CloseHandle(thread8);
            InvalidateRect(0, 0, 0);
            HANDLE thread9 = CreateThread(0, 0, shader9, 0, 0, 0);
            sound9();
            Sleep(30000);
            TerminateThread(thread9, 0);
            CloseHandle(thread9);
            InvalidateRect(0, 0, 0);
            HANDLE thread10 = CreateThread(0, 0, shader10, 0, 0, 0);
            sound10();
            Sleep(30000);
            TerminateThread(thread10, 0);
            CloseHandle(thread10);
            InvalidateRect(0, 0, 0);
            HANDLE thread11 = CreateThread(0, 0, shk, 0, 0, 0);
            HANDLE thread11_2 = CreateThread(0, 0, invcc, 0, 0, 0);
            sound11();
            Sleep(30000);
            TerminateThread(thread11, 0);
            CloseHandle(thread11);
            TerminateThread(thread11_2, 0);
            CloseHandle(thread11_2);
            InvalidateRect(0, 0, 0);
            HANDLE thread12 = CreateThread(0, 0, shader12, 0, 0, 0);
            sound12();
            Sleep(30000);
            TerminateThread(thread12, 0);
            CloseHandle(thread12);
            InvalidateRect(0, 0, 0);
            HANDLE thread13 = CreateThread(0, 0, payload13, 0, 0, 0);
            sound13();
            Sleep(30000);
            TerminateThread(thread13, 0);
            CloseHandle(thread13);
            InvalidateRect(0, 0, 0);
            HANDLE thread14 = CreateThread(0, 0, payload14, 0, 0, 0);
            sound14();
            Sleep(30000);
            TerminateThread(thread14, 0);
            CloseHandle(thread14);
            InvalidateRect(0, 0, 0);
            HANDLE threadlast = CreateThread(0, 0, rgbtrain, 0, 0, 0);
            lastsound();
            Sleep(30000);
            TerminateThread(threadlast, 0);
            CloseHandle(threadlast);
            InvalidateRect(0, 0, 0);
            TerminateThread(threadsmth, 0);
            CloseHandle(threadsmth);
            TerminateThread(threadmsg, 0);
            CloseHandle(threadmsg);
            TerminateThread(threadhexa, 0);
            CloseHandle(threadhexa);
            TerminateThread(tessr, 0);
            CloseHandle(tessr);
            InvalidateRect(0, 0, 0);
        }
    }
    return 0;
}