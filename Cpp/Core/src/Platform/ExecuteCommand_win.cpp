#include "Platform/ExecuteCommand.h"
#ifdef MSVC
#include <objbase.h>
#include <shlobj.h>
#include <shlwapi.h>
#include <strsafe.h>
#include <stdexcept>
namespace {
    bool IsSuccess(HINSTANCE response) {
        return ((INT_PTR)response) > 32;
    }

    std::string ErrorCodeToString(HINSTANCE response) {
        INT_PTR errorCode = (INT_PTR)response;
        switch(errorCode) {
        case 0: return "OOM";
        case ERROR_FILE_NOT_FOUND: return "File not found";
        case ERROR_PATH_NOT_FOUND: return "Path not found";
        case ERROR_BAD_FORMAT: return "Bad Format";
        case SE_ERR_OOM: return "Out of Memory";
        case SE_ERR_ACCESSDENIED: return "Access Denied";
        case SE_ERR_DLLNOTFOUND: return "Dll Not Found";
        case SE_ERR_SHARE: return "Cannot share an open file";
        case SE_ERR_ASSOCINCOMPLETE: return "File association information not complete";
        case SE_ERR_DDETIMEOUT: return "DDE Timeout";
        case SE_ERR_DDEFAIL: return "DDE operation failed";
        case SE_ERR_DDEBUSY: return "DDE operation is busy";
        case SE_ERR_NOASSOC: return "Unknown file extension";
        default: return "Unknown error.  Code: " + errorCode;
        }
    }

    enum struct ShowHide { Show, Hide };
    HINSTANCE Execute(const std::string& command, const std::string& args, ShowHide showWindow) {
        auto cmd = "/K \"" + command + "\" " + args;
        int swOpt = showWindow == ShowHide::Show ? SW_SHOW : SW_HIDE;

        return ShellExecute(NULL, NULL, "cmd.exe", cmd.c_str(), NULL, swOpt);
    }
} // namespace

namespace Platform {
    void ExecuteCommand(const std::string& command, const std::string& args) {
        auto result = Execute(command, args, ShowHide::Hide);
        if(!IsSuccess(result)) {
            throw std::runtime_error(ErrorCodeToString(result));
        }
    }

    bool ExecuteCommand(const std::string& command, const std::string& args, std::string& outErrorMessage) {
        auto result = Execute(command, args, ShowHide::Hide);
        if(IsSuccess(result)) {
            return true;
        } else {
            outErrorMessage = ErrorCodeToString(result);
            return false;
        }
    }

    void ExecuteCommandKeepWindow(const std::string& command, const std::string& args) {
        auto result = Execute(command, args, ShowHide::Show);
        if(!IsSuccess(result)) {
            throw std::runtime_error(ErrorCodeToString(result));
        }
    }

    bool ExecuteCommandKeepWindow(const std::string& command, const std::string& args, std::string& outErrorMessage) {
        auto result = Execute(command, args, ShowHide::Show);
        if(IsSuccess(result)) {
            return true;
        } else {
            outErrorMessage = ErrorCodeToString(result);
            return false;
        }
    }

    std::string ReadLastErrorMessage() {
        TCHAR errBuf[256];
        FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&errBuf, 0, NULL);
        return std::string(errBuf);    

        /*
        DWORD errorCode = GetLastError();
        LPVOID messageBuffer;
        LPVOID lpDisplayBuf;

        FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, errorCode, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&messageBuffer, 0, NULL);
        std::string result{messageBuffer};
        lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT, (lstrlen((LPCTSTR)lpMsgBuf) * sizeof(TCHAR)));
        StringCchPrintf((LPTSTR)lpDisplayBuf, LocalSize(lpDisplayBuf) / sizeof(TCHAR), TEXT("failed with error %d: %s"), moreDetails, lpMsgBuf);

        LocalFree(lpMsgBuf);
        LocalFree(lpDisplayBuf);
        */
    }
} // namespace Platform
#endif