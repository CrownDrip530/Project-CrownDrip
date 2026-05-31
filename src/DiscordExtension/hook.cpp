#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"
#include <d3d11.h>
#include <minhook.h>
#include "ui.cpp"

typedef HRESULT(__stdcall *tPresent)(IDXGISwapChain*, UINT, UINT);
tPresent oPresent = nullptr;

HRESULT __stdcall hkPresent(IDXGISwapChain* pSwapChain, UINT syncInterval, UINT flags) {
    static bool initialized = false;
    if (!initialized) {
        // 初始化 ImGui (需獲取 Device 與 HWND)
        // ...
        initialized = true;
    }

    // 開始幀
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    // 渲染 UI
    RenderUI();

    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    return oPresent(pSwapChain, syncInterval, flags);
}

// 呼叫此函數來啟動 Hook
void SetupHook(IDXGISwapChain* pSwapChain) {
    // 使用 MinHook 攔截 Present
    // MH_CreateHook(pSwapChain->Present, hkPresent, reinterpret_cast<LPVOID*>(&oPresent));
    // MH_EnableHook(MH_ALL_HOOKS);
}
