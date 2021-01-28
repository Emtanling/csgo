#pragma once
#include <Windows.h>
#include <tchar.h>
#include <stdio.h>
union TranlateNumber
{
	DWORD dwNumber;
	float flNumber;
};
struct Game_characters
{
	unsigned health;
	unsigned team;
	TranlateNumber people[3];
	float pos_top;
};
template<typename T>
T read(HANDLE GameProcess,uintptr_t address)
{
	T data_address;
	ReadProcessMemory(GameProcess, (LPCVOID)address, &data_address, sizeof(data_address), NULL);
	return data_address;
}
template<typename T> 
void readlistchar(HANDLE GameProcess, uintptr_t address, BYTE* offset, unsigned int control, Game_characters* gameChar)
{
	for (int i = 0; i < control; i++)
	{
		DWORD temporary_address = read<DWORD>(GameProcess, address + offset[i]) + 0x100;
		DWORD team_address = read<DWORD>(GameProcess, address + offset[i]) + 0xF4;
		DWORD people_address_y = read<DWORD>(GameProcess, address + offset[i]) + 0xA0;
		DWORD people_address_x = read<DWORD>(GameProcess, address + offset[i]) + 0xA0 + 0x4;
		DWORD people_address_z = read<DWORD>(GameProcess, address + offset[i]) + 0xA0 + 0x4 + 0x4;
		(gameChar + i)->health = read<DWORD>(GameProcess, temporary_address);
		(gameChar + i)->team = read<BYTE>(GameProcess, team_address);
		(gameChar + i)->people[0].dwNumber = read<DWORD>(GameProcess, people_address_y);
		(gameChar + i)->people[1].dwNumber = read<DWORD>(GameProcess, people_address_x);
		(gameChar + i)->people[2].dwNumber = read<DWORD>(GameProcess, people_address_z);
		(gameChar + i)->pos_top = (gameChar + i)->people[2].flNumber + 100.f;
	/*	
		if (2 == (gameChar + i)->team)
			printf("data_%d ÉúÃü£º%d ×ø±ê£ºy = %f,x = %f ,z = %f ·ËÍ½!\n", i, (gameChar + i)->health, (gameChar + i)->people[0].flNumber, (gameChar + i)->people[1].flNumber, (gameChar + i)->people[2].flNumber);
		else
			printf("data_%d ÉúÃü£º%d ×ø±ê£ºy = %f,x = %f ,z = %f ¾¯²ì!\n", i, (gameChar + i)->health, (gameChar + i)->people[0].flNumber, (gameChar + i)->people[1].flNumber, (gameChar + i)->people[2].flNumber);
	*/
	}
}
template<typename T>
void readlistrect(HANDLE GameProcess, uintptr_t address, float (*temp)[4])
{
	TranlateNumber temp_float;
	DWORD cound=0x0;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			cound += 0x4;
			temp_float.dwNumber = read<DWORD>(GameProcess, address + cound);
			temp[i][j] = temp_float.flNumber;
		}
	}
}
