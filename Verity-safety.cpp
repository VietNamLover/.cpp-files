#include <windows.h>
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>
#define _USE_MATH_DEFINES 1
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

typedef struct {
    double h;
    double s;
    double l;
} HSL;

typedef struct {
    BYTE rgbBlue;
    BYTE rgbGreen;
    BYTE rgbRed;
    BYTE rgbReserved;
} MY_RGBQUAD;

namespace Colors {
    double hueToRGB(double p, double q, double t) {
        if (t < 0) t += 1;
        if (t > 1) t -= 1;
        if (t < 1/6.0) return p + (q - p) * 6 * t;
        if (t < 1/2.0) return q;
        if (t < 2/3.0) return p + (q - p) * (2/3.0 - t) * 6;
        return p;
    }
    
    HSL rgb2hsl(RGBQUAD color) {
        HSL hsl;
        double r = color.rgbRed / 255.0;
        double g = color.rgbGreen / 255.0;
        double b = color.rgbBlue / 255.0;
        
        double max = fmax(fmax(r, g), b);
        double min = fmin(fmin(r, g), b);
        double delta = max - min;
        
        hsl.l = (max + min) / 2;
        
        if (delta == 0) {
            hsl.h = 0;
            hsl.s = 0;
        } else {
            hsl.s = hsl.l < 0.5 ? delta / (max + min) : delta / (2 - max - min);
            
            if (max == r)
                hsl.h = (g - b) / delta + (g < b ? 6 : 0);
            else if (max == g)
                hsl.h = (b - r) / delta + 2;
            else if (max == b)
                hsl.h = (r - g) / delta + 4;
            
            hsl.h /= 6;
        }
        
        return hsl;
    }
    
    RGBQUAD hsl2rgb(HSL hsl) {
        RGBQUAD rgb;
        double r, g, b;
        
        if (hsl.s == 0) {
            r = g = b = hsl.l;
        } else {
            double q = hsl.l < 0.5 ? hsl.l * (1 + hsl.s) : hsl.l + hsl.s - hsl.l * hsl.s;
            double p = 2 * hsl.l - q;
            
            r = hueToRGB(p, q, hsl.h + 1/3.0);
            g = hueToRGB(p, q, hsl.h);
            b = hueToRGB(p, q, hsl.h - 1/3.0);
        }
        
        rgb.rgbRed = (BYTE)(r * 255);
        rgb.rgbGreen = (BYTE)(g * 255);
        rgb.rgbBlue = (BYTE)(b * 255);
        rgb.rgbReserved = 0;
        
        return rgb;
    }
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
        for (int i = 0; i < w * h; i++) {
            int x = i % w, y = i / h, t = y ^ y | x;
            data[i].rgb -= t;
        }
        SetBitmapBits(hbm, w * h * 4, data);
        BitBlt(desk, 0, 0, w, h, hdcdc, 0, 0, SRCCOPY);
        DeleteObject(hbm);
        DeleteObject(hdcdc);
        DeleteObject(desk);
    }
    return 0;
}

DWORD WINAPI shader2(LPVOID lpParam) {
    HDC desk = GetDC(0); HWND wnd = GetDesktopWindow();
    int sw = GetSystemMetrics(0), sh = GetSystemMetrics(1);
    BITMAPINFO bmi = { 0 };
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = sw;
    bmi.bmiHeader.biHeight = sh;
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 24;
    bmi.bmiHeader.biCompression = BI_RGB;
    RGBTRIPLE* rgbtriple;
    for (;;) {
        desk = GetDC(0);
        HDC deskMem = CreateCompatibleDC(desk);
        HBITMAP scr = CreateDIBSection(desk, &bmi, 0, (void**)&rgbtriple, 0, 0);
        SelectObject(deskMem, scr);
        BitBlt(deskMem, 0, 0, sw, sh, desk, 0, 0, SRCCOPY);
        for (int i = 0; i < sw * sh; i++) {
            int x = i % sw, y = i / sh;
            rgbtriple[i].rgbtRed += x & y;
            rgbtriple[i].rgbtGreen += x & y;
            rgbtriple[i].rgbtBlue += x & y;
        }
        BitBlt(desk, 0, 0, sw, sh, deskMem, 0, 0, SRCCOPY);
        ReleaseDC(wnd, desk);
        DeleteDC(desk); DeleteDC(deskMem); DeleteObject(scr); DeleteObject(wnd); 
    }
}

