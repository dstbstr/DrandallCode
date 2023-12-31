#pragma once
#include "Constexpr/ConstexprGeometry.h"

//Finds the interior area of a Simple (non-intersecting) polygon

//https://en.wikipedia.org/wiki/Shoelace_formula

/*
Determinant:
|a b|
|c d|
==
ad - bc

|0 -5 -2 0|
|3 -3  2 3|

(0*-3 + -5*2 + -2*3) - (3*-5 + -3*-2 + 2*0)
(0 -10 - 6) - (-15 + 6 + 0)
-16 + 9
-7
*/

/*
<1,6> <3,1> <7,2> <4,4> <8,5>
|1 3 7 4 8 1|
|6 1 2 4 5 6|
(1 + 6 + 28 + 20 + 48) - (18 + 7 + 8 + 32 + 5)
or
(1 - 18) + (6 - 7) + (28 - 8) + (20 - 32) + (48 - 5)
103 - 70 = 33
A = |33| / 2 = 16.5
*/

namespace Shoelace {
	constexpr double GetInteriorArea(const std::vector<Coord>& points) {
		auto Determinant = [](Coord a, Coord b) {return (a.X * b.Y) - (a.Y * b.X); };
		s64 running = 0;
		for (auto i = 0; i < points.size() - 1; i++) {
			running += Determinant(points[i], points[i + 1]);
		}
		running += Determinant(points.back(), points[0]);
		if (running < 0) running = -running;

		return running / 2.0;
	}
}