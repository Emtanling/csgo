#include "Point.h"
#include <Tchar.h>
#include <dwmapi.h>
#include <string>
#include <string_view>
#pragma comment(lib,"dwmapi.lib")
static HWND game_hwnd;
static HWND point_hwnd;
PointWindows::PointWindows(std::string_view window_name, std::string_view class_name)
{
	if (window_name.empty() || class_name.empty())
	{
		throw std::exception("窗口获取失败!");
	}
	create_PointWindows(window_name, class_name);
}

PointWindows::~PointWindows()
{
}

void PointWindows::create_PointWindows(std::string_view window_name, std::string_view class_name)
{
	WNDCLASSEXA wnd;         // 初始化
	wnd.cbSize = sizeof(WNDCLASSEXA);
	wnd.lpfnWndProc = WndProc;
	wnd.style = CS_HREDRAW | CS_VREDRAW;
	wnd.cbClsExtra = 0;
	wnd.cbWndExtra = 0;
	wnd.hInstance = NULL;
	wnd.hIcon = NULL;
	wnd.hCursor = LoadCursor(NULL, IDC_ARROW);
	wnd.hbrBackground = (HBRUSH)RGB(0, 0, 0);
	wnd.lpszMenuName = "";
	wnd.lpszClassName = "PointWindow";
	wnd.hIconSm = NULL;
	if (!RegisterClassExA(&wnd))    // 注册窗口
	{
		MessageBox(NULL, L"注册窗口类失败！", _T("创建窗口"), NULL);
		throw std::exception("窗口注册失败！");
	}
	game_hwnd = FindWindowA(class_name.data(), window_name.data());
	GetWindowRect(game_hwnd, &game_rect);    // 获得窗口信息
	point_hwnd = CreateWindowExA(WS_EX_TOPMOST|WS_EX_LAYERED|WS_EX_TRANSPARENT, "PointWindow", "", WS_VISIBLE | WS_POPUP, game_rect.left, game_rect.top, game_rect.width(), game_rect.height(), NULL, NULL, NULL, NULL);
	MARGINS rect = { game_rect.left, game_rect.top, game_rect.width(), game_rect.height() };
	DwmExtendFrameIntoClientArea(point_hwnd, &rect);
	SetLayeredWindowAttributes(point_hwnd, RGB(0, 0, 0), 255, LWA_ALPHA);
	ShowWindow(point_hwnd, SW_SHOW);
	init_dx9();
}

LRESULT PointWindows::WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
	case WM_DESTROY:
		exit(0);
		break;
	default:
		return DefWindowProc(hwnd, msg, wparam, lparam);
	}
}

void PointWindows::init_dx9()
{
	m_d3d = Direct3DCreate9(D3D_SDK_VERSION);

	D3DPRESENT_PARAMETERS pp;
	ZeroMemory(&pp, sizeof(D3DPRESENT_PARAMETERS));
	pp.Windowed = true;
	pp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	pp.BackBufferFormat = D3DFMT_A8R8G8B8;
	pp.BackBufferWidth = game_rect.width();
	pp.BackBufferHeight = game_rect.height();
	pp.hDeviceWindow = point_hwnd;
	pp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
	if (FAILED(m_d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, point_hwnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &pp, &m_device)))
	{
		throw std::exception("m_device设备初始化异常");
	}

}

dx_pointRect::dx_pointRect(IDirect3DDevice9* mdevice)
{
	if (NULL == mdevice)
	{
		throw std::exception("mdevice 为空");
	}
	d3d_device = mdevice;
	if (FAILED(D3DXCreateLine(mdevice, &d3d_line)))
	{
		throw std::exception("线条初始化失败");
	}
	if (FAILED(D3DXCreateFont(d3d_device, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, L"Emtanling", &d3d_font)))
	{
		MessageBox(NULL, L"注册窗口类失败！", _T("创建窗口"), NULL);
		throw std::exception("字体初始化失败");
	}
}

dx_pointRect::~dx_pointRect()
{
	if (d3d_line)
	{
		d3d_line->Release();
	}
	if (d3d_font)
	{
		d3d_font->Release();
	}
}

void dx_pointRect::begain_pointRect()
{
	d3d_device->Clear(NULL, NULL, D3DCLEAR_TARGET, D3DCOLOR_RGBA(0, 0, 0, 0), 1.f, 0);
	d3d_device->BeginScene();
}

void dx_pointRect::end_poinRrect()
{
	d3d_device->EndScene();
	d3d_device->Present(NULL, NULL, NULL, NULL);
}

void dx_pointRect::draw_rect(int x0, int y0, int w, int h, unsigned color)
{
	draw_line(x0, y0, x0 + w, y0, color);
	draw_line(x0, y0, x0, y0 + h, color);
	draw_line(x0 + w, y0, x0 + w, y0 + h, color);
	draw_line(x0, y0 + h, x0 + w, y0 + h, color);
}

void dx_pointRect::draw_line(int x0, int y0, int x1, int y1, unsigned color)
{
	D3DXVECTOR2 line[2] = { D3DXVECTOR2(x0,y0),D3DXVECTOR2(x1,y1) };
	d3d_line->Begin();
	d3d_line->Draw(line, 2, color);
	d3d_line->End();
}

void dx_pointRect::draw_text(std::string_view text, int x, int y, unsigned long color)
{
	RECT r = { x,y,x,y };
	d3d_font->DrawTextA(NULL, text.data(), -1, &r, DT_NOCLIP, color);
}
