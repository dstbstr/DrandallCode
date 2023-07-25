#include "2019/d8_ImageFormat.h"

SOLUTION(2019, 8) {
    template<size_t Rows, size_t Cols>
    using Layer = std::array<std::array<u32, Cols>, Rows>;

    template<size_t Rows, size_t Cols>
    constexpr std::vector<Layer<Rows, Cols>> BuildLayers(const std::string & line) {
        auto layerSize = Rows * Cols;
        std::vector<Layer<Rows, Cols>> result;

        for (size_t layer = 0; layer < line.size() / layerSize; layer++) {
            Layer<Rows, Cols> toAdd;
            for (size_t row = 0; row < Rows; row++) {
                for (size_t col = 0; col < Cols; col++) {
                    auto index = layerSize * layer;
                    index += (row * Cols) + col;
                    toAdd[row][col] = line[index] - '0';
                }
            }
            result.push_back(toAdd);
        }

        return result;
    }

    template<size_t Rows, size_t Cols>
    constexpr size_t CountDigit(const Layer<Rows, Cols>&layer, u32 digit) {
        return std::accumulate(layer.begin(), layer.end(), 0ull, [digit](size_t previous, const auto& row) { return previous + std::count(row.begin(), row.end(), digit); });
    }

    PART_ONE() {
        auto layers = BuildLayers<6, 25>(lines[0]);

        size_t best = 999;
        size_t result = 0;
        for (const auto& layer : layers) {
            auto zeroCount = CountDigit(layer, 0);
            if (zeroCount < best) {
                best = zeroCount;
                result = CountDigit(layer, 1) * CountDigit(layer, 2);
            }
        }

        return Constexpr::ToString(result);
    }

    PART_TWO() {
        auto layers = BuildLayers<6, 25>(lines[0]);

        Layer<6, 25> image{};
        for (auto& r : image) {
            r.fill(99);
        }
        for (const auto& layer : layers) {
            for (size_t row = 0; row < 6; row++) {
                for (size_t col = 0; col < 25; col++) {
                    if (image[row][col] < 2) continue;
                    image[row][col] = layer[row][col];
                }
            }
        }
        std::string result = "\n";
        for (const auto& row : image) {
            for (const auto& c : row) {
                result.push_back(c == 0 ? ' ' : '#');
            }
            result.push_back('\n');
        }

        return result;
    }

    TESTS() {
        return true;
    }
}