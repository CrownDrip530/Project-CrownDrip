#include <windows.h>
#include "ui.cpp" // 簡單起見，直接包含 UI 邏輯

// 這裡應該包含 ImGui 的初始化與 Hook 邏輯
// 為了簡化，假設 Hook 函數在 hook.cpp 中

void SetupImGui(); // 宣告 Hook 設定函數
void RenderFrame(); // 宣告渲染函數

DWORD WINAPI MainThread(LPVOID lpParam) {
    // 1. 初始化 ImGui 與 DirectX Hook
    // SetupImGui(); 
    
    // 2. 主迴圈
    while (true) {
        // RenderFrame(); // 呼叫 Hook 中的渲染邏輯
        // 這裡簡化為直接呼叫 UI (實際專案需整合 DX Hook)
        // RenderUI(); 
        
        Sleep(1);
    }
    return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    if (ul_reason_for_call == DLL_PROCESS_ATTACH) {
        CreateThread(NULL, 0, MainThread, NULL, 0, NULL);
    }
    return TRUE;
}
