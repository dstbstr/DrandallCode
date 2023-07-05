#include "2022/d7_Os.h"

SOLUTION(2022, 7) {
    struct Dir {
        std::string Path;
        std::shared_ptr<Dir> ParentDir;
        std::vector<std::shared_ptr<Dir>> Children;
        std::unordered_map<std::string, u32> Files;
    };

    u32 GetTotalSize(const Dir & dir) {
        u32 result = 0;
        for (auto& [file, size] : dir.Files) {
            result += size;
        }
        for (auto& child : dir.Children) {
            result += GetTotalSize(*child);
        }

        return result;
    }

    void ParseLine(const std::string & line, std::shared_ptr<Dir>&currentDir) {
        auto split = StrUtil::Split(line, " ");

        if (split[0] == "$") {
            if (split[1] == "ls") return;
            if (split[1] == "cd") {
                if (split[2] == "..") {
                    currentDir = currentDir->ParentDir;
                }
                else if (split[2] == "/") {
                    while (currentDir->ParentDir) {
                        currentDir = currentDir->ParentDir;
                    }
                }
                else {
                    auto targetDir = split[2];
                    auto children = currentDir->Children;
                    auto where = std::find_if(children.begin(), children.end(), [&targetDir](std::shared_ptr<Dir> dir) {
                        return StrUtil::EndsWith(dir->Path, targetDir);
                        });
                    if (where != children.end()) {
                        currentDir = *where;
                    }
                }
            }
        }
        else {
            if (split[0] == "dir") {
                auto child = std::make_shared<Dir>();
                child->ParentDir = currentDir;
                child->Path = currentDir->Path + "/" + std::string(split[1]);
                currentDir->Children.push_back(child);
            }
            else {
                u32 size;
                Constexpr::ParseNumber(split[0], size);
                currentDir->Files[std::string(split[1])] = size;
            }
        }
    }

    std::shared_ptr<Dir> ParseAll(const std::vector<std::string>&lines) {
        auto root = std::make_shared<Dir>();
        root->Path = "/";
        for (const auto& line : lines) {
            ParseLine(line, root);
        }

        ParseLine("$ cd /", root); //return to the root
        return root;
    }

    u32 SumDirectoriesLessThan(std::shared_ptr<Dir> root, u32 maxSize) {
        u32 totalSum = 0;
        std::vector<std::shared_ptr<Dir>> toSearch;
        toSearch.push_back(root);
        while (!toSearch.empty()) {
            auto dir = toSearch.back();
            toSearch.pop_back();
            for (auto child : dir->Children) {
                toSearch.push_back(child);
            }

            auto dirSize = GetTotalSize(*dir);
            if (dirSize <= maxSize) {
                totalSum += dirSize;
            }
        }

        return totalSum;
    }

    u32 FindDirToDelete(std::shared_ptr<Dir> root, u32 minSize) {
        u32 best = std::numeric_limits<u32>::max();
        std::vector<std::shared_ptr<Dir>> toSearch;
        toSearch.push_back(root);
        while (!toSearch.empty()) {
            auto dir = toSearch.back();
            toSearch.pop_back();
            for (auto child : dir->Children) {
                toSearch.push_back(child);
            }

            auto dirSize = GetTotalSize(*dir);
            if (dirSize >= minSize) {
                best = std::min(best, dirSize);
            }
        }

        return best;
    }

    std::string RunFirst(const std::vector<std::string>&lines) {
        auto root = ParseAll(lines);
        return Constexpr::ToString(SumDirectoriesLessThan(root, 100000));
    }

    std::string Run(const std::vector<std::string>&lines) {
        auto root = ParseAll(lines);
        u32 allSpace = 70'000'000;
        u32 targetSpace = 30'000'000;
        u32 usedSpace = GetTotalSize(*root);
        u32 neededSize = targetSpace - (allSpace - usedSpace);

        return Constexpr::ToString(FindDirToDelete(root, neededSize));
    }

    bool RunTests() {
        std::vector<std::string> lines;
        lines.push_back("$ cd /");
        lines.push_back("$ ls");
        lines.push_back("dir a");
        lines.push_back("14848514 b.txt");
        lines.push_back("8504156 c.dat");
        lines.push_back("dir d");
        lines.push_back("$ cd a");
        lines.push_back("$ ls");
        lines.push_back("dir e");
        lines.push_back("29116 f");
        lines.push_back("2557 g");
        lines.push_back("62596 h.lst");
        lines.push_back("$ cd e");
        lines.push_back("$ ls");
        lines.push_back("584 i");
        lines.push_back("$ cd ..");
        lines.push_back("$ cd ..");
        lines.push_back("$ cd d");
        lines.push_back("$ ls");
        lines.push_back("4060174 j");
        lines.push_back("8033020 d.log");
        lines.push_back("5626152 d.ext");
        lines.push_back("7214296 k");

        auto root = ParseAll(lines);
        if (root->Children.size() != 2) return false;
        if (root->Files.size() != 2) return false;
        if (SumDirectoriesLessThan(root, 100000) != 95437) return false;
        if (FindDirToDelete(root, 8381165) != 24933642) return false;
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