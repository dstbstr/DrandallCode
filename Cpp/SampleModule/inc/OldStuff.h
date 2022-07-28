#ifndef __OLDSTUFF_H__
#define __OLDSTUFF_H__

#include <string>
#include <utility>
#include <vector>


namespace OldWay {
    /*
    using std::rel_ops::operator!=;
    using std::rel_ops::operator<=;
    using std::rel_ops::operator>=;
    using std::rel_ops::operator>;

    struct SomeThing {
        int Value{0};
        bool operator<(const SomeThing& that) const {
            return Value < that.Value;
        }
        bool operator==(const SomeThing& that) const {
            return Value == that.Value;
        }
    };

    class MyClass {
    public:
        MyClass::MyClass();
        bool MyClass::operator==(const MyClass& that) const;
        void MyClass::SomeMethod();
    };

    class BaseClass {
    protected:
        int m_ProtectedMember{0};
        void SomeMethod(int input);
    };

    class DerivedClass : public BaseClass {
    public:
        void DerivedMethod() {
            SomeMethod(m_ProtectedMember);
        }
    };

    enum EnumOne { One, Two, Three };

    enum EnumTwo { Alpha, Beta, Gamma };

    bool Compare(EnumOne lhs, EnumTwo rhs) {
        return (lhs & rhs) ? true : false;
    }

    class StrictString {
        char* m_String;
        void Init() {
            m_String = "Hello";
        }
    };
*/

    enum SomeEnum;
    constexpr int Adjust(SomeEnum val) {
        return val << 1;
    }

    enum SomeEnum { One, Two, Three };

    /*
        bool EnumFloatOperations(SomeEnum val) {
            if(val * 1.5 > SomeEnum::Two) {
                return true;
            }
            return false;
        }

        int GetOrDefault(int toCheck, int default) {
            return toCheck == 0 ? default : toCheck;
        }
    */

    struct AStruct {
        template<class T>
        static bool Method() {
            return false;
        }

        template<>
        static bool Method<int>() {
            return true;
        }
    };

    /*
        template<class T>
        void NeedsKeyword() {
            auto it = T::iterator{};
            T::unknown_type t;
        }
    */

   int Foo(std::vector<int>& numbers) {
       int result = 0;
       for each(int n in numbers) {
           result += n;
       }
       return result;
   }

} // namespace OldWay

#endif // __OLDSTUFF_H__