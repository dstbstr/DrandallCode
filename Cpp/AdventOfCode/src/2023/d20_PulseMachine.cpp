#include "2023/d20_PulseMachine.h"

SOLUTION(2023, 20) {
    //% = Flip-Flop => High ignored, Low On = !On, Send On(High or Low)
    //& = Conjunction => Send All inputs != High (1111 => 0, 0101 => 1)
    //broadcast => Send same pulse to all nodes
    //button => Send 0 to broadcaster
    //57 modules
    //Pulse <from, to, value>
    //High = 0, Low = 1 (Makes history easier for Con)

    struct Pulse {
        u16 From : 7 {0};
        u16 To : 7 {0};
        u16 High : 1{0};
    };

    struct Module {
        u16 Index;
        bool Con{ false };
        std::array<size_t, 64> InputMap{};
        u16 History{ 0 };
        size_t LastInput{ 1 };
        std::vector<u16> Outputs{};

        constexpr void MapInput(u16 from) {
            auto& bit = InputMap[from];
            if (bit == 0) {
                bit = LastInput++;
                Constexpr::SetBit(History, bit);
            }
        }
        
        constexpr std::vector<Pulse> SendPulse(Pulse pulse) {
            Pulse output{};

            if (Con) {
                auto bit = InputMap[pulse.From];
                pulse.High ? Constexpr::UnsetBit(History, bit) : Constexpr::SetBit(History, bit);
            }
            else {
                if (pulse.High) return {};
                Constexpr::ToggleBit(History, 0);
            }

            output.High = History > 0;
            output.From = Index;
            std::vector<Pulse> result;
            for (const auto& target : Outputs) {
                Pulse p = output;
                p.To = target;
                result.push_back(p);
            }
            return result;
        }
    };

    constexpr Module ParseModule(const std::string& line, Constexpr::SmallMap<std::string, u16>& nameIndexMap, u16& currentIndex) {
        Module result;
        result.Con = line[0] == '&';
        auto s = Constexpr::Split(line, " -> ");
        auto name = std::string(s[0].substr(1));
        if (!nameIndexMap.contains(name)) {
            nameIndexMap[name] = currentIndex++;
        }
        result.Index = nameIndexMap.at(name);
        
        auto s2 = Constexpr::Split(s[1], ", ");
        for (auto out : s2) {
            auto outName = std::string(out);
            if (!nameIndexMap.contains(outName)) {
                nameIndexMap[outName] = currentIndex++;
            }
            result.Outputs.push_back(nameIndexMap.at(outName));
        }

        return result;
    }

    constexpr std::pair<size_t, size_t> PressButton(std::vector<Module>& modules, const Module& broadcaster) {
        std::pair<size_t, size_t> result;
        result.first++;

        Constexpr::Queue<Pulse> q{};

        for (auto target : broadcaster.Outputs) {
            q.push({ broadcaster.Index, target, false });
        }

        while (!q.is_empty()) {
            auto current = q.front();
            q.pop();

            result.first += !current.High;
            result.second += current.High;
            if (current.To >= modules.size()) continue; //output
            auto next = modules[current.To].SendPulse(current);
            for (auto p : next) {
                q.push(p);
            }
        }

        return result;
    }

    constexpr u16 PressButton2(std::vector<Module>& modules, const Module& broadcaster, u16 target) {
        Constexpr::Queue<Pulse> q{};

        for (auto t : broadcaster.Outputs) {
            q.push({ broadcaster.Index, t, false });
        }

        u16 result = 0;
        while (!q.is_empty()) {
            auto current = q.front();
            q.pop();

            if (current.To >= modules.size()) continue; //output
            if (current.To == target && current.High) {
                result = current.From;
            }

            auto next = modules[current.To].SendPulse(current);
            for (auto p : next) {
                q.push(p);
            }
        }

        return result;
    }

    constexpr void Setup(const std::vector<std::string>& lines, Constexpr::SmallMap<std::string, u16>& outNameIndexMap, std::vector<Module>& outModules) {
        u16 currentIndex = 0;

        outModules = ParseLines(lines, ParseModule, outNameIndexMap, currentIndex);
        std::sort(outModules.begin(), outModules.end(), [](const Module& m1, const Module& m2) {
            return m1.Index < m2.Index;
            });

        for (u16 i = 0; i < outModules.size(); i++) {
            if (outModules[i].Index != i) {
                Module missing;
                missing.Index = i;
                outModules.insert(outModules.begin() + i, missing);
                break;
            }
        }
        std::vector<u16> cons;
        for (const auto& mod : outModules) {
            if (mod.Con) cons.push_back(mod.Index);
        }

        for (const auto& mod : outModules) {
            for (auto target : mod.Outputs) {
                for (auto con : cons) {
                    if (target == con) {
                        outModules[con].MapInput(mod.Index);
                        break;
                    }
                }
            }
        }

    }

    PART_ONE() {
        Constexpr::SmallMap<std::string, u16> nameIndexMap;
        std::vector<Module> modules;
        Setup(lines, nameIndexMap, modules);

        Module& broadcaster = modules[nameIndexMap.at("roadcaster")];
        size_t lows = 0;
        size_t highs = 0;

        for (auto press = 0; press < 1000; press++) {
            auto [low, high] = PressButton(modules, broadcaster);
            lows += low;
            highs += high;
        }

        return Constexpr::ToString(lows * highs);
    }

    PART_TWO() {
        Constexpr::SmallMap<std::string, u16> nameIndexMap;
        std::vector<Module> modules;
        Setup(lines, nameIndexMap, modules);

        Module& broadcaster = modules[nameIndexMap.at("roadcaster")];
        u16 rx = nameIndexMap.at("rx");
        u16 target = 0;
        for (const auto& mod : modules) {
            if (mod.Outputs[0] == rx) {
                target = mod.Index;
                break;
            }
        }

        std::vector<size_t> factors;
        size_t press = 1;
        while(factors.size() < modules[target].LastInput - 1) {
            auto from = PressButton2(modules, broadcaster, target);
            if (from != 0) {
                factors.push_back(press);
            }
            press++;
        }

        return Constexpr::ToString(std::accumulate(factors.begin(), factors.end(), 1ull, [](size_t running, size_t factor) {
            return running * factor;
            }));
    }

    TESTS() {
        std::vector<std::string> example = {
            "broadcaster -> a, b, c",
            "%a -> b",
            "%b -> c",
            "%c -> inv",
            "&inv -> a"
        };

        if (PartOne(example) != "32000000") return false;

        example = {
            "broadcaster -> a",
            "%a -> inv, con",
            "&inv -> b",
            "%b -> con",
            "&con -> output",
        };

        if (PartOne(example) != "11687500") return false;
        return true;
    }
}