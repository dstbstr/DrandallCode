#include "2022/d7_Os.h"

SOLUTION(2022, 7) {
    struct Fs {
        struct Dir {
            size_t DirectFileSize = 0;
            size_t TotalFileSize = 0;
        };

        Constexpr::SmallMap<std::string, Dir> Paths;
        Constexpr::Stack<std::string> StackPath;

        constexpr Fs(const auto& lines) {
            Paths["/"];
            for (const auto& line : lines) {
                ParseLine(line);
            }
        }

        constexpr size_t SumDirectoriesLessThan(size_t limit) {
            auto dirs = Paths.GetValues();
            return std::accumulate(dirs.begin(), dirs.end(), 0ull, [limit](size_t prev, const Dir& d) {
                return prev + (d.TotalFileSize < limit ? d.TotalFileSize : 0);
                });
        }

        constexpr size_t FindDirToDelete() {
            size_t allSpace = 70'000'000;
            size_t targetSpace = 30'000'000;
            size_t usedSpace = Paths.at("/").TotalFileSize;
            size_t neededSize = targetSpace - (allSpace - usedSpace);

            auto dirs = Paths.GetValues();
            return RunAllReturnMin(dirs, [neededSize](const Dir& d) {
                return d.TotalFileSize > neededSize ? d.TotalFileSize : std::numeric_limits<size_t>::max();
                });
        }

        constexpr void ParseCommand(std::string_view line) {
            if (line[2] == 'l') return; //ls, no affect on currentDir
            if (line.back() == '/') return; //cd /, already assumed
            auto s = Constexpr::Split(line, " ");
            if (s.back() == "..") {
                StackPath.pop();
            }
            else {
                StackPath.push(std::string(s.back()));
            }
        }

        constexpr std::string GetPath() {
            auto stack = StackPath;
            std::string result = "/";
            while (!stack.is_empty()) {
                result.insert(0, "/" + stack.pop());
            }
            return result;
        }
        constexpr void ParseOutput(std::string_view line) {
            auto s = Constexpr::Split(line, " ");
            if (line[0] == 'd') {
                StackPath.push(std::string(s[1]));
                Paths[GetPath()];
                StackPath.pop();
            }
            else {
                size_t fileSize;
                Constexpr::ParseNumber(s[0], fileSize);
                Paths.at(GetPath()).DirectFileSize += fileSize;
                Paths.at(GetPath()).TotalFileSize += fileSize;
                auto originalPath = StackPath;
                while (!StackPath.is_empty()) {
                    StackPath.pop();
                    Paths.at(GetPath()).TotalFileSize += fileSize;
                }
                StackPath = originalPath;
            }
        }
        constexpr void ParseLine(std::string_view line) {
            if (line[0] == '$') ParseCommand(line);
            else ParseOutput(line);
        }
    };

    PART_ONE() {
        auto fs = Fs(Lines);
        return Constexpr::ToString(fs.SumDirectoriesLessThan(100'000));
    }

    PART_TWO() {
        auto fs = Fs(Lines);
        return Constexpr::ToString(fs.FindDirToDelete());
    }

    TESTS() {
        std::vector<std::string> lines{
            "$ cd /",
            "$ ls",
            "dir a",
            "14848514 b.txt",
            "8504156 c.dat",
            "dir d",
            "$ cd a",
            "$ ls",
            "dir e",
            "29116 f",
            "2557 g",
            "62596 h.lst",
            "$ cd e",
            "$ ls",
            "584 i",
            "$ cd ..",
            "$ cd ..",
            "$ cd d",
            "$ ls",
            "4060174 j",
            "8033020 d.log",
            "5626152 d.ext",
            "7214296 k"
        };
        Fs fs(lines);
        if (fs.SumDirectoriesLessThan(100'000) != 95437) return false;
        if (fs.FindDirToDelete() != 24933642) return false;
        return true;
    }
}