#include <iostream>
#include <tchar.h>
#include <Windows.h>
#include <TlHelp32.h>
#include <psapi.h>
#include <string>
#include "AddBullet.h"
#include "GetDllAddress.h"
#include "ReadProMe.h"
#include "Point.h"
using namespace std;

struct Screen
{
    float screen_x;
    float screen_y;
};
BOOL GameTranScreen_1(float temp[4][4], Screen& out, Game_characters* gameChar)
{
    out.screen_x = temp[0][0] * gameChar->people[0].flNumber + temp[0][1] * gameChar->people[1].flNumber + temp[0][2] * gameChar->people[2].flNumber + temp[0][3];
    out.screen_y = temp[1][0] * gameChar->people[0].flNumber + temp[1][1] * gameChar->people[1].flNumber + temp[1][2] * gameChar->people[2].flNumber + temp[1][3];
    float w = temp[3][0] * gameChar->people[0].flNumber + temp[3][1] * gameChar->people[1].flNumber + temp[3][2] * gameChar->people[2].flNumber + temp[3][3];
    if (w < 0.1f)
    {
        return false;
    }
    float inv_w = 1.f / w;
    out.screen_x *= inv_w;
    out.screen_y *= inv_w;
    float screen_x = 1920 * 0.5f;
    float screen_y = 1080 * 0.5f;
    screen_x += 0.5 * out.screen_x * 1920;
    screen_y += 0.5 * out.screen_y * 1080;
    out.screen_x = screen_x;
    out.screen_y = screen_y;
    return true;
}
BOOL GameTranScreen_2(float temp[4][4], Screen& out, Game_characters* gameChar)
{
    out.screen_x = temp[0][0] * gameChar->people[1].flNumber + temp[0][1] * gameChar->people[0].flNumber + temp[0][2] * gameChar->pos_top + temp[0][3];
    out.screen_y = temp[1][0] * gameChar->people[1].flNumber + temp[1][1] * gameChar->people[0].flNumber + temp[1][2] * gameChar->pos_top + temp[1][3];
    float w = temp[3][0] * gameChar->people[1].flNumber + temp[3][1] * gameChar->people[0].flNumber + temp[3][2] * gameChar->pos_top + temp[3][3];
    if (w < 0.01f)
    {
        return false;
    }
    float inv_w = 1.f / w;
    out.screen_x *= inv_w;
    out.screen_y *= inv_w;
    float screen_x = 1920 * 0.5f;
    float screen_y = 1080 * 0.5f;
    screen_x += 0.5 * out.screen_x * 1920;
    screen_y += 0.5 * out.screen_y * 1080;
    out.screen_x = screen_x;
    out.screen_y = screen_y;
    return true;
}

int _tmain(int argc, _TCHAR* argv[])
{
    int CS_GO_PID;
    std::cout << "csgoID£º";
    std::cin >> CS_GO_PID;
    HANDLE GameProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, CS_GO_PID);
    GetDllAdd(CS_GO_PID);
    DWORD clientdll_Bullet_offset = 0x4DA0D54;   
    DWORD clientdll_rect_offset = 0x4D92760;
    DWORD clientdll_base_offset_Bullet = clientdll_Bullet_offset + GetDllAdd(CS_GO_PID);
    DWORD clientdll_base_offset_rect = clientdll_rect_offset + GetDllAdd(CS_GO_PID);
    BYTE offset[10]{ 0x0,0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80,0x90 };
    Game_characters gameChar[10];
    float temp[4][4];
    PointWindows pointwnd("Counter-Strike: Global Offensive", "Valve001");  // ´°¿Ú³õÊ¼»¯
    dx_pointRect dx(pointwnd.m_device);

    while (true)
    {

        dx.begain_pointRect();
        for (int i = 0; i < 10; i++)
        {
            readlistchar<void>(GameProcess, clientdll_base_offset_Bullet, offset, 10, gameChar);
            readlistrect<void>(GameProcess, clientdll_base_offset_rect, temp);
            if (0 == i)
            {
                continue;
            }
            if ((gameChar + i)->health == 0)
                continue;
            Screen v1, v2;
            if (!GameTranScreen_1(temp, v1, (gameChar + i)) || !GameTranScreen_2(temp, v2, (gameChar + i)))
            {
                continue;
            }
            int box_height = v1.screen_y - v2.screen_y;
            int box_weight = box_height / 2.0;
            dx.draw_rect(v2.screen_x - box_weight + 100.f + 150.f, v2.screen_y, box_weight, box_height, D3DCOLOR_RGBA(55, 66, 177, 255));
            dx.draw_text(to_string((gameChar + i)->health) + "HP", v1.screen_x - 300.f, v2.screen_y, 0xffffffff);
        }
        //dx.begain_pointRect();
        //dx.draw_rect(33, 55, 100, 100, D3DCOLOR_RGBA(44, 55, 66, 177, 255));
        //dx.draw_text("100", 66, 77, D3DCOLOR_RGBA(44, 55, 66, 255));
        dx.end_poinRrect();
        
    }
    
    
}