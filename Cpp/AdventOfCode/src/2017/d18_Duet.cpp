#include "2017/d18_Duet.h"

#include <queue>

SOLUTION(2017, 18) {
    using Registers = std::array<s64, 26>;

    constexpr size_t Unset = 9919;

    constexpr auto GenCommand(const std::string & line) {
        auto s = Constexpr::Split(line, " ");
        auto cmd = std::string(s[0]);
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

            if (cmd == "snd") lastPlayed = lhs;
            else if (cmd == "set") lhs = rhs;
            else if (cmd == "add") lhs += rhs;
            else if (cmd == "mul") lhs *= rhs;
            else if (cmd == "mod") lhs %= rhs;
            else if (cmd == "jgz") ip += (rhs - 1) * (lhs > 0);
            else if (cmd == "rcv") return lastPlayed * (lhs != 0);
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

    constexpr auto GenCommand2(const std::string& line) {
        auto s = Constexpr::Split(line, " ");
        auto cmd = std::string(s[0]);
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

            if (cmd == "snd") outQueue.push(lhs);
            else if (cmd == "set") lhs = rhs;
            else if (cmd == "add") lhs += rhs;
            else if (cmd == "mul") lhs *= rhs;
            else if (cmd == "mod") lhs %= rhs;
            else if (cmd == "jgz") ip += (rhs - 1) * (lhs > 0);
            else if (cmd == "rcv") {
                if (inQueue.is_empty()) {
                    ip--;
                    return false;
                };
                lhs = inQueue.front();
                inQueue.pop();
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