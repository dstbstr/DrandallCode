#include <vector>

using u32 = unsigned long;
using s32 = long;

void Foo() {
    u32 a = 0x80u << 24 | (0x00 << 16) | (0x50 << 8) | 0x00;
}
