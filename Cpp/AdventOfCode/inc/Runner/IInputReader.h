#pragma once

#include <vector>
#include <string>

struct IInputReader {
	virtual ~IInputReader() = default;
	virtual std::vector<std::string> ReadInput(size_t year, size_t day) const = 0;
};
