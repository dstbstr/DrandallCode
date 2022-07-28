#ifndef __NEWSTUFF_H__
#define __NEWSTUFF_H__

#include <vcruntime.h>
#if _HAS_CXX20

namespace NewWay {
    struct SomeThing {
        int Value{0};
        auto operator<=>(const SomeThing& that) const = default;
    };

    class MyClass {
    public:
        MyClass();
        bool operator==(const MyClass& that) const;
        void SomeMethod();
    };

    class BaseClass {
    protected:
        int m_ProtectedMember{0};
        void SomeMethod(int input);
    };

    class DerivedClass : public BaseClass {
    public:
        void DerivedMethod() {
            this->SomeMethod(this->m_ProtectedMember);
        }
    };

    class DerivedClass2 : public BaseClass {
    protected:
        using BaseClass::m_ProtectedMember;
        using BaseClass::SomeMethod;

    public:
        void DerivedMethod() {
            SomeMethod(m_ProtectedMember);
        }
    };

    enum EnumOne { Alef, Bet, Gimel };

    enum EnumTwo { Alpha, Beta, Gamma };

    bool Compare(EnumOne lhs, EnumTwo rhs) {
        return (static_cast<int>(lhs) & static_cast<int>(rhs)) ? true : false;
    }

    class StrictString {
        const char* m_String;
        void Init() {
            m_String = "Hello";
        }
    };

    enum SomeEnum : uint8_t;
    constexpr int Adjust(SomeEnum val) {
        return val << 1;
    }

    enum SomeEnum : uint8_t { One, Two, Three };

    bool EnumFloatOperations(SomeEnum val) {
        if(static_cast<int>(val) * 1.5 > static_cast<int>(SomeEnum::Two)) {
            return true;
        }
        return false;
    }

    struct AStruct {
        template<class T>
        bool Method() {
            return false;
        }

        template<>
        bool Method<int>() {
            return true;
        }
    };

    template<class T>
    void NeedsKeyword() {
        auto it = T::template iterator{};
        typename T::unknown_type t;
    }

    int Foo(std::vector<int>& numbers) {
        int result = 0;
        for(int n: numbers) {
            result += n;
        }
        return result;
    }

} // namespace NewWay
#endif
#endif // __NEWSTUFF_H__