DWORD WINAPI shader3(LPVOID lpParam) {
    HDC hdcScreen = GetDC(0), hdcMem = CreateCompatibleDC(hdcScreen);
    INT w = GetSystemMetrics(0), h = GetSystemMetrics(1);
    BITMAPINFO bmi = { 0 };
    PRGBQUAD rgbScreen = { 0 };
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biBitCount = 32;
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biWidth = w;
    bmi.bmiHeader.biHeight = h;
    bmi.bmiHeader.biCompression = BI_RGB;
    HBITMAP hbmTemp = CreateDIBSection(hdcScreen, &bmi, 0, (void**)&rgbScreen, 0, 0);
    SelectObject(hdcMem, hbmTemp);
    for (;;) {
        hdcScreen = GetDC(0);
        BitBlt(hdcMem, 0, 0, w, h, hdcScreen, 0, 0, SRCCOPY);
        for (INT i = 0; i < w * h; i++) {
            rgbScreen[i].rgb = (rgbScreen[i].rgb * 2) % (RGB(255, 255, 255));
        }
        BitBlt(hdcScreen, 0, 0, w, h, hdcMem, -30, 0, SRCCOPY);
        BitBlt(hdcScreen, 0, 0, w, h, hdcMem, w - 30, 0, SRCCOPY);
        BitBlt(hdcScreen, 0, 0, w, h, hdcMem, 0, -30, SRCCOPY);
        BitBlt(hdcScreen, 0, 0, w, h, hdcMem, 0, h - 30, SRCCOPY);
        ReleaseDC(NULL, hdcScreen); DeleteDC(hdcScreen);
    }
}

DWORD WINAPI shader4(LPVOID lpParam) {
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
            if (i % h && rand() % 110)
                v = rand() % 100;
            *((BYTE*)data + 4 * i + v) -= 5;
        }
        SetBitmapBits(hbm, w * h * 4, data);
        BitBlt(desk, 0, 0, w, h, hdcdc, 0, 0, SRCCOPY);
        DeleteObject(hbm);
        DeleteObject(hdcdc);
        DeleteObject(desk);
    }
    return 0;
}

DWORD WINAPI thing5(LPVOID lpParam) {
    while (1) {
        HDC hdc = GetDC(0);
        HDC hdcMem = CreateCompatibleDC(hdc);
        int sw = GetSystemMetrics(0);
        int sh = GetSystemMetrics(1);
        HBITMAP bm = CreateCompatibleBitmap(hdc, sw, sh);
        SelectObject(hdcMem, bm);
        RECT rect;
        GetWindowRect(GetDesktopWindow(), &rect);
        POINT pt[3];
        int inc3 = rand() % 700;
        int v = rand() % 2;
        if (v == 1) inc3 = -700;
        inc3++;
        pt[0].x = rect.left - inc3;
        pt[0].y = rect.top + inc3;
        pt[1].x = rect.right - inc3;
        pt[1].y = rect.top - inc3;
        pt[2].x = rect.left + inc3;
        pt[2].y = rect.bottom - inc3;
        PlgBlt(hdcMem, pt, hdc, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, 0, 0, 0);
        SelectObject(hdc, CreateSolidBrush(RGB(rand() % 255, rand() % 255, rand() % 255)));
        BitBlt(hdc, rand() % 20, rand() % 20, sw, sh, hdcMem, rand() % 20, rand() % 20, 0x123456);
        DeleteObject(hdcMem); DeleteObject(bm);
        Sleep(1);
    }
}

DWORD WINAPI cubes(LPVOID lpParam) {
    while (1) {
        HDC hdc = GetDC(0);
        int x = GetSystemMetrics(0);
        int y = GetSystemMetrics(1);
        StretchBlt(hdc, -10, -10, x + 20, y + 20, hdc, 0, 0, x, y, SRCCOPY);
        StretchBlt(hdc, 10, 10, x - 20, y - 20, hdc, 0, 0, x, y, SRCCOPY);
        ReleaseDC(0, hdc);
    }
}

