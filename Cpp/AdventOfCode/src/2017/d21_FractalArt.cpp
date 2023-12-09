#include "2017/d21_FractalArt.h"

SOLUTION(2017, 21) {
    static constexpr auto Start = ".#./..#/###";

    /*
    12 => 31
    34    42

    123    741
    456 => 852
    789    963
    */

    constexpr std::string Rotate(std::string_view in) {
        if (in.size() == 5) {
            return std::string({ in[3], in[0], in[2], in[4], in[1] });
        }
        return std::string({ in[8], in[4], in[0], in[3], in[9], in[5], in[1], in[3], in[10], in[6], in[2] });
    }

    /*
    12 => 21
    34    43

    123    321
    456 => 654
    789    987
    */
    constexpr std::string Flip(std::string_view in) {
        if (in.size() == 5) {
            return std::string({ in[1], in[0], in[2], in[4], in[3] });
        }
        return std::string({ in[2], in[1], in[0], in[3], in[6], in[5], in[4], in[7], in[10], in[9], in[8] });
    }

    using Transforms = Constexpr::SmallMap<std::string, std::string>;

    constexpr Transforms GetTransforms(const auto& lines) {
        Transforms result;
        for (const auto& line : lines) {
            auto s = Constexpr::Split(line, " => ");
            auto key = std::string(s[0]);
            auto transform = std::string(s[1]);

            std::vector<std::string> keys {key};
            for (auto i = 0; i < 4; i++) {
                keys.push_back(Rotate(keys.back()));
            }
            keys.push_back(Flip(keys.back()));
            for (auto i = 0; i < 4; i++) {
                keys.push_back(Rotate(keys.back()));
            }

            for (const auto& k : keys) {
                result[k] = transform;
            }
        }
        return result;
    }

    constexpr std::vector<std::string> Split(const std::string& grid) {
        std::vector<std::string> result;
        auto separator = '/';
        
        if (grid.size() == 5) { //2x2 => 1 2x2
            return { grid };
        }
        if (grid.size() == 11) { //3x3 => 1 3x3
            return { grid };
        }
        else if (grid.size() == 19) { //4x4 => 4 2x2
            //0123/5678/0123/5678
            return {
                std::string{grid[0], grid[1], separator, grid[5], grid[6]},
                std::string{grid[2], grid[3], separator, grid[7], grid[8]},
                std::string{grid[10], grid[11], separator, grid[15], grid[16]},
                std::string{grid[12], grid[13], separator, grid[17], grid[18]}
            };
        }
        else if(grid.size() == 41) { //6x6 => 9 2x2
            //012345/789012/456789/123456/890123/567890
            return {
                std::string{grid[0], grid[1], separator, grid[7], grid[8]},
                std::string{grid[2], grid[3], separator, grid[9], grid[10]},
                std::string{grid[4], grid[5], separator, grid[11], grid[12]},

                std::string{grid[14], grid[15], separator, grid[21], grid[22]},
                std::string{grid[16], grid[17], separator, grid[23], grid[24]},
                std::string{grid[18], grid[19], separator, grid[25], grid[26]},

                std::string{grid[28], grid[29], separator, grid[35], grid[36]},
                std::string{grid[30], grid[31], separator, grid[37], grid[38]},
                std::string{grid[32], grid[33], separator, grid[39], grid[40]}
            };
        }
        else {
            return {}; //
        }
    }

    constexpr std::string Combine4x2x2(const std::vector<std::string>& chunks) {
        /*
        ".#/#.",
        "../##",
        "#./.#",
        "##/##"
         =>

        ".#../#.##/#.##/.###"
        */
        std::string result;
        result.reserve(19);
        result += chunks[0].substr(0, 2);
        result += chunks[1].substr(0, 2);
        result += "/";

        result += chunks[0].substr(3, 2);
        result += chunks[1].substr(3, 2);
        result += "/";

        result += chunks[2].substr(0, 2);
        result += chunks[3].substr(0, 2);
        result += "/";

        result += chunks[2].substr(3, 2);
        result += chunks[3].substr(3, 2);

        return result;
    }
    constexpr std::string Combine4x3x3(const std::vector<std::string>& chunks) {
        /*
        "..#/#.#/.##"
        "#../.#./..."
        "##./#../##."
        "##./..#/#.#"
        
        =>
        "..##../#.#.#./.##.../##.##./#....#/##.#.#"
        
        */

        std::string result;
        result.reserve(36);

        result += chunks[0].substr(0, 3);
        result += chunks[1].substr(0, 3);
        result += "/";

        result += chunks[0].substr(4, 3);
        result += chunks[1].substr(4, 3);
        result += "/";

        result += chunks[0].substr(8, 3);
        result += chunks[1].substr(8, 3);
        result += "/";

        result += chunks[2].substr(0, 3);
        result += chunks[3].substr(0, 3);
        result += "/";

        result += chunks[2].substr(4, 3);
        result += chunks[3].substr(4, 3);
        result += "/";

        result += chunks[2].substr(8, 3);
        result += chunks[3].substr(8, 3);
        
        return result;
    }

    constexpr std::string Combine(const std::vector<std::string>& chunks) {
        if (chunks.size() == 1) {
            return chunks[0];
        }
        else if (chunks.size() == 4) {
            if (chunks[0].size() == 5) {
                return Combine4x2x2(chunks);
            }
            else {
                return Combine4x3x3(chunks);
            }
        }
        return "";
    }

    constexpr std::vector<std::string> Next(const std::string& pattern, const Transforms& transforms) {
        auto s = Split(pattern);
        std::vector<std::string> result;
        for (const auto& v : s) {
            result.push_back(transforms.at(v));
        }
        if (result.size() == 1 || result.size() == 9) {
            return result;
        }
        else {
            return { Combine(result) };
        }
    }

    constexpr auto Solve(const auto& lines, size_t iterations) {
        auto transforms = GetTransforms(lines);

        Constexpr::SmallMap<std::string, std::vector<std::string>> cache{};
        std::vector<std::string> q{Start};
        std::vector<std::string> next;

        for (auto i = 0; i < iterations; i++) {
            for (const auto& pattern : q) {
                auto n = cache.contains(pattern) ? cache.at(pattern) : Next(pattern, transforms);
                cache[pattern] = n;
                std::copy(n.begin(), n.end(), std::back_inserter(next));
            }
            q = next;
            next.clear();
        }

        size_t result = 0;
        for (const auto& pattern : q) {
            result += std::count(pattern.begin(), pattern.end(), '#');
        }
        return result;
    }

    PART_ONE() {
        return Constexpr::ToString(Solve(lines, 5));
    }

    PART_TWO() {
        return Constexpr::ToString(Solve(lines, 18));
    }

    TESTS() {
        return true;
    }
}