#include <Windows.h>
#include "Structures.h"
#include "Helpers.h"

int dmg = 0;
DWORD start = 0x0511EE2;
DWORD len = 7;
DWORD back = start + len;

void GetDmg()
{
	TNTClient* client = **(TNTClient***)(0x006DB738);

	char* dmgStr = client->m_pNosRevCmdList->m_pStringList->Get(12);
	dmg = atoi(dmgStr);
}

__declspec(naked) void Hook()
{
	__asm
	{
		PUSHAD;
		PUSHFD;
		CALL GetDmg;
		POPFD;
		POPAD;
		MOV EAX, dmg;
		LEA EDX, DWORD PTR SS : [EBP - 0xC];
		JMP back;
	}
}

DWORD WINAPI DLLStart(LPVOID param)
{
	MakeJMP((BYTE*)start, (DWORD)&Hook, 7);
	return 0;
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
		CreateThread(0, 0, DLLStart, 0, 0, 0);
		break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

