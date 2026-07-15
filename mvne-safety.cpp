#include <Windows.h>
#pragma comment(lib, "winmm.lib")
#pragma comment(lib,"Msimg32.lib")
#include <math.h>
#include <tchar.h>
#include <algorithm>
#include <cmath>

using namespace std;

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

int stage = 0;
int r = 0, g = 0, b = 0;

COLORREF Hue(int shift) {
    switch (stage) {
    case 0:
        r = 255;
        b = 0;
        g < 255 ? g += shift : stage++;
        break;
    case 1:
        g = 255;
        b = 0;
        r > 0 ? r -= shift : stage++;
        break;
    case 2:
        g = 255;
        r = 0;
        b < 255 ? b += shift : stage++;
        break;
    case 3:
        b = 255;
        r = 0;
        g > 0 ? g -= shift : stage++;
        break;
    case 4:
        b = 255;
        g = 0;
        r < 255 ? r += shift : stage++;
        break;
    case 5:
        r = 255;
        g = 0;
        b > 0 ? b -= shift : stage = 0;
        break;
    }

    return RGB(r, g, b);
}

DWORD WINAPI bez(LPVOID lpvd)
{
    int sw = GetSystemMetrics(0);
    int sh = GetSystemMetrics(1);
    while (1) {
        HDC hdc = GetDC(0);
        HPEN hPen = CreatePen(PS_SOLID, 5, Hue(3));
        SelectObject(hdc, hPen);
        Arc(hdc, rand() % sw, rand() % sh, rand() % sw, rand() % sh, rand() % sw, rand() % sh, rand() % sw, rand() % sh);
        DeleteObject(hPen);
        ReleaseDC(0, hdc);
        Sleep(10);
    }
}

DWORD WINAPI beziers(LPVOID lpParam) {
	int sw = GetSystemMetrics(0);
	int sh = GetSystemMetrics(1);
	while (1) {
		HDC hdc = GetDC(0);
		POINT p[4] = { rand() % sw, rand() % sh, rand() % sw, rand() % sh,  rand() % sw, rand() % sh };
		HPEN hPen = CreatePen(PS_SOLID, 5, Hue(239));
		SelectObject(hdc, hPen);
		PolyBezier(hdc, p, 4);
		DeleteObject(hPen);
		ReleaseDC(0, hdc);
		Sleep(4);
	}
}

DWORD WINAPI shader1(LPVOID lpvd)
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

DWORD WINAPI shader2(LPVOID lpvd)
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

                int fx = (int)((i ^ 4) + (i * 4) * cbrt(sin(g * 3)));

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

DWORD WINAPI shader3(LPVOID lpvd)
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

                int fx = (int)((i ^ 4) + (i * 4) * sqrt((g ^ x & y)));

                hslcolor = Colors::rgb2hsl(rgbquad[index]);
                hslcolor.h = fmod(fx / 400.f + y / screenHeight * .2f, 1.f);

                rgbquad[index] = Colors::hsl2rgb(hslcolor);
            }
        }

        i++;

        StretchBlt(hdc, 0, 0, screenWidth, screenHeight, hdcCopy, 0, 0, screenWidth, screenHeight, NOTSRCCOPY);
        ReleaseDC(NULL, hdc);
        DeleteDC(hdc);
    }

    return 0x00;
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
        BitBlt(dcCopy, 5, 5, w, h, hdc, -10, -10, SRCCOPY);
        AlphaBlend(hdc, 0, 0, w, h, dcCopy, 0, 0, w, h, blur);
        ReleaseDC(0, hdc);
    }
    return 0x00;
}

