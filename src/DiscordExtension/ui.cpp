#include "imgui.h"
#include <windows.h>
#include "../Shared/protocol.h"

// 全域變數
static char g_scriptBuffer[1024] = "print('Hello from Kernel!')";
static HANDLE g_hDriver = NULL;

void RenderUI() {
    // 初始化驅動句柄 (如果還沒)
    if (g_hDriver == NULL) {
        g_hDriver = CreateFileA(
            "\\\\.\\RobloxDriver", 
            GENERIC_READ | GENERIC_WRITE, 
            0, NULL, OPEN_EXISTING, 0, NULL
        );
    }

    // 建立視窗
    ImGui::Begin("Roblox Executor", NULL, ImGuiWindowFlags_NoCollapse);

    // 1. 上方的程式碼輸入框 (InputTextMultiline)
    // ImVec2(-1, 200) 表示寬度填滿，高度 200 像素
    ImGui::InputTextMultiline("##Script", g_scriptBuffer, IM_ARRAYSIZE(g_scriptBuffer), ImVec2(-1, 200));

    ImGui::Separator(); // 分隔線
    ImGui::Spacing();

    // 2. 下方的 Inject 按鈕
    // ImVec2(-1, 30) 表示按鈕寬度填滿，高度 30 像素
    if (ImGui::Button("Inject Script", ImVec2(-1, 30))) {
        if (g_hDriver != INVALID_HANDLE_VALUE) {
            EXEC_SCRIPT execInfo = {0};
            execInfo.Pid = GetCurrentProcessId(); // 實際應改為 Roblox 的 PID
            strncpy_s(execInfo.Script, g_scriptBuffer, sizeof(execInfo.Script) - 1);
            
            DWORD bytesReturned = 0;
            DeviceIoControl(
                g_hDriver, 
                IOCTL_EXEC_SCRIPT, 
                &execInfo, sizeof(execInfo), 
                NULL, 0, 
                &bytesReturned, NULL
            );
        }
    }

    ImGui::End();
}
