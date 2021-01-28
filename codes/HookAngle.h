#pragma once
#include <windows.h>
#pragma push(1)
class HookAngle
{
private:
	const BYTE JMP;
	DWORD HookAddress;
	const BYTE NOP;
public:
	HookAngle(DWORD srcAddress,DWORD hookAddress) :JMP(0xE9), NOP(0x90)
	{
		setHookAddress(srcAddress,hookAddress);
	}
	void setHookAddress(DWORD srcAddress,DWORD hookAddress)
	{
		HookAddress = srcAddress - hookAddress - 0x5;
	}
};

#pragma pop(1)