DWORD WINAPI textout1(LPVOID lpvd)
{
    int x = GetSystemMetrics(0); int y = GetSystemMetrics(1);
    LPCSTR text1 = 0;
    LPCSTR text2 = 0;
    LPCSTR text3 = 0;
    LPCSTR text4 = 0;
    LPCSTR text5 = 0;
    LPCSTR text6 = 0;
    LPCSTR text7 = 0;

    while (1)
    {
        HDC hdc = GetDC(0);
        text1 = "GODDBYE, STUPID USER!";
        text2 = "ARE YOU DUMB?!";
        text3 = "YOUR STUPID PC HAS BEEN INFECTED BY MVNE.EXE!";
        text4 = "YOUR DATA WILL BE LOST!";
        text5 = "ARE YOU READY TO REINSTALL WINDOWS?!";
        text6 = "R.I.P DUMB PC!";
        text7 = "R.I.P YOUR DATA!";
        SetBkColor(hdc, RGB(rand() % 225, rand() % 225, rand() % 225));
        SetTextColor(hdc, RGB(rand() % 255, rand() % 255, rand() % 255));
        HFONT font = CreateFontA(43, 32, rand() % 360, rand() % 360, FW_EXTRALIGHT, 0, 0, 0, ANSI_CHARSET, 0, 0, 0, 0, "Times New Roman");
        SelectObject(hdc, font);
        TextOutA(hdc, rand() % x, rand() % y, text1, strlen(text1));
        TextOutA(hdc, rand() % x, rand() % y, text2, strlen(text2));
        TextOutA(hdc, rand() % x, rand() % y, text3, strlen(text3));
        TextOutA(hdc, rand() % x, rand() % y, text4, strlen(text4));
        TextOutA(hdc, rand() % x, rand() % y, text5, strlen(text5));
        TextOutA(hdc, rand() % x, rand() % y, text6, strlen(text6));
        TextOutA(hdc, rand() % x, rand() % y, text7, strlen(text7));

        DeleteObject(font);
        ReleaseDC(0, hdc);
        Sleep(50);
    }
}

DWORD WINAPI shk(LPVOID lpParam) {
    while (1) {
        HDC hdc = GetDC(0);
        int w = GetSystemMetrics(0);
        int h = GetSystemMetrics(1);
        BitBlt(hdc, rand() % 2, rand() % 2, w, h, hdc, rand() % 2, rand() % 2, SRCCOPY);
        PatBlt(hdc, 0, 0, w, h, PATINVERT);
        ReleaseDC(0, hdc);
    }
}

DWORD WINAPI se(LPVOID lpParam) {
    int w = GetSystemMetrics(0);
    int h = GetSystemMetrics(1);
    while (1) {
        HDC hdc = GetDC(0);
        HBRUSH brush = CreateSolidBrush(Hue(3));
        SelectObject(hdc, brush);
        BitBlt(hdc, -1, 0, w, h, hdc, 0, 0, SRCCOPY);
        BitBlt(hdc, w - 1, 0, w, h, hdc, 0, 0, PATCOPY);
        DeleteObject(brush);
        ReleaseDC(0, hdc);
    }
}

DWORD WINAPI shader4(LPVOID lpParam) {
    HDC hdcScreen = GetDC(0), hdcMem = CreateCompatibleDC(hdcScreen);
    INT w = GetSystemMetrics(0), h = GetSystemMetrics(1);
    BITMAPINFO bmi = { 0 };
    RGBQUAD* rgbScreen = NULL;
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
            rgbScreen[i].rgbRed += x >> y | x + y | x >> y;
            rgbScreen[i].rgbGreen += x >> y | x + y | x >> y;
            rgbScreen[i].rgbBlue += x >> y | x + y | x >> y;
        }
        BitBlt(hdcScreen, 0, 0, w, h, hdcMem, 0, 0, SRCCOPY);
        ReleaseDC(NULL, hdcScreen); DeleteDC(hdcScreen);
    }
}

DWORD WINAPI fnl(LPVOID lpParam) {
    HDC hdcScreen = GetDC(0), hdcMem = CreateCompatibleDC(hdcScreen);
    INT w = GetSystemMetrics(0), h = GetSystemMetrics(1);
    BITMAPINFO bmi = { 0 };
    RGBQUAD* rgbScreen = NULL;
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
            rgbScreen[i].rgbRed = x & y;
            rgbScreen[i].rgbGreen = x & y;
            rgbScreen[i].rgbBlue = x & y;
        }
        BitBlt(hdcScreen, 0, 0, w, h, hdcMem, 0, 0, SRCERASE);
        Sleep(10);
        ReleaseDC(NULL, hdcScreen); DeleteDC(hdcScreen);
    }
}

