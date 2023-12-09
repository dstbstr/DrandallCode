#include "2017/d23_Coprocessor.h"

SOLUTION(2017, 23) {
    using Registers = std::array<s64, 8>;

    constexpr size_t Unset = 9919;
    enum struct Cmd {Set, Add, Sub, Mul, Mod, Jnz};

    constexpr auto GenCommand(std::string_view line) {
        auto s = Constexpr::Split(line, " ");
        Cmd cmd = Cmd::Set;
        if (s[0] == "set") cmd = Cmd::Set;
        else if (s[0] == "add") cmd = Cmd::Add;
        else if (s[0] == "sub") cmd = Cmd::Sub;
        else if (s[0] == "mul") cmd = Cmd::Mul;
        else if (s[0] == "mod") cmd = Cmd::Mod;
        else if (s[0] == "jnz") cmd = Cmd::Jnz;

        size_t regIndex = s[1][0] >= 'a' ? s[1][0] - 'a' : Constexpr::ParseNumber(s[1], regIndex);
        s64 v = Unset;
        size_t i = Unset;

        if (s.size() > 2) {
            s[2][0] >= 'a' ? i = s[2][0] - 'a' : Constexpr::ParseNumber(s[2], v);
        }

        return [cmd, regIndex, v, i](s64& ip, Registers& regs, size_t& result) {
            auto& lhs = regs[regIndex];
            auto rhs = i == Unset ? v : regs[i];

            ip++;

            switch (cmd) {
            case Cmd::Set: lhs = rhs; break;
            case Cmd::Add: lhs += rhs; break;
            case Cmd::Sub: lhs -= rhs; break; 
            case Cmd::Mul: lhs *= rhs; result++; break;
            case Cmd::Mod: lhs %= rhs; break;
            case Cmd::Jnz: ip += (rhs - 1) * (lhs != 0);
            }
        };
    }

    PART_ONE() {
        Registers regs{};

        s64 ip = 0;
        size_t result = 0;

        auto maxIp = static_cast<s64>(lines.size());
        auto cmds = ParseLines(lines, GenCommand);
        while (ip < maxIp) {
            cmds[ip](ip, regs, result);
        }

        return Constexpr::ToString(result);
    }

    PART_TWO() {
        Registers regs{};
        regs[0] = 1;

        auto unused = 0ull;
        auto ip = 0ll;
        auto cmds = ParseLines(lines, GenCommand);

        for (auto i = 0; i < 8; i++) {
            cmds[ip](ip, regs, unused);
        }

        auto minPrime = regs[1];
        auto maxPrime = regs[2];
        auto primes = Constexpr::GetPrimes<s64>(maxPrime);
        
        s32 count = 0;
        for (auto i = minPrime; i <= maxPrime; i += 17) {
            if (std::find(primes.cbegin(), primes.cend(), i) == primes.cend()) {
                count++;
            }
        }

        return Constexpr::ToString(count);       
    }

    TESTS() {
        return true;
    }
}