#include "Core/Algorithms/Shoelace.h"

constexpr bool TestShoelace() {
	std::vector<Coord> points = {
		{1, 6}, {3, 1}, {7, 2}, {4, 4}, {8, 5}
	};

	return Shoelace::GetInteriorArea(points) == 16.5;
}

static_assert(TestShoelace());