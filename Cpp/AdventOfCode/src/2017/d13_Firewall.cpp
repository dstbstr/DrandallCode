#include "2017/d13_Firewall.h"

SOLUTION(2017, 13) {
    struct Scanner {
        s32 Layer = 0;
        s32 CurrentDepth = 0;
        s32 MaxDepth = 0;
        bool MovingUp = false;
    };

    constexpr std::vector<Scanner> GetScanners(const std::vector<std::string>&lines) {
        std::vector<Scanner> result;

        for (const auto& line : lines) {
            auto split = Constexpr::Split(line, ": ");
            u32 layer;
            u32 depth;
            Constexpr::ParseNumber(split[0], layer);
            Constexpr::ParseNumber(split[1], depth);

            Scanner scanner;
            scanner.Layer = layer;
            scanner.MaxDepth = depth;
            result.push_back(scanner);
        }

        return result;
    }

    static_assert(GetScanners({ "0: 3", "1: 2", "4: 4", "6: 4" }).size() == 4);

    constexpr void MoveScanners(std::vector<Scanner>&scanners) {
        for (auto& scanner : scanners) {
            if (scanner.MovingUp) {
                scanner.CurrentDepth--;
                if (scanner.CurrentDepth == 0) scanner.MovingUp = false;
            }
            else {
                scanner.CurrentDepth++;
                if (scanner.CurrentDepth == scanner.MaxDepth - 1) scanner.MovingUp = true;
            }
        }
    }

    constexpr u32 MovePacket(s32 & layer, const std::vector<Scanner>&scanners) {
        layer++;
        for (size_t i = 0; i < scanners.size(); i++) {
            if (scanners[i].Layer == layer) {
                if (scanners[i].CurrentDepth == 0) {
                    return scanners[i].MaxDepth * layer;
                }
                else {
                    return 0;
                }
            }
            if (scanners[i].Layer > layer) break;
        }

        return 0;
    }

    auto Part1(const std::vector<std::string>&lines) {
        s32 packetLayer = -1;
        auto scanners = GetScanners(lines);
        u32 result = 0;

        for (s32 i = 0; i <= scanners.back().Layer; i++) {
            result += MovePacket(packetLayer, scanners);
            MoveScanners(scanners);
        }

        return result;
    }

    void PrintState(const std::vector<Scanner>&scanners, s32 currentLayer) {
        auto maxLayer = scanners.back().Layer;
        s32 maxDepth = 0;
        std::vector<s32> depths;
        size_t index = 0;

        for (const auto& scanner : scanners) {
            while (index < scanner.Layer) {
                depths.push_back(0);
                index++;
            }

            depths.push_back(scanner.MaxDepth);
            index++;

            maxDepth = std::max(maxDepth, scanner.MaxDepth);
        }

        for (s32 row = 0; row < maxDepth; row++) {
            for (s32 col = 0; col <= maxLayer; col++) {
                if (col == currentLayer && row == 0) {
                    std::cout << '(';
                }
                if (depths[col] == 0) {
                    if (col != currentLayer || row > 0) {
                        std::cout << " . ";
                    }
                    else {
                        std::cout << '.';
                    }
                }
                else {
                    for (const auto& scanner : scanners) {
                        if (scanner.Layer == col) {
                            if (row > 0) {
                                if (row < scanner.MaxDepth) std::cout << '[';
                                else std::cout << ' ';
                            }
                            else if (col != currentLayer) std::cout << '[';

                            if (scanner.CurrentDepth == row) {
                                std::cout << 'S';
                            }
                            else {
                                std::cout << " ";
                            }
                            if (row > 0) {
                                if (row < scanner.MaxDepth) std::cout << ']';
                                else std::cout << ' ';
                            }
                            else if (col != currentLayer) std::cout << ']';
                        }
                    }
                }
                if (col == currentLayer && row == 0) {
                    std::cout << ')';
                }
            }

            std::cout << '\n';
        }
    }

    constexpr void SetStartingState(std::vector<Scanner>&scanners, u32 delay) {
        for (auto& scanner : scanners) {
            s32 period = (scanner.MaxDepth - 1) * 2;
            s32 remainder = delay % period;
            if (remainder < (period / 2)) {
                scanner.CurrentDepth = remainder;
                scanner.MovingUp = false;
            }
            else {
                scanner.CurrentDepth = period - remainder;
                scanner.MovingUp = true;
            }
        }
    }
    auto Part2(const std::vector<std::string>&lines) {
        u32 attempt = 0;
        const auto initialScanners = GetScanners(lines);

        while (true) {
            auto scanners = initialScanners;
            SetStartingState(scanners, attempt);
            s32 layer = -1;
            bool valid = true;

            if (scanners.front().CurrentDepth == 0) {
                attempt++;
                continue;
            }

            for (s32 i = 0; i <= scanners.back().Layer; i++) {
                if (MovePacket(layer, scanners) > 0) {
                    valid = false;
                    break;
                }
                MoveScanners(scanners);
            }

            if (valid) {
                return attempt;
            }

            attempt++;
        }

        throw std::logic_error("Wat");
    }

    std::string Run(const std::vector<std::string>&lines) {
        //return Constexpr::ToString(Part1(lines));
        return Constexpr::ToString(Part2(lines));
    }

    bool RunTests() {
        std::vector<std::string> lines = {
            "0: 3",
            "1: 2",
            "4: 4",
            "6: 4"
        };

        if (Part1(lines) != 24) return false;
        if (Part2(lines) != 10) return false;

        return true;
    }

    PART_ONE() {
        return lines[0];
    }

    PART_TWO() {
        return lines[0];
    }

    TESTS() {
        return true;
    }
}