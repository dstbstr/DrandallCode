#pragma once

#include "IInputReader.h"

struct ExeInputReader : public IInputReader {
	std::vector<std::string> ReadInput(size_t year, size_t day) const override;
};