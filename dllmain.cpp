#include "header.h"

extern auto initialize(void* instance) -> void;

static auto WINAPI cheat(const LPVOID hModule) -> DWORD
{
    initialize(hModule);
    return 0;
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    if (ul_reason_for_call == DLL_PROCESS_ATTACH)
    {
        if (hModule)
            DisableThreadLibraryCalls(hModule);

        auto a = CreateThread(NULL, 0, cheat, hModule, 0, NULL);

        if (a)
           CloseHandle(a);
    }

    return TRUE;
}