DWORD WINAPI Verity(LPVOID lpvd) {
    HDC hdc = GetDC(NULL);
    HDC hdcCopy = CreateCompatibleDC(hdc);
    int w = GetSystemMetrics(0);
    int h = GetSystemMetrics(1);
    BITMAPINFO bmpi = { 0 };
    HBITMAP bmp;

    bmpi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
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

                FLOAT fx = (float)(x ^ (x + y)) + (i * 4);

                rgbquadCopy = rgbquad[index];

                hslcolor = Colors::rgb2hsl(rgbquadCopy);
                hslcolor.h = fmod(fx / 300.f + y / (float)h * .1f, 1.f);

                rgbquad[index] = Colors::hsl2rgb(hslcolor);
            }
        }

        i++;
        StretchBlt(hdc, 0, 0, w, h, hdcCopy, 0, 0, w, h, SRCCOPY);
        ReleaseDC(NULL, hdc); DeleteDC(hdc);
    }

    return 0x00;
}

DWORD WINAPI textouts(LPVOID lpvd)
{
	int x = GetSystemMetrics(0); int y = GetSystemMetrics(1);
	LPCSTR text = 0;
	LPCSTR text1 = 0;
	LPCSTR text2 = 0;
	LPCSTR text3 = 0;
	while (1)
	{
		HDC hdc = GetDC(0);
		SetBkMode(hdc, 0);
		text = "R.I.P YOUR COMPUTER!";
		text1 = "SYSTEM DESTROYED!";
		text2 = "THIS IS THE END OF YOUR PC!";
		text3 = "YOUR DATA WILL BE LOST!";
		SetTextColor(hdc, RGB(rand() % 255, rand() % 255, rand() % 255));
		HFONT font = CreateFontA(43, 32, 0, 0, FW_EXTRALIGHT, 0, 1, 0, ANSI_CHARSET, 0, 0, 0, 0, "Engravers MT");
		SelectObject(hdc, font);
		TextOutA(hdc, rand() % x, rand() % y, text, strlen(text));
		Sleep(10);
		TextOutA(hdc, rand() % x, rand() % y, text1, strlen(text1));
		Sleep(10);
		TextOutA(hdc, rand() % x, rand() % y, text2, strlen(text2));
		Sleep(10);
		TextOutA(hdc, rand() % x, rand() % y, text3, strlen(text3));
		DeleteObject(font);
		ReleaseDC(0, hdc);
	}
}

DWORD WINAPI textouts1(LPVOID lpvd)
{
    int x = GetSystemMetrics(0); int y = GetSystemMetrics(1);
    LPCSTR text = 0;
    LPCSTR text1 = 0;
    LPCSTR text2 = 0;
    LPCSTR text3 = 0;
    while(1)
    {
        HDC hdc = GetDC(0);
        SetBkMode(hdc, 0);
        text = "Verity.exe";
        text1 = "R.I.P TO THE PERSON IN FRONT OF THE SCREEN!";
        text2 = "WHAT WERE YOU THINK?!"; // credits to N17Pro3426 for this text
        text3 = "ARE YOU STUPID?!";
        SetTextColor(hdc, RGB(rand() % 255, rand() % 255, rand() % 255));
        HFONT font = CreateFontA(43, 32, 0, 0, FW_THIN, 0, 1, 0, ANSI_CHARSET, 0, 0, 0, 0, "Baby Kruffy");
        SelectObject(hdc, font);
        TextOutA(hdc, rand() % x, rand() % y, text, strlen(text));
        Sleep(10);
        TextOutA(hdc, rand() % x, rand() % y, text1, strlen(text1));
        Sleep(10);
        TextOutA(hdc, rand() % x, rand() % y, text2, strlen(text2));
        Sleep(10);
        TextOutA(hdc, rand() % x, rand() % y, text3, strlen(text3));
        DeleteObject(font);
        ReleaseDC(0, hdc);
    }
}

