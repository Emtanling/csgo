#pragma once
#include <Windows.h>

uintptr_t GetDllAdd(DWORD CS_GO_PID)
{
    PROCESSENTRY32 pe32;
    pe32.dwSize = sizeof(pe32);
    HANDLE hProcessSnap = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, CS_GO_PID);
    HANDLE hModuleSnap = NULL;
    if (hProcessSnap == INVALID_HANDLE_VALUE)
    {
        printf("CreateTollHelp32Snapshot Error!!\n");
        return -1;
    }
    BOOL bMore = ::Process32First(hProcessSnap, &pe32);
    HANDLE hProcess = 0;
    WCHAR procPath[_MAX_PATH] = { 0 };
    MODULEENTRY32 lpme;
    lpme.dwSize = sizeof(MODULEENTRY32); 
    BOOL bRet = FALSE;
    uintptr_t Dll_Base = 0;
    wchar_t csgo_clientdll_Path[] = L"d:\\steam\\steamapps\\common\\counter-strike global offensive\\csgo\\bin\\client.dll";
    //wchar_t csgo_clientdll[] = L"client.dll";
    while (bMore)
    {
        hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, CS_GO_PID);
        GetModuleFileNameEx(hProcess, NULL, procPath, _MAX_PATH);
        hModuleSnap = ::CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pe32.th32ProcessID);
        bRet = ::Module32First(hModuleSnap, &lpme);
        while (bRet)
        {
            if (!wcscmp(csgo_clientdll_Path, (const wchar_t*)lpme.szExePath))
            {
                Dll_Base = (uintptr_t)lpme.modBaseAddr;
                printf("%x\n", Dll_Base);
            }
            bRet = ::Module32Next(hModuleSnap, &lpme);
        }
        ::CloseHandle(hModuleSnap);
        bMore = ::Process32Next(hProcessSnap, &pe32);
    }
    ::CloseHandle(hProcessSnap);
    return Dll_Base;
}
