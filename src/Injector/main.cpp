#include <windows.h>
#include <iostream>
#include <tlhelp32.h>

DWORD GetPidByName(const char* name) {
    HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    PROCESSENTRY32 proc;
    proc.dwSize = sizeof(proc);
    if (Process32First(snap, &proc)) {
        do {
            if (strcmp(proc.szExeFile, name) == 0) return proc.th32ProcessID;
        } while (Process32Next(snap, &proc));
    }
    return 0;
}

int main() {
    DWORD pid = GetPidByName("Discord.exe");
    if (!pid) {
        std::cout << "Discord not found!\n";
        return 1;
    }

    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
    char dllPath[] = "C:\\Path\\To\\DiscordExtension.dll"; // 替換為你的 DLL 路徑
    
    LPVOID remoteMem = VirtualAllocEx(hProcess, NULL, strlen(dllPath) + 1, MEM_COMMIT, PAGE_READWRITE);
    WriteProcessMemory(hProcess, remoteMem, dllPath, strlen(dllPath) + 1, NULL);
    
    HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)LoadLibraryA, remoteMem, 0, NULL);
    
    if (hThread) std::cout << "Injected successfully!\n";
    else std::cout << "Injection failed!\n";

    system("pause");
    return 0;
}
