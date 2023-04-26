#pragma once

#include "Common.h"

template<size_t Rows, size_t Cols>
using Layer = std::array<std::array<u32, Cols>, Rows>;

template<size_t Rows, size_t Cols>
constexpr std::vector<Layer<Rows, Cols>> BuildLayers(const std::string& line) {
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
constexpr u32 CountDigit(const Layer<Rows, Cols>& layer, u32 digit) {
    u32 result = 0;
    for (size_t row = 0; row < Rows; row++) {
        for (size_t col = 0; col < Cols; col++) {
            if (layer[row][col] == digit) {
                result++;
            }
        }
    }

    return result;
}

template<size_t Rows, size_t Cols>
auto PartOne(const std::string& line) {
    auto layers = BuildLayers<Rows, Cols>(line);

    u32 best = 999;
    Layer<Rows, Cols> bestLayer;
    for (const auto& layer : layers) {
        auto zeroCount = CountDigit(layer, 0);
        if (zeroCount < best) {
            best = zeroCount;
            bestLayer = layer;
        }
    }

    return CountDigit<Rows, Cols>(bestLayer, 1) * CountDigit<Rows, Cols>(bestLayer, 2);
}

enum struct Pixel {Black = 0, White = 1, Transparent = 2};

template<size_t Rows, size_t Cols>
auto PartTwo(const std::string& line) {
    auto layers = BuildLayers<Rows, Cols>(line);

    std::unordered_map<RowCol, Pixel> image;

    for (size_t layer = 0; layer < layers.size(); layer++) {
        for (size_t row = 0; row < Rows; row++) {
            for (size_t col = 0; col < Cols; col++) {
                RowCol rc = { row, col };
                if (image.find(rc) != image.end()) continue;
                auto pixel = static_cast<Pixel>(layers[layer][row][col]);
                if (pixel != Pixel::Transparent) {
                    image[rc] = pixel;
                }
            }
        }
    }

    std::string result;
    for (size_t row = 0; row < Rows; row++) {
        for (size_t col = 0; col < Cols; col++) {
            RowCol rc = { row, col };
            char c = image[rc] == Pixel::White ? '#' : ' ';
            result.push_back(c);
        }
        result.push_back('\n');
    }

    return result;
}

std::string Run(const std::vector<std::string>& lines) {
    //return ToString(PartOne<6, 25>(lines[0]));
    return ToString(PartTwo<6, 25>(lines[0]));
}

bool RunTests() {
    std::string line = "123456789012";

    if (PartOne<2, 3>(line) != 1) return false;
    return true;
}