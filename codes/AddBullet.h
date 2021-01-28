#pragma once
#include <Windows.h>
#pragma pack (1)
struct JmpCode	// 32位程序，JMP地址
{
private:
    const BYTE JMP;
    DWORD address;
    const BYTE NOP;

public:
    JmpCode(DWORD srcAddr, DWORD dstAddr) :JMP(0xE9), NOP(0x90)
    {
        setAddress(srcAddr, dstAddr);
    }

    void setAddress(DWORD srcAddr, DWORD dstAddr)
    {
        address = dstAddr - srcAddr - 0x5; // JMP指令长度 = sizeof(JmpCode) = 5
    }
};
struct JmpHookCode
{
private:
    const BYTE HackCode[11] = { 0xBF,0x40,0x00,0x00,0x00,0x89,0xBE,0x14,0x14,0x00,0x00 };
    const BYTE JMP;
    DWORD retaddress;
public:
    JmpHookCode(DWORD srcAddr, DWORD dstAddr) :JMP(0xE9)
    {
        setAddress(srcAddr, dstAddr);
    }

    void setAddress(DWORD srcAddr, DWORD dstAddr)
    {
        retaddress = dstAddr - srcAddr - 0x5 - 0x5; // JMP指令长度 = sizeof(JmpCode) = 5
    }
};
void hook_serverdllAddress(HANDLE GameProcess, DWORD serverdll_base_offset, DWORD hook_base)
{
    JmpCode code(serverdll_base_offset, hook_base);
    WriteProcessMemory(GameProcess, (LPVOID)serverdll_base_offset, &code, sizeof(code), NULL);
}
void hook_hackAddress(HANDLE GameProcess, DWORD hack_address, DWORD hack_retAddress)
{
    JmpHookCode code(hack_address, hack_retAddress);
    WriteProcessMemory(GameProcess, (LPVOID)hack_address, &code, sizeof(code), NULL);
}
#pragma pop(1)
