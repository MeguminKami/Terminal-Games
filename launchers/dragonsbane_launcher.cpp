#if defined(_WIN32)
#include <windows.h>
#include <string>

int main() {
    wchar_t modulePath[MAX_PATH];
    DWORD len = GetModuleFileNameW(nullptr, modulePath, MAX_PATH);
    std::wstring exeDir(modulePath, modulePath + len);
    size_t pos = exeDir.find_last_of(L"\\/");
    if (pos != std::wstring::npos) exeDir = exeDir.substr(0, pos);

    std::wstring target = exeDir + L"\\dragonsbane.exe";
    std::wstring cmd = L"\"" + target + L"\"";

    STARTUPINFOW si{};
    si.cb = sizeof(si);
    PROCESS_INFORMATION pi{};
    if (!CreateProcessW(nullptr, cmd.data(), nullptr, nullptr, FALSE, CREATE_NEW_CONSOLE, nullptr, exeDir.c_str(), &si, &pi)) {
        return 1;
    }
    CloseHandle(pi.hThread);
    CloseHandle(pi.hProcess);
    return 0;
}
#else
#include <cstdlib>
int main(){ return system("./dragonsbane"); }
#endif
