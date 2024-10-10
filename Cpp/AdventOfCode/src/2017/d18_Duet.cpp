#include "2017/d18_Duet.h"

SOLUTION(2017, 18) {
    using Registers = std::array<s64, 26>;

    constexpr size_t Unset = 9919;

    enum struct Command { Unknown, Snd, Set, Add, Mul, Mod, Jgz, Rcv};

    constexpr auto GenCommand(std::string_view line) {
        auto s = Constexpr::Split(line, " ");
        auto cmdStr = std::string(s[0]);
        Command cmd = Command::Unknown;
        if (cmdStr == "snd") cmd = Command::Snd;
        else if (cmdStr == "set") cmd = Command::Set;
        else if (cmdStr == "add") cmd = Command::Add;
        else if (cmdStr == "mul") cmd = Command::Mul;
        else if (cmdStr == "mod") cmd = Command::Mod;
        else if (cmdStr == "jgz") cmd = Command::Jgz;
        else if (cmdStr == "rcv") cmd = Command::Rcv;

        size_t regIndex = s[1][0] >= 'a' ? s[1][0] - 'a' : Constexpr::ParseNumber(s[1], regIndex);
        s64 v = Unset;
        size_t i = Unset;

        if (s.size() > 2) {
            s[2][0] >= 'a' ? i = s[2][0] - 'a' : Constexpr::ParseNumber(s[2], v);
        }

        return [cmd, regIndex, v, i](s64& ip, Registers& regs, s64& lastPlayed) {
            auto& lhs = regs[regIndex];
            auto rhs = i == Unset ? v : regs[i];

            ip++;
            switch (cmd) {
            case Command::Snd: lastPlayed = lhs; break;
            case Command::Set: lhs = rhs; break;
            case Command::Add: lhs += rhs; break;
            case Command::Mul: lhs *= rhs; break;
            case Command::Mod: lhs %= rhs; break;
            case Command::Jgz: ip += (rhs - 1) * (lhs > 0); break;
            case Command::Rcv: return lastPlayed * (lhs != 0);
            }
            return 0ll;
        };
    }

    //seems like there must be a way to do p1 with p2's commands, but I didn't figure it out
    PART_ONE() {
        s64 ip = 0;
        s64 lastPlayed = 0;
        Registers registers{};

        auto cmds = ParseLines(lines, GenCommand);
        s64 maxIp = static_cast<s64>(lines.size());
        while (ip < maxIp) {
            auto recovered = cmds[ip](ip, registers, lastPlayed);
            if (recovered > 0) {
                return Constexpr::ToString(recovered);
            }
        }

        return "Not Found";
    }

    constexpr auto GenCommand2(std::string_view line) {
        auto s = Constexpr::Split(line, " ");
        auto cmdStr = std::string(s[0]);
        Command cmd = Command::Unknown;
        if (cmdStr == "snd") cmd = Command::Snd;
        else if (cmdStr == "set") cmd = Command::Set;
        else if (cmdStr == "add") cmd = Command::Add;
        else if (cmdStr == "mul") cmd = Command::Mul;
        else if (cmdStr == "mod") cmd = Command::Mod;
        else if (cmdStr == "jgz") cmd = Command::Jgz;
        else if (cmdStr == "rcv") cmd = Command::Rcv;

        size_t regIndex = s[1][0] >= 'a' ? s[1][0] - 'a' : Constexpr::ParseNumber(s[1], regIndex);
        s64 v = Unset;
        size_t i = Unset;

        if (s.size() > 2) {
            s[2][0] >= 'a' ? i = s[2][0] - 'a' : Constexpr::ParseNumber(s[2], v);
        }

        return [cmd, regIndex, v, i](s64& ip, Registers& regs, Constexpr::Queue<s64>& inQueue, Constexpr::Queue<s64>& outQueue) {
            auto& lhs = regs[regIndex];
            auto rhs = i == Unset ? v : regs[i];

            ip++;
            switch (cmd) {
            case Command::Snd: outQueue.push(lhs); break;
            case Command::Set: lhs = rhs; break;
            case Command::Add: lhs += rhs; break;
            case Command::Mul: lhs *= rhs; break;
            case Command::Mod: lhs %= rhs; break;
            case Command::Jgz: ip += (rhs - 1) * (lhs > 0); break;
            case Command::Rcv: {
                if (inQueue.is_empty()) {
                    ip--;
                    return false;
                }
                lhs = inQueue.front();
                inQueue.pop();
                break;
            }
            }

            return true;
        };
    }


    PART_TWO() {
        s64 p0Ip = 0;
        s64 p1Ip = 0;

        Registers p0Regs{};
        Registers p1Regs{};
        p1Regs['p' - 'a'] = 1;

        Constexpr::Queue<s64> p0Q;
        Constexpr::Queue<s64> p1Q;
        s64 maxIp = static_cast<s64>(lines.size());

        auto p0Cmds = ParseLines(lines, GenCommand2);
        auto p1Cmds = p0Cmds;

        u32 p1SendCount = 0;
        while (true) {
            bool progress = false;

            if (p0Ip < maxIp) {
                progress = p0Cmds[p0Ip](p0Ip, p0Regs, p0Q, p1Q);
            }
            if (p1Ip < maxIp) {
                auto initialSize = p0Q.size();
                progress |= p1Cmds[p1Ip](p1Ip, p1Regs, p1Q, p0Q);
                p1SendCount += p0Q.size() != initialSize;
            }

            if (!progress) {
                break;
            }
        }

        return Constexpr::ToString(p1SendCount);
    }

    TESTS() {
        return true;
    }
}