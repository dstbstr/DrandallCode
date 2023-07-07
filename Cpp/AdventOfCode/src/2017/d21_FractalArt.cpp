#include "2017/d21_FractalArt.h"

SOLUTION(2017, 21) {
    using Pattern = std::vector<std::vector<bool>>;

    constexpr std::string ToString(const Pattern & pattern) {
        std::string result;
        for (const auto& row : pattern) {
            for (const auto& col : row) {
                result += (col ? '#' : '.');
            }
            result += '\n';
        }
        return result;
    }

    constexpr Pattern ParsePattern(std::string_view line) {
        Pattern result;
        auto split = Constexpr::Split(line, "/");
        for (auto row : split) {
            std::vector<bool> r;
            for (auto c : row) {
                r.push_back(c == '#');
            }
            result.push_back(r);
        }

        return result;
    }

    constexpr std::vector<Pattern> GetTransforms(const Pattern & input) {
       
        auto transpose = [](const Pattern& p) {
            Pattern result = p;
            const auto rows = p.size();
            const auto cols = p[0].size();

            for (size_t row = 0; row < rows; row++) {
                for (size_t col = 0; col < cols; col++) {
                    result[col][row] = p[rows - row - 1][cols - col - 1];
                }
            }
            return result;
        };
        auto flip = [](const Pattern& p) {
            Pattern result = p;
            size_t size = p.size();
            for (size_t row = 0; row < size; row++) {
                for (size_t col = 0; col < size; col++) {
                    result[row][col] = p[row][size - col - 1];
                }
            }
            return result;
        };

        std::vector<Pattern> result;
        result.push_back(input);

        for (auto i = 0; i < 4; i++) {
            result.push_back(transpose(result.back()));
            result.push_back(flip(result.back()));
        }

        return result;
    }

    using InputMap = Constexpr::SmallMap<std::string, std::vector<Pattern>>;
    using OutputMap = Constexpr::SmallMap<std::string, Pattern>;

    constexpr InputMap ParsePatternInputs(const std::vector<std::string>& lines) {
        InputMap result;
        for (const auto& line : lines) {
            auto s = Constexpr::Split(line, " => ");
            auto key = std::string(s[0]);
            auto pattern = ParsePattern(s[0]);
            result[key] = GetTransforms(pattern);
        }

        return result;
    }

    constexpr OutputMap ParsePatternOutputs(const std::vector<std::string>&lines) {
        OutputMap result;

        for (const auto& line : lines) {
            auto s = Constexpr::Split(line, " => ");
            auto key = std::string(s[0]);
            auto input = ParsePattern(s[0]);
            auto output = ParsePattern(s[1]);
            result[key] = output;
        }

        return result;
    }

    constexpr std::vector<std::vector<Pattern>> GetChunks(const Pattern& pattern) {
        std::vector<std::vector<Pattern>> result;
        if (pattern.size() % 2 == 0) {
            for (auto row = 0; row < pattern.size(); row += 2) {
                std::vector<Pattern> chunkRow;
                for (auto col = 0; col < pattern.size(); col += 2) {
                    Pattern chunk;
                    chunk.push_back({ pattern.at(row).begin() + col, pattern.at(row).begin() + col + 2 });
                    chunk.push_back({ pattern.at(row + 1).begin() + col, pattern.at(row + 1).begin() + col + 2 });
                    chunkRow.push_back(chunk);
                }
                result.push_back(chunkRow);
            }
        }
        else {
            for (auto row = 0; row < pattern.size(); row += 3) {
                std::vector<Pattern> chunkRow;
                for (auto col = 0; col < pattern.size(); col += 3) {
                    Pattern chunk;
                    chunk.push_back({ pattern.at(row).begin() + col, pattern.at(row).begin() + col + 3 });
                    chunk.push_back({ pattern.at(row + 1).begin() + col, pattern.at(row + 1).begin() + col + 3 });
                    chunk.push_back({ pattern.at(row + 2).begin() + col, pattern.at(row + 2).begin() + col + 3 });
                    chunkRow.push_back(chunk);
                }
                result.push_back(chunkRow);
            }
        }
        return result;
    }

    constexpr void UpdateChunk(Pattern& chunk, const InputMap& inputPatterns, const OutputMap& outputPatterns) {
        std::string matchedPattern;
        for (const auto& [key, transforms] : inputPatterns) {
            if (transforms[0].size() != chunk[0].size()) continue;
            for (const auto& transform : transforms) {
                if (chunk == transform) {
                    chunk = outputPatterns.at(key);
                    return;
                }
            }
        }
    }

    constexpr Pattern CombineChunks(const std::vector<std::vector<Pattern>>& chunks) {
        Pattern result;
        for (const auto& chunkRow : chunks) {
            for (size_t currentRow = 0; currentRow < chunkRow[0].size(); currentRow++) {
                std::vector<bool> patternRow;
                for (const auto& chunk : chunkRow) {
                    for (size_t col = 0; col < chunk.size(); col++) {
                        patternRow.push_back(chunk[currentRow][col]);
                    }
                }
                result.push_back(patternRow);
            }
        }
        return result;
    }

    static constexpr auto Start = ".#./..#/###";
    /*
    auto Part1(const std::vector<std::string>&lines, size_t iterations) {
        auto inputPatterns = ParsePatternInputs(lines);
        auto outputPatterns = ParsePatternOutputs(lines);

        auto pattern = ParsePattern(Start);

        for (auto i = 0; i < iterations; i++) {
            //std::cout << ToString(pattern) << '\n';

            auto chunks = GetChunks(pattern);
            for (auto& chunkRow : chunks) {
                for (auto& chunk : chunkRow) {
                    UpdateChunk(chunk, inputPatterns, outputPatterns);
                }
            }

            pattern = CombineChunks(chunks);
        }

        u32 count = 0;
        Constexpr::ApplyToRowCol(pattern, [&](size_t row, size_t col) {
            count += pattern[row][col];
            });
        return count;
    }

    auto Part2(const std::vector<std::string>&lines) {
        return lines.size();
    }

    std::string Run(const std::vector<std::string>&lines) {
        //return Constexpr::ToString(Part1(lines, 5));
        return Constexpr::ToString(Part1(lines, 18));
    }

    bool RunTests() {
        std::vector<std::string> lines = {
            "../.# => ##./#../...",
            ".#./..#/### => #..#/..../..../#..#"
        };

        if (Part1(lines, 2) != 12) return false;
        return true;
    }
    */

    constexpr auto Solve(const std::vector<std::string>& lines, size_t iterations) {
        auto inputPatterns = ParsePatternInputs(lines);
        auto outputPatterns = ParsePatternOutputs(lines);

        auto pattern = ParsePattern(Start);

        for (auto i = 0; i < iterations; i++) {
            auto chunks = GetChunks(pattern);
            for (auto& chunkRow : chunks) {
                for (auto& chunk : chunkRow) {
                    UpdateChunk(chunk, inputPatterns, outputPatterns);
                }
            }

            pattern = CombineChunks(chunks);
        }

        u32 count = 0;
        Constexpr::ApplyToMatrixValueConst(pattern, [&count](bool v) { count += v; });
        return count;
    }
    PART_ONE() {
        /*
        Constexpr::ApplyToRowCol(pattern, [&](size_t row, size_t col) {
            count += pattern[row][col];
            });
        */
        /*
        for (size_t row = 0; row < pattern.size(); row++) {
            for (size_t col = 0; col < pattern[row].size(); col++) {
                count += pattern[row][col];
            }
        }
        */
        return Constexpr::ToString(Solve(lines, 5));
    }

    PART_TWO() {
        return Constexpr::ToString(Solve(lines, 18));
    }

    TESTS() {
        return true;
    }
}