DWORD WINAPI shader5(LPVOID lpParam) {
    int time = GetTickCount();
    int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
    RGBQUAD* data = (RGBQUAD*)VirtualAlloc(0, (w * h + w) * sizeof(RGBQUAD), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    for (int i = 0;; i++, i %= 3) {
        HDC desk = GetDC(NULL);
        HDC hdcdc = CreateCompatibleDC(desk);
        HBITMAP hbm = CreateBitmap(w, h, 1, 32, data);
        SelectObject(hdcdc, hbm);
        BitBlt(hdcdc, 0, 0, w, h, desk, 0, 0, SRCCOPY);
        GetBitmapBits(hbm, 4 * h * w, data);
        int v = 0;
        BYTE byte = 0;
        if ((GetTickCount() - time) > 10)
            byte = rand() % 0xff;
        for (int i = 0; w * h > i; ++i) {
            if (!(i % h) && !(rand() % 110))
                v = rand() % 24;
            *((BYTE*)data + 4 * i + v) -= 5;
        }
        SetBitmapBits(hbm, w * h * 4, data);
        BitBlt(desk, 0, 0, w, h, hdcdc, 0, 0, SRCCOPY);
        DeleteObject(hbm);
        DeleteObject(hdcdc);
        DeleteObject(desk);
    }
    return 0;
}

DWORD WINAPI Verityy(LPVOID lpParam) {
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

DWORD WINAPI pacmans(LPVOID lpParam) {
    while (1) {
        HDC hdc = GetDC(0);
        int x = GetSystemMetrics(0);
        int y = GetSystemMetrics(1);
        HBRUSH brush = CreateSolidBrush(RGB(rand() % 255, rand() % 255, rand() % 255));
        SelectObject(hdc, brush);
        Pie(hdc, rand() % x, rand() % y, rand() % x, rand() % y, rand() % x, rand() % y, rand() % x, rand() % y);
        DeleteObject(brush);
        ReleaseDC(NULL, hdc);
        Sleep(10);
    }
}

DWORD WINAPI vERitY3(LPVOID lpvd) {
    HDC hdc = GetDC(NULL);
    HDC hdcCopy = CreateCompatibleDC(hdc);
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);
    BITMAPINFO bmpi = { 0 };
    HBITMAP bmp;

    bmpi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
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

    while (1) {
        hdc = GetDC(NULL);
        StretchBlt(hdcCopy, 0, 0, screenWidth, screenHeight, hdc, 0, 0, screenWidth, screenHeight, SRCCOPY);

        for (int x = 0; x < screenWidth; x++) {
            for (int y = 0; y < screenHeight; y++) {
                int index = y * screenWidth + x;

                int fx = (int)((i ^ 4) + (i * 4) * cbrt((x * x + i) + (y * y + i)));

                hslcolor = Colors::rgb2hsl(rgbquad[index]);
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

DWORD WINAPI dwnm(LPVOID lpParam) {
	int w = GetSystemMetrics(0);
	int h = GetSystemMetrics(1);
	while (1) {
		HDC hdc = GetDC(0);
		BitBlt(hdc, 0, 0, w, h, hdc, -30, 0, SRCCOPY);
		BitBlt(hdc, 0, 0, w, h, hdc, 0, -30, SRCCOPY);
		HBRUSH brush2 = CreateSolidBrush(RGB(rand() % 255, 0, 0));
		SelectObject(hdc, brush2);
		BitBlt(hdc, 0, 0, w, h, hdc, 0, 0, PATINVERT);
		DeleteObject(brush2);
		BitBlt(hdc, rand() % 5, rand() % 5, rand() % w, rand() % h, hdc, rand() % 5, rand() % 5, SRCCOPY);
		ReleaseDC(0, hdc);
		if ((rand() % 100 + 1) % 67 == 0) InvalidateRect(0, 0, 0);
	}
}

DWORD WINAPI HatchBrush(LPVOID lpParam) {
    HDC hdc = 0;
    int sx = 0, sy = 0;
    srand((unsigned)time(NULL));

    while (1) {
        hdc = GetWindowDC(GetDesktopWindow());
        sx = GetSystemMetrics(SM_CXSCREEN);
        sy = GetSystemMetrics(SM_CYSCREEN);
        HBRUSH brush = CreateHatchBrush(rand() % 7, RGB(rand() % 225, rand() % 225, rand() % 225));
        SelectObject(hdc, brush);
        PatBlt(hdc, 0, 0, sx, sy, PATINVERT);
        DeleteObject(brush);
        ReleaseDC(0, hdc);
        Sleep(100);
    }
    return 0;
}

DWORD WINAPI invcc(LPVOID lpParam) {
    int sw = GetSystemMetrics(0), sh = GetSystemMetrics(1);
    srand((unsigned)time(NULL));
    while (1) {
        HDC hdc = GetDC(0);
        int rand_num_x = rand() % sw;
        int rand_num_y = rand() % sh;

        int top_x = 0 + rand_num_x;
        int top_y = 0 + rand_num_y;

        int bottom_x = rand() % 200 + rand_num_x;
        int bottom_y = rand() % 200 + rand_num_y;
        HRGN circle = CreateEllipticRgn(top_x, top_y, bottom_x, bottom_y);
        InvertRgn(hdc, circle);
        DeleteObject(circle);
        ReleaseDC(0, hdc);
        Sleep(rand() % 25);
    }
    return 0;
}

DWORD WINAPI jhjhh(LPVOID lpParam) {
	HDC desktop = GetDC(NULL);
	int xs = GetSystemMetrics(SM_CXSCREEN);
	int ys = GetSystemMetrics(SM_CYSCREEN);
	while (true) {
		desktop = GetDC(NULL);
		BitBlt(desktop, 1,1, xs, ys, desktop, -2,2, SRCERASE);
		ReleaseDC(0, desktop);
		Sleep(10);
	}
}

DWORD WINAPI dfgvfh(LPVOID lpvd) {
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

DWORD WINAPI textouts2(LPVOID lpvd) {
    int x = GetSystemMetrics(0), y = GetSystemMetrics(1);
    const char* messages[] = {
        "Verity.exe",
        "VERITY!",
        "VERITY.EXE HAS BEEN DESTROYED YOUR DUMB PC!",
        "vERitY?!",
        "R.I.P THIS WINDOWS USER!",
        "THE CREATOR WON'T BE RESPONSIBLE FOR ANY OF THE DAMAGE CAUSED!",
    };
    
    while (1) {
        HDC hdc = GetDC(0);
        SetBkMode(hdc, 0);
        for (int i = 0; i < 6; i++) {
            SetTextColor(hdc, RGB(rand() % 255, rand() % 255, rand() % 255));
            TextOutA(hdc, rand() % x, rand() % y, messages[i], strlen(messages[i]));
            Sleep(10);
        }
        ReleaseDC(0, hdc);
    }
}

DWORD WINAPI dsfv(LPVOID lpvd) {
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

DWORD WINAPI thing(LPVOID lpParam) {
    POINT wPt[3];
    RECT wRect;
    srand((unsigned)time(NULL));
    while (1) {
        HDC hdc = GetDC(0);
        HDC hdcMem = CreateCompatibleDC(hdc);
        int sw = GetSystemMetrics(0);
        int sh = GetSystemMetrics(1);
        HBITMAP bm = CreateCompatibleBitmap(hdc, sw, sh);
        SelectObject(hdcMem, bm);
        GetWindowRect(GetDesktopWindow(), &wRect);
        int c = 10;

        wPt[0].x = wRect.left + rand() % 11 - 5;
        wPt[0].y = wRect.top + rand() % 21 - 10;

        wPt[1].x = wRect.right + rand() % 21 - 10;
        wPt[1].y = wRect.top + rand() % 41 - 20;

        wPt[2].x = wRect.left + c - rand() % 21 - c;
        wPt[2].y = wRect.bottom - c + rand() % 21 - c;

        PlgBlt(hdcMem, wPt, hdc, wRect.left, wRect.top, wRect.right - wRect.left, wRect.bottom - wRect.top, 0, 0, 0);
        HBRUSH brush = CreateHatchBrush(rand() % 7, RGB(rand() % 255, rand() % 255, rand() % 255));
        SelectObject(hdc, brush);
        BitBlt(hdc, rand() % 20, rand() % 20, sw, sh, hdcMem, rand() % 20, rand() % 20, 0x123456);
        DeleteObject(brush);
        DeleteObject(hdcMem);
        DeleteObject(bm);
        ReleaseDC(0, hdc);
    }
    return 0;
}

VOID WINAPI sound1() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 11025, 11025, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[11025 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>(t & (t >> 8) - t & t >> 8);

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
        buffer[t] = static_cast<char>(t&4096?t/2*(t^t%255)|t>>5:t/8|(t&8192?4*t:t));

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
		buffer[t] = static_cast<char>((((t/10|0)^(t/10|0)-1280)%11*t/2&127)+(((t/640|0)^(t/640|0)-2)%13*t/2&127));

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
        buffer[t] = static_cast<char>(t * t / (1 + (t >> 9 & t >> 8)) & 129);

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
        buffer[t] = static_cast<char>((t*(3+(4+(t>>12)%2)*(t>>10)%7)&128)*(.15+(t>>9)%2)/2);

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
        buffer[t] = static_cast<char>(t * (0xFFF998 >> (t >> 9 & 14) & 15) | t >> 4);

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
        buffer[t] = static_cast<char>((0<t&65535>t?t%32>t/1E4?t>>4:t>>6:65535<0+t&131070>t?t%32>t%43?t>>4:t>>6:131070<0+t&196605>t?t%36>t%43?t>>4:t>>6:196605<0+t&262140>t?t%64>t%43?t>>4:t>>6:262140<0+t&327675>t?5<t%43?t<<3:t>>6:327675<0+t&393210>t?5<t%27?t<<3:t>>6:393210<0+t&524280>t?5<t%63?t<<3:t>>6:0)&t>>4);

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
        buffer[t] = static_cast<char>((t >> 0) ^ (t >> 4) ^ (t >> 7));

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
        buffer[t] = static_cast<char>(64 * ((t >> 8 ^ (t >> 9) - 1 ^ 1) % 13 * t / 5) + 128);

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
		buffer[t] = static_cast<char>(461 * t * (t >> 8 | 4) >> (t >> 19));

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
        buffer[t] = static_cast<char>(t ^ (t >> 8) & t);

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
        buffer[t] = static_cast<char>((t >> 7 | t << 2) + (t >> 3 | t << 3 | t >> 3) | 
                                      t >> 2 | t << 1);
    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI sound13() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[0300 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>(t * (0x21CA52CA >> (t >> 9 & 30) & 12) + 
                                      t * (0xCACACACA >> (t >> 9 & 30) & 14) & t >> 4);
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
        buffer[t] = static_cast<char>(t * ((t & 4096 ? t % 65536 < 59392 ? 7 : t & 7 : 16) + 
                                           (1 & t >> 14)) >> (3 & t >> (t & 2048 ? 2 : 10)) | 
                                      t >> (t & 16384 ? t & 4096 ? 10 : 2 : 2));
    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI sound15() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 32000, 32000, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[32000 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>((t * ((log(700)))));
        
    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

int CALLBACK WinMain(
    HINSTANCE hInstance, HINSTANCE hPrevInstance,
    LPSTR     lpCmdLine, int       nCmdShow
) {
    srand((unsigned int)time(NULL));
    
    if (MessageBoxW(NULL, L"This is a safety version of Verity.exe, run?", L"Verity-safety.exe by VietNamLover", MB_YESNO | MB_ICONEXCLAMATION) == IDNO) {
        ExitProcess(0);
    }
    else {
        if (MessageBoxW(NULL, L"It contains flashing lights and it not for epilepsy! If you really want to run this, press the yes button!", L"Verity-safety.exe - LAST WARNING!", MB_YESNO | MB_ICONEXCLAMATION) == IDNO) {
            ExitProcess(0);
        }
        else {
            Sleep(5000);
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

            HANDLE thread3 = CreateThread(0, 0, shader3, 0, 0, 0);
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

            HANDLE thread5 = CreateThread(0, 0, thing5, 0, 0, 0);
            HANDLE thread5dot1 = CreateThread(0, 0, cubes, 0, 0, 0);
            HANDLE thread5dot2 = CreateThread(0, 0, pacmans, 0, 0, 0);
            sound5();
            Sleep(30000);
            TerminateThread(thread5, 0);
            CloseHandle(thread5);
            TerminateThread(thread5dot1, 0);
            CloseHandle(thread5dot1);
            TerminateThread(thread5dot2, 0);
            CloseHandle(thread5dot2);
            InvalidateRect(0, 0, 0);

            HANDLE thread6 = CreateThread(0, 0, Verity, 0, 0, 0);
            HANDLE thread6dot1 = CreateThread(0, 0, textouts, 0, 0, 0);
            sound6();
            Sleep(30000);
            TerminateThread(thread6, 0);
            CloseHandle(thread6);
            TerminateThread(thread6dot1, 0);
            CloseHandle(thread6dot1);
            InvalidateRect(0, 0, 0);

            HANDLE thread7 = CreateThread(0, 0, shader5, 0, 0, 0);
            HANDLE thread7dot1 = CreateThread(0, 0, cubes, 0, 0, 0);
            HANDLE thread7dot2 = CreateThread(0, 0, textouts1, 0, 0, 0);
            sound7();
            Sleep(30000);
            TerminateThread(thread7, 0);
            CloseHandle(thread7);
            TerminateThread(thread7dot1, 0);
            CloseHandle(thread7dot1);
            TerminateThread(thread7dot2, 0);
            CloseHandle(thread7dot2);
            InvalidateRect(0, 0, 0);

            HANDLE thread8 = CreateThread(0, 0, Verityy, 0, 0, 0);
            sound8();
            Sleep(30000);
            TerminateThread(thread8, 0);
            CloseHandle(thread8);
            InvalidateRect(0, 0, 0);

            HANDLE thread9 = CreateThread(0, 0, vERitY3, 0, 0, 0);
            sound9();
            Sleep(30000);
            TerminateThread(thread9, 0);
            CloseHandle(thread9);
            InvalidateRect(0, 0, 0);

			HANDLE thread10 = CreateThread(0, 0, dwnm, 0, 0, 0);
			sound10();
			Sleep(30000);
			TerminateThread(thread10, 0);
			CloseHandle(thread10);
			InvalidateRect(0, 0, 0);

            HANDLE thread11 = CreateThread(0, 0, HatchBrush, 0, 0, 0);
            HANDLE thread11dot1 = CreateThread(0, 0, invcc, 0, 0, 0);
            sound11();
            Sleep(30000);
            TerminateThread(thread11, 0);
            CloseHandle(thread11);
            TerminateThread(thread11dot1, 0);
            CloseHandle(thread11dot1);
            InvalidateRect(0, 0, 0);
            
            HANDLE thread12 = CreateThread(0, 0, jhjhh, 0, 0, 0);
            sound12(); 
			Sleep(30000);
            TerminateThread(thread12, 0); 
			CloseHandle(thread12);
            InvalidateRect(0, 0, 0);
            
            HANDLE thread13 = CreateThread(0, 0, dfgvfh, 0, 0, 0);
            sound13(); 
			Sleep(30000);
            TerminateThread(thread13, 0); 
			CloseHandle(thread13);
            InvalidateRect(0, 0, 0);
            
            HANDLE thread14 = CreateThread(0, 0, dsfv, 0, 0, 0);
            HANDLE thread14dot1 = CreateThread(0, 0, textouts2, 0, 0, 0);
            sound14(); 
		    Sleep(30000);
            TerminateThread(thread14, 0); 
		    CloseHandle(thread14);
            InvalidateRect(0, 0, 0);
            
            HANDLE thread15 = CreateThread(0, 0, thing, 0, 0, 0);
            sound15();
            Sleep(30000);
            TerminateThread(thread15, 0); 
		    CloseHandle(thread15);
            TerminateThread(thread14dot1, 0); 
		    CloseHandle(thread14dot1);
            InvalidateRect(0, 0, 0);
        }
    }    
    return 0;
}