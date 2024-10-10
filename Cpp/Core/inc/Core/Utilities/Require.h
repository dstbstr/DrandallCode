#ifndef __REQUIRE_H__
#define __REQUIRE_H__

#include "Core/Utilities/StringUtils.h"

#include <stdexcept>
#include <string>
#include <vector>

namespace Require {
    inline void True(bool val, std::string message) {
        if(!val) {
            throw std::runtime_error(message);
        }
    }

    inline void True(bool val) {
        True(val, "Expected value to be true, but was false");
    }

    inline void False(bool val, std::string message) {
        if(val) {
            throw std::runtime_error(message);
        }
    }

    inline void False(bool val) {
        False(val, "Expected value to be false, but was true");
    }

    inline void NotNull(void* ptr, std::string message) {
        if(!ptr) {
            throw std::runtime_error(message);
        }
    }

    inline void NotNull(void* ptr) {
        NotNull(ptr, "Received unexpected null pointer");
    }

    inline void Null(void* ptr, std::string message) {
        if(ptr) {
            throw std::runtime_error(message);
        }
    }

    inline void Null(void* ptr) {
        Null(ptr, "Expected pointer to be null, but was not");
    }

#if __has_include("winerror.h")
#include "winerror.h"
    inline void Success(HRESULT hr, std::string message) {
        if(!(SUCCEEDED(hr))) {
            throw std::runtime_error(message);
        }
    }

    inline void Success(HRESULT hr) {
        Success(hr, "Provided HRESULT was not a success");
    }
#endif

    template<class T>
    inline void Empty(const T& t, std::string message) {
        if(!t.empty()) {
            throw std::runtime_error(message);
        }
    }

    template<class T>
    inline void Empty(const T& t) {
        Empty(t, "Received a filled object, expected empty");
    }

    template<class T>
    inline void NotEmpty(const T& t, std::string message) {
        if(t.empty()) {
            throw std::runtime_error(message);
        }
    }

    template<class T>
    inline void NotEmpty(const T& t) {
        NotEmpty(t, "Received unexpected empty object");
    }

    inline void NotJustWhitespace(const std::string& str, std::string message) {
        std::string cpy = str;
        if(StrUtil::Trim(cpy).empty()) {
            throw std::runtime_error(message);
        }
    }

    inline void NotJustWhitespace(const std::string& str) {
        NotJustWhitespace(str, "Received unexpected blank or whitespace string");
    }

} // namespace Require
#endif // __REQUIRE_H__