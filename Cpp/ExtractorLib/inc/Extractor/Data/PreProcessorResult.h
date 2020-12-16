#ifndef __PREPROCESSORRESULT_H__
#define __PREPROCESSORRESULT_H__

#include <string>
#include <unordered_map>

struct PreProcessorResult {
    bool HasConditionalDefines;
    std::unordered_map<std::string, std::string> Defines;
};

#endif // __PREPROCESSORRESULT_H__