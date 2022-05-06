#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>
#include <direct.h>
#pragma warning(disable:4996)

DWORD GetProcId(const wchar_t* procName)
{
    DWORD procId = 0;
    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnap != INVALID_HANDLE_VALUE)
    {
        PROCESSENTRY32 procEntry;
        procEntry.dwSize = sizeof(procEntry);

        if (Process32First(hSnap, &procEntry))
        {
            do
            {
                if (!_wcsicmp(procEntry.szExeFile, procName))
                {
                    procId = procEntry.th32ProcessID;
                    break;
                }
            } while (Process32Next(hSnap, &procEntry));

        }
    }
    CloseHandle(hSnap);
    return procId;
}

int main()
{
    char tmp[256];
    _getcwd(tmp, 256);
    const char* dllPath = strcat(tmp, "\\cheat.dll");
    DWORD procId = 0;
    std::wcout << "DLL path: " << dllPath<< std::endl;

    while (!procId)
    {
        procId = GetProcId(L"ac_client.exe");
        Sleep(30);
        if (procId != 0)
        {

            std::wcout << std::endl << "Process ID: " << procId << std::endl;
        }
        else 
        {
            for (int boo = 0; boo < 1; boo++)
            {
                std::cout << "\rac_client.exe not found      \rac_client.exe not found";
                for (int foo = 0; foo < 3; foo++)
                {
                    std::cout << ".";
                    Sleep(500);
                }
            }
        }

    }

    HANDLE hProc = OpenProcess(PROCESS_ALL_ACCESS, 0, procId);

    if (hProc && hProc != INVALID_HANDLE_VALUE)
    {
        void* loc = VirtualAllocEx(hProc, 0, MAX_PATH, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

        WriteProcessMemory(hProc, loc, dllPath, strlen(dllPath) + 1, 0);

        HANDLE hThread = CreateRemoteThread(hProc, 0, 0, (LPTHREAD_START_ROUTINE)LoadLibraryA, loc, 0, 0);

        if (hThread)
        {
            CloseHandle(hThread);
        }
    }

    if (hProc)
    {
        CloseHandle(hProc);
    }
    return 0;
}