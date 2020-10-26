#ifndef DEBUGCONTEXT_H
#define DEBUGCONTEXT_H

#define DEBUG_CONTEXT Debug::Context(__FILE__, __LINE__, __FUNCSIG__, GetCurrentThreadId())

namespace Debug {
    struct Context {
        Context(std::string fileName, u32 lineNumber, std::string functionSignature, u32 threadId) : FileName(fileName), LineNumber(lineNumber), FunctionSignature(functionSignature), ThreadId(threadId) {
            auto name = FunctionSignature.substr(FunctionSignature.find(' ') + 1); // strip return type
            name = name.substr(name.find(' ') + 1); // strip out cdecl

            auto classSeparatorIndex = name.find("::");
            if(classSeparatorIndex == std::string::npos) {
                FunctionName = name;
            } else {
                FunctionClass = name.substr(0, classSeparatorIndex);
                FunctionName = name.substr(classSeparatorIndex + 2);
            }

            FileNameShort = FileName.substr(FileName.find_last_of('\\') + 1);
        }

        std::string FileName;
        std::string FileNameShort;
        u32 LineNumber{0};
        std::string FunctionSignature;
        std::string FunctionClass;
        std::string FunctionName;
        u32 ThreadId{0};
    };
} // namespace Debug
#endif