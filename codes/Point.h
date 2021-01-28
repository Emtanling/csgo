#pragma once
#include <windows.h>
#include <string>
#include <d3d9.h>
#include <d3dx9.h>
#pragma comment (lib,"d3d9.lib")
#pragma comment (lib,"d3dx9.lib")
class PointWindows
{
	struct win_rect :public RECT
	{
		int width()
		{
			return right - left;
		}
		int height()
		{
			return bottom - top;
		}
	};
public:
	IDirect3DDevice9* m_device = nullptr;
	PointWindows(std::string_view window_name, std::string_view class_name);
	~PointWindows();
	void create_PointWindows(std::string_view window_name, std::string_view class_name);
private:
	static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
	win_rect game_rect;
	win_rect PointWnd_rect;
	void init_dx9();
	IDirect3D9* m_d3d = nullptr;
	BOOL m_initialized = false;
};
class dx_pointRect
{
public:
	dx_pointRect(IDirect3DDevice9* mdevice);
	~dx_pointRect();
	void begain_pointRect();
	void end_poinRrect();
	void draw_rect(int x0, int y0, int w, int h, unsigned color);
	void draw_line(int x0, int y0, int x1, int y1, unsigned color);
	void draw_text(std::string_view text, int x, int y, unsigned long color);
private:
	IDirect3DDevice9* d3d_device;
	ID3DXFont* d3d_font = nullptr;
	ID3DXLine* d3d_line = nullptr;
};