DWORD WINAPI snds(LPVOID lpParam) {
    int start = GetTickCount();
    int sw = GetSystemMetrics(0), sh = GetSystemMetrics(1);
    double angle = 0;
    while(GetTickCount() - start < 30000) {
        HDC hdc = GetDC(0);
        if(hdc) {
            for(float i = 0; i < sw + sh; i += 0.99f) {
                int a = (int)(sin(angle) * 50);
                BitBlt(hdc, 0, (int)i, sw, 1, hdc, a, (int)i, SRCCOPY);
                angle +=60;
            }
            ReleaseDC(0, hdc);
        }
        Sleep(16);
    }
    return 0;
}

DWORD WINAPI pdx(LPVOID lpParam) {
    int start = GetTickCount();
    int sw = GetSystemMetrics(0), sh = GetSystemMetrics(1);
    while(GetTickCount() - start < 30000) {
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

DWORD WINAPI shr999(LPVOID lpvd)
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

				FLOAT fx = (x ^ (x + y)) + (i * 4);

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

DWORD WINAPI patshake(LPVOID lpParam) {
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

DWORD WINAPI payload367(LPVOID lpParam)
{
    while (1) {
        HDC hdc = GetDC(NULL);
        int w = GetSystemMetrics(SM_CXSCREEN),
            h = GetSystemMetrics(SM_CYSCREEN);
        StretchBlt(hdc, 10, 10, w - 20, h - 20, hdc, 0, 0, w, h, SRCINVERT);
        StretchBlt(hdc, -10, -10, w + 20, h + 20, hdc, 0, 0, w, h, SRCINVERT);
        ReleaseDC(NULL, hdc);
        Sleep(10);
    }
}

DWORD WINAPI pacmans367(LPVOID lpParam)
{
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

DWORD WINAPI VietNamLover(LPVOID lpParam) {
    int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
    RGBQUAD* data = (RGBQUAD*)VirtualAlloc(0, (w * h + w) * sizeof(RGBQUAD), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    for (int i = 0;; i++, i %= 3) {
        HDC desk = GetDC(NULL);
        HDC hdcdc = CreateCompatibleDC(desk);
        HBITMAP hbm = CreateBitmap(w, h, 1, 32, data);
        SelectObject(hdcdc, hbm);
        BitBlt(hdcdc, 0, 0, w, h, desk, 0, 0, SRCCOPY);
        GetBitmapBits(hbm, w * h * 4, data);
        
        for (int i = 0; i < w * h; i++) {
        int randPixel = rand() % (w * h);
        BYTE tempR = data[i].rgbRed;
        BYTE tempG = data[i].rgbGreen;
        BYTE tempB = data[i].rgbBlue;
    
        data[i].rgbRed = data[randPixel].rgbRed;
        data[i].rgbGreen = data[randPixel].rgbGreen;
        data[i].rgbBlue = data[randPixel].rgbBlue;
    
        data[randPixel].rgbRed = tempR;
        data[randPixel].rgbGreen = tempG;
        data[randPixel].rgbBlue = tempB;
        }
        
        SetBitmapBits(hbm, w * h * 4, data);
        BitBlt(desk, 0, 0, w, h, hdcdc, 0, 0, SRCCOPY);
        DeleteObject(hbm);
        DeleteObject(hdcdc);
        ReleaseDC(NULL, desk);
    }
    return 0;
}
DWORD WINAPI blurrrrr(LPVOID lpvd) {
    HDC hdc = GetDC(NULL);
    HDC hdcCopy = CreateCompatibleDC(hdc);
    int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
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
        StretchBlt(hdcCopy, 0, 10, w, h + 20, hdc, 0, 0, w, h, SRCERASE);
        AlphaBlend(hdc, 0, 0, w, h, hdcCopy, 0, 0, w, h, blur);
        ReleaseDC(0, hdc);
    }
    return 0;
}

DWORD WINAPI iconnn(LPVOID lpParam) {
    int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
    int xf = 0, yf = 0, signX = 15, signY = 5, radius = 100;
    double angle = 0;
    int x, y, centerX, centerY;
    int origX = (w / 4) - (radius / 2), origY = (h / 4) - (radius / 2);
    while (1) {
        HDC hdc = GetDC(HWND_DESKTOP);
        xf += signX; yf += signY;
        centerX = origX - (w / 4); centerY = origY - (h / 4);
        x = (cos(angle) * radius) + centerX; y = (sin(angle) * radius) + centerY;
        int icon_x = GetSystemMetrics(SM_CXICON);
        int icon_y = GetSystemMetrics(SM_CYICON);
        for (INT i = 64; i > 8; i -= 8) {
            DrawIconEx(hdc, (50 + x - i + xf) - icon_x, (50 + y - i + yf) - icon_y, LoadIcon(0, IDI_QUESTION), 5 * GetSystemMetrics(SM_CXICON), 5 * GetSystemMetrics(SM_CYICON), 0, NULL, DI_NORMAL);
        }
        angle = fmod(angle + M_PI / radius, M_PI * radius);
        if (yf == 0) signY = 5;
        if (xf == 0) signX = 15;
        if (yf >= GetSystemMetrics(1)) signY -= 5;
        if (xf >= GetSystemMetrics(0)) signX -= 15;
        ReleaseDC(0, hdc);
        Sleep(10);
    }
    return 1;
}

DWORD WINAPI dwmn(LPVOID lpParam) {
	HDC hdc = GetDC(0); HWND wnd = GetDesktopWindow();
	int sw = GetSystemMetrics(0), sh = GetSystemMetrics(1);
	double angle = 0;
	while (1) {
		hdc = GetDC(0);
		for (float i = 0; i < sw + sh; i += 0.99f) {
			int a = sin(angle) * 20;
			HBRUSH brush = CreateSolidBrush(RGB(rand() % 255, rand() % 255, rand() % 255));
			SelectObject(hdc, brush);
            BitBlt(hdc, 0, i, sw, 1, hdc, a, i, (DWORD)(0x1900ac010e & 0xFFFFFFFF));
            BitBlt(hdc, i, 0, 1, sh, hdc, i, a, (DWORD)(0x1900ac010e & 0xFFFFFFFF));
			angle += M_PI / 40;
			DeleteObject(brush);
		}
		ReleaseDC(wnd, hdc);
	}
}

DWORD WINAPI jki(LPVOID lpParam) {
	while (1) {
		int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
		HDC hdc = GetDC(0);
		HPEN hPen = CreatePen(PS_SOLID, 2, Hue(239));
		HPEN hOldPen = (hdc, hPen);

		HBRUSH hBrush = CreateSolidBrush(Hue(239));
		HBRUSH hOldBrush = (hdc, hBrush);

		POINT vertices[] = { {rand() % w, rand() % h}, {rand() % w, rand() % h}, {rand() % w, rand() % h} };
		Polygon(hdc, vertices, sizeof(vertices) / sizeof(vertices[0]));

		(hdc, hOldBrush);
		DeleteObject(hBrush);

		(hdc, hOldPen);
		DeleteObject(hPen);
		ReleaseDC(0, hdc);
		Sleep(10);
	}
}

DWORD WINAPI yxdf(LPVOID lpParam) {
	int sw, sh;

	while (1) {
		HDC hdc = GetDC(0);
		sw = GetSystemMetrics(0);
		sh = GetSystemMetrics(1);
		StretchBlt(hdc, -20, -20, sw + 40, sh + 40, hdc, 0, 0, sw, sh, SRCCOPY);
		ReleaseDC(0, hdc);
		Sleep(4);
	}
}

COLORREF RndRGB() {
    return RGB(rand() % 255, rand() % 255, rand() % 255);
}

DWORD WINAPI final(LPVOID lpParam) {
    while (1) {
        HDC hdc = GetDC(NULL);
        int w = GetSystemMetrics(SM_CXSCREEN), h = GetSystemMetrics(SM_CYSCREEN);
        HBRUSH brush = CreateSolidBrush(RndRGB());
        SelectObject(hdc, brush);
        PatBlt(hdc, 0, 0, w, h, PATCOPY);
        BitBlt(hdc, rand() % 2, rand() % 2, w, h, hdc, rand() % 2, rand() % 2, SRCCOPY);
        DeleteObject(brush);
        ReleaseDC(NULL, hdc);
    }
    return 0;
}

VOID WINAPI sound1() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 16000, 16000, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[16000 * 30] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		buffer[t] = static_cast<char>((12 * t * (t >> 15 ^ t >> 7 | t >> 2)));

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
		buffer[t] = static_cast<char>(t * t + (1 * (t >> 9 & t >> 8)) & 128);

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
		buffer[t] = static_cast<char>(t & 583 ? t >> 4 : t >> 10);

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
		buffer[t] = static_cast<char>(12 * t * (t >> 11 | t >> 9 | t >> 1));

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
		buffer[t] = static_cast<char>(12 * t * log(t >> 15 | t >> 10 | t >> 2));

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
		buffer[t] = static_cast<char>((t * ((t >> 6 | t >> 12) & 15 & t >> 8)) * t >> 10);

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
		buffer[t] = static_cast<char>(20*t*t*(t>>11)/7);

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
        buffer[t] = static_cast<char>(t >> 12 | t * 9) & (t >> 8 | t * 4) & (t >> 4 | t * 6);

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
        buffer[t] = static_cast<char>((t ^ (t >> 5)) + ((t << 3) & (t >> 5)));

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI sound10() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[8000 * 30] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		buffer[t] = static_cast<char>(t * (0xCA98 >> (t >> 9 & 14) & 15) | t >> 2);
	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}

VOID WINAPI sound11() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 11025, 11025, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[11025 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>(t * (t >> 5 | t >> 8) >> (t >> 16));
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
    char buffer[11025 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>(t * (t >> 9 | t << 7));

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
        buffer[t] = static_cast<char>(((t << 1) ^ ((t << 1) + (t >> 7) & t >> 6)) | t >> (4 - (1 ^ 7 & (t >> 19))) | t >> 7);

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
        buffer[t] = static_cast<char>(8 * t & t >> 18 & t >> 6 | 5 * t & t >> 9);
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
		buffer[t] = static_cast<char>(3 % 255 * (t * ((t >> 9) | (t << 5))));

	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}

VOID WINAPI sound16() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 11025, 11025, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[11025 * 60] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		buffer[t] = static_cast<char>(t * (t ^ t + (t >> 15 | 1) / (t - 1290 ^ t) >> 11));

	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}

VOID WINAPI sound17() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 48000, 48000, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[48000 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>((t * 127) % 256);

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

int CALLBACK WinMain(
	HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine, int       nCmdShow
)
{
	if (MessageBoxW(NULL, L"This malware contains strong language, loud noises and flashing colors. The creator is not responsible. ARE YOU SURE 100% TO RUN THIS MALWARE?!", L"mvne-safety.exe by VietNamLover", MB_YESNO | MB_ICONEXCLAMATION) == IDNO)
	{
		ExitProcess(0);
	}
	else
	{
		if (MessageBoxW(NULL, L"REALLY?!", L"mvne-safety.exe", MB_YESNO | MB_ICONEXCLAMATION) == IDNO)
		{
			ExitProcess(0);
		}
		else
		{
			HANDLE thread1 = CreateThread(0, 0, bez, 0, 0, 0);
			sound1();
			Sleep(30000);
			TerminateThread(thread1, 0);
			CloseHandle(thread1);
			InvalidateRect(0, 0, 0);
			Sleep(100);
			HANDLE thread2 = CreateThread(0, 0, shader1, 0, 0, 0);
			sound2();
			Sleep(30000);
			TerminateThread(thread2, 0);
			CloseHandle(thread2);
			InvalidateRect(0, 0, 0);
			Sleep(100);
			HANDLE thread3 = CreateThread(0, 0, shader2, 0, 0, 0);
			HANDLE thread3dot1 = CreateThread(0, 0, blur1, 0, 0, 0);
			sound3();
			Sleep(30000);
			TerminateThread(thread3, 0);
			CloseHandle(thread3);
			TerminateThread(thread3dot1, 0);
			CloseHandle(thread3dot1);
			InvalidateRect(0, 0, 0);
			Sleep(100);
			HANDLE thread4 = CreateThread(0, 0, shader3, 0, 0, 0);
			HANDLE thread4dot1 = CreateThread(0, 0, textout1, 0, 0, 0);
			sound4();
			Sleep(30000);
			TerminateThread(thread4, 0);
			CloseHandle(thread4);
			TerminateThread(thread4dot1, 0);
			CloseHandle(thread4dot1);
			InvalidateRect(0, 0, 0);
			Sleep(100);
			HANDLE thread5 = CreateThread(0, 0, shk, 0, 0, 0);
			sound5();
			Sleep(30000);
			TerminateThread(thread5, 0);
			CloseHandle(thread5);
			InvalidateRect(0, 0, 0);
			Sleep(100);
			HANDLE thread6 = CreateThread(0, 0, se, 0, 0, 0);
			HANDLE thread6dot1 = CreateThread(0, 0, bez, 0, 0, 0);
			HANDLE thread6dot2 = CreateThread(0, 0, beziers, 0, 0, 0);
			sound6();
			Sleep(30000);
			TerminateThread(thread6, 0);
			CloseHandle(thread6);
			TerminateThread(thread6dot1, 0);
			CloseHandle(thread6dot1);
			TerminateThread(thread6dot2, 0);
			CloseHandle(thread6dot2);
			InvalidateRect(0, 0, 0);
			Sleep(100);
			HANDLE thread7 = CreateThread(0, 0, shader4, 0, 0, 0);
			sound7();
			Sleep(30000);
			TerminateThread(thread7, 0);
			CloseHandle(thread7);
			InvalidateRect(0, 0, 0);
			Sleep(100);
			HANDLE thread8 = CreateThread(0, 0, fnl, 0, 0, 0);
			sound8();
			Sleep(30000);
            TerminateThread(thread8, 0); 
			CloseHandle(thread8);
            InvalidateRect(0, 0, 0);
            Sleep(100);
            HANDLE thread9 = CreateThread(0, 0, snds, 0, 0, 0);
            HANDLE thread9dot1 = CreateThread(0, 0, pdx, 0, 0, 0);
            sound9();
            Sleep(30000);
            TerminateThread(thread9, 0); 
			CloseHandle(thread9);
            TerminateThread(thread9dot1, 0); 
			CloseHandle(thread9dot1);
            InvalidateRect(0, 0, 0);
            Sleep(100);
			HANDLE thread10 = CreateThread(0, 0, shr999, 0, 0, 0);
			sound10();
			Sleep(30000);
			TerminateThread(thread10, 0);
			CloseHandle(thread10);
			InvalidateRect(0, 0, 0);
			Sleep(100);
            HANDLE thread11 = CreateThread(0, 0, patshake, 0, 0, 0);
            sound11(); 
			Sleep(30000);
			TerminateThread(thread11, 0);
			CloseHandle(thread11);
			InvalidateRect(0, 0, 0);
			Sleep(100);
            HANDLE thread12 = CreateThread(0, 0, payload367, 0, 0, 0);
            HANDLE thread12dot1 = CreateThread(0, 0, pacmans367, 0, 0, 0);
            sound12();
            Sleep(30000);
            TerminateThread(thread12, 0);
            CloseHandle(thread12);
            TerminateThread(thread12dot1, 0);
            CloseHandle(thread12dot1);
            InvalidateRect(0, 0, 0);
            Sleep(100);
            HANDLE thread13 = CreateThread(0, 0, VietNamLover, 0, 0, 0);
            sound13();
            Sleep(30000);
            TerminateThread(thread13, 0);
            CloseHandle(thread13);
            InvalidateRect(0, 0, 0);
            Sleep(100);
            HANDLE thread14 = CreateThread(0, 0, blurrrrr, 0, 0, 0);
            HANDLE thread14dot1 = CreateThread(0, 0, iconnn, 0, 0, 0);
            sound14();
            Sleep(30000);
            TerminateThread(thread14, 0);
            CloseHandle(thread14);
            TerminateThread(thread14dot1, 0);
            CloseHandle(thread14dot1);
            InvalidateRect(0, 0, 0);
            Sleep(100);
			HANDLE thread15 = CreateThread(0, 0, dwmn, 0, 0, 0);
			HANDLE thread15dot1 = CreateThread(0, 0, jki, 0, 0, 0);
			sound15();
			Sleep(30000);
			TerminateThread(thread15, 0);
			CloseHandle(thread15);
			TerminateThread(thread15dot1, 0);
			CloseHandle(thread15dot1);
			InvalidateRect(0, 0, 0);
			Sleep(100);
			HANDLE thread16 = CreateThread(0, 0, yxdf, 0, 0, 0);
			sound16();
			Sleep(30000);
			TerminateThread(thread16, 0);
			CloseHandle(thread16);
			InvalidateRect(0, 0, 0);
			Sleep(100);
            HANDLE thread17 = CreateThread(0, 0, final, 0, 0, 0);
            sound17();
            Sleep(30000);
		}
	}
	return 0;
}